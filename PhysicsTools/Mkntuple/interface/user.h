#ifndef USER_H
#define USER_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Add user-defined methods
// Created:     Tue Jan 19, 2010 HBP
// Updated:     Mon Mar 08, 2010 Sezen & HBP - add triggerBits class
//              Thu Apr 08, 2010 Sezen & HBP - add GParticle class
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
//-----------------------------------------------------------------------------

class GParticle : public reco::GenParticle
{
public:
  GParticle();

  /// Copy reco::GenParticle object to GParticle object.
  GParticle(const reco::GenParticle& o);

  ~GParticle();

  ///
  int firstDaughter() const;

  ///
  int lastDaughter()  const;

private:
  // one daughterpos per GenParticle
  std::vector<int> daughterpos;

  // declare static so that we have one amap and one count per
  // event
  static std::map<std::string, int> amap;
  static int count;
};

//-----------------------------------------------------------------------------
// provides method
// bool value(triggername)
//-----------------------------------------------------------------------------
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

