#ifndef BUFFEREVENTADDON_H
#define BUFFEREVENTADDON_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Specialized buffer for edm::EventAddon
// Created:     Thu Aug 26, 2010 Harrison B. Prosper
//$Revision: 1.4 $
//-----------------------------------------------------------------------------
#include "FWCore/Framework/interface/Event.h"
#include "PhysicsTools/Mkntuple/interface/user.h"
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
//-----------------------------------------------------------------------------
///
template <>
struct UserBuffer<edm::Event, 
                  edm::EventAddon, true>  : public BufferThing
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
      singleton_(true),
      message_(""),
      debug_(0)
  {
    std::cout << "Specialized Buffer created for edm::EventAddon" << std::endl;
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

    initBuffer<edm::EventAddon>(out,
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
  
  /** Fill buffer.
   */
  bool fill(const edm::Event& event)
  {
    if ( debug_ > 0 ) 
      std::cout << BLACK
                << "Begin (specialized) UserBuffer::fill\n\t" 
                << RED 
                << "X: edm::EventAddon"
                << BLACK
                << std::endl;
    count_ = 1;
    message_ = "";

    edm::EventAddon helper(event);
    helper.cache(event);
    callMethods(0, (const edm::EventAddon)helper, variables_, debug_);  

    if ( debug_ > 0 ) std::cout << BLACK
                                << "End UserBuffer::fill" 
                                << std::endl;
    return true;
  }

  std::string& message() { return message_; }

  std::string name() { return std::string("edm::EventAddon"); }

  /// Shrink buffer size using specified array of indices.
  void shrink(std::vector<int>& index) {}

private:
  otreestream* out_;  
  std::string  label_;
  std::string  label1_;
  std::string  label2_;
  std::string  prefix_;
  BufferType buffertype_;
  std::vector<VariableDescriptor> var_;
  std::vector<Variable<edm::EventAddon> > variables_;
  int  maxcount_;
  int  count_;
  bool singleton_;
  std::string message_;
  int  debug_;
};

#endif

