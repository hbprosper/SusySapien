#ifndef MYPLUGINS_H
#define MYPLUGINS_H
//-----------------------------------------------------------------------------
// Package:     SO10
// Sub-Package: Analysis
// Class:       patMuonExtra
// Description: Simple plugin for
//              class:   pat::Muon
// Created:     Tue Jan 19 HBP
//-----------------------------------------------------------------------------
#include "DataFormats/PatCandidates/interface/Muon.h"
//-----------------------------------------------------------------------------
class patMuonExtra : public pat::Muon
{
public:
  patMuonExtra() {}
  patMuonExtra(const pat::Muon& o)
    : pat::Muon(o) {}
  ~patMuonExtra() {}

  bool isGlobalMuonPromptTight() const 
  {
    return isGood(reco::Muon::GlobalMuonPromptTight);
  }

  double relIso() const
  {
    if ( pt() > 0 )
      return (ecalIso()+hcalIso()+trackIso())/pt();
    else
      return -1;
  }
};
#endif

