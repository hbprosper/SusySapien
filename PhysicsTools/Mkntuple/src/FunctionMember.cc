// -*- C++ -*-
//
// Package:    PhysicsTools
// Class:      FunctionMember
// 
/**\class FunctionMember FunctionMember.cc 
   PhysicsTools/Mkntuple/src/FunctionMember.cc

   Description: model simple or compound methods

   Implementation:
   Common sense and a sense of beauty.
*/
//
// Original Author:  Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
// $Id: FunctionMember.cc,v 1.2 2010/09/19 14:09:53 prosper Exp $
//-----------------------------------------------------------------------------
#include <Python.h>
#include <boost/python.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <string>
#include <stdlib.h>
//-----------------------------------------------------------------------------
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "PhysicsTools/Mkntuple/interface/FunctionMember.h"
#include "PhysicsTools/Mkntuple/interface/colors.h"
//-----------------------------------------------------------------------------
using namespace std;
using namespace ROOT::Reflex;
//-----------------------------------------------------------------------------
static bool DBFunctionMember = 
  getenv("DBFunctionMember")>0 ? (bool)getenv("DBFunctionMember") : false; 
  
FunctionMember::FunctionMember()
  : classname_(""),
    expression_("")
{}


FunctionMember::~FunctionMember() 
{ 
  for(unsigned depth=0; depth < fd_.size(); ++depth)
    {
      FunctionDescriptor& fd = fd_[depth]; // NB: get an alias NOT a copy!

      if ( fd.deallocate )
        { 
          if ( DBFunctionMember )
            cout << "       - deallocate memory for: " 
                 << BLUE 
                 << fd.classname << "::" << fd.expression
                 << DEFAULT_COLOR
                 << endl;
          fd.returntype.Deallocate(fd.memory);
        }
      for(unsigned j=0; j < fd.values.size(); ++j) delete fd.values[j];
    }
}

// Model an instantiable method using the Reflex tools

