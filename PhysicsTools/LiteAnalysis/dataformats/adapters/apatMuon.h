#ifndef APATMUON_H
#define APATMUON_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       apatMuon
// Description: Simple adapter for:
//              class:   pat::Muon
//              header:  DataFormats/PatCandidates/interface/Muon.h
//              
// Created:     Thu Jan 14 23:48:23 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/PatCandidates/interface/Muon.h"
//-----------------------------------------------------------------------------
class apatMuon : public pat::Muon
{
 public: 
  apatMuon() {}
  apatMuon(const pat::Muon& o)
    : pat::Muon(o) {}
  ~apatMuon() {}

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
  double ecalIsoDeposit_candEnergy() const 	{return ecalIsoDeposit() ? ecalIsoDeposit()->candEnergy() : -999999;}
  double ecalIsoDeposit_eta() const 	{return ecalIsoDeposit() ? ecalIsoDeposit()->eta() : -999999;}
  double ecalIsoDeposit_phi() const 	{return ecalIsoDeposit() ? ecalIsoDeposit()->phi() : -999999;}
  float efficiency_error(const std::string x) const 	{return efficiency(x).error();}
  float efficiency_value(const std::string x) const 	{return efficiency(x).value();}
  int genLepton_charge() const 	{return genLepton() ? genLepton()->charge() : -999999;}
  double genLepton_energy() const 	{return genLepton() ? genLepton()->energy() : -999999;}
  double genLepton_et() const 	{return genLepton() ? genLepton()->et() : -999999;}
  double genLepton_eta() const 	{return genLepton() ? genLepton()->eta() : -999999;}
  bool genLepton_longLived() const 	{return genLepton() ? genLepton()->longLived() : false;}
  double genLepton_mass() const 	{return genLepton() ? genLepton()->mass() : -999999;}
  double genLepton_massSqr() const 	{return genLepton() ? genLepton()->massSqr() : -999999;}
  double genLepton_mt() const 	{return genLepton() ? genLepton()->mt() : -999999;}
  double genLepton_mtSqr() const 	{return genLepton() ? genLepton()->mtSqr() : -999999;}
  size_t genLepton_numberOfDaughters() const 	{return genLepton() ? genLepton()->numberOfDaughters() : 999999;}
  size_t genLepton_numberOfMothers() const 	{return genLepton() ? genLepton()->numberOfMothers() : 999999;}
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
  double genParticle_energy(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->energy() : -999999;}
  double genParticle_et(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->et() : -999999;}
  double genParticle_eta(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->eta() : -999999;}
  bool genParticle_longLived(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->longLived() : false;}
  double genParticle_mass(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->mass() : -999999;}
  double genParticle_massSqr(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->massSqr() : -999999;}
  double genParticle_mt(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->mt() : -999999;}
  double genParticle_mtSqr(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->mtSqr() : -999999;}
  size_t genParticle_numberOfDaughters(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->numberOfDaughters() : 999999;}
  size_t genParticle_numberOfMothers(size_t x=0) const 	{return genParticle(x) ? genParticle(x)->numberOfMothers() : 999999;}
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
  double genParticleRef_energy(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->energy() : -999999;}
  double genParticleRef_et(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->et() : -999999;}
  double genParticleRef_eta(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->eta() : -999999;}
  bool genParticleRef_longLived(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->longLived() : false;}
  double genParticleRef_mass(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->mass() : -999999;}
  double genParticleRef_massSqr(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->massSqr() : -999999;}
  double genParticleRef_mt(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->mt() : -999999;}
  double genParticleRef_mtSqr(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->mtSqr() : -999999;}
  size_t genParticleRef_numberOfDaughters(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->numberOfDaughters() : 999999;}
  size_t genParticleRef_numberOfMothers(size_t x=0) const 	{return genParticleRef(x).isNonnull() ? genParticleRef(x)->numberOfMothers() : 999999;}
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
  int globalTrack_charge() const 	{return globalTrack().isNonnull() ? globalTrack()->charge() : -999999;}
  double globalTrack_chi2() const 	{return globalTrack().isNonnull() ? globalTrack()->chi2() : -999999;}
  double globalTrack_d0() const 	{return globalTrack().isNonnull() ? globalTrack()->d0() : -999999;}
  double globalTrack_d0Error() const 	{return globalTrack().isNonnull() ? globalTrack()->d0Error() : -999999;}
  double globalTrack_dsz() const 	{return globalTrack().isNonnull() ? globalTrack()->dsz() : -999999;}
  double globalTrack_dszError() const 	{return globalTrack().isNonnull() ? globalTrack()->dszError() : -999999;}
  double globalTrack_dxy() const 	{return globalTrack().isNonnull() ? globalTrack()->dxy() : -999999;}
  double globalTrack_dxyError() const 	{return globalTrack().isNonnull() ? globalTrack()->dxyError() : -999999;}
  double globalTrack_dz() const 	{return globalTrack().isNonnull() ? globalTrack()->dz() : -999999;}
  double globalTrack_dzError() const 	{return globalTrack().isNonnull() ? globalTrack()->dzError() : -999999;}
  double globalTrack_eta() const 	{return globalTrack().isNonnull() ? globalTrack()->eta() : -999999;}
  double globalTrack_etaError() const 	{return globalTrack().isNonnull() ? globalTrack()->etaError() : -999999;}
  unsigned short globalTrack_found() const 	{return globalTrack().isNonnull() ? globalTrack()->found() : 9999;}
  unsigned int globalTrack_innerDetId() const 	{return globalTrack().isNonnull() ? globalTrack()->innerDetId() : 999999;}
  bool globalTrack_innerOk() const 	{return globalTrack().isNonnull() ? globalTrack()->innerOk() : false;}
  double globalTrack_lambda() const 	{return globalTrack().isNonnull() ? globalTrack()->lambda() : -999999;}
  double globalTrack_lambdaError() const 	{return globalTrack().isNonnull() ? globalTrack()->lambdaError() : -999999;}
  unsigned short globalTrack_lost() const 	{return globalTrack().isNonnull() ? globalTrack()->lost() : 9999;}
  double globalTrack_ndof() const 	{return globalTrack().isNonnull() ? globalTrack()->ndof() : -999999;}
  double globalTrack_normalizedChi2() const 	{return globalTrack().isNonnull() ? globalTrack()->normalizedChi2() : -999999;}
  unsigned short globalTrack_numberOfLostHits() const 	{return globalTrack().isNonnull() ? globalTrack()->numberOfLostHits() : 9999;}
  unsigned short globalTrack_numberOfValidHits() const 	{return globalTrack().isNonnull() ? globalTrack()->numberOfValidHits() : 9999;}
  unsigned int globalTrack_outerDetId() const 	{return globalTrack().isNonnull() ? globalTrack()->outerDetId() : 999999;}
  double globalTrack_outerEta() const 	{return globalTrack().isNonnull() ? globalTrack()->outerEta() : -999999;}
  bool globalTrack_outerOk() const 	{return globalTrack().isNonnull() ? globalTrack()->outerOk() : false;}
  double globalTrack_outerP() const 	{return globalTrack().isNonnull() ? globalTrack()->outerP() : -999999;}
  double globalTrack_outerPhi() const 	{return globalTrack().isNonnull() ? globalTrack()->outerPhi() : -999999;}
  double globalTrack_outerPt() const 	{return globalTrack().isNonnull() ? globalTrack()->outerPt() : -999999;}
  double globalTrack_outerPx() const 	{return globalTrack().isNonnull() ? globalTrack()->outerPx() : -999999;}
  double globalTrack_outerPy() const 	{return globalTrack().isNonnull() ? globalTrack()->outerPy() : -999999;}
  double globalTrack_outerPz() const 	{return globalTrack().isNonnull() ? globalTrack()->outerPz() : -999999;}
  double globalTrack_outerRadius() const 	{return globalTrack().isNonnull() ? globalTrack()->outerRadius() : -999999;}
  double globalTrack_outerTheta() const 	{return globalTrack().isNonnull() ? globalTrack()->outerTheta() : -999999;}
  double globalTrack_outerX() const 	{return globalTrack().isNonnull() ? globalTrack()->outerX() : -999999;}
  double globalTrack_outerY() const 	{return globalTrack().isNonnull() ? globalTrack()->outerY() : -999999;}
  double globalTrack_outerZ() const 	{return globalTrack().isNonnull() ? globalTrack()->outerZ() : -999999;}
  double globalTrack_p() const 	{return globalTrack().isNonnull() ? globalTrack()->p() : -999999;}
  double globalTrack_phi() const 	{return globalTrack().isNonnull() ? globalTrack()->phi() : -999999;}
  double globalTrack_phiError() const 	{return globalTrack().isNonnull() ? globalTrack()->phiError() : -999999;}
  double globalTrack_pt() const 	{return globalTrack().isNonnull() ? globalTrack()->pt() : -999999;}
  double globalTrack_ptError() const 	{return globalTrack().isNonnull() ? globalTrack()->ptError() : -999999;}
  double globalTrack_px() const 	{return globalTrack().isNonnull() ? globalTrack()->px() : -999999;}
  double globalTrack_py() const 	{return globalTrack().isNonnull() ? globalTrack()->py() : -999999;}
  double globalTrack_pz() const 	{return globalTrack().isNonnull() ? globalTrack()->pz() : -999999;}
  double globalTrack_qoverp() const 	{return globalTrack().isNonnull() ? globalTrack()->qoverp() : -999999;}
  double globalTrack_qoverpError() const 	{return globalTrack().isNonnull() ? globalTrack()->qoverpError() : -999999;}
  int globalTrack_qualityMask() const 	{return globalTrack().isNonnull() ? globalTrack()->qualityMask() : -999999;}
  size_t globalTrack_recHitsSize() const 	{return globalTrack().isNonnull() ? globalTrack()->recHitsSize() : 999999;}
  double globalTrack_theta() const 	{return globalTrack().isNonnull() ? globalTrack()->theta() : -999999;}
  double globalTrack_thetaError() const 	{return globalTrack().isNonnull() ? globalTrack()->thetaError() : -999999;}
  double globalTrack_vx() const 	{return globalTrack().isNonnull() ? globalTrack()->vx() : -999999;}
  double globalTrack_vy() const 	{return globalTrack().isNonnull() ? globalTrack()->vy() : -999999;}
  double globalTrack_vz() const 	{return globalTrack().isNonnull() ? globalTrack()->vz() : -999999;}
  double hcalIsoDeposit_candEnergy() const 	{return hcalIsoDeposit() ? hcalIsoDeposit()->candEnergy() : -999999;}
  double hcalIsoDeposit_eta() const 	{return hcalIsoDeposit() ? hcalIsoDeposit()->eta() : -999999;}
  double hcalIsoDeposit_phi() const 	{return hcalIsoDeposit() ? hcalIsoDeposit()->phi() : -999999;}
  int innerTrack_charge() const 	{return innerTrack().isNonnull() ? innerTrack()->charge() : -999999;}
  double innerTrack_chi2() const 	{return innerTrack().isNonnull() ? innerTrack()->chi2() : -999999;}
  double innerTrack_d0() const 	{return innerTrack().isNonnull() ? innerTrack()->d0() : -999999;}
  double innerTrack_d0Error() const 	{return innerTrack().isNonnull() ? innerTrack()->d0Error() : -999999;}
  double innerTrack_dsz() const 	{return innerTrack().isNonnull() ? innerTrack()->dsz() : -999999;}
  double innerTrack_dszError() const 	{return innerTrack().isNonnull() ? innerTrack()->dszError() : -999999;}
  double innerTrack_dxy() const 	{return innerTrack().isNonnull() ? innerTrack()->dxy() : -999999;}
  double innerTrack_dxyError() const 	{return innerTrack().isNonnull() ? innerTrack()->dxyError() : -999999;}
  double innerTrack_dz() const 	{return innerTrack().isNonnull() ? innerTrack()->dz() : -999999;}
  double innerTrack_dzError() const 	{return innerTrack().isNonnull() ? innerTrack()->dzError() : -999999;}
  double innerTrack_eta() const 	{return innerTrack().isNonnull() ? innerTrack()->eta() : -999999;}
  double innerTrack_etaError() const 	{return innerTrack().isNonnull() ? innerTrack()->etaError() : -999999;}
  unsigned short innerTrack_found() const 	{return innerTrack().isNonnull() ? innerTrack()->found() : 9999;}
  unsigned int innerTrack_innerDetId() const 	{return innerTrack().isNonnull() ? innerTrack()->innerDetId() : 999999;}
  bool innerTrack_innerOk() const 	{return innerTrack().isNonnull() ? innerTrack()->innerOk() : false;}
  double innerTrack_lambda() const 	{return innerTrack().isNonnull() ? innerTrack()->lambda() : -999999;}
  double innerTrack_lambdaError() const 	{return innerTrack().isNonnull() ? innerTrack()->lambdaError() : -999999;}
  unsigned short innerTrack_lost() const 	{return innerTrack().isNonnull() ? innerTrack()->lost() : 9999;}
  double innerTrack_ndof() const 	{return innerTrack().isNonnull() ? innerTrack()->ndof() : -999999;}
  double innerTrack_normalizedChi2() const 	{return innerTrack().isNonnull() ? innerTrack()->normalizedChi2() : -999999;}
  unsigned short innerTrack_numberOfLostHits() const 	{return innerTrack().isNonnull() ? innerTrack()->numberOfLostHits() : 9999;}
  unsigned short innerTrack_numberOfValidHits() const 	{return innerTrack().isNonnull() ? innerTrack()->numberOfValidHits() : 9999;}
  unsigned int innerTrack_outerDetId() const 	{return innerTrack().isNonnull() ? innerTrack()->outerDetId() : 999999;}
  double innerTrack_outerEta() const 	{return innerTrack().isNonnull() ? innerTrack()->outerEta() : -999999;}
  bool innerTrack_outerOk() const 	{return innerTrack().isNonnull() ? innerTrack()->outerOk() : false;}
  double innerTrack_outerP() const 	{return innerTrack().isNonnull() ? innerTrack()->outerP() : -999999;}
  double innerTrack_outerPhi() const 	{return innerTrack().isNonnull() ? innerTrack()->outerPhi() : -999999;}
  double innerTrack_outerPt() const 	{return innerTrack().isNonnull() ? innerTrack()->outerPt() : -999999;}
  double innerTrack_outerPx() const 	{return innerTrack().isNonnull() ? innerTrack()->outerPx() : -999999;}
  double innerTrack_outerPy() const 	{return innerTrack().isNonnull() ? innerTrack()->outerPy() : -999999;}
  double innerTrack_outerPz() const 	{return innerTrack().isNonnull() ? innerTrack()->outerPz() : -999999;}
  double innerTrack_outerRadius() const 	{return innerTrack().isNonnull() ? innerTrack()->outerRadius() : -999999;}
  double innerTrack_outerTheta() const 	{return innerTrack().isNonnull() ? innerTrack()->outerTheta() : -999999;}
  double innerTrack_outerX() const 	{return innerTrack().isNonnull() ? innerTrack()->outerX() : -999999;}
  double innerTrack_outerY() const 	{return innerTrack().isNonnull() ? innerTrack()->outerY() : -999999;}
  double innerTrack_outerZ() const 	{return innerTrack().isNonnull() ? innerTrack()->outerZ() : -999999;}
  double innerTrack_p() const 	{return innerTrack().isNonnull() ? innerTrack()->p() : -999999;}
  double innerTrack_phi() const 	{return innerTrack().isNonnull() ? innerTrack()->phi() : -999999;}
  double innerTrack_phiError() const 	{return innerTrack().isNonnull() ? innerTrack()->phiError() : -999999;}
  double innerTrack_pt() const 	{return innerTrack().isNonnull() ? innerTrack()->pt() : -999999;}
  double innerTrack_ptError() const 	{return innerTrack().isNonnull() ? innerTrack()->ptError() : -999999;}
  double innerTrack_px() const 	{return innerTrack().isNonnull() ? innerTrack()->px() : -999999;}
  double innerTrack_py() const 	{return innerTrack().isNonnull() ? innerTrack()->py() : -999999;}
  double innerTrack_pz() const 	{return innerTrack().isNonnull() ? innerTrack()->pz() : -999999;}
  double innerTrack_qoverp() const 	{return innerTrack().isNonnull() ? innerTrack()->qoverp() : -999999;}
  double innerTrack_qoverpError() const 	{return innerTrack().isNonnull() ? innerTrack()->qoverpError() : -999999;}
  int innerTrack_qualityMask() const 	{return innerTrack().isNonnull() ? innerTrack()->qualityMask() : -999999;}
  size_t innerTrack_recHitsSize() const 	{return innerTrack().isNonnull() ? innerTrack()->recHitsSize() : 999999;}
  double innerTrack_theta() const 	{return innerTrack().isNonnull() ? innerTrack()->theta() : -999999;}
  double innerTrack_thetaError() const 	{return innerTrack().isNonnull() ? innerTrack()->thetaError() : -999999;}
  double innerTrack_vx() const 	{return innerTrack().isNonnull() ? innerTrack()->vx() : -999999;}
  double innerTrack_vy() const 	{return innerTrack().isNonnull() ? innerTrack()->vy() : -999999;}
  double innerTrack_vz() const 	{return innerTrack().isNonnull() ? innerTrack()->vz() : -999999;}
  int originalObject_charge() const 	{return originalObject() ? originalObject()->charge() : -999999;}
  double originalObject_energy() const 	{return originalObject() ? originalObject()->energy() : -999999;}
  double originalObject_et() const 	{return originalObject() ? originalObject()->et() : -999999;}
  double originalObject_eta() const 	{return originalObject() ? originalObject()->eta() : -999999;}
  bool originalObject_hasMasterClone() const 	{return originalObject() ? originalObject()->hasMasterClone() : false;}
  bool originalObject_hasMasterClonePtr() const 	{return originalObject() ? originalObject()->hasMasterClonePtr() : false;}
  bool originalObject_isCaloMuon() const 	{return originalObject() ? originalObject()->isCaloMuon() : false;}
  bool originalObject_isConvertedPhoton() const 	{return originalObject() ? originalObject()->isConvertedPhoton() : false;}
  bool originalObject_isElectron() const 	{return originalObject() ? originalObject()->isElectron() : false;}
  bool originalObject_isGlobalMuon() const 	{return originalObject() ? originalObject()->isGlobalMuon() : false;}
  bool originalObject_isJet() const 	{return originalObject() ? originalObject()->isJet() : false;}
  bool originalObject_isMuon() const 	{return originalObject() ? originalObject()->isMuon() : false;}
  bool originalObject_isPhoton() const 	{return originalObject() ? originalObject()->isPhoton() : false;}
  bool originalObject_isStandAloneMuon() const 	{return originalObject() ? originalObject()->isStandAloneMuon() : false;}
  bool originalObject_isTrackerMuon() const 	{return originalObject() ? originalObject()->isTrackerMuon() : false;}
  bool originalObject_longLived() const 	{return originalObject() ? originalObject()->longLived() : false;}
  double originalObject_mass() const 	{return originalObject() ? originalObject()->mass() : -999999;}
  double originalObject_massSqr() const 	{return originalObject() ? originalObject()->massSqr() : -999999;}
  double originalObject_mt() const 	{return originalObject() ? originalObject()->mt() : -999999;}
  double originalObject_mtSqr() const 	{return originalObject() ? originalObject()->mtSqr() : -999999;}
  size_t originalObject_numberOfDaughters() const 	{return originalObject() ? originalObject()->numberOfDaughters() : 999999;}
  size_t originalObject_numberOfMothers() const 	{return originalObject() ? originalObject()->numberOfMothers() : 999999;}
  size_t originalObject_numberOfSourceCandidatePtrs() const 	{return originalObject() ? originalObject()->numberOfSourceCandidatePtrs() : 999999;}
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
  double originalObject_vertexChi2() const 	{return originalObject() ? originalObject()->vertexChi2() : -999999;}
  double originalObject_vertexNdof() const 	{return originalObject() ? originalObject()->vertexNdof() : -999999;}
  double originalObject_vertexNormalizedChi2() const 	{return originalObject() ? originalObject()->vertexNormalizedChi2() : -999999;}
  double originalObject_vx() const 	{return originalObject() ? originalObject()->vx() : -999999;}
  double originalObject_vy() const 	{return originalObject() ? originalObject()->vy() : -999999;}
  double originalObject_vz() const 	{return originalObject() ? originalObject()->vz() : -999999;}
  double originalObject_y() const 	{return originalObject() ? originalObject()->y() : -999999;}
  int outerTrack_charge() const 	{return outerTrack().isNonnull() ? outerTrack()->charge() : -999999;}
  double outerTrack_chi2() const 	{return outerTrack().isNonnull() ? outerTrack()->chi2() : -999999;}
  double outerTrack_d0() const 	{return outerTrack().isNonnull() ? outerTrack()->d0() : -999999;}
  double outerTrack_d0Error() const 	{return outerTrack().isNonnull() ? outerTrack()->d0Error() : -999999;}
  double outerTrack_dsz() const 	{return outerTrack().isNonnull() ? outerTrack()->dsz() : -999999;}
  double outerTrack_dszError() const 	{return outerTrack().isNonnull() ? outerTrack()->dszError() : -999999;}
  double outerTrack_dxy() const 	{return outerTrack().isNonnull() ? outerTrack()->dxy() : -999999;}
  double outerTrack_dxyError() const 	{return outerTrack().isNonnull() ? outerTrack()->dxyError() : -999999;}
  double outerTrack_dz() const 	{return outerTrack().isNonnull() ? outerTrack()->dz() : -999999;}
  double outerTrack_dzError() const 	{return outerTrack().isNonnull() ? outerTrack()->dzError() : -999999;}
  double outerTrack_eta() const 	{return outerTrack().isNonnull() ? outerTrack()->eta() : -999999;}
  double outerTrack_etaError() const 	{return outerTrack().isNonnull() ? outerTrack()->etaError() : -999999;}
  unsigned short outerTrack_found() const 	{return outerTrack().isNonnull() ? outerTrack()->found() : 9999;}
  unsigned int outerTrack_innerDetId() const 	{return outerTrack().isNonnull() ? outerTrack()->innerDetId() : 999999;}
  bool outerTrack_innerOk() const 	{return outerTrack().isNonnull() ? outerTrack()->innerOk() : false;}
  double outerTrack_lambda() const 	{return outerTrack().isNonnull() ? outerTrack()->lambda() : -999999;}
  double outerTrack_lambdaError() const 	{return outerTrack().isNonnull() ? outerTrack()->lambdaError() : -999999;}
  unsigned short outerTrack_lost() const 	{return outerTrack().isNonnull() ? outerTrack()->lost() : 9999;}
  double outerTrack_ndof() const 	{return outerTrack().isNonnull() ? outerTrack()->ndof() : -999999;}
  double outerTrack_normalizedChi2() const 	{return outerTrack().isNonnull() ? outerTrack()->normalizedChi2() : -999999;}
  unsigned short outerTrack_numberOfLostHits() const 	{return outerTrack().isNonnull() ? outerTrack()->numberOfLostHits() : 9999;}
  unsigned short outerTrack_numberOfValidHits() const 	{return outerTrack().isNonnull() ? outerTrack()->numberOfValidHits() : 9999;}
  unsigned int outerTrack_outerDetId() const 	{return outerTrack().isNonnull() ? outerTrack()->outerDetId() : 999999;}
  double outerTrack_outerEta() const 	{return outerTrack().isNonnull() ? outerTrack()->outerEta() : -999999;}
  bool outerTrack_outerOk() const 	{return outerTrack().isNonnull() ? outerTrack()->outerOk() : false;}
  double outerTrack_outerP() const 	{return outerTrack().isNonnull() ? outerTrack()->outerP() : -999999;}
  double outerTrack_outerPhi() const 	{return outerTrack().isNonnull() ? outerTrack()->outerPhi() : -999999;}
  double outerTrack_outerPt() const 	{return outerTrack().isNonnull() ? outerTrack()->outerPt() : -999999;}
  double outerTrack_outerPx() const 	{return outerTrack().isNonnull() ? outerTrack()->outerPx() : -999999;}
  double outerTrack_outerPy() const 	{return outerTrack().isNonnull() ? outerTrack()->outerPy() : -999999;}
  double outerTrack_outerPz() const 	{return outerTrack().isNonnull() ? outerTrack()->outerPz() : -999999;}
  double outerTrack_outerRadius() const 	{return outerTrack().isNonnull() ? outerTrack()->outerRadius() : -999999;}
  double outerTrack_outerTheta() const 	{return outerTrack().isNonnull() ? outerTrack()->outerTheta() : -999999;}
  double outerTrack_outerX() const 	{return outerTrack().isNonnull() ? outerTrack()->outerX() : -999999;}
  double outerTrack_outerY() const 	{return outerTrack().isNonnull() ? outerTrack()->outerY() : -999999;}
  double outerTrack_outerZ() const 	{return outerTrack().isNonnull() ? outerTrack()->outerZ() : -999999;}
  double outerTrack_p() const 	{return outerTrack().isNonnull() ? outerTrack()->p() : -999999;}
  double outerTrack_phi() const 	{return outerTrack().isNonnull() ? outerTrack()->phi() : -999999;}
  double outerTrack_phiError() const 	{return outerTrack().isNonnull() ? outerTrack()->phiError() : -999999;}
  double outerTrack_pt() const 	{return outerTrack().isNonnull() ? outerTrack()->pt() : -999999;}
  double outerTrack_ptError() const 	{return outerTrack().isNonnull() ? outerTrack()->ptError() : -999999;}
  double outerTrack_px() const 	{return outerTrack().isNonnull() ? outerTrack()->px() : -999999;}
  double outerTrack_py() const 	{return outerTrack().isNonnull() ? outerTrack()->py() : -999999;}
  double outerTrack_pz() const 	{return outerTrack().isNonnull() ? outerTrack()->pz() : -999999;}
  double outerTrack_qoverp() const 	{return outerTrack().isNonnull() ? outerTrack()->qoverp() : -999999;}
  double outerTrack_qoverpError() const 	{return outerTrack().isNonnull() ? outerTrack()->qoverpError() : -999999;}
  int outerTrack_qualityMask() const 	{return outerTrack().isNonnull() ? outerTrack()->qualityMask() : -999999;}
  size_t outerTrack_recHitsSize() const 	{return outerTrack().isNonnull() ? outerTrack()->recHitsSize() : 999999;}
  double outerTrack_theta() const 	{return outerTrack().isNonnull() ? outerTrack()->theta() : -999999;}
  double outerTrack_thetaError() const 	{return outerTrack().isNonnull() ? outerTrack()->thetaError() : -999999;}
  double outerTrack_vx() const 	{return outerTrack().isNonnull() ? outerTrack()->vx() : -999999;}
  double outerTrack_vy() const 	{return outerTrack().isNonnull() ? outerTrack()->vy() : -999999;}
  double outerTrack_vz() const 	{return outerTrack().isNonnull() ? outerTrack()->vz() : -999999;}
  int pfCandidateRef_charge() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->charge() : -999999;}
  double pfCandidateRef_deltaP() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->deltaP() : -999999;}
  double pfCandidateRef_ecalEnergy() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->ecalEnergy() : -999999;}
  double pfCandidateRef_energy() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->energy() : -999999;}
  double pfCandidateRef_et() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->et() : -999999;}
  double pfCandidateRef_eta() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->eta() : -999999;}
  double pfCandidateRef_hcalEnergy() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->hcalEnergy() : -999999;}
  bool pfCandidateRef_longLived() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->longLived() : false;}
  double pfCandidateRef_mass() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mass() : -999999;}
  double pfCandidateRef_massSqr() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->massSqr() : -999999;}
  double pfCandidateRef_mt() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mt() : -999999;}
  double pfCandidateRef_mtSqr() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mtSqr() : -999999;}
  float pfCandidateRef_mva_e_mu() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mva_e_mu() : -999999;}
  float pfCandidateRef_mva_e_pi() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mva_e_pi() : -999999;}
  float pfCandidateRef_mva_gamma_nh() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mva_gamma_nh() : -999999;}
  float pfCandidateRef_mva_nothing_gamma() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mva_nothing_gamma() : -999999;}
  float pfCandidateRef_mva_nothing_nh() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mva_nothing_nh() : -999999;}
  float pfCandidateRef_mva_pi_mu() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->mva_pi_mu() : -999999;}
  size_t pfCandidateRef_numberOfDaughters() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->numberOfDaughters() : 999999;}
  size_t pfCandidateRef_numberOfMothers() const 	{return pfCandidateRef().isNonnull() ? pfCandidateRef()->numberOfMothers() : 999999;}
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
  int pickyMuon_charge() const 	{return pickyMuon().isNonnull() ? pickyMuon()->charge() : -999999;}
  double pickyMuon_chi2() const 	{return pickyMuon().isNonnull() ? pickyMuon()->chi2() : -999999;}
  double pickyMuon_d0() const 	{return pickyMuon().isNonnull() ? pickyMuon()->d0() : -999999;}
  double pickyMuon_d0Error() const 	{return pickyMuon().isNonnull() ? pickyMuon()->d0Error() : -999999;}
  double pickyMuon_dsz() const 	{return pickyMuon().isNonnull() ? pickyMuon()->dsz() : -999999;}
  double pickyMuon_dszError() const 	{return pickyMuon().isNonnull() ? pickyMuon()->dszError() : -999999;}
  double pickyMuon_dxy() const 	{return pickyMuon().isNonnull() ? pickyMuon()->dxy() : -999999;}
  double pickyMuon_dxyError() const 	{return pickyMuon().isNonnull() ? pickyMuon()->dxyError() : -999999;}
  double pickyMuon_dz() const 	{return pickyMuon().isNonnull() ? pickyMuon()->dz() : -999999;}
  double pickyMuon_dzError() const 	{return pickyMuon().isNonnull() ? pickyMuon()->dzError() : -999999;}
  double pickyMuon_eta() const 	{return pickyMuon().isNonnull() ? pickyMuon()->eta() : -999999;}
  double pickyMuon_etaError() const 	{return pickyMuon().isNonnull() ? pickyMuon()->etaError() : -999999;}
  unsigned short pickyMuon_found() const 	{return pickyMuon().isNonnull() ? pickyMuon()->found() : 9999;}
  unsigned int pickyMuon_innerDetId() const 	{return pickyMuon().isNonnull() ? pickyMuon()->innerDetId() : 999999;}
  bool pickyMuon_innerOk() const 	{return pickyMuon().isNonnull() ? pickyMuon()->innerOk() : false;}
  double pickyMuon_lambda() const 	{return pickyMuon().isNonnull() ? pickyMuon()->lambda() : -999999;}
  double pickyMuon_lambdaError() const 	{return pickyMuon().isNonnull() ? pickyMuon()->lambdaError() : -999999;}
  unsigned short pickyMuon_lost() const 	{return pickyMuon().isNonnull() ? pickyMuon()->lost() : 9999;}
  double pickyMuon_ndof() const 	{return pickyMuon().isNonnull() ? pickyMuon()->ndof() : -999999;}
  double pickyMuon_normalizedChi2() const 	{return pickyMuon().isNonnull() ? pickyMuon()->normalizedChi2() : -999999;}
  unsigned short pickyMuon_numberOfLostHits() const 	{return pickyMuon().isNonnull() ? pickyMuon()->numberOfLostHits() : 9999;}
  unsigned short pickyMuon_numberOfValidHits() const 	{return pickyMuon().isNonnull() ? pickyMuon()->numberOfValidHits() : 9999;}
  unsigned int pickyMuon_outerDetId() const 	{return pickyMuon().isNonnull() ? pickyMuon()->outerDetId() : 999999;}
  double pickyMuon_outerEta() const 	{return pickyMuon().isNonnull() ? pickyMuon()->outerEta() : -999999;}
  bool pickyMuon_outerOk() const 	{return pickyMuon().isNonnull() ? pickyMuon()->outerOk() : false;}
  double pickyMuon_outerP() const 	{return pickyMuon().isNonnull() ? pickyMuon()->outerP() : -999999;}
  double pickyMuon_outerPhi() const 	{return pickyMuon().isNonnull() ? pickyMuon()->outerPhi() : -999999;}
  double pickyMuon_outerPt() const 	{return pickyMuon().isNonnull() ? pickyMuon()->outerPt() : -999999;}
  double pickyMuon_outerPx() const 	{return pickyMuon().isNonnull() ? pickyMuon()->outerPx() : -999999;}
  double pickyMuon_outerPy() const 	{return pickyMuon().isNonnull() ? pickyMuon()->outerPy() : -999999;}
  double pickyMuon_outerPz() const 	{return pickyMuon().isNonnull() ? pickyMuon()->outerPz() : -999999;}
  double pickyMuon_outerRadius() const 	{return pickyMuon().isNonnull() ? pickyMuon()->outerRadius() : -999999;}
  double pickyMuon_outerTheta() const 	{return pickyMuon().isNonnull() ? pickyMuon()->outerTheta() : -999999;}
  double pickyMuon_outerX() const 	{return pickyMuon().isNonnull() ? pickyMuon()->outerX() : -999999;}
  double pickyMuon_outerY() const 	{return pickyMuon().isNonnull() ? pickyMuon()->outerY() : -999999;}
  double pickyMuon_outerZ() const 	{return pickyMuon().isNonnull() ? pickyMuon()->outerZ() : -999999;}
  double pickyMuon_p() const 	{return pickyMuon().isNonnull() ? pickyMuon()->p() : -999999;}
  double pickyMuon_phi() const 	{return pickyMuon().isNonnull() ? pickyMuon()->phi() : -999999;}
  double pickyMuon_phiError() const 	{return pickyMuon().isNonnull() ? pickyMuon()->phiError() : -999999;}
  double pickyMuon_pt() const 	{return pickyMuon().isNonnull() ? pickyMuon()->pt() : -999999;}
  double pickyMuon_ptError() const 	{return pickyMuon().isNonnull() ? pickyMuon()->ptError() : -999999;}
  double pickyMuon_px() const 	{return pickyMuon().isNonnull() ? pickyMuon()->px() : -999999;}
  double pickyMuon_py() const 	{return pickyMuon().isNonnull() ? pickyMuon()->py() : -999999;}
  double pickyMuon_pz() const 	{return pickyMuon().isNonnull() ? pickyMuon()->pz() : -999999;}
  double pickyMuon_qoverp() const 	{return pickyMuon().isNonnull() ? pickyMuon()->qoverp() : -999999;}
  double pickyMuon_qoverpError() const 	{return pickyMuon().isNonnull() ? pickyMuon()->qoverpError() : -999999;}
  int pickyMuon_qualityMask() const 	{return pickyMuon().isNonnull() ? pickyMuon()->qualityMask() : -999999;}
  size_t pickyMuon_recHitsSize() const 	{return pickyMuon().isNonnull() ? pickyMuon()->recHitsSize() : 999999;}
  double pickyMuon_theta() const 	{return pickyMuon().isNonnull() ? pickyMuon()->theta() : -999999;}
  double pickyMuon_thetaError() const 	{return pickyMuon().isNonnull() ? pickyMuon()->thetaError() : -999999;}
  double pickyMuon_vx() const 	{return pickyMuon().isNonnull() ? pickyMuon()->vx() : -999999;}
  double pickyMuon_vy() const 	{return pickyMuon().isNonnull() ? pickyMuon()->vy() : -999999;}
  double pickyMuon_vz() const 	{return pickyMuon().isNonnull() ? pickyMuon()->vz() : -999999;}
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
  int tpfmsMuon_charge() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->charge() : -999999;}
  double tpfmsMuon_chi2() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->chi2() : -999999;}
  double tpfmsMuon_d0() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->d0() : -999999;}
  double tpfmsMuon_d0Error() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->d0Error() : -999999;}
  double tpfmsMuon_dsz() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->dsz() : -999999;}
  double tpfmsMuon_dszError() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->dszError() : -999999;}
  double tpfmsMuon_dxy() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->dxy() : -999999;}
  double tpfmsMuon_dxyError() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->dxyError() : -999999;}
  double tpfmsMuon_dz() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->dz() : -999999;}
  double tpfmsMuon_dzError() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->dzError() : -999999;}
  double tpfmsMuon_eta() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->eta() : -999999;}
  double tpfmsMuon_etaError() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->etaError() : -999999;}
  unsigned short tpfmsMuon_found() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->found() : 9999;}
  unsigned int tpfmsMuon_innerDetId() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->innerDetId() : 999999;}
  bool tpfmsMuon_innerOk() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->innerOk() : false;}
  double tpfmsMuon_lambda() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->lambda() : -999999;}
  double tpfmsMuon_lambdaError() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->lambdaError() : -999999;}
  unsigned short tpfmsMuon_lost() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->lost() : 9999;}
  double tpfmsMuon_ndof() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->ndof() : -999999;}
  double tpfmsMuon_normalizedChi2() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->normalizedChi2() : -999999;}
  unsigned short tpfmsMuon_numberOfLostHits() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->numberOfLostHits() : 9999;}
  unsigned short tpfmsMuon_numberOfValidHits() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->numberOfValidHits() : 9999;}
  unsigned int tpfmsMuon_outerDetId() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->outerDetId() : 999999;}
  double tpfmsMuon_outerEta() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->outerEta() : -999999;}
  bool tpfmsMuon_outerOk() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->outerOk() : false;}
  double tpfmsMuon_outerP() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->outerP() : -999999;}
  double tpfmsMuon_outerPhi() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->outerPhi() : -999999;}
  double tpfmsMuon_outerPt() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->outerPt() : -999999;}
  double tpfmsMuon_outerPx() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->outerPx() : -999999;}
  double tpfmsMuon_outerPy() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->outerPy() : -999999;}
  double tpfmsMuon_outerPz() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->outerPz() : -999999;}
  double tpfmsMuon_outerRadius() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->outerRadius() : -999999;}
  double tpfmsMuon_outerTheta() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->outerTheta() : -999999;}
  double tpfmsMuon_outerX() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->outerX() : -999999;}
  double tpfmsMuon_outerY() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->outerY() : -999999;}
  double tpfmsMuon_outerZ() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->outerZ() : -999999;}
  double tpfmsMuon_p() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->p() : -999999;}
  double tpfmsMuon_phi() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->phi() : -999999;}
  double tpfmsMuon_phiError() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->phiError() : -999999;}
  double tpfmsMuon_pt() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->pt() : -999999;}
  double tpfmsMuon_ptError() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->ptError() : -999999;}
  double tpfmsMuon_px() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->px() : -999999;}
  double tpfmsMuon_py() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->py() : -999999;}
  double tpfmsMuon_pz() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->pz() : -999999;}
  double tpfmsMuon_qoverp() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->qoverp() : -999999;}
  double tpfmsMuon_qoverpError() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->qoverpError() : -999999;}
  int tpfmsMuon_qualityMask() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->qualityMask() : -999999;}
  size_t tpfmsMuon_recHitsSize() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->recHitsSize() : 999999;}
  double tpfmsMuon_theta() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->theta() : -999999;}
  double tpfmsMuon_thetaError() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->thetaError() : -999999;}
  double tpfmsMuon_vx() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->vx() : -999999;}
  double tpfmsMuon_vy() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->vy() : -999999;}
  double tpfmsMuon_vz() const 	{return tpfmsMuon().isNonnull() ? tpfmsMuon()->vz() : -999999;}
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
  double trackerIsoDeposit_candEnergy() const 	{return trackerIsoDeposit() ? trackerIsoDeposit()->candEnergy() : -999999;}
  double trackerIsoDeposit_eta() const 	{return trackerIsoDeposit() ? trackerIsoDeposit()->eta() : -999999;}
  double trackerIsoDeposit_phi() const 	{return trackerIsoDeposit() ? trackerIsoDeposit()->phi() : -999999;}
};
#endif

