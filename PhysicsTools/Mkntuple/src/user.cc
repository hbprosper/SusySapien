//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Add user-defined methods
// Created:     Tue Jan 19, 2010 HBP
// Updated:     Mon Mar 08, 2010 Sezen & HBP - add triggerBits class
//$Revision: 1.2 $
//-----------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <map>
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "PhysicsTools/Mkntuple/interface/CurrentEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Event.h"
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/user.h"
//-----------------------------------------------------------------------------
GenMother::GenMother() {}

/// Copy reco::GenParticle object to recoGenP object.
GenMother::GenMother(const reco::GenParticle& o) : reco::GenParticle(o) {}

GenMother::~GenMother() {}

int 
GenMother::charge() const
{
  if ( numberOfMothers() < 1 ) return -999;
  return mother(0)->charge();
}

int 
GenMother::pdgId() const
{
  if ( numberOfMothers() < 1 ) return -1;
  return mother(0)->pdgId();
}

int 
GenMother::status() const
{
  if ( numberOfMothers() < 1 ) return -1;
  return mother(0)->status();
}

double 
GenMother::pt() const
{
  if ( numberOfMothers() < 1 ) return -1;
  return mother(0)->pt();
}

double 
GenMother::eta() const
{
  if ( numberOfMothers() < 1 ) return -1;
  return mother(0)->eta();
}

double 
GenMother::phi() const
{
  if ( numberOfMothers() < 1 ) return -1;
  return mother(0)->phi();
}

double 
GenMother::mass() const
{
  if ( numberOfMothers() < 1 ) return -1;
  return mother(0)->mass();
}

//-----------------------------------------------------------------------------
// provides method
// bool value(triggername)
//-----------------------------------------------------------------------------
triggerBits::triggerBits() {}
  
/// Copy edm::TriggerResults object to triggerBits object.
triggerBits::triggerBits(const edm::TriggerResults& o) 
  : edm::TriggerResults(o) {}
    
triggerBits::~triggerBits() {}

///
bool 
triggerBits::value(std::string tname) const
{
  // Get cached event
  const edm::Event* event = CurrentEvent::instance().get();
  if ( event == 0 )
    throw edm::Exception(edm::errors::Configuration,
                         "\ntriggerBits - " 
                         "event pointer is ZERO");
  
  // NB: use a reference to avoid expensive copying
  const edm::TriggerNames& tNames 
    = event->triggerNames(*dynamic_cast<const edm::TriggerResults*>(this));
  
  // Get bit associated with trigger name
  unsigned int bit = tNames.triggerIndex(tname);
  
  // If trigger does not exist, crash and burn!
  if ( bit == tNames.size() )
    throw edm::Exception(edm::errors::Configuration,
                         "\ntriggerBits - " 
                         "trigger \"" + tname + "\" NOT FOUND");
  else
    return accept(bit);
}

