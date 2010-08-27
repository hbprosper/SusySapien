//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Add user-defined methods
// Created:     Tue Jan 19, 2010 HBP
// Updated:     Mon Mar 08, 2010 Sezen & HBP - add triggerBits class
//              Tue Aug 24, 2010 HBP - add HcalNoiseRBXHelper
//$Revision: 1.5 $
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
int GenParticleAddon::count=0;
int GenParticleAddon::index=0;
std::map<std::string, int> GenParticleAddon::amap;

GenParticleAddon::GenParticleAddon() {}

/// pass reco::GenParticle object to its add-on object.
GenParticleAddon::GenParticleAddon(const reco::GenParticle& o)
{
  // Initialize string representation/position map
  // This need be done only once per event. We ensure this by
  // comparing the current event count to the previously cached one. If they
  // differ this triggers the re-population of the map.

  char particle[255]; // character array for unique string representation
  
  int count = CurrentEvent::instance().count();
  if ( count != GenParticleAddon::count )
    {
      // NB: remember to cache the current event count 
      // to ensure this code segment is called once/event only
      GenParticleAddon::count = count;
      GenParticleAddon::index = 0;

      //DB
      //cout << "\t*** fill amap for event: " 
      // << GenParticleAddon::count << endl;

      // Get cached event
      const edm::Event* event = CurrentEvent::instance().get();
      if ( event == 0 )
        throw edm::Exception(edm::errors::Configuration,
                             "\nGenParticleAddon - " 
                             "event pointer is ZERO");
  
      // Get genparticles:
      edm::Handle<GenParticleCollection> handle;
      // For now, hard-code getByLabel
      event->getByLabel("genParticles", handle);
      if (!handle.isValid())
        throw edm::Exception(edm::errors::Configuration,
                             "\nGenParticleAddon - " 
                             "GenParticle handle is invalid");

      // Write a unique string for each genparticle:
      GenParticleAddon::amap.clear();

      for(unsigned int i = 0; i < handle->size(); i++) 
        {
          const GenParticle* p = &((*handle)[i]);
          sprintf(particle,"%d%d%f%f%f%f",
                  p->pdgId(), 
                  p->status(), 
                  p->px(), 
                  p->py(), 
                  p->pz(), 
                  p->energy());
          GenParticleAddon::amap[string(particle)] = i;
        }
    }

  // Find the ordinal value of first and last mothers by comparing the 
  // string representation of mothers with the string representation of 
  // each gen-particle in the list:

  mothers_.clear();
  for(unsigned int j=0; j < o.numberOfMothers(); j++) 
    {
      // Must use original object because its copy constructor does not
      // do a deep copy so the pointers are not copied
      const GenParticle* m = dynamic_cast<const GenParticle*>(o.mother(j));
      if ( m == 0 ) continue;
      sprintf(particle,"%d%d%f%f%f%f",
              m->pdgId(), 
              m->status(), 
              m->px(), 
              m->py(), 
              m->pz(), 
              m->energy());
      if (GenParticleAddon::amap.find(string(particle)) 
          != GenParticleAddon::amap.end() ) 
        mothers_.push_back( GenParticleAddon::amap[string(particle)] );
    }

  // Find the ordinal value of first and last daughters by comparing the 
  // string representation of mothers with the string representation of 
  // each gen-particle in the list:

  daughters_.clear();
  for(unsigned int j=0; j < o.numberOfDaughters(); j++) 
    {
      const GenParticle* d = dynamic_cast<const GenParticle*>(o.daughter(j));
      if ( d == 0 ) continue;
      sprintf(particle,"%d%d%f%f%f%f", 
              d->pdgId(),  
              d->status(), 
              d->px(), 
              d->py(), 
              d->pz(), 
              d->energy());
      if (GenParticleAddon::amap.find(string(particle)) 
          != GenParticleAddon::amap.end() ) 
        daughters_.push_back( GenParticleAddon::amap[string(particle)] );
    }
}

GenParticleAddon::~GenParticleAddon() {}

int 
GenParticleAddon::firstMother() const
{
  if ( mothers_.size() > 0 )
    return mothers_.front();
  else
    return -1;
}

int 
GenParticleAddon::lastMother() const
{
  if ( mothers_.size() > 0 )
    return mothers_.back();
  else
    return -1;
}

int 
GenParticleAddon::firstDaughter() const
{
  if ( daughters_.size() > 0 )
    return daughters_.front();
  else
    return -1;
}

int 
GenParticleAddon::lastDaughter() const
{
  if ( daughters_.size() > 0 )
    return daughters_.back();
  else
    return -1;
}


//-----------------------------------------------------------------------------
// TriggerResults add-on 
//-----------------------------------------------------------------------------
bool TriggerResultsAddon::first=true;

TriggerResultsAddon::TriggerResultsAddon() : object_(0) {}
  
/// cache edm::TriggerResults object in TriggerResultsAddon object.
TriggerResultsAddon::TriggerResultsAddon(const edm::TriggerResults& o) 
  : object_(&o)
{}
    
