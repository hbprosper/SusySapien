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
//                   Sat Mar 06 HBP - write out variables to be used by
//                                    mkntanalyzer.py
//                   Wed Aug 25 HBP - merged UserBuffer into Buffer and
//                              added BufferAddon, BufferHelper
//                   Fri Aug 27 HBP - on second thoughts...go back to a
//                                    UserBuffer class!
//
// $Id: Buffer.h,v 1.18 2010/09/06 20:15:15 prosper Exp $
//
//
// If using Python, include its header first to avoid annoying compiler
// complaints.
#include <Python.h>
#include <boost/python/type_id.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cassert>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "PhysicsTools/LiteAnalysis/interface/treestream.hpp"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
#include "PhysicsTools/LiteAnalysis/interface/MethodT.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
// ---------------------------------------------------------------------
// We need a few templates to make the code generic.
// ---------------------------------------------------------------------

///
enum BufferType
  {
    EVENT,            // Buffer for event
    RUNINFO,          // Buffer for RunInfo object
    HELPER,           // Buffer that performs complicated accesses
    DEFAULT
  };

/** Model a variable.
    A (CMS) variable is a thing with a<br>
    1 - name
    2 - value (a vector of doubles)
    3 - a function to access data from the associated RECO or PAT object
*/
template <typename X>
struct Variable 
{
  /** Construct a variable.
      @param namen - name of variable
      @param count - maximum number of values/variable
      @param f - name of accessor function
   */
  Variable(std::string namen, int count, std::string f) 
    : name(namen),
      fname(f),
      value(std::vector<double>(count,0)),
      function(MethodT<X>(f))
  {}

  std::string         name;
  std::string         fname;
  std::vector<double> value;
  MethodT<X>          function;
};

/// Function to initialize a Buffer.
template <typename Y>
void initBuffer(otreestream& out,  
                std::string& label,
                std::string& label1,
                std::string& label2,
                std::string& prefix,
                std::vector<VariableDescriptor>& var,
                std::vector<Variable<Y> >& variables,
                int&  count,
                bool  singleton,
                int   maxcount,
                std::ofstream& log,
                int   debug)

