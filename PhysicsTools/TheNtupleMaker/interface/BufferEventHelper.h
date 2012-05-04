#ifndef BUFFEREVENTHELPER_H
#define BUFFEREVENTHELPER_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: TheNtupleMaker
// Description: Specialized buffer for edm::Event
// Created:     Thu Aug 26, 2010 Harrison B. Prosper
//$Revision: 1.1.1.1 $
//-----------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/UserBuffer.h"
//-----------------------------------------------------------------------------
///
template <>
struct UserBuffer<edm::Event, 
                  edm::EventHelper, SINGLETON>  : public BufferThing
{
  ///
  UserBuffer() 
    : out_(0),
      label_(""),
      label1_(""),
      label2_(""),
      prefix_(""),
      buffertype_(EVENT),
      var_(std::vector<VariableDescriptor>()),
      maxcount_(1),
      count_(0),
      ctype_(SINGLETON),
      message_(""),
      debug_(0)
  {
    std::cout << "Specialized Buffer created for edm::EventHelper" 
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
    out_     = &out;
    var_     = var;
    label_   = label;
    label1_  = label;
    prefix_  = prefix;
    maxcount_= 1;
    debug_   = debug;
    count_   = 1;
    classname_ = "edm::EventHelper";

    initBuffer<edm::EventHelper>(out,
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
  
  /** Fill buffer.
   */
  bool fill(const edm::Event& event, const edm::EventSetup& eventsetup)
  {
    if ( debug_ > 0 ) 
      std::cout << DEFAULT_COLOR
                << "Begin (specialized) UserBuffer::fill\n\t"
                << BLUE
                << "X: edm::EventHelper"
                << DEFAULT_COLOR
                << std::endl;
    count_ = 1;
    message_ = "";

    helper_.cacheObject(event);

    callMethods(0, (const edm::EventHelper)helper_, variables_, debug_);  

    if ( debug_ > 0 ) std::cout << DEFAULT_COLOR 
                                << "End UserBuffer::fill" << std::endl;
    return true;
  }

  std::string& message() { return message_; }

  std::string name() { return std::string("edm::EventHelper"); }

  /// Shrink buffer size using specified array of indices.
  void shrink(std::vector<int>& index) {}

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
  std::string  label_;
  std::string  label1_;
  std::string  label2_;
  std::string  prefix_;
  BufferType buffertype_;
  std::vector<VariableDescriptor> var_;
  boost::ptr_vector<Variable<edm::EventHelper> > variables_;
  std::vector<std::string> varnames_; 
  std::map<std::string, countvalue> varmap_;
  int  maxcount_;
  int  count_;
  ClassType ctype_;
  std::string message_;
  int  debug_;
  std::string classname_;
  std::string bufferkey_;
  edm::EventHelper helper_;
};

#endif

