//
// Package:    Mkntuple
// 
//
// Original Author:  Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
// $Id$
//
//
// Include its header first to avoid annoying compiler complaints.
#include <Python.h>
#include <boost/python/type_id.hpp>
#include <iostream>
#include <cassert>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "PhysicsTools/Utilities/src/ExpressionPtr.h"
#include "PhysicsTools/Utilities/src/ExpressionBase.h"
#include "PhysicsTools/Utilities/interface/expressionParser.h"

using namespace std;

const string BLACK("\x1b[0;30;48m"); // Ctrl[attribute;foreground;backgroundm
const string RED  ("\x1b[0;31;48m"); // Ctrl[attribute;foreground;backgroundm
const string GREEN("\x1b[0;32;48m"); // \x1b 0,1,...   30+color   40+color

// ---------------------------------------------------------------------
// We need a few templates to make the code generic.
// ---------------------------------------------------------------------

// Model a function that can be instantiated.
// This class is basically a copy of StringObjectFunction by Luca Lista. 
// It models functions of the form
//
//  <simple-return-type> function(<simple-type>)
//
// where <simple-return-type> is a simple type like float and <simple-type> 
// is typically void.

template <typename T>
struct SimpleFunction 
{
  SimpleFunction() {}
  
  SimpleFunction(const std::string& expr) 
    : type_(ROOT::Reflex::Type::ByTypeInfo(typeid(T))) 
  {
    if(!reco::parser::expressionParser<T>(expr, expr_)) 
      {
        throw edm::Exception(edm::errors::Configuration,
                             "Since I'm a cyber ignoramous, "
                             "I'm too stupid to understand \"" 
                             + RED + expr + BLACK + "\"\n");
      }
  }
  
  double operator()(const T & t) const 
  {
    using namespace ROOT::Reflex;
    Object o(type_, const_cast<T *>(& t));
    return expr_->value(o);
  }
  
private:
  reco::parser::ExpressionPtr expr_;
  ROOT::Reflex::Type type_;
};

// Model a variable as a thing with
// 1. a name
// 2. a value (a vector of doubles)
// 3. a function to access data from the associated RECO or PAT object

template <typename X>
struct Variable 
{
  Variable(std::string namen, int count, std::string f) 
    : name(namen),
      value(std::vector<double>(count,0)),
      function(SimpleFunction<X>(f))
  {}

  std::string name;
  std::vector<double> value;
  SimpleFunction<X>   function;
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
// We use a base class (BufferThing) to permit polymorthic behavior, that is,
// to allow generic calls to the buffer methods init(...) and fill(...) that
// operate on objects of differing type.
//
// typenames:
//   X = class of object to be extracted using getByLabel
//   Y = template vale of objects of class Variable<Y>. Y can be the same
//   as X.

template <typename X, typename Y>
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
       std::string label, 
       std::string prefix,
       std::vector<VariableDescriptor>& var,
       int maxcount)
  {
    out_    = &out;
    label_  = label;
    label1_ = label;
    int i = label.find("_");
    if ( i > 0 )
      {
        label1_ = label.substr(0,i);
        label2_= label.substr(i+1, label.size()-i-1);
      }
    prefix_ = prefix;
    var_    = var;
    maxcount_ = maxcount;
    singleton_ = maxcount == 1;

    // Define variables destined for the output tree

    cout << "   n-tuple variables:" << endl;

    std::string counter("");
    if ( !singleton_ )
      {
        // Add leaf counter to tree
        counter = "n" + prefix_;        
        out_->add(counter, count_);
        cout << "      counter: " << counter << endl;
      }

    for(unsigned i=0; i < var_.size(); i++)
      {    
        string method = var_[i].first;
        string varname= var_[i].second;
        std::string name = prefix_ + "." + varname;
        
        if ( !singleton_ ) name += "[" + counter + "]";
        
        variable_.push_back(Variable<Y>(name, 
                                        maxcount_,
                                        method));
        cout << "   " << i << ":\t" << name 
             << endl
             << "\t\t" << GREEN << method << BLACK << endl;
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
  void fill(const edm::Event& event)
  {
    if ( singleton_ )
      {
        edm::Handle<X> handle;
        if ( label2_ == "" )
          event.getByLabel(label1_, handle);
        else
          event.getByLabel(label1_, label2_, handle);

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
        for(unsigned i=0; i < variable_.size(); i++)
          {
            const Y object((*handle));
            variable_[i].value[0] = variable_[i].function(object);
          }
      }
    else
      {
        edm::Handle< edm::View<X> > handle;
        if ( label2_ == "" ) 
          event.getByLabel(label1_, handle);
        else
          event.getByLabel(label1_, label2_, handle);

        if ( !handle.isValid() )
          throw edm::Exception(edm::errors::Configuration,
                               "\nBuffer - "
                               "getByLabel failed on label \"" + 
                               label_ + "\"\n" + RED +
                               "thou art a waste of space...\n" + BLACK);
        
        // update data count. Use the smaller of count and maxcount.
        count_ = (int)handle->size() < maxcount_ ? handle->size() : maxcount_;

        // extract datum for each variable
        for(unsigned i=0; i < variable_.size(); i++)
          {
            for(int j=0; j < count_; j++)
              {
                const Y object((*handle)[j]);
                variable_[i].value[j] = variable_[i].function(object);
              } 
          }
      }
  }
  
private:
  otreestream* out_;  
  std::string label_;
  std::string label1_;
  std::string label2_;
  std::string prefix_;
  std::vector<VariableDescriptor> var_;
  int maxcount_;
  bool singleton_;
  int count_;
  std::vector<Variable<Y> > variable_;
};


// Define all plugins

#include "PhysicsTools/Mkntuple/plugins/plugins.icc"