{
  // Define regular expressions to check for compound methods; i.e., methods
  // of the form y = a()->b() or a().b(), or a().b

  boost::regex stripargs("[(].*?[)]");
  boost::regex stripme("-[>]|[.]|\"|[(]|[)]");
  boost::regex stripcolon("[a-zA-Z]+::");
  boost::regex strip3_("___");
  boost::regex strip2_("__");
  boost::regex strip2_atend("_$");

  // Type name of object that exports methods, that is, whose methods
  // return values that can be stored in the n-tuple

  std::string classname = boost::python::type_id<Y>().name();

   if ( debug > 0 )
    {
      if ( singleton )
        std::cout << RED  << "\tSINGLETON( " << classname << " )" 
                  << BLACK << std::endl;
      else
        std::cout << BLUE << "\tCOLLECTION( " << classname << " )" 
                  << BLACK << std::endl;
    }

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
        << classname
        << std::endl
        << "** and getByLabel \"" << label << "\""
        << std::endl;
      return;
    }
  
  // create name of object

  std::string objectname = classname;
  TString objname = TString(boost::regex_replace(objectname, 
                                                 stripcolon, "").c_str());
  objname.ToLower();
  objectname = std::string(objname.Data());

  // Define variables destined for the output tree
  
  std::cout << "   n-tuple variables:" << std::endl;
  
  // Root is able to store vector<..> types. However, since we want the 
  // resulting n-tuple to be as simple as possible, we'll handle the mapping 
  // to and from vectors ourselves.

  // If we have a vector variable, create a counter variable for it.
  // Root calls this a "leaf counter". 
  //
  // There is one exception however: if a helper class for a collection object,
  // say a collection of pat::Jets, maps these objects to a single instance of
  // each variable, for example to HT, we shall assume that the n-tuple 
  // variable is to be a simple non-vector type.

  std::string counter("");
  if ( !singleton && count > 1 )
    {
      // Add leaf counter to tree
      counter = "n" + prefix;        
      out.add(counter, count);
      std::cout << "      counter: " << counter << std::endl;
      log << "int/" 
          << counter << "/"
          << "n"+objectname << "/"
          << 1 
          << std::endl;
    }
  
  // For every method, create the associated n-tuple variable name

  for(unsigned i=0; i < var.size(); i++)
    {    
      std::string rtype   = var[i].rtype;
      std::string method  = var[i].method;
      std::string varname = var[i].varname;
      if ( debug > 0 )
        std::cout << "Buffer -"
                  << RED
                  << " varname(" << varname << ")"
                  << BLACK
                  << std::endl;

      // Replace "->", ".", "(", ")" and quotes by "_"
      varname = boost::regex_replace(varname, stripme,  "_");

      // Replace "___" by "_"
      varname = boost::regex_replace(varname, strip3_,  "_");

      // Replace "__" by "_"
      varname = boost::regex_replace(varname, strip2_,  "_");

      // Strip away possible "_" at the end 
      varname = boost::regex_replace(varname, strip2_atend,  "");

      if ( debug > 0 )
        std::cout << "        "
                  << RED 
                  << " varname(" << varname << ")"
                  << BLACK
                  << std::endl;      

      std::string name = prefix + "." + varname;

      log << rtype << "/" 
          << name  << "/"
          << objectname + "_" + varname << "/"
          << maxcount 
          << std::endl;
        
      if ( !singleton && count > 1 ) name += "[" + counter + "]";
      
      variables.push_back(Variable<Y>(name, 
                                      maxcount,
                                      method));
      std::cout << "   " << i << ":\t" << name 
                << std::endl
                << "\t\t" << method << std::endl;
    }

  // Add variables to output tree. This must be done after all
  // variables have been defined, because it is only then that their
  // addresses are guaranteed to be stable.
  
  for(unsigned i=0; i < var.size(); i++)
      out.add(variables[i].name, variables[i].value);
}

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
/// Function to handle getByLabel.
template <typename X>
bool getByLabel(const edm::Event& event, 
                X& handle, 
                std::string& label1, std::string& label2, std::string& message,
                BufferType buffertype)
{ 
  // Try to do a getByLabel and fall on sword if it fails.
  try
    {
      if ( buffertype == RUNINFO )
        event.getRun().getByLabel(edm::InputTag(label1), handle);
      else
        {
          if ( label2 == "" )
            event.getByLabel(edm::InputTag(label1), handle);
          else
            event.getByLabel(label1, label2, handle);
        }
    }
  catch (cms::Exception& e)
    {
      // Fall on sword...
      std::ostringstream out;
      out << "getByLabel with label \"" 
          << label1 << " " << label2 << "\" failed on " 
          << boost::python::type_id<X>().name() << std::endl  
          << e.explainSelf(); 
      std::cout << out.str() << std::endl;
      message += out.str();
      return false;
    }
        
  // getByLabel succeeded, check that we have a valid handle,
  // otherwise again fall on sword...
  
  if ( !handle.isValid() )
    {
      // Ok, throw up!
      std::string m("\nBuffer::fill - getByLabel failed on ");
      m += boost::python::type_id<X>().name();
      m += std::string(" with label ") + label1 + std::string(" ") + label2;
      m += std::string("\n\tso go boil your head!\n");
      throw edm::Exception(edm::errors::Configuration, m);
    }
  return true;
}

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
///
template <typename X, typename Y>
void callMethods(int j, 
                 const X& object, 
                 std::vector<Variable<Y> >& variables, 
                 int debug)
{
  for(unsigned i=0; i < variables.size(); i++)
    {
      if ( debug > 0 ) 
        std::cout << "\t" << j << "\tcall: " 
                  << variables[i].fname << std::endl;
      try
        {
          variables[i].value[j] = variables[i].function(object);
        }
      catch (cms::Exception& e)
        {
          throw cms::Exception("BufferFillFailure",
                               "failed on call to \"" + 
                               variables[i].fname+"\"\n" +
                               "thou lump of foul deformity...", e);
        }
      if ( debug > 0 ) 
        std::cout << "\t\t\tvalue = " 
                  << BLUE 
                  << variables[i].value[j] 
                  << BLACK 
                  << std::endl;
    }
}


// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
/** Model a buffer.
    A buffer is a thing with<br>
    1 - a maximum count
    2 - a count of the number of values per variable
    3 - a vector of variables, each with the same <i>maxcount</i> and 
    a <i>count</i> that may vary from event to event.
    <p>

    The name of the ith n-tuple variable is constructed as follows:<br>
    \code
    name = prefix + "_" + var[i].second
    \endcode
    <br>
    where var[i] is a pair of strings with
    var[i].first the name of the method to be called
    var[i].second is the name of the n-tuple variable
    <p>
    We use a base class (BufferThing) to permit polymorphic behavior, that is,
    to allow generic calls to the buffer methods init(...) and fill(...) that
    operate on objects of differing type.
    <p>
    <i>typenames</i>:<br>
    - X = type of object extracted using getByLabel (extractable object)
*/
template <typename X, bool SINGLETON>
struct Buffer  : public BufferThing
{
  ///
  Buffer() 
    : out_(0),
      label_(""),
      label1_(""),
      label2_(""),
      prefix_(""),
      buffertype_(DEFAULT),
      var_(std::vector<VariableDescriptor>()),
      maxcount_(0),
      count_(0),
      singleton_(SINGLETON),
      message_(""),
      debug_(0)
  {
    std::cout << "Buffer created for objects of type: " 
              << name()
              << std::endl;
  }

