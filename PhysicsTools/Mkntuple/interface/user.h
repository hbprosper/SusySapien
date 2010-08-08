#ifndef USER_H
#define USER_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Add user-defined methods
// Created:     Tue Jan 19, 2010 HBP
// Updated:     Mon Mar 08, 2010 Sezen & HBP - add triggerBits class
//              Thu Apr 08, 2010 Sezen & HBP - add GParticle class
//$Revision: 1.3 $
//-----------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <map>
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "PhysicsTools/Mkntuple/interface/CurrentEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Event.h"
//-----------------------------------------------------------------------------

/** Model a generator level particle.
    This class extends the class reco::GenParticle.
 */
class GParticle : public reco::GenParticle
{
public:
  GParticle();

  /// Copy reco::GenParticle object to GParticle object.
  GParticle(const reco::GenParticle& o);

  ~GParticle();

  ///
  int firstMother() const;

  ///
  int lastMother()  const;

  ///
  int firstDaughter() const;

  ///
  int lastDaughter()  const;

private:
  // one mothers vector and daughters veector per GenParticle
  std::vector<int> mothers_;
  std::vector<int> daughters_;

  // declare static so that we have one amap and one count per event
  static std::map<std::string, int> amap;
  static int count;
  static int index;
};

//-----------------------------------------------------------------------------
// provides method
// bool value(triggername)
//-----------------------------------------------------------------------------

///
class triggerBits : public edm::TriggerResults
{
 public:
  triggerBits();
  
  /// Copy edm::TriggerResults object to triggerBits object.
  triggerBits(const edm::TriggerResults& o);
    
  ~triggerBits();

  ///
  bool value(std::string tname) const;

private:
  static bool first;
  const edm::TriggerResults* tresults;
};

#endif

