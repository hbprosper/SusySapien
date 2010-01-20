#ifndef ARECONUCLEARINTERACTION_H
#define ARECONUCLEARINTERACTION_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       arecoNuclearInteraction
// Description: Simple adapter for:
//              class:   reco::NuclearInteraction
//              header:  DataFormats/VertexReco/interface/NuclearInteraction.h
//              
// Created:     Thu Jan 14 23:48:37 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/VertexReco/interface/NuclearInteraction.h"
//-----------------------------------------------------------------------------
class arecoNuclearInteraction : public reco::NuclearInteraction
{
 public: 
  arecoNuclearInteraction() {}
  arecoNuclearInteraction(const reco::NuclearInteraction& o)
    : reco::NuclearInteraction(o) {}
  ~arecoNuclearInteraction() {}

  double vertex_chi2() const 	{return vertex().chi2();}
  bool vertex_hasRefittedTracks() const 	{return vertex().hasRefittedTracks();}
  bool vertex_isFake() const 	{return vertex().isFake();}
  bool vertex_isValid() const 	{return vertex().isValid();}
  double vertex_ndof() const 	{return vertex().ndof();}
  double vertex_normalizedChi2() const 	{return vertex().normalizedChi2();}
  size_t vertex_tracksSize() const 	{return vertex().tracksSize();}
  double vertex_x() const 	{return vertex().x();}
  double vertex_xError() const 	{return vertex().xError();}
  double vertex_y() const 	{return vertex().y();}
  double vertex_yError() const 	{return vertex().yError();}
  double vertex_z() const 	{return vertex().z();}
  double vertex_zError() const 	{return vertex().zError();}
};
#endif

