#ifndef USERPLUGINS_H
#define USERPLUGINS_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Add user-defined methods
// Created:     Tue Jan 19 HBP
//$Revision: 1.4 $
//-----------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <map>
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
//-----------------------------------------------------------------------------
class recoMuonExtra : public reco::Muon
{
public:
  recoMuonExtra() {}

  /// Copy reco::Muon object to recoMuonExtra object.
  recoMuonExtra(const reco::Muon& o) : reco::Muon(o) {}
  ~recoMuonExtra() {}

  double sumPt() const { return isolationR03().sumPt; }
  double emEt()  const { return isolationR03().emEt; }
  double hadEt() const { return isolationR03().hadEt; }
  double hoEt()  const { return isolationR03().hoEt; }
  int nTracks()  const { return isolationR03().nTracks; }

};

//-----------------------------------------------------------------------------
class recoGenMother : public reco::GenParticle
{
public:
  recoGenMother() {}

  /// Copy reco::GenParticle object to recoGenP object.
  recoGenMother(const reco::GenParticle& o) : reco::GenParticle(o) {}
  ~recoGenMother() {}

  int motherPdgId() const
  {
    if ( numberOfMothers() < 1 ) return -1;
    return mother(0)->pdgId();
  }

  int motherStatus() const
  {
    if ( numberOfMothers() < 1 ) return -1;
    return mother(0)->status();
  }

  double motherPt() const
  {
    if ( numberOfMothers() < 1 ) return -1;
    return mother(0)->pt();
  }

  double motherEta() const
  {
    if ( numberOfMothers() < 1 ) return -1;
    return mother(0)->eta();
  }

  double motherPhi() const
  {
    if ( numberOfMothers() < 1 ) return -1;
    return mother(0)->phi();
  }

  double motherMass() const
  {
    if ( numberOfMothers() < 1 ) return -1;
    return mother(0)->mass();
  }
};
#endif

