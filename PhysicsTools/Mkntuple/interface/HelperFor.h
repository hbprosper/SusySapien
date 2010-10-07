#ifndef HELPERFOR_H
#define HELPERFOR_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Base class for helpers
// Created:     Aug, 2010 Harrison B. Prosper
//$Revision: 1.5 $
//-----------------------------------------------------------------------------
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/Mkntuple/interface/Configuration.h"
//-----------------------------------------------------------------------------
/// Base class for helpers.
template <typename X>
class HelperFor
{
public:
  HelperFor() : config(Configuration::instance().get()),
                event(0),   // pointer to current event
                object(0),  // pointer to current helped object
                oindex(0),  // index of current helped object (dumb pointer)
                index(0),   // index of current helper object (dumb pointer)
                count(1)    // number of instances returned by helper
  {}

  virtual ~HelperFor() {}
  ///
  void cacheEvent(const edm::Event& e) { event = &e; }

  ///
  void cacheObject(const X& o, int n=0) 
  { 
    object = &o; 
    oindex = n; 
    count  = 1; 
  }

  /// return number of items per cached object
  int size() const { return count; }

  /// set index of item to be retrieved.
  void set(int n) { index = n; }

  // ---------------- can be overridden

  /// do some event-level analysis, if needed.
  virtual void analyzeEvent() {}

  /// do some object-level analysis, if needed.
  virtual void analyzeObject() {}

  // ---------------- available to user
  /// Pointer to ParameterSet initialized from config file.
  const edm::ParameterSet* config;
  /// Pointer to event.
  const edm::Event* event;
  /// Pointer to current (helped) object
  const X* object;
  /// Index of current (helped) object (with count starting at zero).
  int oindex;
  /// Index of current helper object, which could differ from oindex..
  int index;
  /// 
  int count;
};

#endif

