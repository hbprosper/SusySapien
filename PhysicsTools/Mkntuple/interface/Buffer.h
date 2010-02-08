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
// $Id: Buffer.h,v 1.3 2010/01/20 06:11:34 prosper Exp $
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
              << boost::python::type_id<X>().name() << BLACK 
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

    boost::regex stripargs("[(].*[)]");
    boost::regex stripptr("-[>]");

    // Store variables needed for later

    out_    = &out;
    label_  = label;
    label1_ = label;
    int i = label.find("_");
    if ( i > 0 )
      {
        label1_ = label.substr(0,i);
        label2_ = label.substr(i+1, label.size()-i-1);
        label_  = label1_ + ", " + label2_;
      }
    prefix_ = prefix;
    var_    = var;
    maxcount_ = maxcount;
    singleton_ = maxcount == 1;
    debug_ = debug;

    // Just return if no variables were specified

    if ( var_.size() == 0 ) 
      {
        std::cout 
          << "** Warning! Buffer::init - no variables defined for class " 
          << boost::python::type_id<X>().name()
          << std::endl
          << "** and getByLabel \"" << label_ << "\""
          << std::endl;
        return;
      }
    
    // Define variables destined for the output tree

    std::cout << "   n-tuple variables:" << std::endl;

    std::string counter("");
    if ( !singleton_ )
      {
        // Add leaf counter to tree
        counter = "n" + prefix_;        
        out_->add(counter, count_);
        std::cout << "      counter: " << counter << std::endl;
      }

    for(unsigned i=0; i < var_.size(); i++)
      {    
        std::string method  = var_[i].first;
        std::string varname = var_[i].second;
        varname = boost::regex_replace(varname, stripargs, "");
        varname = boost::regex_replace(varname, stripptr,  ".");
                             
        std::string name = prefix_ + "." + varname;
        
        if ( !singleton_ ) name += "[" + counter + "]";
        
        variable_.push_back(Variable<X>(name, 
                                        maxcount_,
                                        method));
        std::cout << "   " << i << ":\t" << name 
                  << std::endl
                  << "\t\t" << GREEN << method << BLACK << std::endl;
      }
 
    // Add variables to output tree. This must be done after all
    // variables have been defined, because it is only then that their
    // addresses are guaranteed to be stable.

    for(unsigned i=0; i < var_.size(); i++)
      out_->add(variable_[i].name, variable_[i].value);
  }
  
  // -----------------------------------------------------
  // fill output tree
  // -----------------------------------------------------
  bool fill(const edm::Event& event)
  {
    if ( debug_ > 0 ) std::cout << RED + "Begin Buffer::fill " + BLACK + 
                        "objects of type: " 
                                << RED 
                                << boost::python::type_id<X>().name() << BLACK 
                                << std::endl;

    count_ = 0; // reset count, just in case we have to bail out
    message_ = "";

    if ( singleton_ )
      {
        edm::Handle<X> handle;
        try
          {
            if ( label2_ == "" )
              event.getByLabel(label1_, handle);
            else
              event.getByLabel(label1_, label2_, handle);
          }
        catch (cms::Exception& e)
          {
            std::ostringstream out;
            out << "getByLabel with label \"" << label_ << "\" failed on " 
                << boost::python::type_id<X>().name() << std::endl  
                << e.explainSelf(); 
            std::cout << out.str() << std::endl;
            message_ += out.str();
            return false;
          }

        if ( !handle.isValid() )
          throw edm::Exception(edm::errors::Configuration,
                               "\n" + RED + "singleton " + BLACK + 
                               "Buffer - "
                               "getByLabel failed on label \"" + 
                               label_ + "\"\n" + RED +
                               "\tmay I humbly suggest you "
                               "go boil your head!\n"
                               + BLACK);

        // extract datum for each variable
        
        const X& object = *handle;

        for(unsigned i=0; i < variable_.size(); i++)
          {
            try
              {
                variable_[i].value[0] = variable_[i].function(object);
              }
            catch (cms::Exception& e)
              {
                throw cms::Exception("\nBufferFillFailure",
                                     "failed on call to \"" + 
                                     RED + 
                                     variable_[i].fname + "\"\n" +
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
            if ( label2_ == "" )
              event.getByLabel(label1_, handle);
            else
              event.getByLabel(label1_, label2_, handle);
          }
        catch (cms::Exception& e)
          {
            std::ostringstream out;
            out << "getByLabel with label \"" << label_ << "\" failed on " 
                << boost::python::type_id<X>().name() << std::endl  
                << e.explainSelf(); 
            std::cout << out.str() << std::endl;
            message_ += out.str();
            return false;
          }

        if ( !handle.isValid() )
          throw edm::Exception(edm::errors::Configuration,
                               "\nBuffer - " + 
                               RED +
                               "getByLabel failed on label \"" + 
                               BLACK +
                               label_ + "\"\n" + RED +
                               "you're a waste of space..." + 
                               BLACK);

        // update data count. Use the smaller of count and maxcount.
        count_ = (int)handle->size() < maxcount_ ? handle->size() : maxcount_;

        // extract datum for each variable

        for(int j=0; j < count_; j++)
          {
            const X& object = (*handle)[j];

            for(unsigned i=0; i < variable_.size(); i++)
              {
                if ( debug_ > 0 ) 
                  std::cout << RED +"\t" << j << "\tcall: " + BLACK
                            << variable_[i].fname << std::endl;
                try
                  {
                    variable_[i].value[j] 
                      = variable_[i].function(object);
                  }
                catch (cms::Exception& e)
                  {
                    throw cms::Exception("BufferFillFailure",
                                         "failed on call to \"" + 
                                         RED + 
                                         variable_[i].fname+"\"\n" +
                                         GREEN +
                                         "thou lump of foul deformity..." 
                                         + BLACK, e);
                  }
              } 
          }
      }

    if ( debug_ > 0 ) std::cout << RED + "End Buffer::fill " + BLACK + 
                        "objects of type: " 
                                << RED  
                                << boost::python::type_id<X>().name() 
                                << BLACK 
                                << std::endl;

    return true;
  }
  
  std::string& message()
  {
    return message_;
  }

  std::string name() { return boost::python::type_id<X>().name(); }

private:
  otreestream* out_;  
  std::string label_;
  std::string label1_;
  std::string label2_;
  std::string prefix_;
  std::vector<VariableDescriptor> var_;
  int maxcount_;
  bool singleton_;
  int debug_;
  int count_;
  std::string message_;
  std::vector<Variable<X> > variable_;
};

#endif