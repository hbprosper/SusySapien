#ifndef MYPLUGINS_H
#define MYPLUGINS_H
//-----------------------------------------------------------------------------
// Package:     SO10
// Sub-Package: Analysis
// Class:       patMuonExtra
// Description: Simple plugin for
//              class:   pat::Muon
// Created:     Tue Jan 19 HBP
//$Revision: 1.2 $
//-----------------------------------------------------------------------------
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
//-----------------------------------------------------------------------------
class patMuonExtra : public pat::Muon
{
public:
  patMuonExtra() {}
  patMuonExtra(const pat::Muon& o) : pat::Muon(o) {}
  ~patMuonExtra() {}

  double relIso() const
  {
    if ( pt() > 0 )
      return (ecalIso()+hcalIso()+trackIso())/pt();
    else
      return -1;
  }
};

class recoMuonIsol : public reco::Muon
{
public:
  recoMuonIsol() {}
  recoMuonIsol(const reco::Muon& o) : reco::Muon(o) {}
  ~recoMuonIsol() {}

  float sumPt() const { return isolationR03().sumPt; }
  float emEt()  const { return isolationR03().emEt; }
  float hadEt() const { return isolationR03().hadEt; }
  int nTracks() const { return isolationR03().nTracks; }

};

#endif

