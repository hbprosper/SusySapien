#ifndef ARECOPFSIMPARTICLE_H
#define ARECOPFSIMPARTICLE_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       arecoPFSimParticle
// Description: Simple adapter for:
//              class:   reco::PFSimParticle
//              header:  DataFormats/ParticleFlowReco/interface/PFSimParticle.h
//              
// Created:     Thu Jan 14 23:48:14 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/ParticleFlowReco/interface/PFSimParticle.h"
//-----------------------------------------------------------------------------
class arecoPFSimParticle : public reco::PFSimParticle
{
 public: 
  arecoPFSimParticle() {}
  arecoPFSimParticle(const reco::PFSimParticle& o)
    : reco::PFSimParticle(o) {}
  ~arecoPFSimParticle() {}

  int extrapolatedPoint_detId(unsigned x) const 	{return extrapolatedPoint(x).detId();}
  bool extrapolatedPoint_isTrackerLayer(unsigned x) const 	{return extrapolatedPoint(x).isTrackerLayer();}
  bool extrapolatedPoint_isValid(unsigned x) const 	{return extrapolatedPoint(x).isValid();}
  int extrapolatedPoint_layer(unsigned x) const 	{return extrapolatedPoint(x).layer();}
  int trajectoryPoint_detId(unsigned x) const 	{return trajectoryPoint(x).detId();}
  bool trajectoryPoint_isTrackerLayer(unsigned x) const 	{return trajectoryPoint(x).isTrackerLayer();}
  bool trajectoryPoint_isValid(unsigned x) const 	{return trajectoryPoint(x).isValid();}
  int trajectoryPoint_layer(unsigned x) const 	{return trajectoryPoint(x).layer();}
};
#endif

