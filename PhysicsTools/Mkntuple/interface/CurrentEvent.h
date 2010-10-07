#ifndef CURRENTEVENT_H
#define CURRENTEVENT_H
//
// Package:    PhysicsTools/Mkntuple
//             CurrentEvent.h
//
//             A singleton object to make event available to whoever needs it
//
// Original Author:  Harrison B. Prosper
//         Created:  Mon Mar  8, 2010
//
// $Id: CurrentEvent.h,v 1.3 2010/08/08 16:26:06 prosper Exp $

#include <map>
#include <vector>
#include <string>
#include "FWCore/Framework/interface/Event.h"
#include "PhysicsTools/Mkntuple/interface/BufferUtil.h"

/// A singleton class to cache event.
class CurrentEvent
{
public:
  ///
  static CurrentEvent& instance()
  {
    static CurrentEvent singleton;
    return singleton;
  }

  ///
  void set(const edm::Event& event, int count) 
  { 
    event_ = &event;
    count_ = count;
  }

  ///
  const edm::Event* get() const { return event_; }

  ///
  int count() const { return count_; }

private:
  CurrentEvent() {}        // prevent explicit creation
  ~CurrentEvent() {}                  
  CurrentEvent(const CurrentEvent&);             // prevent copy
  CurrentEvent& operator=(const CurrentEvent&);  // prevent assignment
  
  const edm::Event* event_;
  int count_;
};

#endif
