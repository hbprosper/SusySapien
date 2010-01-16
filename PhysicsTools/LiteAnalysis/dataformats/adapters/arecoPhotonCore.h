#ifndef ARECOPHOTONCORE_H
#define ARECOPHOTONCORE_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       arecoPhotonCore
// Description: Simple adapter for:
//              class:   reco::PhotonCore
//              header:  DataFormats/EgammaCandidates/interface/PhotonCore.h
//              
// Created:     Fri Jan 15 21:58:01 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/EgammaCandidates/interface/PhotonCore.h"
//-----------------------------------------------------------------------------
class arecoPhotonCore : public reco::PhotonCore
{
 public: 
  arecoPhotonCore() {}
  arecoPhotonCore(const reco::PhotonCore& o)
    : reco::PhotonCore(o) {}
  ~arecoPhotonCore() {}

  size_t superCluster_clustersSize() const 	{return superCluster().isNonnull() ? superCluster()->clustersSize() : 999999;}
  double superCluster_energy() const 	{return superCluster().isNonnull() ? superCluster()->energy() : -999999;}
  double superCluster_eta() const 	{return superCluster().isNonnull() ? superCluster()->eta() : -999999;}
  double superCluster_etaWidth() const 	{return superCluster().isNonnull() ? superCluster()->etaWidth() : -999999;}
  double superCluster_phi() const 	{return superCluster().isNonnull() ? superCluster()->phi() : -999999;}
  double superCluster_phiWidth() const 	{return superCluster().isNonnull() ? superCluster()->phiWidth() : -999999;}
  double superCluster_preshowerEnergy() const 	{return superCluster().isNonnull() ? superCluster()->preshowerEnergy() : -999999;}
  double superCluster_rawEnergy() const 	{return superCluster().isNonnull() ? superCluster()->rawEnergy() : -999999;}
  size_t superCluster_size() const 	{return superCluster().isNonnull() ? superCluster()->size() : 999999;}
  double superCluster_x() const 	{return superCluster().isNonnull() ? superCluster()->x() : -999999;}
  double superCluster_y() const 	{return superCluster().isNonnull() ? superCluster()->y() : -999999;}
  double superCluster_z() const 	{return superCluster().isNonnull() ? superCluster()->z() : -999999;}
};
#endif