FunctionMember::FunctionMember(std::string classname, 
                               std::string expression)
  : classname_(classname),
    expression_(expression)
{
  if ( DBFunctionMember )
    cout << endl 
         << "FunctionMember - classname:  " << classname_  << endl
         << "                 expression: " << expression_ << endl;

  if ( classname_ == "" ) 
    throw cms::Exception("InvalidClassname") << "null classname" << endl;

  if ( expression_ == "" ) 
    throw cms::Exception("InvalidExpression") << "null expression" << endl;

  //--------------------------------------------------------------------------
  // Split method into its parts. Surely, even for CMS, a maximum 
  // indirection depth of 5 is sufficient!
  //--------------------------------------------------------------------------
  int maxDepth = 5;

  //--------------------------------------------------------------------------
  // Now that we've split the thing up, complete the analysis of the method
  // Note:
  // classname  - type of parent object
  // expression - method/member of parent object to be invoked
  // retname    - type of returned object or data member
  // fd         - a vector of function descriptors
  //--------------------------------------------------------------------------

  string retname("");
  for(int depth=0; depth < maxDepth; ++depth)
    {
      if ( classname == "" )
        throw cms::Exception("InvalidClassname") << "null classname" << endl;

      if ( expression == "" )
        throw cms::Exception("InvalidExpression") << "null expression" << endl;

     if ( DBFunctionMember )
        cout << BLUE
             << "       - allocate function descriptor " << depth 
             << DEFAULT_COLOR << endl; 

      // Allocate a descriptor. We use the descriptor to cache everything 
      // that is needed to render the calling of the method as efficient 
      // as possible.

      fd_.push_back( FunctionDescriptor() );
      FunctionDescriptor& fd = fd_[depth]; // NB: get an alias NOT a copy!

      fd.classname   = classname;
      fd.expression  = expression;      
      fd.datamember  = false;
      fd.simple      = false;
      fd.pointer     = false;
      fd.reference   = false;
      fd.smartpointer= false;
      fd.deallocate  = false;
      fd.memory      = (void*)(&fd.scratch);
      
      // If method is compound, split in two and reset "expression"

      string delim("");
      string expr2("");
      if ( rfx::isCompoundMethod(expression, delim) ) 
        rfx::bisplit(expression, fd.expression, expr2, delim);

      if ( DBFunctionMember )
        cout << "       - method/datamember " << ": " 
             << fd.classname << "::" 
             << BLUE << fd.expression << DEFAULT_COLOR << endl
             << "       - join (" << delim << ")"<< endl;

      // Determine whether this is a function or data member
  
      boost::regex dregex("^[a-zA-Z_]+[a-zA-Z0-9_:]*[(]");
      boost::smatch dmatch;
      if ( ! boost::regex_search(fd.expression, dmatch, dregex) )
        {
          //---------------------------------
          // This seems to be a data member
          //---------------------------------
          if ( DBFunctionMember )
            cout << "       - is " << RED << "data member" 
                 << DEFAULT_COLOR << endl;

          fd.datamember = true;

          // Get the ... well it's obvious isn't it?

          fd.method = rfx::getDataMember(fd.classname, fd.expression);

          // Fall on sword if we did not find a valid data member

          if ( !rfx::memberValid(fd.method) )
            throw cms::Exception("decodeDataMemberFailure")
              << " can't decode data member: " 
              << fd.classname << "::" 
              << fd.expression << endl;

          // We have a valid data member. Get type
          // Note: simple types can be returned 
          // by value, pointer or reference.

          fd.returntype = fd.method.TypeOf();
          if ( fd.returntype.IsFundamental() )
            fd.simple = true;

          if ( fd.returntype.IsPointer() )
            fd.pointer = true;
          else if ( fd.returntype.IsReference() )
            fd.reference = true;

          // Get name of return type

          retname = fd.returntype.Name(SCOPED+FINAL);
          if ( fd.pointer || fd.reference )
            retname = retname.substr(0,retname.size()-1);

          // Fall on sword if we cannot get data member type name
          if ( retname == "" )
            throw cms::Exception("datamemberTypeFailure")
              << " can't get type for data member "
              << fd.classname << "::"
              << fd.expression << endl;

          if ( DBFunctionMember )
            cout << "       - data member type: " 
                 << retname << endl;
        }
      else
        {
          //---------------------------------          
          // This seems to be a method
          //---------------------------------
          if ( DBFunctionMember )
            cout << "       - is " << RED << "method " 
                 << DEFAULT_COLOR << endl;
          
          fd.datamember = false;

          // Decode method and return a Reflex model of it

          fd.method = rfx::decodeMethod(fd.classname, 
                                        fd.expression, 
                                        fd.values, 
                                        fd.args);

          // Fall on sword if we did not find a valid method

          if ( !rfx::memberValid(fd.method) )
            throw cms::Exception("decodeMethodFailure")
              << " can't decode method: " 
              << fd.classname << "::" 
              << fd.expression << endl;

          // We have a valid method so get a model of its return type
          // Note: simple types can be returned 
          // by value, pointer or reference.

          int code=0;
          fd.returntype = rfx::returnType(fd.method, code);

          if ( fd.returntype.IsFundamental() )
            fd.simple = true;

          if ( code == 2 )
            fd.pointer = true;
          else if ( code == 3 )
            fd.reference = true;

          // Get type name of returned object

          retname = fd.returntype.Name(SCOPED+FINAL);
          if ( retname == "" )
            throw cms::Exception("returnTypeFailure")
              << " can't get return type for method " 
              << fd.method.Name() << endl;

          if ( DBFunctionMember )
            cout << "       - return type: " << retname << endl;

  
          // Check for non-simple return types

          if ( ! fd.simple )
            {
              //---------------------------------          
              // We have a non-simple return type
              //---------------------------------          
              // Check if this is a CMSSW "smart" pointer. 
              // If it is then (alas) we have a lot of work to do:
              //
              // 1. We first need to call isAvailable() to make sure that 
              //    the collection containing the object is available. 
              //
              // 2. If the collection is available, we call isNull() 
              //    to check for a null pointer.
              //
              // 3. If the collection is available and the the pointer is not
              //    null, we need to call get() to return the address of 
              //    the object referenced by the smart pointer.
              //
              // 4. Then we call the method of the returned object. 
              //
              // A truly smart pointer would handle 1 to 3 automatically
              // and have the get() method return 0 if no object exists.
              
              fd.isNull = rfx::getisNull(fd.method);
              if ( rfx::memberValid(fd.isNull) )
                {
                  fd.smartpointer = true;
                  fd.isAvailable = rfx::getisAvailable(fd.method);
                  
                  if ( !rfx::memberValid(fd.isAvailable) )
                    throw cms::Exception("decodeMethodFailure")
                      << " can't find method: " 
                      << fd.classname << "::isAvailable()" << endl;

                  if ( DBFunctionMember )
                    cout << "FunctionMember - " << fd.method.Name() 
                         << " returns a smart pointer" 
                         << endl << "\t of type: " << retname << endl;
                  
                  // get its "get" method
                  vector<rfx::ValueThing*> vals;
                  std::vector<void*> args;
                  fd.get = rfx::decodeMethod(retname, "get()", vals, args);
                  
                  if ( !rfx::memberValid(fd.get) )
                    throw cms::Exception("decodeMethodFailure")
                      << " can't find method: " 
                      << fd.classname << "::get()" << endl;
                }
            }
        }

      if ( DBFunctionMember )
        {
          cout << "       - is " << GREEN;
          if ( fd.simple )
            cout << "SIMPLE "; 
          
          if ( fd.pointer )
            cout << "POINTER"; 
          else if ( fd.reference )
            cout << "REFERENCE";
          cout << DEFAULT_COLOR << endl;                    
        }
    
      // Allocate memory for objects returned by value

      if ( !fd.simple )
        {
          if ( !fd.pointer )
            {
              if ( !fd.reference )
                {
                  if ( DBFunctionMember )
                    cout << "       - allocate memory for: " 
                         << BLUE 
                         << fd.classname << "::" << fd.expression 
                         << DEFAULT_COLOR 
                         << endl;
                  fd.memory = fd.returntype.Allocate();
                  fd.deallocate = true;
                }
            }
        }

      // If the return type is simple, then we need to break out of this
      // loop because the analysis of the method is complete.    

      if ( fd.simple ) break;

      // The return type is not simple, therefore, we need to invoke
      // recursion: the 2nd part of the compound method becomes
      // the expression on the next round and the return type becomes the
      // next classname 
      expression = expr2;
      classname  = retname;
    }
}

