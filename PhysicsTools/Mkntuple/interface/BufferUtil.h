#ifndef BUFFERUTIL_H
#define BUFFERUTIL_H
// ----------------------------------------------------------------------------
//
// Package:    PhysicsTools/Mkntuple
//             BufferUtil.h
//
// Original Author:  Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
//         Updated:  Sun Sep 19 HBP move some code from Buffer.h 
//
// $Id: BufferUtil.h,v 1.1 2010/09/19 14:09:53 prosper Exp $
// ----------------------------------------------------------------------------
#include <Python.h>
#include <boost/python/type_id.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

#include "PhysicsTools/Mkntuple/interface/treestream.h"
#include "PhysicsTools/Mkntuple/interface/colors.h"
#include "PhysicsTools/Mkntuple/interface/MethodT.h"
// ----------------------------------------------------------------------------

struct VariableDescriptor
{
  VariableDescriptor(std::string r, std::string m, std::string v)
    : rtype(r),
      method(m),
      varname(v)
  {}

  ~VariableDescriptor() {}

  std::string rtype;
  std::string method;
  std::string varname;
  std::string name;
  int maxcount;
};

/// Abstract base class for Buffer objects.
struct BufferThing
{
  virtual ~BufferThing();
      
  /// Initialize the buffer. 
  virtual void init(otreestream& out,
                    std::string label, 
                    std::string prefix,
                    std::vector<VariableDescriptor>& var,
                    int  maxcount,
                    std::ofstream& log,
                    int debug=0)=0;
  
  /// Call requested methods of selected objects and fill buffer.
  virtual bool fill(const edm::Event& event)=0;
  ///
  virtual std::string& message()=0;
  ///
  virtual std::string name()=0;
  ///
  virtual void shrink(std::vector<int>& index)=0;

};

///
enum BufferType
  {
    EVENT,            // Buffer for event
    RUNINFO,          // Buffer for RunInfo object
    HELPER,           // Buffer that performs complicated accesses
    DEFAULT
  };

///
void initializeBuffer(otreestream& out,  
                      std::string& classname,
                      std::string& label,
                      std::string& label1,
                      std::string& label2,
                      std::string& prefix,
                      std::vector<VariableDescriptor>& var,
                      int&  count,
                      bool  singleton,
                      int   maxcount,
                      std::ofstream& log,
                      int   debug);
// ----------------------------------------------------------------------------
// We need a few templates to make the code generic. 
// WARNING: keep code as short as possible to minimize code bloat due to 
// template instantiation diarrhoea
// ----------------------------------------------------------------------------
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
// ----------------------------------------------------------------------------
template <typename X>
void initBuffer(otreestream& out,  
                std::string& label,
                std::string& label1,
                std::string& label2,
                std::string& prefix,
                std::vector<VariableDescriptor>& var,
                std::vector<Variable<X> >& variables,
                int&  count,
                bool  singleton,
                int   maxcount,
                std::ofstream& log,
                int   debug)
{
  std::string classname = boost::python::type_id<X>().name();
  initializeBuffer(out,
                   classname,
                   label,
                   label1,
                   label2,
                   prefix,
                   var,
                   count,
                   singleton,
                   maxcount,
                   log,
                   debug);

  // Create a variable object for each method
  for(unsigned i=0; i < var.size(); i++)
    variables.push_back(Variable<X>(var[i].name, 
                                    var[i].maxcount,
                                    var[i].method));
  
  // Add variables to output tree. This must be done after all
  // variables have been defined, because it is only then that their
  // addresses are guaranteed to be stable.
  
  for(unsigned i=0; i < var.size(); i++)
    out.add(variables[i].name, variables[i].value);
}

/// Function to handle getByLabel.
template <typename X>
bool getByLabel(const edm::Event& event, 
                X& handle, 
                std::string& label1, 
                std::string& label2, 
                std::string& message,
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
      m += std::string("\n\t...you blocks you stones you worse than"
                       " senseless things!\n");
      throw edm::Exception(edm::errors::Configuration, m);
    }
  return true;
}

// ----------------------------------------------------------------------------
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
                  << variables[i].value[j] 
                  << std::endl;
    }
}

#endif
