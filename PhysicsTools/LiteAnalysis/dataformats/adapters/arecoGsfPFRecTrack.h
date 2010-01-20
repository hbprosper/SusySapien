#ifndef ARECOGSFPFRECTRACK_H
#define ARECOGSFPFRECTRACK_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       arecoGsfPFRecTrack
// Description: Simple adapter for:
//              class:   reco::GsfPFRecTrack
//              header:  DataFormats/ParticleFlowReco/interface/GsfPFRecTrack.h
//              
// Created:     Thu Jan 14 23:48:10 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/ParticleFlowReco/interface/GsfPFRecTrack.h"
//-----------------------------------------------------------------------------
class arecoGsfPFRecTrack : public reco::GsfPFRecTrack
{
 public: 
  arecoGsfPFRecTrack() {}
  arecoGsfPFRecTrack(const reco::GsfPFRecTrack& o)
    : reco::GsfPFRecTrack(o) {}
  ~arecoGsfPFRecTrack() {}

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

