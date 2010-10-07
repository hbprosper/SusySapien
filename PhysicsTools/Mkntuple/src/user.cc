//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Add user-defined methods
// Created:     Tue Jan 19, 2010 HBP
// Updated:     Mon Mar 08, 2010 Sezen & HBP - add triggerBits class
//              Tue Aug 24, 2010 HBP - add HcalNoiseRBXHelper
//              Thu Sep 02, 2010 HBP - update to new version of HelperFor
//$Revision: 1.14 $
//-----------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

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
namespace {
  void split(string str, vector<string>& vstr)
  {
    vstr.clear();
    istringstream stream(str);
    while ( stream )
      {
        string str;
        stream >> str;
        if ( stream ) vstr.push_back(str);
      }
  }
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
  split(record, field);
  if ( field.size() < 2 ) return label;

  label = field[1];
  return label;
}
