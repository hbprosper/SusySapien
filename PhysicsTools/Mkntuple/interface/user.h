#ifndef USER_H
#define USER_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Add user-defined methods
// Created:     Tue Jan 19, 2010 HBP
// Updated:     Mon Mar 08, 2010 Sezen & HBP - add triggerBits class
//              Thu Apr 08, 2010 Sezen & HBP - add GParticle class
//              Tue Aug 24, 2010 HBP - add HcalNoiseRBXCaloTower
//                                   - add TriggerResultsAddon
//                                   - add GenParticleAddon
//$Revision: 1.7 $
//-----------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <map>
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "PhysicsTools/Mkntuple/interface/CurrentEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/METReco/interface/HcalNoiseRBX.h"
//-----------------------------------------------------------------------------
/// Base class for helpers.
template <typename X>
class HelperFor
{
public:
  HelperFor() : event(0), object(0), index(0), count(0) {}

  HelperFor(const edm::Event& e) : event(&e), object(0), index(0), count(1) {}

  virtual ~HelperFor() {}

  /// cache object to be helped.
  virtual void cache(const X& o) { object = &o; }

  /// set index of item to be retrieved.
  virtual void set(int ind) { index = ind; }

  /// return number of items per cached object
  virtual int  size() const { return count; }
  ///
  const edm::Event* event;
  ///
  const X* object;
  ///
  int index;
  ///
  int count;
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace reco
{
  //---------------------------------------------------------------------------
  /// A helper class for reco::GenParticle.
  class GenParticleAddon : public HelperFor<reco::GenParticle>
  {
  public:
    GenParticleAddon();
    ///
    GenParticleAddon(const edm::Event& e);

    virtual ~GenParticleAddon();

    /// Cache a GenParticle and initialize id map.
    void cache(const reco::GenParticle& o);
    ///
    int   charge() const;
    ///
    int   pdgId() const;
    ///
    int   status() const;
    ///
    double   pt() const;
    ///
    double   eta() const;
    ///
    double   phi() const;
    ///
    double   mass() const;
    ///
    int firstMother() const;
    ///
    int lastMother()  const;
    ///
    int firstDaughter() const;
    ///
    int lastDaughter()  const;
    
  private:
    // Filled once per cached object
    std::vector<int> mothers_;
    std::vector<int> daughters_;

    // Filled once per event
    std::map<std::string, int> amap;
  };
  //---------------------------------------------------------------------------
  /// Helper class for HcalNoisRBX that unpacks the associated CaloTowers.
  class HcalNoiseRBXCaloTower : public HelperFor<reco::HcalNoiseRBX>
  {
  public:
    HcalNoiseRBXCaloTower();
    
    ///
    HcalNoiseRBXCaloTower(const edm::Event& e);
    
    virtual ~HcalNoiseRBXCaloTower();

    /// cache CaloTowers info for current RBX.
    void cache(const reco::HcalNoiseRBX& o);

    /// get the z-side of the tower (1/-1)
    int zside() const;
    
    /// get the tower ieta
    int ieta() const;

    /// get the tower iphi
    int iphi() const;
    
    /// 
    double hadEnergy() const;

  private:
    std::vector<int> zside_;
    std::vector<int> ieta_;
    std::vector<int> iphi_;
    std::vector<double> hadEnergy_;
  };
}

namespace edm 
{
  //---------------------------------------------------------------------------
  /// Helper class for TriggerResults.
  class TriggerResultsAddon : public HelperFor<edm::TriggerResults>
  {
  public:
    TriggerResultsAddon();
    
    ///
    TriggerResultsAddon(const edm::Event& e);
    
    virtual ~TriggerResultsAddon();
    
    ///
    bool value(std::string tname) const;

  private:
    static bool first;
  };
  //---------------------------------------------------------------------------
  /// Helper class for edm::Event.
  class EventAddon : public HelperFor<edm::Event>
  {
  public:
    EventAddon();    
    ///
    EventAddon(const edm::Event& e);
    
    virtual ~EventAddon();
    
    ///
    int run() const;
    ///
    int event() const;
    ///
    int luminosityBlock() const;
    ///
    int bunchCrossing() const;
    ///
    int orbitNumber() const;
    ///
    bool isRealData() const;
    ///
    unsigned int unixTime() const;
    ///
    unsigned int nanosecondOffset() const;
  }; 
}

// ----------------------------------------------------------------------------
// Synonyms
// ----------------------------------------------------------------------------
class GParticle : public reco::GenParticleAddon
{
public:
  GParticle();
  GParticle(const edm::Event& e);
  virtual ~GParticle();
};

class triggerBits : public edm::TriggerResultsAddon
{
public:
  triggerBits();
  triggerBits(const edm::Event& e);
  virtual ~triggerBits();
};

#endif

