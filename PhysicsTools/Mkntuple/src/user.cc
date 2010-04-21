//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Add user-defined methods
// Created:     Tue Jan 19, 2010 HBP
// Updated:     Mon Mar 08, 2010 Sezen & HBP - add triggerBits class
//$Revision: 1.1 $
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
using namespace edm;
using namespace reco;
using namespace std;

// Initialize static variables
int GParticle::count=0;
std::map<std::string, int> GParticle::amap;

GParticle::GParticle() {}

/// Copy reco::GenParticle object to recoGenP object.
GParticle::GParticle(const reco::GenParticle& o) : reco::GenParticle(o) 
{
  // Initialize string representation to position map
  // This need be done only once per event. We ensure this by
  // comparing the current event count to the previously cached one. If they
  // differ this triggers the re-population of the map.

  char particle[80]; // character array for unique string representation
  
  int count = CurrentEvent::instance().count();
  if ( count != GParticle::count )
    {
      // NB: remember to cache the current event count 
      // to ensure this code segment is called once/event
      GParticle::count = count;
 
      //DB
      cout << "\t*** fill amap for event: " << GParticle::count << endl;

      // Get cached event
      const edm::Event* event = CurrentEvent::instance().get();
      if ( event == 0 )
        throw edm::Exception(edm::errors::Configuration,
                             "\nGParticle - " 
                             "event pointer is ZERO");
  
      // Get genparticles:
      Handle<GenParticleCollection> genParticles;
      event->getByLabel("genParticles", genParticles);
  
      // Write a unique string for each genparticle:
      GParticle::amap.clear();
      for(unsigned int i = 0; i < genParticles->size(); i++) 
        {
          const GenParticle* p = &((*genParticles)[i]);
          sprintf(particle,"%d%d%f%f%f%f", 
                  p->pdgId(), p->status(), 
                  p->px(), p->py(), p->pz(), p->energy());
          GParticle::amap[particle] = i;
        }
    }


  // Find the place of first and last daughters by comparing 
  // the strings belonging to daughters
  // with the strings of each individual genpartle in the list:

  daughterpos.clear();

  for(unsigned int j=0; j < numberOfDaughters(); j++) 
    {
      const Candidate* d = daughter(j);
      if (status()==3 && d->status()!=3) continue;
      sprintf(particle,"%d%d%f%f%f%f", 
              d->pdgId(), d->status(), 
              d->px(), d->py(), d->pz(), d->energy());
      if (GParticle::amap.find(particle) != GParticle::amap.end() ) 
        {
          int k = GParticle::amap[particle];
          daughterpos.push_back(k);
        }
    }
}

GParticle::~GParticle() {}

int 
GParticle::firstDaughter() const
{
  if ( daughterpos.size() > 0 )
    return daughterpos.front();
  else
    return -1;
}

int 
GParticle::lastDaughter() const
{
  if ( daughterpos.size() > 0 )
    return daughterpos.back();
  else
    return -1;
}


//-----------------------------------------------------------------------------
// provides method
// bool value(triggername)
//-----------------------------------------------------------------------------
bool triggerBits::first=true;

triggerBits::triggerBits() {}
  
/// Copy edm::TriggerResults object to triggerBits object.
triggerBits::triggerBits(const edm::TriggerResults& o) 
  : edm::TriggerResults(o) {}
    
triggerBits::~triggerBits() {}

///
bool 
triggerBits::value(std::string name) const
{
  // Get cached event
  const edm::Event* event = CurrentEvent::instance().get();
  if ( event == 0 )
    throw edm::Exception(edm::errors::Configuration,
                         "\ntriggerBits - " 
                         "event pointer is ZERO");
  
  // NB: use a reference to avoid expensive copying
  const edm::TriggerNames& tnames 
    = event->triggerNames(*dynamic_cast<const edm::TriggerResults*>(this));

  // Print out trigger names upon first call
  if ( triggerBits::first )
    {
      triggerBits::first = false;
      std::cout << std::endl << "Bit" << "\t" << "Trigger Name" << std::endl;

      for(unsigned  bit=0; bit < tnames.size(); bit++)
        {
          std::cout << bit << "\t" << tnames.triggerName(bit) << std::endl;
        }
      std::cout << std::endl;
    }
  // Get bit associated with trigger name
  unsigned int bit = tnames.triggerIndex(name);
  
  // If trigger does not exist, crash and burn!
  if ( bit == tnames.size() )
    throw edm::Exception(edm::errors::Configuration,
                         "\ntriggerBits - " 
                         "trigger \"" + name + "\" NOT FOUND");
  else
    return accept(bit);
}

