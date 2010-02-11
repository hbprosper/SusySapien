#ifndef BUFFER_H
#define BUFFER_H
//
// Package:    PhysicsTools/Mkntuple
//             Buffer.h
//
// Original Author:  Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
//         Updated:  Sat Jan 16 HBP add error handling in fill method
//                   Sun Jan 17 HBP add even more error handling in fill
//
//         This code used to look simple, but with all the error handling a
//         silk purse has been turned into a sow's ear!
//
//         Updated:  Wed Feb 10 HBP add UserBuffer, which allows for the
//                   insertion of user-defined variables into the n-tuple.
//
// $Id: Buffer.h,v 1.4 2010/02/08 03:16:19 prosper Exp $
//
//
// If using Python, include its header first to avoid annoying compiler
// complaints.
#include <Python.h>
#include <boost/python/type_id.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "PhysicsTools/LiteAnalysis/interface/treestream.hpp"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
#include "PhysicsTools/LiteAnalysis/interface/Method.h"

// ---------------------------------------------------------------------
// We need a few templates to make the code generic.
// ---------------------------------------------------------------------

// Model a variable as a thing with
// 1. a name
// 2. a value (a vector of doubles)
// 3. a function to access data from the associated RECO or PAT object

template <typename X>
struct Variable 
{
  Variable(std::string namen, int count, std::string f) 
    : name(namen),
      fname(f),
      value(std::vector<double>(count,0)),
      function(Method<X>(f))
  {}

  std::string         name;
  std::string         fname;
  std::vector<double> value;
  Method<X>           function;
};

// Function to initialize a Buffer

template <typename Y>
void initBuffer(otreestream& out,  
                std::string& label,
                std::string& label1,
                std::string& label2,
                std::string& prefix,
                std::vector<VariableDescriptor>& var,
                int&  maxcount,
                bool& singleton,
                int&  debug,
                int&  count,
                std::vector<Variable<Y> >& variable)
{
  // Define regular expressions to check for compound methods; i.e., methods
  // of the form y = a()->b()

  boost::regex stripargs("[(].*[)]");
  boost::regex stripptr("-[>]");

  // Split getByLabel into its component parts

  label1 = label;
  int i = label.find("_");
  if ( i > 0 )
    {
      label1 = label.substr(0,i);
      label2 = label.substr(i+1, label.size()-i-1);
      label  = label1 + ", " + label2;
    }

  // Just return if no variables were specified
  
  if ( var.size() == 0 ) 
    {
      std::cout 
        << "** Warning! Buffer::init - no variables defined for class " 
        << boost::python::type_id<Y>().name()
        << std::endl
        << "** and getByLabel \"" << label << "\""
        << std::endl;
      return;
    }
  
  // Define variables destined for the output tree
  
  std::cout << "   n-tuple variables:" << std::endl;
  
  // If we have an vector variable, create a counter variable for it.
  // Root calls this a "leaf counter"

  std::string counter("");
  if ( !singleton )
    {
      // Add leaf counter to tree
      counter = "n" + prefix;        
      out.add(counter, count);
      std::cout << "      counter: " << counter << std::endl;
    }
  
  // For every method, create the associated n-tuple variable name

  for(unsigned i=0; i < var.size(); i++)
    {    
      std::string method  = var[i].first;
      std::string varname = var[i].second;
      varname = boost::regex_replace(varname, stripargs, "");
      varname = boost::regex_replace(varname, stripptr,  ".");
      
      std::string name = prefix + "." + varname;
        
      if ( !singleton ) name += "[" + counter + "]";
      
      variable.push_back(Variable<Y>(name, 
                                     maxcount,
                                     method));
      std::cout << "   " << i << ":\t" << name 
                << std::endl
                << "\t\t" << GREEN << method << BLACK << std::endl;
    }
  
  // Add variables to output tree. This must be done after all
  // variables have been defined, because it is only then that their
  // addresses are guaranteed to be stable.
  
  for(unsigned i=0; i < var.size(); i++)
      out.add(variable[i].name, variable[i].value);
}


// Function to fill a Buffer

