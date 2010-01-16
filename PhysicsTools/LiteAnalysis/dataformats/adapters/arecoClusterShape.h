#ifndef ARECOCLUSTERSHAPE_H
#define ARECOCLUSTERSHAPE_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       arecoClusterShape
// Description: Simple adapter for:
//              class:   reco::ClusterShape
//              header:  DataFormats/EgammaReco/interface/ClusterShape.h
//              
// Created:     Thu Jan 14 23:47:56 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
//-----------------------------------------------------------------------------
class arecoClusterShape : public reco::ClusterShape
{
 public: 
  arecoClusterShape() {}
  arecoClusterShape(const reco::ClusterShape& o)
    : reco::ClusterShape(o) {}
  ~arecoClusterShape() {}

  bool e2ndId_null() const 	{return e2ndId().null();}
  int e2ndId_subdetId() const 	{return e2ndId().subdetId();}
  bool eMaxId_null() const 	{return eMaxId().null();}
  int eMaxId_subdetId() const 	{return eMaxId().subdetId();}
};
#endif

