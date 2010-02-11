#ifndef MYPLUGINS_H
#define MYPLUGINS_H
//-----------------------------------------------------------------------------
// Package:     SO10
// Sub-Package: Analysis
// Class:       patMuonExtra
// Description: Simple plugin for
//              class:   pat::Muon
// Created:     Tue Jan 19 HBP
//$Revision: 1.3 $
//-----------------------------------------------------------------------------
#include "DataFormats/PatCandidates/interface/Muon.h"
//-----------------------------------------------------------------------------
// Note: patMuonExtra inherits all the methods of pat::Muon
class patMuonExtra : public pat::Muon
{
public:
  patMuonExtra() {}

  /// Copy pat::Muon object to patMuonExtra object.
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

#endif

