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
// Created:     Thu Jan 14 23:48:25 2010 by mkadapter.py  HBP
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

  double ecalIsoDeposit_candEnergy() const 	{return ecalIsoDeposit() ? ecalIsoDeposit()->candEnergy() : -999999;}
  double ecalIsoDeposit_eta() const 	{return ecalIsoDeposit() ? ecalIsoDeposit()->eta() : -999999;}
  double ecalIsoDeposit_phi() const 	{return ecalIsoDeposit() ? ecalIsoDeposit()->phi() : -999999;}
  float efficiency_error(const std::string x) const 	{return efficiency(x).error();}
  float efficiency_value(const std::string x) const 	{return efficiency(x).value();}
  float genJet_auxiliaryEnergy() const 	{return genJet() ? genJet()->auxiliaryEnergy() : -999999;}
  int genJet_charge() const 	{return genJet() ? genJet()->charge() : -999999;}
  float genJet_emEnergy() const 	{return genJet() ? genJet()->emEnergy() : -999999;}
  double genJet_energy() const 	{return genJet() ? genJet()->energy() : -999999;}
  double genJet_et() const 	{return genJet() ? genJet()->et() : -999999;}
  double genJet_eta() const 	{return genJet() ? genJet()->eta() : -999999;}
  float genJet_etaetaMoment() const 	{return genJet() ? genJet()->etaetaMoment() : -999999;}
  float genJet_etaphiMoment() const 	{return genJet() ? genJet()->etaphiMoment() : -999999;}
  float genJet_hadEnergy() const 	{return genJet() ? genJet()->hadEnergy() : -999999;}
  float genJet_invisibleEnergy() const 	{return genJet() ? genJet()->invisibleEnergy() : -999999;}
  bool genJet_longLived() const 	{return genJet() ? genJet()->longLived() : false;}
  double genJet_mass() const 	{return genJet() ? genJet()->mass() : -999999;}
  double genJet_massSqr() const 	{return genJet() ? genJet()->massSqr() : -999999;}
  float genJet_maxDistance() const 	{return genJet() ? genJet()->maxDistance() : -999999;}
  double genJet_mt() const 	{return genJet() ? genJet()->mt() : -999999;}
  double genJet_mtSqr() const 	{return genJet() ? genJet()->mtSqr() : -999999;}
  int genJet_nConstituents() const 	{return genJet() ? genJet()->nConstituents() : -999999;}
  size_t genJet_numberOfDaughters() const 	{return genJet() ? genJet()->numberOfDaughters() : 999999;}
  size_t genJet_numberOfMothers() const 	{return genJet() ? genJet()->numberOfMothers() : 999999;}
  size_t genJet_numberOfSourceCandidatePtrs() const 	{return genJet() ? genJet()->numberOfSourceCandidatePtrs() : 999999;}
  double genJet_p() const 	{return genJet() ? genJet()->p() : -999999;}
  int genJet_pdgId() const 	{return genJet() ? genJet()->pdgId() : -999999;}
  double genJet_phi() const 	{return genJet() ? genJet()->phi() : -999999;}
  float genJet_phiphiMoment() const 	{return genJet() ? genJet()->phiphiMoment() : -999999;}
  double genJet_pt() const 	{return genJet() ? genJet()->pt() : -999999;}
  double genJet_px() const 	{return genJet() ? genJet()->px() : -999999;}
  double genJet_py() const 	{return genJet() ? genJet()->py() : -999999;}
  double genJet_pz() const 	{return genJet() ? genJet()->pz() : -999999;}
  double genJet_rapidity() const 	{return genJet() ? genJet()->rapidity() : -999999;}
  int genJet_status() const 	{return genJet() ? genJet()->status() : -999999;}
  double genJet_theta() const 	{return genJet() ? genJet()->theta() : -999999;}
  int genJet_threeCharge() const 	{return genJet() ? genJet()->threeCharge() : -999999;}
  double genJet_vx() const 	{return genJet() ? genJet()->vx() : -999999;}
  double genJet_vy() const 	{return genJet() ? genJet()->vy() : -999999;}
  double genJet_vz() const 	{return genJet() ? genJet()->vz() : -999999;}
  double genJet_y() const 	{return genJet() ? genJet()->y() : -999999;}
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
  double hcalIsoDeposit_candEnergy() const 	{return hcalIsoDeposit() ? hcalIsoDeposit()->candEnergy() : -999999;}
  double hcalIsoDeposit_eta() const 	{return hcalIsoDeposit() ? hcalIsoDeposit()->eta() : -999999;}
  double hcalIsoDeposit_phi() const 	{return hcalIsoDeposit() ? hcalIsoDeposit()->phi() : -999999;}
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
  double trackerIsoDeposit_candEnergy() const 	{return trackerIsoDeposit() ? trackerIsoDeposit()->candEnergy() : -999999;}
  double trackerIsoDeposit_eta() const 	{return trackerIsoDeposit() ? trackerIsoDeposit()->eta() : -999999;}
  double trackerIsoDeposit_phi() const 	{return trackerIsoDeposit() ? trackerIsoDeposit()->phi() : -999999;}
};
#endif

