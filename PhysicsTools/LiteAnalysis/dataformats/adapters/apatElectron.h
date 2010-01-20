#ifndef APATELECTRON_H
#define APATELECTRON_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       apatElectron
// Description: Simple adapter for:
//              class:   pat::Electron
//              header:  DataFormats/PatCandidates/interface/Electron.h
//              
// Created:     Fri Jan 15 21:58:18 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/PatCandidates/interface/Electron.h"
//-----------------------------------------------------------------------------
class apatElectron : public pat::Electron
{
 public: 
  apatElectron() {}
  apatElectron(const pat::Electron& o)
    : pat::Electron(o) {}
  ~apatElectron() {}

  float efficiency_error(const std::string x) const 	{return efficiency(x).error();}
  float efficiency_value(const std::string x) const 	{return efficiency(x).value();}
  int genLepton_charge() const 	{return genLepton() ? genLepton()->charge() : -999999;}
  int genLepton_collisionId() const 	{return genLepton() ? genLepton()->collisionId() : -999999;}
  double genLepton_energy() const 	{return genLepton() ? genLepton()->energy() : -999999;}
  double genLepton_et() const 	{return genLepton() ? genLepton()->et() : -999999;}
  double genLepton_eta() const 	{return genLepton() ? genLepton()->eta() : -999999;}
  bool genLepton_longLived() const 	{return genLepton() ? genLepton()->longLived() : false;}
  double genLepton_mass() const 	{return genLepton() ? genLepton()->mass() : -999999;}
  bool genLepton_massConstraint() const 	{return genLepton() ? genLepton()->massConstraint() : false;}
  double genLepton_massSqr() const 	{return genLepton() ? genLepton()->massSqr() : -999999;}
  double genLepton_mt() const 	{return genLepton() ? genLepton()->mt() : -999999;}
  double genLepton_mtSqr() const 	{return genLepton() ? genLepton()->mtSqr() : -999999;}
  double genLepton_p() const 	{return genLepton() ? genLepton()->p() : -999999;}
  int genLepton_pdgId() const 	{return genLepton() ? genLepton()->pdgId() : -999999;}
  double genLepton_phi() const 	{return genLepton() ? genLepton()->phi() : -999999;}
  double genLepton_pt() const 	{return genLepton() ? genLepton()->pt() : -999999;}
  double genLepton_px() const 	{return genLepton() ? genLepton()->px() : -999999;}
  double genLepton_py() const 	{return genLepton() ? genLepton()->py() : -999999;}
  double genLepton_pz() const 	{return genLepton() ? genLepton()->pz() : -999999;}
  double genLepton_rapidity() const 	{return genLepton() ? genLepton()->rapidity() : -999999;}
  int genLepton_status() const 	{return genLepton() ? genLepton()->status() : -999999;}
  double genLepton_theta() const 	{return genLepton() ? genLepton()->theta() : -999999;}
  int genLepton_threeCharge() const 	{return genLepton() ? genLepton()->threeCharge() : -999999;}
  double genLepton_vx() const 	{return genLepton() ? genLepton()->vx() : -999999;}
  double genLepton_vy() const 	{return genLepton() ? genLepton()->vy() : -999999;}
  double genLepton_vz() const 	{return genLepton() ? genLepton()->vz() : -999999;}
  double genLepton_y() const 	{return genLepton() ? genLepton()->y() : -999999;}
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
  int gsfTrack_charge() const 	{return gsfTrack().isNonnull() ? gsfTrack()->charge() : -999999;}
  int gsfTrack_chargeMode() const 	{return gsfTrack().isNonnull() ? gsfTrack()->chargeMode() : -999999;}
  double gsfTrack_chi2() const 	{return gsfTrack().isNonnull() ? gsfTrack()->chi2() : -999999;}
  double gsfTrack_d0() const 	{return gsfTrack().isNonnull() ? gsfTrack()->d0() : -999999;}
  double gsfTrack_d0Error() const 	{return gsfTrack().isNonnull() ? gsfTrack()->d0Error() : -999999;}
  double gsfTrack_dsz() const 	{return gsfTrack().isNonnull() ? gsfTrack()->dsz() : -999999;}
  double gsfTrack_dszError() const 	{return gsfTrack().isNonnull() ? gsfTrack()->dszError() : -999999;}
  double gsfTrack_dxy() const 	{return gsfTrack().isNonnull() ? gsfTrack()->dxy() : -999999;}
  double gsfTrack_dxyError() const 	{return gsfTrack().isNonnull() ? gsfTrack()->dxyError() : -999999;}
  double gsfTrack_dz() const 	{return gsfTrack().isNonnull() ? gsfTrack()->dz() : -999999;}
  double gsfTrack_dzError() const 	{return gsfTrack().isNonnull() ? gsfTrack()->dzError() : -999999;}
  double gsfTrack_eta() const 	{return gsfTrack().isNonnull() ? gsfTrack()->eta() : -999999;}
  double gsfTrack_etaError() const 	{return gsfTrack().isNonnull() ? gsfTrack()->etaError() : -999999;}
  double gsfTrack_etaMode() const 	{return gsfTrack().isNonnull() ? gsfTrack()->etaMode() : -999999;}
  double gsfTrack_etaModeError() const 	{return gsfTrack().isNonnull() ? gsfTrack()->etaModeError() : -999999;}
  unsigned short gsfTrack_found() const 	{return gsfTrack().isNonnull() ? gsfTrack()->found() : 9999;}
  unsigned int gsfTrack_innerDetId() const 	{return gsfTrack().isNonnull() ? gsfTrack()->innerDetId() : 999999;}
  bool gsfTrack_innerOk() const 	{return gsfTrack().isNonnull() ? gsfTrack()->innerOk() : false;}
  double gsfTrack_lambda() const 	{return gsfTrack().isNonnull() ? gsfTrack()->lambda() : -999999;}
  double gsfTrack_lambdaError() const 	{return gsfTrack().isNonnull() ? gsfTrack()->lambdaError() : -999999;}
  double gsfTrack_lambdaMode() const 	{return gsfTrack().isNonnull() ? gsfTrack()->lambdaMode() : -999999;}
  double gsfTrack_lambdaModeError() const 	{return gsfTrack().isNonnull() ? gsfTrack()->lambdaModeError() : -999999;}
  unsigned short gsfTrack_lost() const 	{return gsfTrack().isNonnull() ? gsfTrack()->lost() : 9999;}
  double gsfTrack_ndof() const 	{return gsfTrack().isNonnull() ? gsfTrack()->ndof() : -999999;}
  double gsfTrack_normalizedChi2() const 	{return gsfTrack().isNonnull() ? gsfTrack()->normalizedChi2() : -999999;}
  unsigned short gsfTrack_numberOfLostHits() const 	{return gsfTrack().isNonnull() ? gsfTrack()->numberOfLostHits() : 9999;}
  unsigned short gsfTrack_numberOfValidHits() const 	{return gsfTrack().isNonnull() ? gsfTrack()->numberOfValidHits() : 9999;}
  unsigned int gsfTrack_outerDetId() const 	{return gsfTrack().isNonnull() ? gsfTrack()->outerDetId() : 999999;}
  double gsfTrack_outerEta() const 	{return gsfTrack().isNonnull() ? gsfTrack()->outerEta() : -999999;}
  bool gsfTrack_outerOk() const 	{return gsfTrack().isNonnull() ? gsfTrack()->outerOk() : false;}
  double gsfTrack_outerP() const 	{return gsfTrack().isNonnull() ? gsfTrack()->outerP() : -999999;}
  double gsfTrack_outerPhi() const 	{return gsfTrack().isNonnull() ? gsfTrack()->outerPhi() : -999999;}
  double gsfTrack_outerPt() const 	{return gsfTrack().isNonnull() ? gsfTrack()->outerPt() : -999999;}
  double gsfTrack_outerPx() const 	{return gsfTrack().isNonnull() ? gsfTrack()->outerPx() : -999999;}
  double gsfTrack_outerPy() const 	{return gsfTrack().isNonnull() ? gsfTrack()->outerPy() : -999999;}
  double gsfTrack_outerPz() const 	{return gsfTrack().isNonnull() ? gsfTrack()->outerPz() : -999999;}
  double gsfTrack_outerRadius() const 	{return gsfTrack().isNonnull() ? gsfTrack()->outerRadius() : -999999;}
  double gsfTrack_outerTheta() const 	{return gsfTrack().isNonnull() ? gsfTrack()->outerTheta() : -999999;}
  double gsfTrack_outerX() const 	{return gsfTrack().isNonnull() ? gsfTrack()->outerX() : -999999;}
  double gsfTrack_outerY() const 	{return gsfTrack().isNonnull() ? gsfTrack()->outerY() : -999999;}
  double gsfTrack_outerZ() const 	{return gsfTrack().isNonnull() ? gsfTrack()->outerZ() : -999999;}
  double gsfTrack_p() const 	{return gsfTrack().isNonnull() ? gsfTrack()->p() : -999999;}
  double gsfTrack_pMode() const 	{return gsfTrack().isNonnull() ? gsfTrack()->pMode() : -999999;}
  double gsfTrack_phi() const 	{return gsfTrack().isNonnull() ? gsfTrack()->phi() : -999999;}
  double gsfTrack_phiError() const 	{return gsfTrack().isNonnull() ? gsfTrack()->phiError() : -999999;}
  double gsfTrack_phiMode() const 	{return gsfTrack().isNonnull() ? gsfTrack()->phiMode() : -999999;}
  double gsfTrack_phiModeError() const 	{return gsfTrack().isNonnull() ? gsfTrack()->phiModeError() : -999999;}
  double gsfTrack_pt() const 	{return gsfTrack().isNonnull() ? gsfTrack()->pt() : -999999;}
  double gsfTrack_ptError() const 	{return gsfTrack().isNonnull() ? gsfTrack()->ptError() : -999999;}
  double gsfTrack_ptMode() const 	{return gsfTrack().isNonnull() ? gsfTrack()->ptMode() : -999999;}
  double gsfTrack_ptModeError() const 	{return gsfTrack().isNonnull() ? gsfTrack()->ptModeError() : -999999;}
  double gsfTrack_px() const 	{return gsfTrack().isNonnull() ? gsfTrack()->px() : -999999;}
  double gsfTrack_pxMode() const 	{return gsfTrack().isNonnull() ? gsfTrack()->pxMode() : -999999;}
  double gsfTrack_py() const 	{return gsfTrack().isNonnull() ? gsfTrack()->py() : -999999;}
  double gsfTrack_pyMode() const 	{return gsfTrack().isNonnull() ? gsfTrack()->pyMode() : -999999;}
  double gsfTrack_pz() const 	{return gsfTrack().isNonnull() ? gsfTrack()->pz() : -999999;}
  double gsfTrack_pzMode() const 	{return gsfTrack().isNonnull() ? gsfTrack()->pzMode() : -999999;}
  double gsfTrack_qoverp() const 	{return gsfTrack().isNonnull() ? gsfTrack()->qoverp() : -999999;}
  double gsfTrack_qoverpError() const 	{return gsfTrack().isNonnull() ? gsfTrack()->qoverpError() : -999999;}
  double gsfTrack_qoverpMode() const 	{return gsfTrack().isNonnull() ? gsfTrack()->qoverpMode() : -999999;}
  double gsfTrack_qoverpModeError() const 	{return gsfTrack().isNonnull() ? gsfTrack()->qoverpModeError() : -999999;}
  int gsfTrack_qualityMask() const 	{return gsfTrack().isNonnull() ? gsfTrack()->qualityMask() : -999999;}
  size_t gsfTrack_recHitsSize() const 	{return gsfTrack().isNonnull() ? gsfTrack()->recHitsSize() : 999999;}
  double gsfTrack_theta() const 	{return gsfTrack().isNonnull() ? gsfTrack()->theta() : -999999;}
  double gsfTrack_thetaError() const 	{return gsfTrack().isNonnull() ? gsfTrack()->thetaError() : -999999;}
  double gsfTrack_thetaMode() const 	{return gsfTrack().isNonnull() ? gsfTrack()->thetaMode() : -999999;}
  double gsfTrack_thetaModeError() const 	{return gsfTrack().isNonnull() ? gsfTrack()->thetaModeError() : -999999;}
  double gsfTrack_vx() const 	{return gsfTrack().isNonnull() ? gsfTrack()->vx() : -999999;}
  double gsfTrack_vy() const 	{return gsfTrack().isNonnull() ? gsfTrack()->vy() : -999999;}
  double gsfTrack_vz() const 	{return gsfTrack().isNonnull() ? gsfTrack()->vz() : -999999;}
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
  int pfCandidateRef_charge() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->charge() : -999999;}
  double pfCandidateRef_deltaP() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->deltaP() : -999999;}
  double pfCandidateRef_ecalEnergy() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->ecalEnergy() : -999999;}
  double pfCandidateRef_energy() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->energy() : -999999;}
  double pfCandidateRef_et() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->et() : -999999;}
  double pfCandidateRef_eta() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->eta() : -999999;}
  double pfCandidateRef_hcalEnergy() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->hcalEnergy() : -999999;}
  bool pfCandidateRef_longLived() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->longLived() : false;}
  double pfCandidateRef_mass() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mass() : -999999;}
  bool pfCandidateRef_massConstraint() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->massConstraint() : false;}
  double pfCandidateRef_massSqr() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->massSqr() : -999999;}
  double pfCandidateRef_mt() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mt() : -999999;}
  double pfCandidateRef_mtSqr() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mtSqr() : -999999;}
  float pfCandidateRef_mva_e_mu() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mva_e_mu() : -999999;}
  float pfCandidateRef_mva_e_pi() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mva_e_pi() : -999999;}
  float pfCandidateRef_mva_gamma_nh() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mva_gamma_nh() : -999999;}
  float pfCandidateRef_mva_nothing_gamma() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mva_nothing_gamma() : -999999;}
  float pfCandidateRef_mva_nothing_nh() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mva_nothing_nh() : -999999;}
  float pfCandidateRef_mva_pi_mu() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mva_pi_mu() : -999999;}
  size_t pfCandidateRef_numberOfSourceCandidatePtrs() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->numberOfSourceCandidatePtrs() : 999999;}
  double pfCandidateRef_p() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->p() : -999999;}
  double pfCandidateRef_pS1Energy() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->pS1Energy() : -999999;}
  double pfCandidateRef_pS2Energy() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->pS2Energy() : -999999;}
  int pfCandidateRef_pdgId() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->pdgId() : -999999;}
  double pfCandidateRef_phi() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->phi() : -999999;}
  double pfCandidateRef_pt() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->pt() : -999999;}
  double pfCandidateRef_px() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->px() : -999999;}
  double pfCandidateRef_py() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->py() : -999999;}
  double pfCandidateRef_pz() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->pz() : -999999;}
  double pfCandidateRef_rapidity() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->rapidity() : -999999;}
  double pfCandidateRef_rawEcalEnergy() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->rawEcalEnergy() : -999999;}
  double pfCandidateRef_rawHcalEnergy() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->rawHcalEnergy() : -999999;}
  int pfCandidateRef_status() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->status() : -999999;}
  double pfCandidateRef_theta() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->theta() : -999999;}
  int pfCandidateRef_threeCharge() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->threeCharge() : -999999;}
  double pfCandidateRef_vx() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->vx() : -999999;}
  double pfCandidateRef_vy() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->vy() : -999999;}
  double pfCandidateRef_vz() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->vz() : -999999;}
  double pfCandidateRef_y() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->y() : -999999;}
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
  int track_charge() const 	{return track().isNonnull() ? track()->charge() : -999999;}
  double track_chi2() const 	{return track().isNonnull() ? track()->chi2() : -999999;}
  double track_d0() const 	{return track().isNonnull() ? track()->d0() : -999999;}
  double track_d0Error() const 	{return track().isNonnull() ? track()->d0Error() : -999999;}
  double track_dsz() const 	{return track().isNonnull() ? track()->dsz() : -999999;}
  double track_dszError() const 	{return track().isNonnull() ? track()->dszError() : -999999;}
  double track_dxy() const 	{return track().isNonnull() ? track()->dxy() : -999999;}
  double track_dxyError() const 	{return track().isNonnull() ? track()->dxyError() : -999999;}
  double track_dz() const 	{return track().isNonnull() ? track()->dz() : -999999;}
  double track_dzError() const 	{return track().isNonnull() ? track()->dzError() : -999999;}
  double track_eta() const 	{return track().isNonnull() ? track()->eta() : -999999;}
  double track_etaError() const 	{return track().isNonnull() ? track()->etaError() : -999999;}
  unsigned short track_found() const 	{return track().isNonnull() ? track()->found() : 9999;}
  unsigned int track_innerDetId() const 	{return track().isNonnull() ? track()->innerDetId() : 999999;}
  bool track_innerOk() const 	{return track().isNonnull() ? track()->innerOk() : false;}
  double track_lambda() const 	{return track().isNonnull() ? track()->lambda() : -999999;}
  double track_lambdaError() const 	{return track().isNonnull() ? track()->lambdaError() : -999999;}
  unsigned short track_lost() const 	{return track().isNonnull() ? track()->lost() : 9999;}
  double track_ndof() const 	{return track().isNonnull() ? track()->ndof() : -999999;}
  double track_normalizedChi2() const 	{return track().isNonnull() ? track()->normalizedChi2() : -999999;}
  unsigned short track_numberOfLostHits() const 	{return track().isNonnull() ? track()->numberOfLostHits() : 9999;}
  unsigned short track_numberOfValidHits() const 	{return track().isNonnull() ? track()->numberOfValidHits() : 9999;}
  unsigned int track_outerDetId() const 	{return track().isNonnull() ? track()->outerDetId() : 999999;}
  double track_outerEta() const 	{return track().isNonnull() ? track()->outerEta() : -999999;}
  bool track_outerOk() const 	{return track().isNonnull() ? track()->outerOk() : false;}
  double track_outerP() const 	{return track().isNonnull() ? track()->outerP() : -999999;}
  double track_outerPhi() const 	{return track().isNonnull() ? track()->outerPhi() : -999999;}
  double track_outerPt() const 	{return track().isNonnull() ? track()->outerPt() : -999999;}
  double track_outerPx() const 	{return track().isNonnull() ? track()->outerPx() : -999999;}
  double track_outerPy() const 	{return track().isNonnull() ? track()->outerPy() : -999999;}
  double track_outerPz() const 	{return track().isNonnull() ? track()->outerPz() : -999999;}
  double track_outerRadius() const 	{return track().isNonnull() ? track()->outerRadius() : -999999;}
  double track_outerTheta() const 	{return track().isNonnull() ? track()->outerTheta() : -999999;}
  double track_outerX() const 	{return track().isNonnull() ? track()->outerX() : -999999;}
  double track_outerY() const 	{return track().isNonnull() ? track()->outerY() : -999999;}
  double track_outerZ() const 	{return track().isNonnull() ? track()->outerZ() : -999999;}
  double track_p() const 	{return track().isNonnull() ? track()->p() : -999999;}
  double track_phi() const 	{return track().isNonnull() ? track()->phi() : -999999;}
  double track_phiError() const 	{return track().isNonnull() ? track()->phiError() : -999999;}
  double track_pt() const 	{return track().isNonnull() ? track()->pt() : -999999;}
  double track_ptError() const 	{return track().isNonnull() ? track()->ptError() : -999999;}
  double track_px() const 	{return track().isNonnull() ? track()->px() : -999999;}
  double track_py() const 	{return track().isNonnull() ? track()->py() : -999999;}
  double track_pz() const 	{return track().isNonnull() ? track()->pz() : -999999;}
  double track_qoverp() const 	{return track().isNonnull() ? track()->qoverp() : -999999;}
  double track_qoverpError() const 	{return track().isNonnull() ? track()->qoverpError() : -999999;}
  int track_qualityMask() const 	{return track().isNonnull() ? track()->qualityMask() : -999999;}
  size_t track_recHitsSize() const 	{return track().isNonnull() ? track()->recHitsSize() : 999999;}
  double track_theta() const 	{return track().isNonnull() ? track()->theta() : -999999;}
  double track_thetaError() const 	{return track().isNonnull() ? track()->thetaError() : -999999;}
  double track_vx() const 	{return track().isNonnull() ? track()->vx() : -999999;}
  double track_vy() const 	{return track().isNonnull() ? track()->vy() : -999999;}
  double track_vz() const 	{return track().isNonnull() ? track()->vz() : -999999;}
};
#endif

