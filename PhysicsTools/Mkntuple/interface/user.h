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
//$Revision: 1.4 $
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

/** Abstract base class for helper classes.
 */
class HelperThing
{
public:
  HelperThing() {}
  virtual ~HelperThing() {}
  virtual int size() const=0;
  virtual void at(int index)=0;
};

/** Add-on class for reco::GenParticle.
 */
class GenParticleAddon
{
public:
  GenParticleAddon();

  /// pass reco::GenParticle object to GenParticleAddon object.
  GenParticleAddon(const reco::GenParticle& o);

  ~GenParticleAddon();

  ///
  int firstMother() const;

  ///
  int lastMother()  const;

  ///
  int firstDaughter() const;

  ///
  int lastDaughter()  const;

private:
  // one mothers vector and daughters vector per GenParticle
  std::vector<int> mothers_;
  std::vector<int> daughters_;

  // declare static so that we have one amap and one count per event
  static std::map<std::string, int> amap;
  static int count;
  static int index;
};


/** Add-on class for TriggerResults.
 */
class TriggerResultsAddon
{
 public:
  TriggerResultsAddon();
  
  /// pass edm::TriggerResults object to add-on object
  TriggerResultsAddon(const edm::TriggerResults& o);
    
  ~TriggerResultsAddon();

  ///
  bool value(std::string tname) const;

private:
  static bool first;
  const edm::TriggerResults* object_;
};

/** Helper class for HcalNoisRBX that unpacks the associated CaloTowers.
 */
class HcalNoiseRBXCaloTower : public HelperThing
{
 public:
  HcalNoiseRBXCaloTower();
  
  ///
  HcalNoiseRBXCaloTower(const reco::HcalNoiseRBX& o);
    
  virtual ~HcalNoiseRBXCaloTower();

  /// Required method.
  void at(int index);

  /// Required method.
  int size() const;

  /// get the z-side of the tower (1/-1)
  int zside() const;

  /// get the tower ieta
  int ieta() const;

  /// get the tower iphi
  int iphi() const;

  /// 
  double hadEnergy() const;

private:
  int index_;
  std::vector<int> zside_;
  std::vector<int> ieta_;
  std::vector<int> iphi_;
  std::vector<double> hadEnergy_;
};

#endif

