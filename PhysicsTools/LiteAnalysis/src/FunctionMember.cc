// -*- C++ -*-
//
// Package:    PhysicsTools
// Class:      FunctionMember
// 
/**\class FunctionMember FunctionMember.cc 
   PhysicsTools/LiteAnalysis/src/FunctionMember.cc

   Description: Some simple utilities

   Implementation:
   Common sense and a sense of beauty.
*/
//
// Original Author:  Harrison B. Prosper
//         Created:  Wed Jun 20 19:53:47 EDT 2007
//         Updated:  Sat Oct 25 2008 - make matchInDeltaR saner
// $Id: FunctionMember.cc,v 1.1 2010/09/06 05:43:07 prosper Exp $
//
//
//-----------------------------------------------------------------------------
#include <Python.h>
#include <boost/python.hpp>
#include <boost/regex.hpp>
#include <string>
#include <stdlib.h>
//-----------------------------------------------------------------------------
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "PhysicsTools/LiteAnalysis/interface/FunctionMember.h"
//-----------------------------------------------------------------------------
using namespace std;
using namespace ROOT::Reflex;
//-----------------------------------------------------------------------------

FunctionMember::FunctionMember()
  : classname1_(""),
    methodrecord_(""),
    debug_(0),
    method1_(Member()),
    classname2_(""),
    method2_(Member()),
    simplemethod_(true),
    smartpointer_(false),
    datamember1_(false),
    datamember2_(false)
{}

// Model an instantiable method using the Reflex tools