void*
FunctionMember::invoke(void* address)
{

  void* raddr = 0;

  for(unsigned int depth=0; depth < fd_.size(); ++depth)
    {
      FunctionDescriptor& fd = fd_[depth]; // NB: get an alias NOT a copy!

      // The deallocate flag will be set to true by the invokeMethod routine 
      // if the returned entity is an object returned by value. invokeMethod
      // allocates space for such an object and informs its caller that
      // it has done so by setting deallocate = true. If deallocate is true,
      // then the allocated memory must be freed before we exit this routine.


      // classname   the parent class to which method/data member belongs
      // address     address of object whose method/data member is being called
      // method      object that models a method or a data member
      // args_       the arguments of the method to be called
      // deallocate  true if memory has been allocated.

      if ( DBFunctionMember )
        cout << depth << "\tFunctionMember::invoke - " 
             << fd.classname << "::" << fd.method.Name() 
             << " address: " << address << endl;

      if ( fd.datamember )
        {
          if ( DBFunctionMember )
            cout << "\t\tFunctionMember::invoke - DATA MEMBER" << endl; 

          raddr = rfx::datamemberValue(fd.classname, address, fd.expression);
        }
      else
        {
          if ( DBFunctionMember )
            cout << "\t\tFunctionMember::invoke - FUNCTION MEMBER" << endl;

          raddr = rfx::invokeMethod(fd.method, address, fd.memory, fd.args);
        }
      
      // At some point, the return type should become simple

      if ( fd.simple )
        {
          //---------------------------------------
          // Simple return type
          //---------------------------------------
          if ( DBFunctionMember )
            {
              cout << "\t\t\tRETURN TYPE: " << RED << "SIMPLE"
                   << DEFAULT_COLOR
                   << endl;

              cout << "\t\t\tVALUE:       "
                   << RED
                   << *static_cast<double*>(raddr)
                   << DEFAULT_COLOR
                   << "\t" << raddr << endl;
            }

          // If return type is simple and it is not a pointer
          // its address cannot be null
          if ( ! fd.pointer )
            {
              if ( raddr == 0 )
                {
                  // shouldn't happen!
                  throw cms::Exception("NullReturn") 
                    << " simple return type has a null address: " 
                    << fd.classname << "::" << fd.expression << endl;
                }
            }
        }
      else
        {
          //---------------------------------------
          // Non-simple return type
          //---------------------------------------
 
          if ( DBFunctionMember )
            cout << "\t\t\tRETURN TYPE: " << RED << "OBJECT"
                 << DEFAULT_COLOR
                 << endl;

          // Check if this is a smart pointer because, if it is, we have
          // to do a lot of work. A truly smart pointer would do this work
          // for you.

          bool available = true;

          if ( fd.smartpointer )
            {
              if ( DBFunctionMember )
                cout << "FunctionMember::invoke - smart pointer " << endl;

              // This is a smart pointer, so call its isAvailable() method and
              // if it returns true, call its isNull() method
              // and check whether it returns true or false.

              void* addr = rfx::invokeMethod(fd.isAvailable, raddr,
                                             fd.memory);
              available = *static_cast<bool*>(addr);
              if ( available )
                {
                  // The collection is available, so check the smart pointer's
                  // isNull() method
                  addr = rfx::invokeMethod(fd.isNull, raddr, fd.memory);
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
                      raddr = rfx::invokeMethod(fd.get, raddr, fd.memory);
                    }
                }
              else
                {
                  // The collection is not available, so return a null pointer
                  raddr = 0;
                }
            }

          // Check validity of address

          if ( raddr == 0 )
            {
              if ( fd.pointer )
                edm::LogWarning("NullPointer")
                  << "\t" << fd.classname << "::" << fd.expression << endl; 

              else if ( ! available )
                edm::LogWarning("CollectionNotFound")
                  << "\t" << fd.classname << "::" << fd.expression << endl;  
              
              else if ( fd.smartpointer )
                edm::LogWarning("NullSmartPointer")
                  << "\t" << fd.classname << "::" << fd.expression << endl;
              break;
            }
        }

      // Now do recursion: the address of data member or returned object
      // becomes the address of the object whose method is to be called
      // next
      address = raddr;
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

  for(unsigned int depth=0; depth < fd_.size(); ++depth)
    {
      FunctionDescriptor& fd = fd_[depth];
      os << "DeclaringType : " << fd.method.DeclaringType().Name() << endl;
      os << "   MethodName : " << fd.method.Name() << endl;
      os << "     Signature: " << fd.method.TypeOf().Name(SCOPED) << endl;
      if ( fd.values.size() > (unsigned)0 )
        {
          os << "     ArgValues: " << endl;
          for(unsigned i=0; i < fd.values.size(); i++)
            {
              os << "         " 
                 << i 
                 << "  " << fd.args[i]
                 << ": " << fd.values[i]->str()  << endl;
            }
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


