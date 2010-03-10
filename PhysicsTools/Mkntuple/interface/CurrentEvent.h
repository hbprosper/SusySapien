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
// $Id: CurrentEvent.h,v 1.7 2010/02/19 05:09:41 prosper Exp $

#include "FWCore/Framework/interface/Event.h"

class CurrentEvent
{
public:
  static CurrentEvent& instance()
  {
    static CurrentEvent singleton;
    return singleton;
  }

  void set(const edm::Event& event) { event_ = &event; }
  const edm::Event* get() const { return event_; }

private:
  CurrentEvent() {}                   // prevent explicit creation
  ~CurrentEvent() {}                  
  CurrentEvent(const CurrentEvent&);  // prevent copy
  CurrentEvent& operator=(const CurrentEvent&);  // prevent assignment
  
  const edm::Event* event_;
};

#endif
