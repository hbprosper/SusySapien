#ifndef BUFFER_H
#define BUFFER_H
// ----------------------------------------------------------------------------
//
// Package:    PhysicsTools/TheNtupleMaker
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
//                   Wed Apr 20 HBP - Add GenRun
//                   Sun May 01 HBP - Place Specialized buffer in a separate
//                                    file, BufferEvent.h
//                   Sun Apr 22 2012 HBP - Use Caller object
//
// $Id: Buffer.h,v 1.4 2011/07/20 16:19:54 prosper Exp $
//
// ----------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/BufferUtil.h"
#include "PhysicsTools/TheNtupleMaker/interface/Configuration.h"
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
    - CTYPE = <i>SINGLETON, COLLECTION, CONTAINER</i> 
*/
template <typename X, std::string* CNAME, ClassType CTYPE>
struct Buffer  : public BufferThing
{
  ///
  Buffer() 
    : out_(0),
      classname_(*CNAME),
      //classname_(boost::python::type_id<X>().name()),
      label_(""),
      label1_(""),
      label2_(""),
      prefix_(""),
      buffertype_(DEFAULT),
      var_(std::vector<VariableDescriptor>()),
      maxcount_(0),
      count_(0),
      ctype_(CTYPE),
      message_(""),
      debug_(0),
      skipme_(false),
      call_(Caller<X, X, CTYPE>())
  {
    std::cout << "Buffer created for objects of type: " 
              << name()
              << std::endl;

    // We need to skip these classes, if we are running over real data
    boost::regex getname("GenEvent|GenParticle|GenJet|GenRun");
    boost::smatch m;
    skipme_ = boost::regex_search(classname_, m, getname);
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
  virtual void
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

    std::cout << "\t=== Initialize Buffer for (" 
              << classname_ << ")"
              << std::endl;

    // Get optional crash switch

    try
      {
        crash_ = 
          (bool)(Configuration::instance().
                 getConfig()->
                 getUntrackedParameter<int>("crashOnInvalidHandle"));
      }
    catch (...)
      {
        crash_ = true;
      }

    if ( crash_ )
      std::cout << "\t=== CRASH on InvalidHandle (" 
                << classname_ << ")"
                << std::endl;
    else
      std::cout << "\t=== WARN on InvalidHandle (" 
                << classname_ << ")"
                << std::endl;  

    // Is this a RunInfo object?
    // Data for these classes must be extracted using the getRun() 
    // method of the event object.
    // Definition: An extractable object is one that can be extracted from an
    // event using getByLabel

    boost::regex re("RunInfo");
    boost::smatch match;
    if ( boost::regex_search(classname_, match, re) ) buffertype_ = RUNINFO;

    initBuffer<X>(out,
                  classname_,
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
  virtual bool 
  fill(const edm::Event& event, const edm::EventSetup& eventsetup)
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
        if ( skipme_ ) return true;      
      }
    
    // Note: We use the handle edm::Handle<X> for singletons and
    //       containers, but edm::Handle< View<X> > for vector types

    bool status = call_(event, 
                        label1_, 
                        label2_, 
                        message_,
                        buffertype_,
                        crash_,
                        variables_, 
                        count_,
                        maxcount_,
                        debug_);

    if ( debug_ > 0 ) 
      std::cout << DEFAULT_COLOR << "End Buffer::fill " << std::endl; 
    return status;
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
  std::string key() {return bufferkey_;}

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
  ClassType ctype_;
  std::string message_;
  int  debug_;
  bool skipme_;
  bool crash_;
  std::string bufferkey_;

  Caller<X, X, CTYPE> call_;
};

#endif