TriggerResultsAddon::~TriggerResultsAddon() {}

///
bool 
TriggerResultsAddon::value(std::string name) const
{
  // Get cached event
  const edm::Event* event = CurrentEvent::instance().get();
  if ( event == 0 )
    throw edm::Exception(edm::errors::Configuration,
                         "\nTriggerResultsAddon - " 
                         "event pointer is ZERO");
  
  // NB: use a reference to avoid expensive copying
  const edm::TriggerNames& tnames = event->triggerNames(*object_);

  // Write out trigger names upon first call
  if ( TriggerResultsAddon::first )
    {
      TriggerResultsAddon::first = false;
      ofstream fout("triggerNames.txt");
      fout << std::endl << "Bit" << "\t" "Trigger Name" << std::endl;
      for(unsigned  bit=0; bit < tnames.size(); bit++)
        fout << bit << "\t" << tnames.triggerName(bit) << std::endl;
      fout.close();
    }

  // Get bit associated with trigger name
  unsigned int bit = tnames.triggerIndex(name);
  
  // If trigger does not exist, crash and burn!
  if ( bit == tnames.size() )
    throw edm::Exception(edm::errors::Configuration,
                         "\nTriggerResultsAddon - " 
                         "trigger \"" + name + "\" NOT FOUND");
  else
    return object_->accept(bit);
}


//-----------------------------------------------------------------------------
// Add on for edm::Event
//-----------------------------------------------------------------------------
EventAddon::EventAddon() : object_(0) {}
  
///
EventAddon::EventAddon(const edm::Event& o) : object_(&o) {}
    
EventAddon::~EventAddon() {}

///
int EventAddon::run() const { return object_->id().run(); }

///
int EventAddon::event() const { return object_->id().event(); }

///
int EventAddon::luminosityBlock() const { return object_->luminosityBlock(); }

///
int EventAddon::bunchCrossing() const { return object_->bunchCrossing(); }

///
int EventAddon::orbitNumber() const { return object_->orbitNumber(); }

///
bool EventAddon::isRealData() const { return object_->isRealData(); }

///
unsigned int EventAddon::unixTime() const {return object_->time().unixTime();}

///
unsigned int EventAddon::nanosecondOffset() const 
{ return object_->time().nanosecondOffset(); }

//-----------------------------------------------------------------------------
// HcalNoiseRBX helper
//-----------------------------------------------------------------------------

HcalNoiseRBXCaloTower::HcalNoiseRBXCaloTower() {}
  
/// pass object to helper.
HcalNoiseRBXCaloTower::HcalNoiseRBXCaloTower(const reco::HcalNoiseRBX& o) 
  : index_(0)
{
  // Cache results

  // loop over HPDs for given RBX
  for(std::vector<HcalNoiseHPD>::const_iterator 
        hpd = o.HPDsBegin(); hpd != o.HPDsEnd(); hpd++)
    {
      // loop over calo towers
      const RefVector<CaloTowerCollection> towers = hpd->caloTowers();
      for(unsigned int tower=0; tower < towers.size(); tower++)
        {
          zside_.push_back(towers[tower]->id().zside());
          ieta_.push_back(towers[tower]->id().ieta());
          iphi_.push_back(towers[tower]->id().iphi());
          hadEnergy_.push_back(towers[tower]->hadEnergy());
        }
    } 
}
    
HcalNoiseRBXCaloTower::~HcalNoiseRBXCaloTower() {}

int 
HcalNoiseRBXCaloTower::size() const
{
  return zside_.size();
}

void 
HcalNoiseRBXCaloTower::at(int index)
{
  index_ = index;
}

///
int 
HcalNoiseRBXCaloTower::zside() const
{
  if ( index_ < (int)zside_.size() ) 
    return zside_[index_]; 
  else
    return -9999;
}

///
int 
HcalNoiseRBXCaloTower::ieta() const
{
  if ( index_ < (int)ieta_.size() ) 
    return ieta_[index_];
  else
    return -9999;
}

///
int 
HcalNoiseRBXCaloTower::iphi() const
{
  if ( index_ < (int)iphi_.size() ) 
    return iphi_[index_];
  else
    return -9999;
}

///
double 
HcalNoiseRBXCaloTower::hadEnergy() const
{
  if ( index_ < (int)hadEnergy_.size() ) 
    return hadEnergy_[index_]; 
  else
    return -9999;
}


//-----------------------------------------------------------------------------
// Deprecated:
//-----------------------------------------------------------------------------
int GParticle::count=0;
int GParticle::index=0;
std::map<std::string, int> GParticle::amap;

GParticle::GParticle() : object_(0) {}

