#ifndef APATTAU_H
#define APATTAU_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       apatTau
// Description: Simple adapter for:
//              class:   pat::Tau
//              header:  DataFormats/PatCandidates/interface/Tau.h
//              
// Created:     Fri Jan 15 21:58:25 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/PatCandidates/interface/Tau.h"
//-----------------------------------------------------------------------------
class apatTau : public pat::Tau
{
 public: 
  apatTau() {}
  apatTau(const pat::Tau& o)
    : pat::Tau(o) {}
  ~apatTau() {}

  int combinedMuon_charge() const 	{return combinedMuon().isNonnull() ? combinedMuon()->charge() : -999999;}
  double combinedMuon_chi2() const 	{return combinedMuon().isNonnull() ? combinedMuon()->chi2() : -999999;}
  double combinedMuon_d0() const 	{return combinedMuon().isNonnull() ? combinedMuon()->d0() : -999999;}
  double combinedMuon_d0Error() const 	{return combinedMuon().isNonnull() ? combinedMuon()->d0Error() : -999999;}
  double combinedMuon_dsz() const 	{return combinedMuon().isNonnull() ? combinedMuon()->dsz() : -999999;}
  double combinedMuon_dszError() const 	{return combinedMuon().isNonnull() ? combinedMuon()->dszError() : -999999;}
  double combinedMuon_dxy() const 	{return combinedMuon().isNonnull() ? combinedMuon()->dxy() : -999999;}
  double combinedMuon_dxyError() const 	{return combinedMuon().isNonnull() ? combinedMuon()->dxyError() : -999999;}
  double combinedMuon_dz() const 	{return combinedMuon().isNonnull() ? combinedMuon()->dz() : -999999;}
  double combinedMuon_dzError() const 	{return combinedMuon().isNonnull() ? combinedMuon()->dzError() : -999999;}
  double combinedMuon_eta() const 	{return combinedMuon().isNonnull() ? combinedMuon()->eta() : -999999;}
  double combinedMuon_etaError() const 	{return combinedMuon().isNonnull() ? combinedMuon()->etaError() : -999999;}
  unsigned short combinedMuon_found() const 	{return combinedMuon().isNonnull() ? combinedMuon()->found() : 9999;}
  unsigned int combinedMuon_innerDetId() const 	{return combinedMuon().isNonnull() ? combinedMuon()->innerDetId() : 999999;}
  bool combinedMuon_innerOk() const 	{return combinedMuon().isNonnull() ? combinedMuon()->innerOk() : false;}
  double combinedMuon_lambda() const 	{return combinedMuon().isNonnull() ? combinedMuon()->lambda() : -999999;}
  double combinedMuon_lambdaError() const 	{return combinedMuon().isNonnull() ? combinedMuon()->lambdaError() : -999999;}
  unsigned short combinedMuon_lost() const 	{return combinedMuon().isNonnull() ? combinedMuon()->lost() : 9999;}
  double combinedMuon_ndof() const 	{return combinedMuon().isNonnull() ? combinedMuon()->ndof() : -999999;}
  double combinedMuon_normalizedChi2() const 	{return combinedMuon().isNonnull() ? combinedMuon()->normalizedChi2() : -999999;}
  unsigned short combinedMuon_numberOfLostHits() const 	{return combinedMuon().isNonnull() ? combinedMuon()->numberOfLostHits() : 9999;}
  unsigned short combinedMuon_numberOfValidHits() const 	{return combinedMuon().isNonnull() ? combinedMuon()->numberOfValidHits() : 9999;}
  unsigned int combinedMuon_outerDetId() const 	{return combinedMuon().isNonnull() ? combinedMuon()->outerDetId() : 999999;}
  double combinedMuon_outerEta() const 	{return combinedMuon().isNonnull() ? combinedMuon()->outerEta() : -999999;}
  bool combinedMuon_outerOk() const 	{return combinedMuon().isNonnull() ? combinedMuon()->outerOk() : false;}
  double combinedMuon_outerP() const 	{return combinedMuon().isNonnull() ? combinedMuon()->outerP() : -999999;}
  double combinedMuon_outerPhi() const 	{return combinedMuon().isNonnull() ? combinedMuon()->outerPhi() : -999999;}
  double combinedMuon_outerPt() const 	{return combinedMuon().isNonnull() ? combinedMuon()->outerPt() : -999999;}
  double combinedMuon_outerPx() const 	{return combinedMuon().isNonnull() ? combinedMuon()->outerPx() : -999999;}
  double combinedMuon_outerPy() const 	{return combinedMuon().isNonnull() ? combinedMuon()->outerPy() : -999999;}
  double combinedMuon_outerPz() const 	{return combinedMuon().isNonnull() ? combinedMuon()->outerPz() : -999999;}
  double combinedMuon_outerRadius() const 	{return combinedMuon().isNonnull() ? combinedMuon()->outerRadius() : -999999;}
  double combinedMuon_outerTheta() const 	{return combinedMuon().isNonnull() ? combinedMuon()->outerTheta() : -999999;}
  double combinedMuon_outerX() const 	{return combinedMuon().isNonnull() ? combinedMuon()->outerX() : -999999;}
  double combinedMuon_outerY() const 	{return combinedMuon().isNonnull() ? combinedMuon()->outerY() : -999999;}
  double combinedMuon_outerZ() const 	{return combinedMuon().isNonnull() ? combinedMuon()->outerZ() : -999999;}
  double combinedMuon_p() const 	{return combinedMuon().isNonnull() ? combinedMuon()->p() : -999999;}
  double combinedMuon_phi() const 	{return combinedMuon().isNonnull() ? combinedMuon()->phi() : -999999;}
  double combinedMuon_phiError() const 	{return combinedMuon().isNonnull() ? combinedMuon()->phiError() : -999999;}
  double combinedMuon_pt() const 	{return combinedMuon().isNonnull() ? combinedMuon()->pt() : -999999;}
  double combinedMuon_ptError() const 	{return combinedMuon().isNonnull() ? combinedMuon()->ptError() : -999999;}
  double combinedMuon_px() const 	{return combinedMuon().isNonnull() ? combinedMuon()->px() : -999999;}
  double combinedMuon_py() const 	{return combinedMuon().isNonnull() ? combinedMuon()->py() : -999999;}
  double combinedMuon_pz() const 	{return combinedMuon().isNonnull() ? combinedMuon()->pz() : -999999;}
  double combinedMuon_qoverp() const 	{return combinedMuon().isNonnull() ? combinedMuon()->qoverp() : -999999;}
  double combinedMuon_qoverpError() const 	{return combinedMuon().isNonnull() ? combinedMuon()->qoverpError() : -999999;}
  int combinedMuon_qualityMask() const 	{return combinedMuon().isNonnull() ? combinedMuon()->qualityMask() : -999999;}
  size_t combinedMuon_recHitsSize() const 	{return combinedMuon().isNonnull() ? combinedMuon()->recHitsSize() : 999999;}
  double combinedMuon_theta() const 	{return combinedMuon().isNonnull() ? combinedMuon()->theta() : -999999;}
  double combinedMuon_thetaError() const 	{return combinedMuon().isNonnull() ? combinedMuon()->thetaError() : -999999;}
  double combinedMuon_vx() const 	{return combinedMuon().isNonnull() ? combinedMuon()->vx() : -999999;}
  double combinedMuon_vy() const 	{return combinedMuon().isNonnull() ? combinedMuon()->vy() : -999999;}
  double combinedMuon_vz() const 	{return combinedMuon().isNonnull() ? combinedMuon()->vz() : -999999;}
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
  int leadTrack_charge() const 	{return leadTrack().isNonnull() ? leadTrack()->charge() : -999999;}
  double leadTrack_chi2() const 	{return leadTrack().isNonnull() ? leadTrack()->chi2() : -999999;}
  double leadTrack_d0() const 	{return leadTrack().isNonnull() ? leadTrack()->d0() : -999999;}
  double leadTrack_d0Error() const 	{return leadTrack().isNonnull() ? leadTrack()->d0Error() : -999999;}
  double leadTrack_dsz() const 	{return leadTrack().isNonnull() ? leadTrack()->dsz() : -999999;}
  double leadTrack_dszError() const 	{return leadTrack().isNonnull() ? leadTrack()->dszError() : -999999;}
  double leadTrack_dxy() const 	{return leadTrack().isNonnull() ? leadTrack()->dxy() : -999999;}
  double leadTrack_dxyError() const 	{return leadTrack().isNonnull() ? leadTrack()->dxyError() : -999999;}
  double leadTrack_dz() const 	{return leadTrack().isNonnull() ? leadTrack()->dz() : -999999;}
  double leadTrack_dzError() const 	{return leadTrack().isNonnull() ? leadTrack()->dzError() : -999999;}
  double leadTrack_eta() const 	{return leadTrack().isNonnull() ? leadTrack()->eta() : -999999;}
  double leadTrack_etaError() const 	{return leadTrack().isNonnull() ? leadTrack()->etaError() : -999999;}
  unsigned short leadTrack_found() const 	{return leadTrack().isNonnull() ? leadTrack()->found() : 9999;}
  unsigned int leadTrack_innerDetId() const 	{return leadTrack().isNonnull() ? leadTrack()->innerDetId() : 999999;}
  bool leadTrack_innerOk() const 	{return leadTrack().isNonnull() ? leadTrack()->innerOk() : false;}
  double leadTrack_lambda() const 	{return leadTrack().isNonnull() ? leadTrack()->lambda() : -999999;}
  double leadTrack_lambdaError() const 	{return leadTrack().isNonnull() ? leadTrack()->lambdaError() : -999999;}
  unsigned short leadTrack_lost() const 	{return leadTrack().isNonnull() ? leadTrack()->lost() : 9999;}
  double leadTrack_ndof() const 	{return leadTrack().isNonnull() ? leadTrack()->ndof() : -999999;}
  double leadTrack_normalizedChi2() const 	{return leadTrack().isNonnull() ? leadTrack()->normalizedChi2() : -999999;}
  unsigned short leadTrack_numberOfLostHits() const 	{return leadTrack().isNonnull() ? leadTrack()->numberOfLostHits() : 9999;}
  unsigned short leadTrack_numberOfValidHits() const 	{return leadTrack().isNonnull() ? leadTrack()->numberOfValidHits() : 9999;}
  unsigned int leadTrack_outerDetId() const 	{return leadTrack().isNonnull() ? leadTrack()->outerDetId() : 999999;}
  double leadTrack_outerEta() const 	{return leadTrack().isNonnull() ? leadTrack()->outerEta() : -999999;}
  bool leadTrack_outerOk() const 	{return leadTrack().isNonnull() ? leadTrack()->outerOk() : false;}
  double leadTrack_outerP() const 	{return leadTrack().isNonnull() ? leadTrack()->outerP() : -999999;}
  double leadTrack_outerPhi() const 	{return leadTrack().isNonnull() ? leadTrack()->outerPhi() : -999999;}
  double leadTrack_outerPt() const 	{return leadTrack().isNonnull() ? leadTrack()->outerPt() : -999999;}
  double leadTrack_outerPx() const 	{return leadTrack().isNonnull() ? leadTrack()->outerPx() : -999999;}
  double leadTrack_outerPy() const 	{return leadTrack().isNonnull() ? leadTrack()->outerPy() : -999999;}
  double leadTrack_outerPz() const 	{return leadTrack().isNonnull() ? leadTrack()->outerPz() : -999999;}
  double leadTrack_outerRadius() const 	{return leadTrack().isNonnull() ? leadTrack()->outerRadius() : -999999;}
  double leadTrack_outerTheta() const 	{return leadTrack().isNonnull() ? leadTrack()->outerTheta() : -999999;}
  double leadTrack_outerX() const 	{return leadTrack().isNonnull() ? leadTrack()->outerX() : -999999;}
  double leadTrack_outerY() const 	{return leadTrack().isNonnull() ? leadTrack()->outerY() : -999999;}
  double leadTrack_outerZ() const 	{return leadTrack().isNonnull() ? leadTrack()->outerZ() : -999999;}
  double leadTrack_p() const 	{return leadTrack().isNonnull() ? leadTrack()->p() : -999999;}
  double leadTrack_phi() const 	{return leadTrack().isNonnull() ? leadTrack()->phi() : -999999;}
  double leadTrack_phiError() const 	{return leadTrack().isNonnull() ? leadTrack()->phiError() : -999999;}
  double leadTrack_pt() const 	{return leadTrack().isNonnull() ? leadTrack()->pt() : -999999;}
  double leadTrack_ptError() const 	{return leadTrack().isNonnull() ? leadTrack()->ptError() : -999999;}
  double leadTrack_px() const 	{return leadTrack().isNonnull() ? leadTrack()->px() : -999999;}
  double leadTrack_py() const 	{return leadTrack().isNonnull() ? leadTrack()->py() : -999999;}
  double leadTrack_pz() const 	{return leadTrack().isNonnull() ? leadTrack()->pz() : -999999;}
  double leadTrack_qoverp() const 	{return leadTrack().isNonnull() ? leadTrack()->qoverp() : -999999;}
  double leadTrack_qoverpError() const 	{return leadTrack().isNonnull() ? leadTrack()->qoverpError() : -999999;}
  int leadTrack_qualityMask() const 	{return leadTrack().isNonnull() ? leadTrack()->qualityMask() : -999999;}
  size_t leadTrack_recHitsSize() const 	{return leadTrack().isNonnull() ? leadTrack()->recHitsSize() : 999999;}
  double leadTrack_theta() const 	{return leadTrack().isNonnull() ? leadTrack()->theta() : -999999;}
  double leadTrack_thetaError() const 	{return leadTrack().isNonnull() ? leadTrack()->thetaError() : -999999;}
  double leadTrack_vx() const 	{return leadTrack().isNonnull() ? leadTrack()->vx() : -999999;}
  double leadTrack_vy() const 	{return leadTrack().isNonnull() ? leadTrack()->vy() : -999999;}
  double leadTrack_vz() const 	{return leadTrack().isNonnull() ? leadTrack()->vz() : -999999;}
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
  int standAloneMuon_charge() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->charge() : -999999;}
  double standAloneMuon_chi2() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->chi2() : -999999;}
  double standAloneMuon_d0() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->d0() : -999999;}
  double standAloneMuon_d0Error() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->d0Error() : -999999;}
  double standAloneMuon_dsz() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->dsz() : -999999;}
  double standAloneMuon_dszError() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->dszError() : -999999;}
  double standAloneMuon_dxy() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->dxy() : -999999;}
  double standAloneMuon_dxyError() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->dxyError() : -999999;}
  double standAloneMuon_dz() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->dz() : -999999;}
  double standAloneMuon_dzError() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->dzError() : -999999;}
  double standAloneMuon_eta() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->eta() : -999999;}
  double standAloneMuon_etaError() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->etaError() : -999999;}
  unsigned short standAloneMuon_found() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->found() : 9999;}
  unsigned int standAloneMuon_innerDetId() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->innerDetId() : 999999;}
  bool standAloneMuon_innerOk() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->innerOk() : false;}
  double standAloneMuon_lambda() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->lambda() : -999999;}
  double standAloneMuon_lambdaError() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->lambdaError() : -999999;}
  unsigned short standAloneMuon_lost() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->lost() : 9999;}
  double standAloneMuon_ndof() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->ndof() : -999999;}
  double standAloneMuon_normalizedChi2() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->normalizedChi2() : -999999;}
  unsigned short standAloneMuon_numberOfLostHits() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->numberOfLostHits() : 9999;}
  unsigned short standAloneMuon_numberOfValidHits() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->numberOfValidHits() : 9999;}
  unsigned int standAloneMuon_outerDetId() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->outerDetId() : 999999;}
  double standAloneMuon_outerEta() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->outerEta() : -999999;}
  bool standAloneMuon_outerOk() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->outerOk() : false;}
  double standAloneMuon_outerP() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->outerP() : -999999;}
  double standAloneMuon_outerPhi() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->outerPhi() : -999999;}
  double standAloneMuon_outerPt() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->outerPt() : -999999;}
  double standAloneMuon_outerPx() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->outerPx() : -999999;}
  double standAloneMuon_outerPy() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->outerPy() : -999999;}
  double standAloneMuon_outerPz() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->outerPz() : -999999;}
  double standAloneMuon_outerRadius() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->outerRadius() : -999999;}
  double standAloneMuon_outerTheta() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->outerTheta() : -999999;}
  double standAloneMuon_outerX() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->outerX() : -999999;}
  double standAloneMuon_outerY() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->outerY() : -999999;}
  double standAloneMuon_outerZ() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->outerZ() : -999999;}
  double standAloneMuon_p() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->p() : -999999;}
  double standAloneMuon_phi() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->phi() : -999999;}
  double standAloneMuon_phiError() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->phiError() : -999999;}
  double standAloneMuon_pt() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->pt() : -999999;}
  double standAloneMuon_ptError() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->ptError() : -999999;}
  double standAloneMuon_px() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->px() : -999999;}
  double standAloneMuon_py() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->py() : -999999;}
  double standAloneMuon_pz() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->pz() : -999999;}
  double standAloneMuon_qoverp() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->qoverp() : -999999;}
  double standAloneMuon_qoverpError() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->qoverpError() : -999999;}
  int standAloneMuon_qualityMask() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->qualityMask() : -999999;}
  size_t standAloneMuon_recHitsSize() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->recHitsSize() : 999999;}
  double standAloneMuon_theta() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->theta() : -999999;}
  double standAloneMuon_thetaError() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->thetaError() : -999999;}
  double standAloneMuon_vx() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->vx() : -999999;}
  double standAloneMuon_vy() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->vy() : -999999;}
  double standAloneMuon_vz() const 	{return standAloneMuon().isNonnull() ? standAloneMuon()->vz() : -999999;}
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

