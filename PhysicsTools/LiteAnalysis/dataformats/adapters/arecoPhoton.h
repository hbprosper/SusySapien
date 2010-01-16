#ifndef ARECOPHOTON_H
#define ARECOPHOTON_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       arecoPhoton
// Description: Simple adapter for:
//              class:   reco::Photon
//              header:  DataFormats/EgammaCandidates/interface/Photon.h
//              
// Created:     Thu Jan 14 23:47:55 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/EgammaCandidates/interface/Photon.h"
//-----------------------------------------------------------------------------
class arecoPhoton : public reco::Photon
{
 public: 
  arecoPhoton() {}
  arecoPhoton(const reco::Photon& o)
    : reco::Photon(o) {}
  ~arecoPhoton() {}

  size_t superCluster_clustersSize() const 	{return superCluster().isNonnull() ? superCluster()->clustersSize() : 999999;}
  double superCluster_energy() const 	{return superCluster().isNonnull() ? superCluster()->energy() : -999999;}
  double superCluster_eta() const 	{return superCluster().isNonnull() ? superCluster()->eta() : -999999;}
  double superCluster_etaWidth() const 	{return superCluster().isNonnull() ? superCluster()->etaWidth() : -999999;}
  double superCluster_phi() const 	{return superCluster().isNonnull() ? superCluster()->phi() : -999999;}
  double superCluster_phiWidth() const 	{return superCluster().isNonnull() ? superCluster()->phiWidth() : -999999;}
  double superCluster_preshowerEnergy() const 	{return superCluster().isNonnull() ? superCluster()->preshowerEnergy() : -999999;}
  double superCluster_rawEnergy() const 	{return superCluster().isNonnull() ? superCluster()->rawEnergy() : -999999;}
  double superCluster_x() const 	{return superCluster().isNonnull() ? superCluster()->x() : -999999;}
  double superCluster_y() const 	{return superCluster().isNonnull() ? superCluster()->y() : -999999;}
  double superCluster_z() const 	{return superCluster().isNonnull() ? superCluster()->z() : -999999;}
};
#endif

