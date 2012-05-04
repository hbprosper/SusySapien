#ifndef BUFFEREVENT_H
#define BUFFEREVENT_H
// ----------------------------------------------------------------------------
//
// Package:    PhysicsTools/TheNtupleMaker
//             BufferEvent.h
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
//                   Wed Apr 20 HBP - Add GenRun
//                   Sun May 01 HBP - split from Buffer and place in this file
//
// $Id: BufferEvent.h,v 1.1.1.1 2011/05/04 13:04:28 prosper Exp $
//
// ----------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
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
template <>
struct Buffer<edm::Event, "edm::Event", SINGLETON>  : public BufferThing
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
      ctype_(SINGLETON),
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
                           ctype_,
                           maxcount_,
                           log,
                           bufferkey_,
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
  std::string key() { return bufferkey_; }

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
  ClassType ctype_;
  std::string message_;
  int  debug_;
  std::string bufferkey_;
};

#endif
