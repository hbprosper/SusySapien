#ifndef BUFFER_H
#define BUFFER_H
// ----------------------------------------------------------------------------
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
//                   Wed Sep 08 HBP - fix array test
//                   Sun Sep 19 HBP - re-organize code to minimize code  bloat
//
// $Id: Buffer.h,v 1.26 2010/11/08 11:00:35 prosper Exp $
//
// ----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/BufferUtil.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
// ----------------------------------------------------------------------------
// We need a few templates to make the code generic. 
// WARNING: keep code as short as possible to minimize code bloat due to 
// template instantiation diarrhoea
// ----------------------------------------------------------------------------
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
    - SINGLETON = <i>true</i> if there can be at most once instance per event
*/
template <typename X, bool SINGLETON>
struct Buffer  : public BufferThing
{
  ///
  Buffer() 
    : out_(0),
      classname_(boost::python::type_id<X>().name()),
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
      debug_(0),
      skipme_(false)
  {
    std::cout << "Buffer created for objects of type: " 
              << name()
              << std::endl;

    // We need to skip these classes, if we are running over real data
    boost::regex getname("GenEvent|GenParticle|GenJet");
    boost::smatch m;
    skipme_ = boost::regex_search(classname_, m, getname);
//     std::cout << "\t==> class(" << classname_ 
//               << ")\t==> skipme_(" << skipme_ << ")" << std::endl;
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
    // Data for these classes must be extracted using the getRun() 
    // method of the event object.
    // Definition: An extractable object is one that can be extracted from an
    // event using getByLabel
    classname_ = boost::python::type_id<X>().name();
    boost::regex re("RunInfo");
    boost::smatch match;
    if ( boost::regex_search(classname_, match, re) ) buffertype_ = RUNINFO;

    initBuffer<X>(out,
                  label_,
                  label1_,
                  label2_,
                  prefix_,
                  var_,
                  variables_,
                  varnames_,
                  varmap_,
                  count_,
                  singleton_,
                  maxcount_,
                  log,
                  debug_);
  }
  
  /// Fill buffer.
  bool fill(const edm::Event& event, const edm::EventSetup& eventsetup)
  {
    if ( debug_ > 0 ) 
      std::cout << DEFAULT_COLOR
                << "Begin Buffer::fill\n\t" 
                << BLUE 
                << "X: " << boost::python::type_id<X>().name() << "\n\t"
                << DEFAULT_COLOR
                << std::endl;

    count_ = 0; // reset count, just in case we have to bail out
    message_ = "";

  // If this is real data ignore generator objects
  if ( event.isRealData() )
    {
      //std::cout << "===> REAL DATA" << std::endl;
      if ( skipme_ ) return true;
      
//       if ( label1_ == std::string("generator") )    return true;
//       if ( label1_ == std::string("genParticles") ) return true;
//       if ( label1_ == std::string("genJets") )      return true;
//       if ( label1_ == std::string("decaySubset") )  return true;
//       if ( label1_ == std::string("initSubset") )   return true;

//       if ( label2_ == std::string("generator") )    return true;
//       if ( label2_ == std::string("genParticles") ) return true;
//       if ( label2_ == std::string("genJets") )      return true;
//       if ( label2_ == std::string("decaySubset") )  return true;
//       if ( label2_ == std::string("initSubset") )   return true;

    }

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
      std::cout << DEFAULT_COLOR << "End Buffer::fill " << std::endl; 
    return true;
  }
  
  std::string& message() { return message_; }

  std::string name() { return classname_; }

  /// Shrink buffer size using specified array of indices.
  void shrink(std::vector<int>& index)
  {
    count_ = index.size();
    for(unsigned i=0; i < variables_.size(); ++i)
      for(int j=0; j < count_; ++j)
        variables_[i].value[j] = variables_[i].value[index[j]];
  }

  countvalue& variable(std::string name)
  {
    if ( varmap_.find(name) != varmap_.end() ) 
      return varmap_[name];
    else
      return varmap_["NONE"];
  }

  std::vector<std::string>& varnames()
  {
    return varnames_;
  }

  int count() { return count_; }
  int maxcount() { return maxcount_; }

private:
  otreestream* out_;
  std::string  classname_;
  std::string  label_;
  std::string  label1_;
  std::string  label2_;
  std::string  prefix_;
  BufferType buffertype_;
  std::vector<VariableDescriptor> var_;
  boost::ptr_vector<Variable<X> > variables_;
  std::vector<std::string> varnames_;
  std::map<std::string, countvalue> varmap_;
  int  maxcount_;
  int  count_;
  bool singleton_;
  std::string message_;
  int  debug_;
  bool skipme_;
};

template <>
struct Buffer<edm::Event, true>  : public BufferThing
{
  ///
  Buffer() 
    : out_(0),
      classname_("edm::Event"),
      label_(""),
      label1_(""),
      label2_(""),
      prefix_(""),
      buffertype_(DEFAULT),
      var_(std::vector<VariableDescriptor>()),
      maxcount_(0),
      count_(0),
      singleton_(true),
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

    initBuffer<edm::Event>(out,
                           label_,
                           label1_,
                           label2_,
                           prefix_,
                           var_,
                           variables_,
                           varnames_,
                           varmap_,
                           count_,
                           singleton_,
                           maxcount_,
                           log,
                           debug_);
  }
  
  /// Fill buffer.
  bool fill(const edm::Event& event, const edm::EventSetup& eventsetup)
  {
    if ( debug_ > 0 ) 
      std::cout << DEFAULT_COLOR
                << "Begin Buffer::fill\n\t" 
                << BLUE 
                << "X: edm::Event\n\t"
                << DEFAULT_COLOR
                << std::endl;

    count_ = 0; // reset count, just in case we have to bail out
    message_ = "";

    callMethods(0, event, variables_, debug_);

    if ( debug_ > 0 ) 
      std::cout << DEFAULT_COLOR << "End Buffer::fill " << std::endl; 
    return true;
  }
  
  std::string& message() { return message_; }

  std::string name() { return classname_; }

  /// Shrink buffer size using specified array of indices.
  void shrink(std::vector<int>& index)
  {}

  countvalue& variable(std::string name)
  {
    if ( varmap_.find(name) != varmap_.end() )
      return varmap_[name];
    else
      return varmap_["NONE"];
  }

  std::vector<std::string>& varnames()
  {
    return varnames_;
  }

  int count() { return count_; }
  int maxcount() { return maxcount_; }

private:
  otreestream* out_;
  std::string  classname_;
  std::string  label_;
  std::string  label1_;
  std::string  label2_;
  std::string  prefix_;
  BufferType buffertype_;
  std::vector<VariableDescriptor> var_;
  boost::ptr_vector<Variable<edm::Event> > variables_;
  std::vector<std::string> varnames_;
  std::map<std::string, countvalue> varmap_;
  int  maxcount_;
  int  count_;
  bool singleton_;
  std::string message_;
  int  debug_;
};

#endif
