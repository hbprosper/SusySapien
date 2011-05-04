#ifndef USERBUFFER_H
#define USERBUFFER_H
// ----------------------------------------------------------------------------
//
// Package:    PhysicsTools/TheNtupleMaker
//             UserBuffer.h
//
// Original Author:  Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
//         Updated:  Sun Sep 19 HBP - copy from Buffer.h
//
// $Id: UserBuffer.h,v 1.9 2011/05/02 23:50:58 prosper Exp $
//
// ----------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/BufferUtil.h"
// ----------------------------------------------------------------------------
/** Model a helper buffer.
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
    - Y = type of helper object
    - SINGLETON = <i>true</i> if there can be at most once instance per event
*/
template <typename X, typename Y, bool SINGLETON>
struct UserBuffer  : public BufferThing
{
  ///
  UserBuffer() 
    : out_(0),
      classname_(boost::python::type_id<X>().name()),
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
      debug_(0),
      skipme_(false)
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
    classname_ = boost::python::type_id<Y>().name();

    initBuffer<Y>(out,
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
                << "Begin UserBuffer::fill\n\t" 
                << BLUE
                << "X: " << boost::python::type_id<X>().name() << "\n\t"
                << "Y: " << boost::python::type_id<Y>().name()
                << DEFAULT_COLOR
                << std::endl;
    
    count_ = 0; // reset count, just in case we have to bail out
    message_ = "";
    
    // If this is real data ignore generator objects
    if ( event.isRealData() )
      {
        if ( skipme_ ) return true;
      }
    
    // Create helper.
    // A helper provides the following methods in addition to its accessors:
    // 1. void analyzeEvent()
    // 2. void analyzeObject()
    
    // Cache event and eventsetup in helper
    helper_.cacheEvent(event, eventsetup);
    
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
      std::cout << DEFAULT_COLOR << "End UserBuffer::fill " << std::endl; 
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
  boost::ptr_vector<Variable<Y> > variables_;
  std::vector<std::string> varnames_;
  std::map<std::string, countvalue> varmap_;
  int  maxcount_;
  int  count_;
  bool singleton_;
  std::string message_;
  int  debug_;
  bool skipme_; 

  // helper object
  Y helper_;
};

#endif
