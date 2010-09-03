#ifndef HELPERFOR_H
#define HELPERFOR_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Base class for helpers
// Created:     Aug, 2010 Harrison B. Prosper
//$Revision:$
//-----------------------------------------------------------------------------
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/Mkntuple/interface/CurrentEvent.h"
#include "PhysicsTools/Mkntuple/interface/Configuration.h"
//-----------------------------------------------------------------------------
/// Base class for helpers.
template <typename X>
class HelperFor
{
public:
  HelperFor() : config(Configuration::instance().get()),
                event(0), 
                object(0), 
                index(0), 
                count(1) 
  {}

  virtual ~HelperFor() {}

  void cacheEvent() { event = CurrentEvent::instance().get(); }
  void cacheObject(const X& o) { object = &o; count = 1; }

  /// return number of items per cached object
  int size() const { return count; }

  /// set index of item to be retrieved.
  void set(int ind) { index = ind; }

  // ---------------- can be overridden

  /// do some event-level analysis, if needed.
  virtual void analyzeEvent() {}

  /// do some object-level analysis, if needed.
  virtual void analyzeObject() {}

  // ---------------- available to user
  ///
  const edm::ParameterSet* config;
  ///
  const edm::Event* event;
  ///
  const X* object;
  ///
  int index;
  ///
  int count;
};

// std::string
// bufferLabel(std::string buffername)
// {
//       const edm::ParameterSet* congig = Configuration::instance().get();
//       vector<string> vrecords = config->
//          getUntrackedParameter<vector<string> >(buffername);
//       if ( vrecords.size() == 0 ) return string("");
//       string record = vrecords[0];
//       vector<string> field;              
//       kit::split(record, field);

//       if ( field.size() < 2 ) return string("");
//       return field[1];
// }

#endif