FunctionMember::FunctionMember(std::string classname, 
                               std::string methodrecord)
  : classname1_(classname),
    methodrecord_(methodrecord),
    debug_(0),
    method1_(Member()),
    classname2_(""),
    method2_(Member()),
    simplemethod_(true),
    smartpointer_(false),
    datamember1_(false),
    datamember2_(false)
{
  if ( getenv("DEBUGMETHOD") > 0 )
    debug_ = atoi(getenv("DEBUGMETHOD"));
  else
    debug_ = 0;
  
  if ( debug_ > 0 )
    cout << "FunctionMember - classname<" << classname1_ << ">\n" 
         << "         method   <" << methodrecord_ << ">" << endl;

  if ( classname1_ == "" ) 
    throw cms::Exception("InvalidClassname")
      << "null classname " << endl;

  // check for compound method
  string delim("");
  bool compoundmethod = kit::isCompoundMethod(methodrecord_, delim);
  simplemethod_ = ! compoundmethod;

  // If method is compound, split it into its components
  methodrecord1_ = methodrecord_;
  methodrecord2_ = "";
  if ( compoundmethod )
    {
      if ( debug_ > 0 )
        cout << "       - delimeter( " << delim << " )" << endl;

      kit::bisplit(methodrecord_, methodrecord1_, methodrecord2_, delim);
    }

  if ( debug_ > 0 )
    cout << "       - method1<" << methodrecord1_ << ">\n" 
         << "       - method2<" << methodrecord2_ << ">" << endl;
  
  //----------------------------------------------
  // method 1
  //----------------------------------------------
  // Check for a data member
  
  boost::regex dregex("^[a-zA-Z_]+[a-zA-Z0-9_]*$");
  boost::smatch dmatch;
  datamember1_ = boost::regex_search(methodrecord1_, dmatch, dregex);
  if ( datamember1_ )
    {
      if ( debug_ > 0 )
        cout << "       - data member1( " << methodrecord1_ << " )" << endl;
      method1_ = kit::getDataMember(classname1_, methodrecord1_);
    }
  else
    {
      if ( debug_ > 0 )
        cout << "       - function member1( " << methodrecord1_ << " )" 
             << endl;
      method1_ = kit::decodeMethod(classname1_, methodrecord1_, 
                                   values1_, args1_);
    }

  // Complain if we did not find a valid method/data member
  if ( !kit::memberValid(method1_) )
    {
      throw cms::Exception("decodeMethodFailure")
        << " can't decode method/data member: " 
        << classname1_ << "::" 
        << method1_.Name() << endl;
    }

  // If this is a simple method or data member, we're done
  if ( simplemethod_ )
    {
      if ( debug_ > 0 )
        cout << "       - SIMPLE( " << methodrecord1_ << " )" 
             << endl;
      return;
    }
  
  //----------------------------------------------
  // method 2 - 2nd part of a compound method
  // Note: since this is a compound method it 
  //       returns an object, either by pointer,
  //       by reference, by value, or by smart 
  //       pointer.
  //----------------------------------------------

  if ( debug_ > 0 )
    cout << "       - COMPOUND(" << methodrecord_ << ")" 
         << endl;

  // The first part of the method could have been a data member.
  // If it isn't, check the return type of method 1
  if ( datamember1_ )
    {
      Type dtype = method1_.TypeOf();
      classname2_ = dtype.Name(SCOPED+FINAL);
      if ( dtype.IsPointer() || dtype.IsReference() )
        classname2_ = classname2_.substr(0,classname2_.size()-1);
      if ( classname2_ == "" )
        {
          throw cms::Exception("datamemberTypeFailure")
            << " can't get type for data member " 
            << method1_.Name() << endl;
        }
      if ( debug_ > 0 )
        cout << "       - is data member of type (" 
             << classname2_ << ")" << endl;
    }
  else
    {
      classname2_ = kit::returnType(method1_);
      if ( classname2_ == "" )
        {
          throw cms::Exception("returnTypeFailure")
            << " can't get return class for method " 
            << method1_.Name() << endl;
        }

      if ( debug_ > 0 )
        cout << "       - return class( " << classname2_ << " )" << endl;
    }

  // Check if we have a data member in returned object  
  datamember2_ = boost::regex_search(methodrecord2_, dmatch, dregex);
  if ( datamember2_ )
    {
      if ( debug_ > 0 )
        cout << "       - data member2( " << methodrecord2_ << " )" << endl;
    }
  else
    {
      if ( debug_ > 0 )
        cout << "       - function member2( " << methodrecord2_ << " )" 
             << endl;
      method2_ = kit::decodeMethod(classname2_, methodrecord2_, 
                                   values2_, args2_);

      // Complain if we did not find a valid method
      if ( !kit::memberValid(method2_) )
        {
          throw cms::Exception("decodeMethodFailure")
            << " can't decode method/data member: " 
            << classname2_ << "::" 
            << method2_.Name() << endl;
        }
    }


  // If this is a data member we're done
  if ( datamember2_ ) return;

  // Check if this is a CMSSW "smart" pointer, which are annoying: we
  // first need to call isAvailable() to make sure that the collection 
  // containing the object is available. If it is, we then call isNull() to 
  // check for a null pointer.
  //
  // Then we need to call get() to return the address of the object
  // pointer to by the smart pointer. If the address is not null,
  // we can then call the method of the returned object. 
  
  isavailablemethod_ = kit::getisAvailable(method1_);
  isnullmethod_ = kit::getisNull(method1_);
  smartpointer_ = kit::memberValid(isnullmethod_);

    if ( smartpointer_ )
    { 
      if ( debug_ > 0 )
        cout << "FunctionMember - " << method1_.Name() 
             << " returns a smart pointer" 
             << endl << "\t of type: " << classname2_ << endl;
      // get its "get" method
      vector<kit::ValueThing*> vals;
      std::vector<void*> args;
      getmethod_ = kit::decodeMethod(classname2_, "get()", vals, args);
    }
}

FunctionMember::~FunctionMember() 
{ 
  for(unsigned i=0; i < values1_.size(); i++) delete values1_[i];
  for(unsigned i=0; i < values2_.size(); i++) delete values2_[i];
}