template <typename X, typename Y>
bool fillBuffer(const edm::Event& event,
                otreestream& out,  
                std::string& label,
                std::string& label1,
                std::string& label2,
                int&  maxcount,
                bool& singleton,
                int&  debug,
                int&  count,
                std::string& message,
                std::vector<Variable<Y> >& variable)
{
  if ( debug > 0 ) 
    std::cout << RED + "Begin Buffer::fill " + BLACK + 
      "objects of type: " 
              << RED 
              << boost::python::type_id<X>().name() << BLACK 
              << std::endl;
  
  count = 0; // reset count, just in case we have to bail out
  message = "";

  if ( singleton )
    {
      edm::Handle<X> handle;
      try
        {
          if ( label2 == "" )
            event.getByLabel(label1, handle);
          else
            event.getByLabel(label1, label2, handle);
        }
      catch (cms::Exception& e)
        {
          // Have a royal tantrum...
          std::ostringstream out;
          out << "getByLabel with label \"" << label << "\" failed on " 
              << boost::python::type_id<X>().name() << std::endl  
              << e.explainSelf(); 
          std::cout << out.str() << std::endl;
          message += out.str();
          return false;
        }
      
      if ( !handle.isValid() )
        // ...and another!
        throw edm::Exception(edm::errors::Configuration,
                             "\n" + RED + "singleton " + BLACK + 
                             "Buffer - "
                             "getByLabel failed on label \"" + 
                             label + "\"\n" + RED +
                             "\tmay I humbly suggest you "
                             "go boil your head!\n"
                             + BLACK);

      // extract datum for each variable
        
      const X& object = *handle;

      for(unsigned i=0; i < variable.size(); i++)
        {
          try
            {
              variable[i].value[0] = variable[i].function(object);
            }
          catch (cms::Exception& e)
            {
              throw cms::Exception("\nBufferFillFailure",
                                   "failed on call to \"" + 
                                   RED + 
                                   variable[i].fname + "\"\n" +
                                   GREEN +
                                   "thou lump of foul deformity..." 
                                   + BLACK, e);
            }
          }
    }
  else
    {
      edm::Handle< edm::View<X> > handle;
      try 
        {
          if ( label2 == "" )
            event.getByLabel(label1, handle);
          else
            event.getByLabel(label1, label2, handle);
        }
      catch (cms::Exception& e)
        {
          std::ostringstream out;
          out << "getByLabel with label \"" << label << "\" failed on " 
              << boost::python::type_id<X>().name() << std::endl  
              << e.explainSelf(); 
          std::cout << out.str() << std::endl;
          message += out.str();
          return false;
        }

      if ( !handle.isValid() )
        throw edm::Exception(edm::errors::Configuration,
                             "\nBuffer - " + 
                             RED +
                             "getByLabel failed on label \"" + 
                             BLACK +
                             label + "\"\n" + RED +
                             "you're a waste of space..." + 
                             BLACK);

      // update data count. Use the smaller of count and maxcount.
      count = (int)handle->size() < maxcount ? handle->size() : maxcount;
      
      // extract datum for each variable
      
      for(int j=0; j < count; j++)
        {
          const X& object = (*handle)[j];
          
          for(unsigned i=0; i < variable.size(); i++)
            {
              if ( debug > 0 ) 
                std::cout << RED +"\t" << j << "\tcall: " + BLACK
                          << variable[i].fname << std::endl;
              try
                {
                  variable[i].value[j] = variable[i].function(object);
                }
              catch (cms::Exception& e)
                {
                  throw cms::Exception("BufferFillFailure",
                                       "failed on call to \"" + 
                                       RED + 
                                       variable[i].fname+"\"\n" +
                                       GREEN +
                                       "thou lump of foul deformity..." 
                                       + BLACK, e);
                }
            } 
        }
    }
  
  if ( debug > 0 ) std::cout << RED + "End Buffer::fill " + BLACK + 
    "objects of type: " 
                              << RED  
                              << boost::python::type_id<X>().name() 
                              << BLACK 
                              << std::endl; 
  return true;
}

// Model a buffer as a thing with
// 1. a maximum count
// 2. a count of the number of values per variable
// 3. a vector of variables, each with the same maxcount and count
//
// The name of the ith n-tuple variable is constructed as follows:
// name = prefix + "_" + var[i].second
//
// where var[i] is a pair of strings with
// var[i].first the name of the (simple) method to be called
// var[i].second is the name of the n-tuple variable
//
// We use a base class (BufferThing) to permit polymorphic behavior, that is,
// to allow generic calls to the buffer methods init(...) and fill(...) that
// operate on objects of differing type.
//
// typenames:
//   X = class of object to be extracted using getByLabel

