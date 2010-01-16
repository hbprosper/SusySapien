#ifndef ARECOCALOCLUSTER_H
#define ARECOCALOCLUSTER_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       arecoCaloCluster
// Description: Simple adapter for:
//              class:   reco::CaloCluster
//              header:  DataFormats/CaloRecHit/interface/CaloCluster.h
//              
// Created:     Fri Jan 15 21:57:33 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
//-----------------------------------------------------------------------------
class arecoCaloCluster : public reco::CaloCluster
{
 public: 
  arecoCaloCluster() {}
  arecoCaloCluster(const reco::CaloCluster& o)
    : reco::CaloCluster(o) {}
  ~arecoCaloCluster() {}

  bool seed_null() const 	{return seed().null();}
  int seed_subdetId() const 	{return seed().subdetId();}
};
#endif

