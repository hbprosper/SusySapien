#ifndef ARECOPFRECTRACK_H
#define ARECOPFRECTRACK_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       arecoPFRecTrack
// Description: Simple adapter for:
//              class:   reco::PFRecTrack
//              header:  DataFormats/ParticleFlowReco/interface/PFRecTrack.h
//              
// Created:     Fri Jan 15 21:58:17 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"
//-----------------------------------------------------------------------------
class arecoPFRecTrack : public reco::PFRecTrack
{
 public: 
  arecoPFRecTrack() {}
  arecoPFRecTrack(const reco::PFRecTrack& o)
    : reco::PFRecTrack(o) {}
  ~arecoPFRecTrack() {}

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