/// pass reco::GenParticle object to its add-on object.
GParticle::GParticle(const reco::GenParticle& o)
  : object_(&o)
{
  // Initialize string representation/position map
  // This need be done only once per event. We ensure this by
  // comparing the current event count to the previously cached one. If they
  // differ this triggers the re-population of the map.

  char particle[255]; // character array for unique string representation
  
  int count = CurrentEvent::instance().count();
  if ( count != GParticle::count )
    {
      // NB: remember to cache the current event count 
      // to ensure this code segment is called once/event only
      GParticle::count = count;
      GParticle::index = 0;

      //DB
      //cout << "\t*** fill amap for event: " 
      // << GParticle::count << endl;

      // Get cached event
      const edm::Event* event = CurrentEvent::instance().get();
      if ( event == 0 )
        throw edm::Exception(edm::errors::Configuration,
                             "\nGParticle - " 
                             "event pointer is ZERO");
  
      // Get genparticles:
      edm::Handle<GenParticleCollection> handle;
      // For now, hard-code getByLabel
      event->getByLabel("genParticles", handle);
      if (!handle.isValid())
        throw edm::Exception(edm::errors::Configuration,
                             "\nGParticle - " 
                             "GenParticle handle is invalid");

      // Write a unique string for each genparticle:
      GParticle::amap.clear();

      for(unsigned int i = 0; i < handle->size(); i++) 
        {
          const GenParticle* p = &((*handle)[i]);
          sprintf(particle,"%d%d%f%f%f%f",
                  p->pdgId(), 
                  p->status(), 
                  p->px(), 
                  p->py(), 
                  p->pz(), 
                  p->energy());
          GParticle::amap[string(particle)] = i;
        }
    }

  // Find the ordinal value of first and last mothers by comparing the 
  // string representation of mothers with the string representation of 
  // each gen-particle in the list:

  mothers_.clear();
  for(unsigned int j=0; j < o.numberOfMothers(); j++) 
    {
      // Must use original object because its copy constructor does not
      // do a deep copy so the pointers are not copied
      const GenParticle* m = dynamic_cast<const GenParticle*>(o.mother(j));
      if ( m == 0 ) continue;
      sprintf(particle,"%d%d%f%f%f%f",
              m->pdgId(), 
              m->status(), 
              m->px(), 
              m->py(), 
              m->pz(), 
              m->energy());
      if (GParticle::amap.find(string(particle)) 
          != GParticle::amap.end() ) 
        mothers_.push_back( GParticle::amap[string(particle)] );
    }

  // Find the ordinal value of first and last daughters by comparing the 
  // string representation of mothers with the string representation of 
  // each gen-particle in the list:

  daughters_.clear();
  for(unsigned int j=0; j < o.numberOfDaughters(); j++) 
    {
      const GenParticle* d = dynamic_cast<const GenParticle*>(o.daughter(j));
      if ( d == 0 ) continue;
      sprintf(particle,"%d%d%f%f%f%f", 
              d->pdgId(),  
              d->status(), 
              d->px(), 
              d->py(), 
              d->pz(), 
              d->energy());
      if (GParticle::amap.find(string(particle)) 
          != GParticle::amap.end() ) 
        daughters_.push_back( GParticle::amap[string(particle)] );
    }
}

GParticle::~GParticle() {}

int      GParticle::charge() const { return object_->charge(); }
int      GParticle::pdgId()  const { return object_->pdgId(); }
int      GParticle::status() const { return object_->status(); }
double   GParticle::pt()     const { return object_->pt(); }
double   GParticle::eta()    const { return object_->eta(); }
double   GParticle::phi()    const { return object_->phi(); }
double   GParticle::mass()   const { return object_->mass(); }

int 
GParticle::firstMother() const
{
  if ( mothers_.size() > 0 )
    return mothers_.front();
  else
    return -1;
}

int 
GParticle::lastMother() const
{
  if ( mothers_.size() > 0 )
    return mothers_.back();
  else
    return -1;
}

int 
GParticle::firstDaughter() const
{
  if ( daughters_.size() > 0 )
    return daughters_.front();
  else
    return -1;
}

int 
GParticle::lastDaughter() const
{
  if ( daughters_.size() > 0 )
    return daughters_.back();
  else
    return -1;
}

//-----------------------------------------------------------------------------
bool triggerBits::first=true;

triggerBits::triggerBits() : object_(0) {}
  
/// cache edm::TriggerResults object in triggerBits object.
triggerBits::triggerBits(const edm::TriggerResults& o) 
  : object_(&o)
{}
    
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
  const edm::TriggerNames& tnames = event->triggerNames(*object_);

  // Write out trigger names upon first call
  if ( triggerBits::first )
    {
      triggerBits::first = false;
      ofstream fout("triggerNames.txt");
      fout << std::endl << "Bit" << "\t" "Trigger Name" << std::endl;
      for(unsigned  bit=0; bit < tnames.size(); bit++)
        fout << bit << "\t" << tnames.triggerName(bit) << std::endl;
      fout.close();
    }

  // Get bit associated with trigger name
  unsigned int bit = tnames.triggerIndex(name);
  
  // If trigger does not exist, crash and burn!
  if ( bit == tnames.size() )
    throw edm::Exception(edm::errors::Configuration,
                         "\ntriggerBits - " 
                         "trigger \"" + name + "\" NOT FOUND");
  else
    return object_->accept(bit);
}

