#ifndef APATMET_H
#define APATMET_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       apatMET
// Description: Simple adapter for:
//              class:   pat::MET
//              header:  DataFormats/PatCandidates/interface/MET.h
//              
// Created:     Fri Jan 15 21:58:22 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/PatCandidates/interface/MET.h"
//-----------------------------------------------------------------------------
class apatMET : public pat::MET
{
 public: 
  apatMET() {}
  apatMET(const pat::MET& o)
    : pat::MET(o) {}
  ~apatMET() {}

  float efficiency_error(const std::string x) const 	{return efficiency(x).error();}
  float efficiency_value(const std::string x) const 	{return efficiency(x).value();}
  double genMET_auxiliaryEnergy() const 	{return genMET() ? genMET()->auxiliaryEnergy() : -999999;}
  int genMET_charge() const 	{return genMET() ? genMET()->charge() : -999999;}
  double genMET_e_longitudinal() const 	{return genMET() ? genMET()->e_longitudinal() : -999999;}
  double genMET_emEnergy() const 	{return genMET() ? genMET()->emEnergy() : -999999;}
  double genMET_energy() const 	{return genMET() ? genMET()->energy() : -999999;}
  double genMET_et() const 	{return genMET() ? genMET()->et() : -999999;}
  double genMET_eta() const 	{return genMET() ? genMET()->eta() : -999999;}
  double genMET_hadEnergy() const 	{return genMET() ? genMET()->hadEnergy() : -999999;}
  double genMET_invisibleEnergy() const 	{return genMET() ? genMET()->invisibleEnergy() : -999999;}
  bool genMET_longLived() const 	{return genMET() ? genMET()->longLived() : false;}
  double genMET_mEtSig() const 	{return genMET() ? genMET()->mEtSig() : -999999;}
  double genMET_mass() const 	{return genMET() ? genMET()->mass() : -999999;}
  bool genMET_massConstraint() const 	{return genMET() ? genMET()->massConstraint() : false;}
  double genMET_massSqr() const 	{return genMET() ? genMET()->massSqr() : -999999;}
  double genMET_mt() const 	{return genMET() ? genMET()->mt() : -999999;}
  double genMET_mtSqr() const 	{return genMET() ? genMET()->mtSqr() : -999999;}
  size_t genMET_numberOfTracks() const 	{return genMET() ? genMET()->numberOfTracks() : 999999;}
  double genMET_p() const 	{return genMET() ? genMET()->p() : -999999;}
  int genMET_pdgId() const 	{return genMET() ? genMET()->pdgId() : -999999;}
  double genMET_phi() const 	{return genMET() ? genMET()->phi() : -999999;}
  double genMET_pt() const 	{return genMET() ? genMET()->pt() : -999999;}
  double genMET_px() const 	{return genMET() ? genMET()->px() : -999999;}
  double genMET_py() const 	{return genMET() ? genMET()->py() : -999999;}
  double genMET_pz() const 	{return genMET() ? genMET()->pz() : -999999;}
  double genMET_rapidity() const 	{return genMET() ? genMET()->rapidity() : -999999;}
  int genMET_status() const 	{return genMET() ? genMET()->status() : -999999;}
  double genMET_sumEt() const 	{return genMET() ? genMET()->sumEt() : -999999;}
  double genMET_theta() const 	{return genMET() ? genMET()->theta() : -999999;}
  int genMET_threeCharge() const 	{return genMET() ? genMET()->threeCharge() : -999999;}
  double genMET_vx() const 	{return genMET() ? genMET()->vx() : -999999;}
  double genMET_vy() const 	{return genMET() ? genMET()->vy() : -999999;}
  double genMET_vz() const 	{return genMET() ? genMET()->vz() : -999999;}
  double genMET_y() const 	{return genMET() ? genMET()->y() : -999999;}
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
};
#endif