  ///
  virtual ~Buffer() {}

  /** Initialize buffer.
      @param out - output ntuple file.
      @param label - getByLabel
      @param prefix - prefix for variable names (and internal name of buffer)
      @param var - variable descriptors
      @param maxcount - maximum count for this buffer
      @param log - log file
   */
  void
  init(otreestream& out,
       std::string  label, 
       std::string  prefix,
       std::vector<VariableDescriptor>& var,
       int maxcount,
       std::ofstream& log,
       int debug=0)
  {
    out_    = &out;
    label_  = label;
    prefix_ = prefix;
    var_    = var;
    maxcount_ = maxcount;
    debug_  = debug;

    // Is this a RunInfo object?
    // Data for these classes must be extracted
    // using the getRun() method of the event object.
    // An extractable object is one that can be extracted using getByLabel
    std::string classname = boost::python::type_id<X>().name();
    boost::regex re("RunInfo");
    boost::smatch match;
    if ( boost::regex_search(classname, match, re) ) buffertype_ = RUNINFO;

    // Complete initialization
    initBuffer<X>(out,
                  label_,
                  label1_,
                  label2_,
                  prefix_,
                  var_,
                  variables_,
                  count_,
                  singleton_,
                  maxcount_,
                  log,
                  debug_);
  }

  /// Fill buffer.
  bool fill(const edm::Event& event)
  {
    if ( debug_ > 0 ) 
      std::cout << BLACK
                << "Begin Buffer::fill\n\t" 
                << RED 
                << "X: " << boost::python::type_id<X>().name() << "\n\t"
                << BLACK
                << std::endl;

    count_ = 0; // reset count, just in case we have to bail out
    message_ = "";

    // Note: We use the handle edm::Handle<X> for singletons and
    //       the handle edm::Handle< View<X> > for collections.
  
    if ( singleton_ )
      {
        edm::Handle<X> handle;
        if ( ! getByLabel(event, handle, label1_, label2_, message_ ,
                          buffertype_) )
          return false;
   
        // OK handle is valid, so extract data for all variables
        const X& object = *handle;
        callMethods(0, object, variables_, debug_);
      }
    else
      {
        edm::Handle< edm::View<X> > handle;
        if ( ! getByLabel(event, handle, label1_, label2_, message_,
                          buffertype_) )
          return false;

        // OK handle is valid, so extract data for all variables.        
        // For the object count, use the smaller of handle size and maxcount.
        count_ = (int)handle->size() < maxcount_ 
          ? handle->size() 
          : maxcount_;

        for(int j=0; j < count_; j++)
          {
            const X& object = (*handle)[j];
            callMethods(j, object, variables_, debug_);
          }
        }
    
    if ( debug_ > 0 ) 
      std::cout << BLACK << "End Buffer::fill " << std::endl; 
    return true;
  }
  
  std::string& message() { return message_; }

  std::string name() { return boost::python::type_id<X>().name(); }

  /// Shrink buffer size using specified array of indices.
  void shrink(std::vector<int>& index)
  {
    count_ = index.size();
    for(unsigned i=0; i < variables_.size(); ++i)
      for(int j=0; j < count_; ++j)
        variables_[i].value[j] = variables_[i].value[index[j]];
  }

private:
  otreestream* out_;  
  std::string  label_;
  std::string  label1_;
  std::string  label2_;
  std::string  prefix_;
  BufferType buffertype_;
  std::vector<VariableDescriptor> var_;
  std::vector<Variable<X> > variables_;
  int  maxcount_;
  int  count_;
  bool singleton_;
  std::string message_;
  int  debug_;
};


// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
/// Buffer to handle Helpers.
template <typename X, typename Y, bool SINGLETON>
struct UserBuffer  : public BufferThing
{
  ///
  UserBuffer() 
    : out_(0),
      label_(""),
      label1_(""),
      label2_(""),
      prefix_(""),
      buffertype_(HELPER),
      var_(std::vector<VariableDescriptor>()),
      maxcount_(0),
      count_(0),
      singleton_(SINGLETON),
      message_(""),
      debug_(0)
  {
    std::cout << "Buffer created for objects of type: " 
              << name()
              << std::endl;
  }

