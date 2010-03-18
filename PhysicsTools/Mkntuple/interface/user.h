#ifndef USER_H
#define USER_H
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
//-----------------------------------------------------------------------------

class GenMother : public reco::GenParticle
{
public:
  GenMother();

  /// Copy reco::GenParticle object to recoGenP object.
  GenMother(const reco::GenParticle& o);

  ~GenMother();

  int charge()  const;
  int pdgId()   const;
  int status()  const;
  double pt()   const;
  double eta()  const;
  double phi()  const;
  double mass() const;
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
};

#endif

