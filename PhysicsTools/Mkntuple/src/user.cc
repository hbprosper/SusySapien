//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Add user-defined methods
// Created:     Tue Jan 19, 2010 HBP
// Updated:     Mon Mar 08, 2010 Sezen & HBP - add triggerBits class
//              Tue Aug 24, 2010 HBP - add HcalNoiseRBXHelper
//$Revision: 1.6 $
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
//-----------------------------------------------------------------------------
GenParticleAddon::GenParticleAddon() {}

// This is called once per event
// Important: remember to initialize base class
GenParticleAddon::GenParticleAddon(const edm::Event& e)
  : HelperFor<reco::GenParticle>(e)
{
  // Initialize string representation/position map
  
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

  // Write a unique string for each genparticle
  // keeping only particles with status=3
  // Note: these occur first in particle list

  amap.clear();
  
  for(unsigned int i = 0; i < handle->size(); i++) 
    {
      const GenParticle* p = &((*handle)[i]);
      if ( p->status() != 3 ) break;

      char particle[255];
      sprintf(particle,"%d%d%f%f%f%f",
              p->pdgId(), 
              p->status(), 
              p->px(), 
              p->py(), 
              p->pz(), 
              p->energy());
      amap[string(particle)] = i;
    }
}

// cache object to be helped.
void 
GenParticleAddon::cache(const reco::GenParticle& o)
{
  // Important: be sure to cache pointer to object to be helped and
  // set count variable correctly.
  object = &o;

  // save only status = 3 particles
  if ( object->status() == 3 )
    {
      count = 1;
    }
  else
    {
      count = 0;
      return;
    }
  
  // Find the ordinal value of first and last mothers by comparing the 
  // string representation of mothers with the string representation of 
  // each gen-particle in the list:

  char particle[255];

  mothers_.clear();
  for(unsigned int j=0; j < object->numberOfMothers(); j++) 
    {
      const GenParticle* m = 
        dynamic_cast<const GenParticle*>(object->mother(j));
      if ( m == 0 ) continue;
      sprintf(particle,"%d%d%f%f%f%f",
              m->pdgId(), 
              m->status(), 
              m->px(), 
              m->py(), 
              m->pz(), 
              m->energy());
      if ( amap.find(string(particle)) != amap.end() ) 
        mothers_.push_back( amap[string(particle)] );
    }

  // Find the ordinal value of first and last daughters by comparing the 
  // string representation of daughters with the string representation of 
  // each gen-particle in the list:

  daughters_.clear();
  for(unsigned int j=0; j < object->numberOfDaughters(); j++) 
    {
      const GenParticle* d = 
        dynamic_cast<const GenParticle*>(object->daughter(j));
      if ( d == 0 ) continue;
      sprintf(particle,"%d%d%f%f%f%f", 
              d->pdgId(),  
              d->status(), 
              d->px(), 
              d->py(), 
              d->pz(), 
              d->energy());
      if ( amap.find(string(particle)) != amap.end() ) 
        daughters_.push_back( amap[string(particle)] );
    }
}

GenParticleAddon::~GenParticleAddon() {}

int      GenParticleAddon::charge() const { return object->charge(); }
int      GenParticleAddon::pdgId()  const { return object->pdgId(); }
int      GenParticleAddon::status() const { return object->status(); }
double   GenParticleAddon::pt()     const { return object->pt(); }
double   GenParticleAddon::eta()    const { return object->eta(); }
double   GenParticleAddon::phi()    const { return object->phi(); }
double   GenParticleAddon::mass()   const { return object->mass(); }

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
// TriggerResults helper
//-----------------------------------------------------------------------------
bool TriggerResultsAddon::first=true;

TriggerResultsAddon::TriggerResultsAddon() {}
  
TriggerResultsAddon::TriggerResultsAddon(const edm::Event& e)
  : HelperFor<edm::TriggerResults>(e)
{}
    
TriggerResultsAddon::~TriggerResultsAddon() {}

///
bool 
TriggerResultsAddon::value(std::string name) const
{
  if ( event == 0 )
    throw edm::Exception(edm::errors::Configuration,
                         "\nTriggerResultsAddon - " 
                         "event pointer is ZERO");
  
  // NB: use a reference to avoid expensive copying
  const edm::TriggerNames& tnames = event->triggerNames(*object);

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
    return object->accept(bit);
}


//-----------------------------------------------------------------------------
// Event helper
//-----------------------------------------------------------------------------
EventAddon::EventAddon() {}
  
///
EventAddon::EventAddon(const edm::Event& e)
  : HelperFor<edm::Event>(e) {}
    
EventAddon::~EventAddon() {}

///
int EventAddon::run() const { return object->id().run(); }

///
int EventAddon::event() const { return object->id().event(); }

///
int EventAddon::luminosityBlock() const { return object->luminosityBlock(); }

///
int EventAddon::bunchCrossing() const { return object->bunchCrossing(); }

///
int EventAddon::orbitNumber() const { return object->orbitNumber(); }

///
bool EventAddon::isRealData() const { return object->isRealData(); }

///
unsigned int EventAddon::unixTime() const {return object->time().unixTime();}

///
unsigned int EventAddon::nanosecondOffset() const 
{ return object->time().nanosecondOffset(); }

//-----------------------------------------------------------------------------
// HcalNoiseRBX helper
//-----------------------------------------------------------------------------

HcalNoiseRBXCaloTower::HcalNoiseRBXCaloTower() {}
  
HcalNoiseRBXCaloTower::HcalNoiseRBXCaloTower(const edm::Event& e)
  : HelperFor<reco::HcalNoiseRBX>(e) {}

void
HcalNoiseRBXCaloTower::cache(const reco::HcalNoiseRBX& o)
{
  // Important: Remember to cache pointer to object to be helped
  object = &o;

  // Cache results

  // loop over HPDs for given RBX
  for(std::vector<HcalNoiseHPD>::const_iterator 
        hpd = object->HPDsBegin(); hpd != object->HPDsEnd(); hpd++)
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
  // Important: Remember to update count
  count = zside_.size();
}
    
HcalNoiseRBXCaloTower::~HcalNoiseRBXCaloTower() {}

///
int 
HcalNoiseRBXCaloTower::zside() const
{
  if ( index < (int)zside_.size() ) 
    return zside_[index]; 
  else
    return -9999;
}

///
int 
HcalNoiseRBXCaloTower::ieta() const
{
  if ( index < (int)ieta_.size() ) 
    return ieta_[index];
  else
    return -9999;
}

///
int 
HcalNoiseRBXCaloTower::iphi() const
{
  if ( index < (int)iphi_.size() ) 
    return iphi_[index];
  else
    return -9999;
}

///
double 
HcalNoiseRBXCaloTower::hadEnergy() const
{
  if ( index < (int)hadEnergy_.size() ) 
    return hadEnergy_[index]; 
  else
    return -9999;
}


//-----------------------------------------------------------------------------
// Synonyms:
//-----------------------------------------------------------------------------
GParticle::GParticle() {}
GParticle::GParticle(const edm::Event& e) : GenParticleAddon(e) {}
GParticle::~GParticle() {}

//-----------------------------------------------------------------------------
triggerBits::triggerBits() {}
triggerBits::triggerBits(const edm::Event& e) : TriggerResultsAddon(e) {}
triggerBits::~triggerBits() {}