  ///
  virtual ~UserBuffer() {}

  /** Initialize buffer.
      @param out - output ntuple file.
      @param label - getByLabel
      @param prefix - prefix for variable names (and internal name of buffer)
      @param var - variable descriptors
      @param maxcount - maximum count for this buffer
      @param log - log file
   */
  void
  init(otreestream& out,
       std::string  label, 
       std::string  prefix,
       std::vector<VariableDescriptor>& var,
       int  maxcount,
       std::ofstream& log,
       int debug=0)
  {
    out_    = &out;
    label_  = label;
    prefix_ = prefix;
    var_    = var;
    maxcount_ = maxcount;
    debug_  = debug;

    initBuffer<Y>(out,
                  label_,
                  label1_,
                  label2_,
                  prefix_,
                  var_,
                  variables_,
                  count_,
                  singleton_,
                  maxcount_,
                  log,
                  debug_);
  }
  
  /// Fill buffer.
  bool fill(const edm::Event& event)
  {
    if ( debug_ > 0 ) 
      std::cout << BLACK
                << "Begin UserBuffer::fill\n\t" 
                << RED 
                << "X: " << boost::python::type_id<X>().name() << "\n\t"
                << GREEN 
                << "Y: " << boost::python::type_id<Y>().name()
                << BLACK
                << std::endl;
    
    count_ = 0; // reset count, just in case we have to bail out
    message_ = "";

    // Create helper.
    // A helper provides the following methods in addition to its accessors:
    // 1. void analyzeEvent()
    // 2. void analyzeObject()

    // Cache event in helper (using CurrentEvent::instance().get())
    helper_.cacheEvent();
 
    // Perform (optional) user event-level analysis
    helper_.analyzeEvent();

    // Note: We use the handle edm::Handle<X> for singletons and
    //       the handle edm::Handle< View<X> > for collections.
    
    if ( singleton_ )
      {
        edm::Handle<X> handle;
        if ( ! getByLabel(event, handle, label1_, label2_, message_,
                          buffertype_) )
          return false;
        
        // OK handle is valid, so extract data for all variables. 

        // cache object in helper, along with its ordinal index - oindex -
        // set to 0 and set the count to its default value of 1.
        // NB: the helper could change "count" in analyzeObject()
        helper_.cacheObject(*handle);

        // Perform (optional) user object-level analysis
        helper_.analyzeObject();

         // Note: size returns the value of the internal variable count
        int k = 0;
        while ( (k < helper_.size()) && (count_ < maxcount_) )
          {
            helper_.set(k);    // set index of items to be returned
            callMethods(count_, (const Y)helper_, variables_, debug_);
            k++;
            count_++;
          }
      }
    else
      {
        edm::Handle< edm::View<X> > handle;      
        if ( ! getByLabel(event, handle, label1_, label2_, message_,
                          buffertype_) )
          return false;
        
        // OK handle is valid, so extract data for all variables  
        int objectcount = (int)handle->size() < maxcount_ 
          ? handle->size() 
          : maxcount_;
        
        for(int j=0; j < objectcount; j++)
          {
            // cache object in helper, along with its ordinal index - oindex -
            // set to "j" and set the count to its default value of 1
            helper_.cacheObject((*handle)[j], j);

            // Perform (optional) user object-level analysis
            helper_.analyzeObject();

            // Note: size returns the value of the internal variable count
            int k = 0;
            while ( (k < helper_.size()) && (count_ < maxcount_) )
              {
                helper_.set(k);
                callMethods(count_, (const Y)helper_, variables_, debug_);
                k++;
                count_++;
              }
          }
      }
  
    if ( debug_ > 0 ) 
      std::cout << BLACK << "End UserBuffer::fill " << std::endl; 
    return true;
  }
  
  std::string& message() { return message_; }

  std::string name() { return boost::python::type_id<Y>().name(); }

  /// Shrink buffer size using specified array of indices.
  void shrink(std::vector<int>& index)
  {
    count_ = index.size();
    for(unsigned i=0; i < variables_.size(); ++i)
      for(int j=0; j < count_; ++j)
        variables_[i].value[j] = variables_[i].value[index[j]];
  }

private:
  otreestream* out_;  
  std::string  label_;
  std::string  label1_;
  std::string  label2_;
  std::string  prefix_;
  BufferType buffertype_;
  std::vector<VariableDescriptor> var_;
  std::vector<Variable<Y> > variables_;
  int  maxcount_;
  int  count_;
  bool singleton_;
  std::string message_;
  int  debug_;

  // helper object
  Y helper_;
};

#endif
