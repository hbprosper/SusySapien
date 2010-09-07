//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Add user-defined methods
// Created:     Tue Jan 19, 2010 HBP
// Updated:     Mon Mar 08, 2010 Sezen & HBP - add triggerBits class
//              Tue Aug 24, 2010 HBP - add HcalNoiseRBXHelper
//              Thu Sep 02, 2010 HBP - update to new version of HelperFor
//$Revision: 1.12 $
//-----------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <map>
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "PhysicsTools/Mkntuple/interface/CurrentEvent.h"
#include "PhysicsTools/Mkntuple/interface/Configuration.h"
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
// This is called once per job
// Important: remember to initialize base class
GenParticleHelper::GenParticleHelper() : HelperFor<reco::GenParticle>() {}

// Called once per event
void
GenParticleHelper::analyzeEvent()
{
  // Initialize string representation/position map
  if ( event == 0 )
    throw edm::Exception(edm::errors::Configuration,
                         "\nGenParticleHelper - " 
                         "event pointer is ZERO");
  
  // Get genparticles:
  edm::Handle<GenParticleCollection> handle;
  // For now, hard-code getByLabel
  event->getByLabel("genParticles", handle);
  if (!handle.isValid())
    throw edm::Exception(edm::errors::Configuration,
                         "\nGenParticleHelper - " 
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

void 
GenParticleHelper::analyzeObject()
{
  if ( object == 0 )
    throw edm::Exception(edm::errors::Configuration,
                         "\nGenParticleHelper - " 
                         "object pointer is ZERO");

  // save only status = 3 particles
  if ( object->status() != 3 )
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

GenParticleHelper::~GenParticleHelper() {}

int      GenParticleHelper::charge() const { return object->charge(); }
int      GenParticleHelper::pdgId()  const { return object->pdgId(); }
int      GenParticleHelper::status() const { return object->status(); }
double   GenParticleHelper::energy() const { return object->energy(); }
double   GenParticleHelper::pt()     const { return object->pt(); }
double   GenParticleHelper::eta()    const { return object->eta(); }
double   GenParticleHelper::phi()    const { return object->phi(); }
double   GenParticleHelper::mass()   const { return object->mass(); }

int 
GenParticleHelper::firstMother() const
{
  if ( mothers_.size() > 0 )
    return mothers_.front();
  else
    return -1;
}

int 
GenParticleHelper::lastMother() const
{
  if ( mothers_.size() > 0 )
    return mothers_.back();
  else
    return -1;
}

int 
GenParticleHelper::firstDaughter() const
{
  if ( daughters_.size() > 0 )
    return daughters_.front();
  else
    return -1;
}

int 
GenParticleHelper::lastDaughter() const
{
  if ( daughters_.size() > 0 )
    return daughters_.back();
  else
    return -1;
}

//-----------------------------------------------------------------------------
// TriggerResults helper
//-----------------------------------------------------------------------------
bool TriggerResultsHelper::first=true;

TriggerResultsHelper::TriggerResultsHelper() : HelperFor<edm::TriggerResults>()
{}
    
TriggerResultsHelper::~TriggerResultsHelper() {}

///
bool 
TriggerResultsHelper::value(std::string name) const
{
  if ( event == 0 )
    throw edm::Exception(edm::errors::Configuration,
                         "\nTriggerResultsHelper - " 
                         "event pointer is ZERO");
  
  // NB: use a reference to avoid expensive copying
  const edm::TriggerNames& tnames = event->triggerNames(*object);

  // Write out trigger names upon first call
  if ( TriggerResultsHelper::first )
    {
      TriggerResultsHelper::first = false;
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
                         "\nTriggerResultsHelper - " 
                         "trigger \"" + name + "\" NOT FOUND");
  else
    return object->accept(bit);
}


//-----------------------------------------------------------------------------
// Event helper
//-----------------------------------------------------------------------------
///
EventHelper::EventHelper() : HelperFor<edm::Event>() {}
    
EventHelper::~EventHelper() {}

///
int EventHelper::run() const { return object->id().run(); }

///
int EventHelper::event() const { return object->id().event(); }

///
int EventHelper::luminosityBlock() const { return object->luminosityBlock(); }

///
int EventHelper::bunchCrossing() const { return object->bunchCrossing(); }

///
int EventHelper::orbitNumber() const { return object->orbitNumber(); }

///
bool EventHelper::isRealData() const { return object->isRealData(); }

///
unsigned int EventHelper::unixTime() const {return object->time().unixTime();}

///
unsigned int EventHelper::nanosecondOffset() const 
{ return object->time().nanosecondOffset(); }

//-----------------------------------------------------------------------------
// HcalNoiseRBX helper
//-----------------------------------------------------------------------------

HcalNoiseRBXCaloTower::HcalNoiseRBXCaloTower() 
  : HelperFor<reco::HcalNoiseRBX>() {}

void
HcalNoiseRBXCaloTower::analyzeObject()
{
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
          number_.push_back(oindex); // dumb pointer to helped object
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

///
int 
HcalNoiseRBXCaloTower::number() const
{
  if ( index < (int)number_.size() ) 
    return number_[index];
  else
    return -9999;
}

//-----------------------------------------------------------------------------
// Synonyms:
//-----------------------------------------------------------------------------
GParticle::GParticle() : GenParticleHelper() {}
GParticle::~GParticle() {}

triggerBits::triggerBits() : TriggerResultsHelper() {}
triggerBits::~triggerBits() {}
//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------
string
bufferLabel(string buffername)
{
  const edm::ParameterSet* config = Configuration::instance().get();
  if ( config == 0 )
    throw cms::Exception("InvalidPointer", "bufferLabel, config = 0");

  vector<string> vrecords = config->
    getUntrackedParameter<vector<string> >(buffername);

  string label("");
  if ( vrecords.size() == 0 ) return label;

  string record = vrecords[0];
  vector<string> field;              
  kit::split(record, field);
  if ( field.size() < 2 ) return label;

  label = field[1];
  return label;
}