void*
FunctionMember::invoke(void* address)
{
  if ( debug_ > 0 ) cout << endl;

  // This flag will be set to true by the invokeMethod routine if the
  // returned entity is an object returned by value. invokeMethod
  // allocates space for such an object and informs its caller that
  // it has done so by setting deallocate = true. If deallocate is true,
  // then the allocated memory must be freed before we exit this routine.

  bool deallocate = false;

  // Call method 1
  // method1_      object that models method 1 or data member 1
  // address       address of object whose method/data member is being called
  // args_         the arguments of the method to be called
  // deallocate    true is memory has been allocated, which therefore
  //               needs to be deallocated

  if ( debug_ > 0 )
    cout << "FunctionMember::invoke - execute: " << methodrecord_ << endl;

  void* raddr = 0;
  if ( datamember1_ )
    raddr = kit::datamemberValue(classname1_, address, methodrecord1_);
  else
    raddr = kit::invokeMethod(method1_, address, args1_, deallocate);

  // Remember address "raddr" of returned object since we may have to
  // deallocate space reserved for it by invokeMethod

  void* address1 = raddr;

  // simple methods return simple types whereas compound methods return
  // objects.
  if ( simplemethod_ )
    {
      if ( debug_ > 0 )
        cout << "FunctionMember::invoke - simple: " << methodrecord_ << endl;

      // simple method with a (presumed) simple return type, or a 
      // simple data member. Address cannot be zero
      if ( raddr == 0 )
        {
          // shouldn't happen!
          throw cms::Exception("NullReturn") 
            << " from method " << methodrecord_ << endl;
        }
    }
  else
    {
      // This is a compound method, so it necessarily returns an object

      if ( debug_ > 0 )
        cout << "FunctionMember::invoke - compound: " 
             << methodrecord_ << endl;

      // check if this is a smart pointer because, if it is, we have
      // to do some more processing...sigh!

      bool available = true;

      if ( smartpointer_ )
        {
          if ( debug_ > 0 )
            cout << "FunctionMember::invoke - smart pointer " << endl;

          // this is a smart pointer, so call its isAvailable() method and
          // if it returns true, call its isNull() method
          // and check whether it returns true or false.
          void* addr = kit::invokeMethod(isavailablemethod_, raddr);
          available = *static_cast<bool*>(addr);
          if ( available )
            {
              // The collection is available, so check the smart pointer's
              // isNull() method
              addr = kit::invokeMethod(isnullmethod_, raddr);
              bool null = *static_cast<bool*>(addr);
              if ( null )
                {
                  // The object pointed to presumably does not exist
                  // so return a null pointer
                  raddr = 0;
                }
              else
                {
                  // we have a valid pointer to an object, so call the 
                  // smart pointer's get() method to  get the address of 
                  // object it points to ...
                  raddr = kit::invokeMethod(getmethod_, raddr);
                }
            }
          else
            {
              // The collection is not available, so return a null pointer
              raddr = 0;
            }
        }

      // if the pointer is valid, call second method on object returned 
      // by first method

      if ( raddr == 0 )
        {
          if ( ! smartpointer_ )
            edm::LogWarning("NullPointer") 
              << "\t" << classname1_ << "::" << methodrecord_;

          else if ( ! available )
            edm::LogWarning("CollectionNotFound") 
              << "\t" << classname1_ << "::" << methodrecord_;

          else
            edm::LogWarning("NullSmartPointer") 
              << "\t" << classname1_ << "::" << methodrecord_;
        }
      else
        {
          if ( datamember2_ )
            raddr = kit::datamemberValue(classname2_, raddr, methodrecord2_);
          else
            raddr = kit::invokeMethod(method2_, raddr, args2_);
        }

      // Check whether we need to deallocate memory
      if ( deallocate ) 
        {
          kit::deallocateMemory(method1_, address1);
        }
    }

  return raddr;
}

double
FunctionMember::operator()(void* address)
{ 
  void* raddr = invoke(address);
  if ( raddr == 0 ) 
    return 0;
  else
    return *static_cast<double*>(raddr); 
}

std::string
FunctionMember::str()
{
  ostringstream os;

  if ( !kit::memberValid(method1_) ) return os.str();

  os << "DeclaringType1 : " << method1_.DeclaringType().Name() << endl;
  os << "   MethodName1 : " << method1_.Name() << endl;
  os << "     Signature1: " << method1_.TypeOf().Name(SCOPED) << endl;
  if ( values1_.size() > (unsigned)0 )
    {
      os << "     ArgValues: " << endl;
      for(unsigned i=0; i < values1_.size(); i++)
        {
          os << "         " 
             << i << " " << args1_[i]
             << ": " << values1_[i]->str()  << endl;
        }
    }
  if ( !kit::memberValid(method2_) ) return os.str();

  os << "DeclaringType2 : " << method2_.DeclaringType().Name() << endl;
  os << "   MethodName2 : " << method2_.Name() << endl;
  os << "     Signature2: " << method2_.TypeOf().Name(SCOPED) << endl;
  if ( values2_.size() > (unsigned)0 )
    {
      os << "     ArgValues: " << endl;
      for(unsigned i=0; i < values2_.size(); i++)
        {
          os << "         " 
             << i << " " << args2_[i]
             << ": " << values2_[i]->str()  << endl;
        }
    }
  return os.str();
}


std::ostream&
FunctionMember::operator<<(std::ostream& os)
{
  os << str() << endl;
  return os;
}


