#ifndef ARECOHFEMCLUSTERSHAPE_H
#define ARECOHFEMCLUSTERSHAPE_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       arecoHFEMClusterShape
// Description: Simple adapter for:
//              class:   reco::HFEMClusterShape
//              header:  DataFormats/EgammaReco/interface/HFEMClusterShape.h
//              
// Created:     Fri Jan 15 21:58:02 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/EgammaReco/interface/HFEMClusterShape.h"
//-----------------------------------------------------------------------------
class arecoHFEMClusterShape : public reco::HFEMClusterShape
{
 public: 
  arecoHFEMClusterShape() {}
  arecoHFEMClusterShape(const reco::HFEMClusterShape& o)
    : reco::HFEMClusterShape(o) {}
  ~arecoHFEMClusterShape() {}

  bool seed_null() const 	{return seed().null();}
  int seed_subdetId() const 	{return seed().subdetId();}
};
#endif

