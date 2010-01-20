#ifndef APATPHOTON_H
#define APATPHOTON_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       apatPhoton
// Description: Simple adapter for:
//              class:   pat::Photon
//              header:  DataFormats/PatCandidates/interface/Photon.h
//              
// Created:     Fri Jan 15 21:58:24 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/PatCandidates/interface/Photon.h"
//-----------------------------------------------------------------------------
class apatPhoton : public pat::Photon
{
 public: 
  apatPhoton() {}
  apatPhoton(const pat::Photon& o)
    : pat::Photon(o) {}
  ~apatPhoton() {}

  float efficiency_error(const std::string x) const 	{return efficiency(x).error();}
  float efficiency_value(const std::string x) const 	{return efficiency(x).value();}
  int genParticle_charge(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->charge() : -999999;}
  int genParticle_collisionId(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->collisionId() : -999999;}
  double genParticle_energy(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->energy() : -999999;}
  double genParticle_et(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->et() : -999999;}
  double genParticle_eta(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->eta() : -999999;}
  bool genParticle_longLived(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->longLived() : false;}
  double genParticle_mass(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->mass() : -999999;}
  bool genParticle_massConstraint(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->massConstraint() : false;}
  double genParticle_massSqr(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->massSqr() : -999999;}
  double genParticle_mt(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->mt() : -999999;}
  double genParticle_mtSqr(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->mtSqr() : -999999;}
  double genParticle_p(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->p() : -999999;}
  int genParticle_pdgId(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->pdgId() : -999999;}
  double genParticle_phi(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->phi() : -999999;}
  double genParticle_pt(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->pt() : -999999;}
  double genParticle_px(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->px() : -999999;}
  double genParticle_py(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->py() : -999999;}
  double genParticle_pz(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->pz() : -999999;}
  double genParticle_rapidity(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->rapidity() : -999999;}
  int genParticle_status(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->status() : -999999;}
  double genParticle_theta(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->theta() : -999999;}
  int genParticle_threeCharge(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->threeCharge() : -999999;}
  double genParticle_vx(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->vx() : -999999;}
  double genParticle_vy(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->vy() : -999999;}
  double genParticle_vz(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->vz() : -999999;}
  double genParticle_y(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->y() : -999999;}
  int genParticleRef_charge(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->charge() : -999999;}
  int genParticleRef_collisionId(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->collisionId() : -999999;}
  double genParticleRef_energy(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->energy() : -999999;}
  double genParticleRef_et(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->et() : -999999;}
  double genParticleRef_eta(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->eta() : -999999;}
  bool genParticleRef_longLived(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->longLived() : false;}
  double genParticleRef_mass(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->mass() : -999999;}
  bool genParticleRef_massConstraint(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->massConstraint() : false;}
  double genParticleRef_massSqr(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->massSqr() : -999999;}
  double genParticleRef_mt(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->mt() : -999999;}
  double genParticleRef_mtSqr(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->mtSqr() : -999999;}
  double genParticleRef_p(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->p() : -999999;}
  int genParticleRef_pdgId(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->pdgId() : -999999;}
  double genParticleRef_phi(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->phi() : -999999;}
  double genParticleRef_pt(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->pt() : -999999;}
  double genParticleRef_px(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->px() : -999999;}
  double genParticleRef_py(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->py() : -999999;}
  double genParticleRef_pz(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->pz() : -999999;}
  double genParticleRef_rapidity(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->rapidity() : -999999;}
  int genParticleRef_status(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->status() : -999999;}
  double genParticleRef_theta(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->theta() : -999999;}
  int genParticleRef_threeCharge(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->threeCharge() : -999999;}
  double genParticleRef_vx(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->vx() : -999999;}
  double genParticleRef_vy(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->vy() : -999999;}
  double genParticleRef_vz(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->vz() : -999999;}
  double genParticleRef_y(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->y() : -999999;}
  int genPhoton_charge() const 	{return genPhoton() ? genPhoton()->charge() : -999999;}
  double genPhoton_energy() const 	{return genPhoton() ? genPhoton()->energy() : -999999;}
  double genPhoton_et() const 	{return genPhoton() ? genPhoton()->et() : -999999;}
  double genPhoton_eta() const 	{return genPhoton() ? genPhoton()->eta() : -999999;}
  bool genPhoton_longLived() const 	{return genPhoton() ? genPhoton()->longLived() : false;}
  double genPhoton_mass() const 	{return genPhoton() ? genPhoton()->mass() : -999999;}
  bool genPhoton_massConstraint() const 	{return genPhoton() ? genPhoton()->massConstraint() : false;}
  double genPhoton_massSqr() const 	{return genPhoton() ? genPhoton()->massSqr() : -999999;}
  double genPhoton_mt() const 	{return genPhoton() ? genPhoton()->mt() : -999999;}
  double genPhoton_mtSqr() const 	{return genPhoton() ? genPhoton()->mtSqr() : -999999;}
  double genPhoton_p() const 	{return genPhoton() ? genPhoton()->p() : -999999;}
  int genPhoton_pdgId() const 	{return genPhoton() ? genPhoton()->pdgId() : -999999;}
  double genPhoton_phi() const 	{return genPhoton() ? genPhoton()->phi() : -999999;}
  double genPhoton_pt() const 	{return genPhoton() ? genPhoton()->pt() : -999999;}
  double genPhoton_px() const 	{return genPhoton() ? genPhoton()->px() : -999999;}
  double genPhoton_py() const 	{return genPhoton() ? genPhoton()->py() : -999999;}
  double genPhoton_pz() const 	{return genPhoton() ? genPhoton()->pz() : -999999;}
  double genPhoton_rapidity() const 	{return genPhoton() ? genPhoton()->rapidity() : -999999;}
  int genPhoton_status() const 	{return genPhoton() ? genPhoton()->status() : -999999;}
  double genPhoton_theta() const 	{return genPhoton() ? genPhoton()->theta() : -999999;}
  int genPhoton_threeCharge() const 	{return genPhoton() ? genPhoton()->threeCharge() : -999999;}
  double genPhoton_vx() const 	{return genPhoton() ? genPhoton()->vx() : -999999;}
  double genPhoton_vy() const 	{return genPhoton() ? genPhoton()->vy() : -999999;}
  double genPhoton_vz() const 	{return genPhoton() ? genPhoton()->vz() : -999999;}
  double genPhoton_y() const 	{return genPhoton() ? genPhoton()->y() : -999999;}
  int originalObject_charge() const 	{return originalObject() ? originalObject()->charge() : -999999;}
  double originalObject_energy() const 	{return originalObject() ? originalObject()->energy() : -999999;}
  double originalObject_et() const 	{return originalObject() ? originalObject()->et() : -999999;}
  double originalObject_eta() const 	{return originalObject() ? originalObject()->eta() : -999999;}
  bool originalObject_longLived() const 	{return originalObject() ? originalObject()->longLived() : false;}
  double originalObject_mass() const 	{return originalObject() ? originalObject()->mass() : -999999;}
  bool originalObject_massConstraint() const 	{return originalObject() ? originalObject()->massConstraint() : false;}
  double originalObject_massSqr() const 	{return originalObject() ? originalObject()->massSqr() : -999999;}
  double originalObject_mt() const 	{return originalObject() ? originalObject()->mt() : -999999;}
  double originalObject_mtSqr() const 	{return originalObject() ? originalObject()->mtSqr() : -999999;}
  double originalObject_p() const 	{return originalObject() ? originalObject()->p() : -999999;}
  int originalObject_pdgId() const 	{return originalObject() ? originalObject()->pdgId() : -999999;}
  double originalObject_phi() const 	{return originalObject() ? originalObject()->phi() : -999999;}
  double originalObject_pt() const 	{return originalObject() ? originalObject()->pt() : -999999;}
  double originalObject_px() const 	{return originalObject() ? originalObject()->px() : -999999;}
  double originalObject_py() const 	{return originalObject() ? originalObject()->py() : -999999;}
  double originalObject_pz() const 	{return originalObject() ? originalObject()->pz() : -999999;}
  double originalObject_rapidity() const 	{return originalObject() ? originalObject()->rapidity() : -999999;}
  int originalObject_status() const 	{return originalObject() ? originalObject()->status() : -999999;}
  double originalObject_theta() const 	{return originalObject() ? originalObject()->theta() : -999999;}
  int originalObject_threeCharge() const 	{return originalObject() ? originalObject()->threeCharge() : -999999;}
  double originalObject_vx() const 	{return originalObject() ? originalObject()->vx() : -999999;}
  double originalObject_vy() const 	{return originalObject() ? originalObject()->vy() : -999999;}
  double originalObject_vz() const 	{return originalObject() ? originalObject()->vz() : -999999;}
  double originalObject_y() const 	{return originalObject() ? originalObject()->y() : -999999;}
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

