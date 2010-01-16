#ifndef ARECOPFTRACK_H
#define ARECOPFTRACK_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       arecoPFTrack
// Description: Simple adapter for:
//              class:   reco::PFTrack
//              header:  DataFormats/ParticleFlowReco/interface/PFTrack.h
//              
// Created:     Fri Jan 15 21:58:17 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/ParticleFlowReco/interface/PFTrack.h"
//-----------------------------------------------------------------------------
class arecoPFTrack : public reco::PFTrack
{
 public: 
  arecoPFTrack() {}
  arecoPFTrack(const reco::PFTrack& o)
    : reco::PFTrack(o) {}
  ~arecoPFTrack() {}

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