template <typename X>
struct Buffer  : public BufferThing
{
  Buffer() 
    : out_(0),
      label_(""),
      label1_(""),
      label2_(""),
      prefix_(""),
      var_(std::vector<VariableDescriptor>()),
      maxcount_(0),
      singleton_(false)
  {
    std::cout << "Buffer created for objects of type: " 
              << GREEN  
              << name() << BLACK 
              << std::endl;
  }

  virtual ~Buffer() {}

  void
  init(otreestream& out,
       std::string  label, 
       std::string  prefix,
       std::vector<VariableDescriptor>& var,
       int maxcount,
       int debug=0)
  {
    out_    = &out;
    label_  = label;
    prefix_ = prefix;
    var_    = var;
    maxcount_  = maxcount;
    singleton_ = maxcount == 1;
    debug_  = debug;

    initBuffer<X>(out,
                  label_,
                  label1_,
                  label2_,
                  prefix_,
                  var_,
                  maxcount_,
                  singleton_,
                  debug_,
                  count_,
                  variable_);
  }
  
  // -----------------------------------------------------
  // fill output tree
  // -----------------------------------------------------
  bool fill(const edm::Event& event)
  {
    return fillBuffer<X, X>(event,
                            *out_,
                            label_,
                            label1_,
                            label2_,
                            maxcount_,
                            singleton_,
                            debug_,
                            count_,
                            message_,
                            variable_);
  }
  
  std::string& message() { return message_; }

  std::string name() { return boost::python::type_id<X>().name(); }

private:
  otreestream* out_;  
  std::string  label_;
  std::string  label1_;
  std::string  label2_;
  std::string  prefix_;
  std::vector<VariableDescriptor> var_;
  int  maxcount_;
  bool singleton_;
  int  debug_;
  int  count_;
  std::string message_;
  std::vector<Variable<X> > variable_;
};


// Model a slightly more general buffer in which the object
// extracted from the event using getByLabel differs from the
// object whose methods are exported as n-tuple variables. UserBuffer
// is useful when one wants to add variables to an n-tuple that are
// functions of the methods of getByLabel extracted object.
//
// typenames:
//   X = class of object extracted using getByLabel
//   Y = class of object exporting methods

template <typename X, typename Y>
struct UserBuffer  : public BufferThing
{
  UserBuffer() 
    : out_(0),
      label_(""),
      label1_(""),
      label2_(""),
      prefix_(""),
      var_(std::vector<VariableDescriptor>()),
      maxcount_(0),
      singleton_(false)
  {
    std::cout << "UserBuffer created for objects of type: " 
              << GREEN  
              << name() << BLACK 
              << std::endl;
  }

  virtual ~UserBuffer() {}

  void
  init(otreestream& out,
       std::string  label, 
       std::string  prefix,
       std::vector<VariableDescriptor>& var,
       int maxcount,
       int debug=0)
  {
    out_    = &out;
    label_  = label;
    prefix_ = prefix;
    var_    = var;
    maxcount_  = maxcount;
    singleton_ = maxcount == 1;
    debug_  = debug;

    initBuffer<Y>(out,
                  label_,
                  label1_,
                  label2_,
                  prefix_,
                  var_,
                  maxcount_,
                  singleton_,
                  debug_,
                  count_,
                  variable_);
  }
  
  // -----------------------------------------------------
  // fill output tree
  // -----------------------------------------------------
  bool fill(const edm::Event& event)
  {
    return fillBuffer<X, Y>(event,
                            *out_,
                            label_,
                            label1_,
                            label2_,
                            maxcount_,
                            singleton_,
                            debug_,
                            count_,
                            message_,
                            variable_);
  }
  
  std::string& message() { return message_; }

  std::string name() { return boost::python::type_id<X>().name(); }

private:
  otreestream* out_;  
  std::string  label_;
  std::string  label1_;
  std::string  label2_;
  std::string  prefix_;
  std::vector<VariableDescriptor> var_;
  int  maxcount_;
  bool singleton_;
  int  debug_;
  int  count_;
  std::string message_;
  std::vector<Variable<Y> > variable_;
};

#endif
