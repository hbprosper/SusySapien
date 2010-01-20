#ifndef APATGENERICPARTICLE_H
#define APATGENERICPARTICLE_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       apatGenericParticle
// Description: Simple adapter for:
//              class:   pat::GenericParticle
//              header:  DataFormats/PatCandidates/interface/GenericParticle.h
//              
// Created:     Thu Jan 14 23:48:19 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/PatCandidates/interface/GenericParticle.h"
//-----------------------------------------------------------------------------
class apatGenericParticle : public pat::GenericParticle
{
 public: 
  apatGenericParticle() {}
  apatGenericParticle(const pat::GenericParticle& o)
    : pat::GenericParticle(o) {}
  ~apatGenericParticle() {}

  int caloTower_charge() const 	{return caloTower().isNonnull() ? caloTower()->charge() : -999999;}
  size_t caloTower_constituentsSize() const 	{return caloTower().isNonnull() ? caloTower()->constituentsSize() : 999999;}
  float caloTower_ecalTime() const 	{return caloTower().isNonnull() ? caloTower()->ecalTime() : -999999;}
  double caloTower_emEnergy() const 	{return caloTower().isNonnull() ? caloTower()->emEnergy() : -999999;}
  double caloTower_emEt() const 	{return caloTower().isNonnull() ? caloTower()->emEt() : -999999;}
  int caloTower_emLvl1() const 	{return caloTower().isNonnull() ? caloTower()->emLvl1() : -999999;}
  double caloTower_energy() const 	{return caloTower().isNonnull() ? caloTower()->energy() : -999999;}
  double caloTower_et() const 	{return caloTower().isNonnull() ? caloTower()->et() : -999999;}
  double caloTower_eta() const 	{return caloTower().isNonnull() ? caloTower()->eta() : -999999;}
  double caloTower_hadEnergy() const 	{return caloTower().isNonnull() ? caloTower()->hadEnergy() : -999999;}
  double caloTower_hadEnergyHeInnerLayer() const 	{return caloTower().isNonnull() ? caloTower()->hadEnergyHeInnerLayer() : -999999;}
  double caloTower_hadEnergyHeOuterLayer() const 	{return caloTower().isNonnull() ? caloTower()->hadEnergyHeOuterLayer() : -999999;}
  double caloTower_hadEt() const 	{return caloTower().isNonnull() ? caloTower()->hadEt() : -999999;}
  int caloTower_hadLv11() const 	{return caloTower().isNonnull() ? caloTower()->hadLv11() : -999999;}
  float caloTower_hcalTime() const 	{return caloTower().isNonnull() ? caloTower()->hcalTime() : -999999;}
  int caloTower_ieta() const 	{return caloTower().isNonnull() ? caloTower()->ieta() : -999999;}
  int caloTower_ietaAbs() const 	{return caloTower().isNonnull() ? caloTower()->ietaAbs() : -999999;}
  int caloTower_iphi() const 	{return caloTower().isNonnull() ? caloTower()->iphi() : -999999;}
  bool caloTower_longLived() const 	{return caloTower().isNonnull() ? caloTower()->longLived() : false;}
  double caloTower_mass() const 	{return caloTower().isNonnull() ? caloTower()->mass() : -999999;}
  double caloTower_massSqr() const 	{return caloTower().isNonnull() ? caloTower()->massSqr() : -999999;}
  double caloTower_mt() const 	{return caloTower().isNonnull() ? caloTower()->mt() : -999999;}
  double caloTower_mtSqr() const 	{return caloTower().isNonnull() ? caloTower()->mtSqr() : -999999;}
  int caloTower_numCrystals() const 	{return caloTower().isNonnull() ? caloTower()->numCrystals() : -999999;}
  size_t caloTower_numberOfDaughters() const 	{return caloTower().isNonnull() ? caloTower()->numberOfDaughters() : 999999;}
  size_t caloTower_numberOfMothers() const 	{return caloTower().isNonnull() ? caloTower()->numberOfMothers() : 999999;}
  double caloTower_outerEnergy() const 	{return caloTower().isNonnull() ? caloTower()->outerEnergy() : -999999;}
  double caloTower_outerEt() const 	{return caloTower().isNonnull() ? caloTower()->outerEt() : -999999;}
  double caloTower_p() const 	{return caloTower().isNonnull() ? caloTower()->p() : -999999;}
  int caloTower_pdgId() const 	{return caloTower().isNonnull() ? caloTower()->pdgId() : -999999;}
  double caloTower_phi() const 	{return caloTower().isNonnull() ? caloTower()->phi() : -999999;}
  double caloTower_pt() const 	{return caloTower().isNonnull() ? caloTower()->pt() : -999999;}
  double caloTower_px() const 	{return caloTower().isNonnull() ? caloTower()->px() : -999999;}
  double caloTower_py() const 	{return caloTower().isNonnull() ? caloTower()->py() : -999999;}
  double caloTower_pz() const 	{return caloTower().isNonnull() ? caloTower()->pz() : -999999;}
  double caloTower_rapidity() const 	{return caloTower().isNonnull() ? caloTower()->rapidity() : -999999;}
  int caloTower_status() const 	{return caloTower().isNonnull() ? caloTower()->status() : -999999;}
  double caloTower_theta() const 	{return caloTower().isNonnull() ? caloTower()->theta() : -999999;}
  int caloTower_threeCharge() const 	{return caloTower().isNonnull() ? caloTower()->threeCharge() : -999999;}
  double caloTower_vx() const 	{return caloTower().isNonnull() ? caloTower()->vx() : -999999;}
  double caloTower_vy() const 	{return caloTower().isNonnull() ? caloTower()->vy() : -999999;}
  double caloTower_vz() const 	{return caloTower().isNonnull() ? caloTower()->vz() : -999999;}
  double caloTower_y() const 	{return caloTower().isNonnull() ? caloTower()->y() : -999999;}
  int caloTower_zside() const 	{return caloTower().isNonnull() ? caloTower()->zside() : -999999;}
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
  int track_charge(size_t x) const 	{return track(x).isNonnull() ? track(x)->charge() : -999999;}
  double track_chi2() const 	{return track().isNonnull() ? track()->chi2() : -999999;}
  double track_chi2(size_t x) const 	{return track(x).isNonnull() ? track(x)->chi2() : -999999;}
  double track_d0() const 	{return track().isNonnull() ? track()->d0() : -999999;}
  double track_d0(size_t x) const 	{return track(x).isNonnull() ? track(x)->d0() : -999999;}
  double track_d0Error() const 	{return track().isNonnull() ? track()->d0Error() : -999999;}
  double track_d0Error(size_t x) const 	{return track(x).isNonnull() ? track(x)->d0Error() : -999999;}
  double track_dsz() const 	{return track().isNonnull() ? track()->dsz() : -999999;}
  double track_dsz(size_t x) const 	{return track(x).isNonnull() ? track(x)->dsz() : -999999;}
  double track_dszError() const 	{return track().isNonnull() ? track()->dszError() : -999999;}
  double track_dszError(size_t x) const 	{return track(x).isNonnull() ? track(x)->dszError() : -999999;}
  double track_dxy() const 	{return track().isNonnull() ? track()->dxy() : -999999;}
  double track_dxy(size_t x) const 	{return track(x).isNonnull() ? track(x)->dxy() : -999999;}
  double track_dxyError() const 	{return track().isNonnull() ? track()->dxyError() : -999999;}
  double track_dxyError(size_t x) const 	{return track(x).isNonnull() ? track(x)->dxyError() : -999999;}
  double track_dz() const 	{return track().isNonnull() ? track()->dz() : -999999;}
  double track_dz(size_t x) const 	{return track(x).isNonnull() ? track(x)->dz() : -999999;}
  double track_dzError() const 	{return track().isNonnull() ? track()->dzError() : -999999;}
  double track_dzError(size_t x) const 	{return track(x).isNonnull() ? track(x)->dzError() : -999999;}
  double track_eta() const 	{return track().isNonnull() ? track()->eta() : -999999;}
  double track_eta(size_t x) const 	{return track(x).isNonnull() ? track(x)->eta() : -999999;}
  double track_etaError() const 	{return track().isNonnull() ? track()->etaError() : -999999;}
  double track_etaError(size_t x) const 	{return track(x).isNonnull() ? track(x)->etaError() : -999999;}
  unsigned short track_found() const 	{return track().isNonnull() ? track()->found() : 9999;}
  unsigned short track_found(size_t x) const 	{return track(x).isNonnull() ? track(x)->found() : 9999;}
  unsigned int track_innerDetId() const 	{return track().isNonnull() ? track()->innerDetId() : 999999;}
  unsigned int track_innerDetId(size_t x) const 	{return track(x).isNonnull() ? track(x)->innerDetId() : 999999;}
  bool track_innerOk() const 	{return track().isNonnull() ? track()->innerOk() : false;}
  bool track_innerOk(size_t x) const 	{return track(x).isNonnull() ? track(x)->innerOk() : false;}
  double track_lambda() const 	{return track().isNonnull() ? track()->lambda() : -999999;}
  double track_lambda(size_t x) const 	{return track(x).isNonnull() ? track(x)->lambda() : -999999;}
  double track_lambdaError() const 	{return track().isNonnull() ? track()->lambdaError() : -999999;}
  double track_lambdaError(size_t x) const 	{return track(x).isNonnull() ? track(x)->lambdaError() : -999999;}
  unsigned short track_lost() const 	{return track().isNonnull() ? track()->lost() : 9999;}
  unsigned short track_lost(size_t x) const 	{return track(x).isNonnull() ? track(x)->lost() : 9999;}
  double track_ndof() const 	{return track().isNonnull() ? track()->ndof() : -999999;}
  double track_ndof(size_t x) const 	{return track(x).isNonnull() ? track(x)->ndof() : -999999;}
  double track_normalizedChi2() const 	{return track().isNonnull() ? track()->normalizedChi2() : -999999;}
  double track_normalizedChi2(size_t x) const 	{return track(x).isNonnull() ? track(x)->normalizedChi2() : -999999;}
  unsigned short track_numberOfLostHits() const 	{return track().isNonnull() ? track()->numberOfLostHits() : 9999;}
  unsigned short track_numberOfLostHits(size_t x) const 	{return track(x).isNonnull() ? track(x)->numberOfLostHits() : 9999;}
  unsigned short track_numberOfValidHits() const 	{return track().isNonnull() ? track()->numberOfValidHits() : 9999;}
  unsigned short track_numberOfValidHits(size_t x) const 	{return track(x).isNonnull() ? track(x)->numberOfValidHits() : 9999;}
  unsigned int track_outerDetId() const 	{return track().isNonnull() ? track()->outerDetId() : 999999;}
  unsigned int track_outerDetId(size_t x) const 	{return track(x).isNonnull() ? track(x)->outerDetId() : 999999;}
  double track_outerEta() const 	{return track().isNonnull() ? track()->outerEta() : -999999;}
  double track_outerEta(size_t x) const 	{return track(x).isNonnull() ? track(x)->outerEta() : -999999;}
  bool track_outerOk() const 	{return track().isNonnull() ? track()->outerOk() : false;}
  bool track_outerOk(size_t x) const 	{return track(x).isNonnull() ? track(x)->outerOk() : false;}
  double track_outerP() const 	{return track().isNonnull() ? track()->outerP() : -999999;}
  double track_outerP(size_t x) const 	{return track(x).isNonnull() ? track(x)->outerP() : -999999;}
  double track_outerPhi() const 	{return track().isNonnull() ? track()->outerPhi() : -999999;}
  double track_outerPhi(size_t x) const 	{return track(x).isNonnull() ? track(x)->outerPhi() : -999999;}
  double track_outerPt() const 	{return track().isNonnull() ? track()->outerPt() : -999999;}
  double track_outerPt(size_t x) const 	{return track(x).isNonnull() ? track(x)->outerPt() : -999999;}
  double track_outerPx() const 	{return track().isNonnull() ? track()->outerPx() : -999999;}
  double track_outerPx(size_t x) const 	{return track(x).isNonnull() ? track(x)->outerPx() : -999999;}
  double track_outerPy() const 	{return track().isNonnull() ? track()->outerPy() : -999999;}
  double track_outerPy(size_t x) const 	{return track(x).isNonnull() ? track(x)->outerPy() : -999999;}
  double track_outerPz() const 	{return track().isNonnull() ? track()->outerPz() : -999999;}
  double track_outerPz(size_t x) const 	{return track(x).isNonnull() ? track(x)->outerPz() : -999999;}
  double track_outerRadius() const 	{return track().isNonnull() ? track()->outerRadius() : -999999;}
  double track_outerRadius(size_t x) const 	{return track(x).isNonnull() ? track(x)->outerRadius() : -999999;}
  double track_outerTheta() const 	{return track().isNonnull() ? track()->outerTheta() : -999999;}
  double track_outerTheta(size_t x) const 	{return track(x).isNonnull() ? track(x)->outerTheta() : -999999;}
  double track_outerX() const 	{return track().isNonnull() ? track()->outerX() : -999999;}
  double track_outerX(size_t x) const 	{return track(x).isNonnull() ? track(x)->outerX() : -999999;}
  double track_outerY() const 	{return track().isNonnull() ? track()->outerY() : -999999;}
  double track_outerY(size_t x) const 	{return track(x).isNonnull() ? track(x)->outerY() : -999999;}
  double track_outerZ() const 	{return track().isNonnull() ? track()->outerZ() : -999999;}
  double track_outerZ(size_t x) const 	{return track(x).isNonnull() ? track(x)->outerZ() : -999999;}
  double track_p() const 	{return track().isNonnull() ? track()->p() : -999999;}
  double track_p(size_t x) const 	{return track(x).isNonnull() ? track(x)->p() : -999999;}
  double track_phi() const 	{return track().isNonnull() ? track()->phi() : -999999;}
  double track_phi(size_t x) const 	{return track(x).isNonnull() ? track(x)->phi() : -999999;}
  double track_phiError() const 	{return track().isNonnull() ? track()->phiError() : -999999;}
  double track_phiError(size_t x) const 	{return track(x).isNonnull() ? track(x)->phiError() : -999999;}
  double track_pt() const 	{return track().isNonnull() ? track()->pt() : -999999;}
  double track_pt(size_t x) const 	{return track(x).isNonnull() ? track(x)->pt() : -999999;}
  double track_ptError() const 	{return track().isNonnull() ? track()->ptError() : -999999;}
  double track_ptError(size_t x) const 	{return track(x).isNonnull() ? track(x)->ptError() : -999999;}
  double track_px() const 	{return track().isNonnull() ? track()->px() : -999999;}
  double track_px(size_t x) const 	{return track(x).isNonnull() ? track(x)->px() : -999999;}
  double track_py() const 	{return track().isNonnull() ? track()->py() : -999999;}
  double track_py(size_t x) const 	{return track(x).isNonnull() ? track(x)->py() : -999999;}
  double track_pz() const 	{return track().isNonnull() ? track()->pz() : -999999;}
  double track_pz(size_t x) const 	{return track(x).isNonnull() ? track(x)->pz() : -999999;}
  double track_qoverp() const 	{return track().isNonnull() ? track()->qoverp() : -999999;}
  double track_qoverp(size_t x) const 	{return track(x).isNonnull() ? track(x)->qoverp() : -999999;}
  double track_qoverpError() const 	{return track().isNonnull() ? track()->qoverpError() : -999999;}
  double track_qoverpError(size_t x) const 	{return track(x).isNonnull() ? track(x)->qoverpError() : -999999;}
  int track_qualityMask() const 	{return track().isNonnull() ? track()->qualityMask() : -999999;}
  int track_qualityMask(size_t x) const 	{return track(x).isNonnull() ? track(x)->qualityMask() : -999999;}
  size_t track_recHitsSize() const 	{return track().isNonnull() ? track()->recHitsSize() : 999999;}
  size_t track_recHitsSize(size_t x) const 	{return track(x).isNonnull() ? track(x)->recHitsSize() : 999999;}
  double track_theta() const 	{return track().isNonnull() ? track()->theta() : -999999;}
  double track_theta(size_t x) const 	{return track(x).isNonnull() ? track(x)->theta() : -999999;}
  double track_thetaError() const 	{return track().isNonnull() ? track()->thetaError() : -999999;}
  double track_thetaError(size_t x) const 	{return track(x).isNonnull() ? track(x)->thetaError() : -999999;}
  double track_vx() const 	{return track().isNonnull() ? track()->vx() : -999999;}
  double track_vx(size_t x) const 	{return track(x).isNonnull() ? track(x)->vx() : -999999;}
  double track_vy() const 	{return track().isNonnull() ? track()->vy() : -999999;}
  double track_vy(size_t x) const 	{return track(x).isNonnull() ? track(x)->vy() : -999999;}
  double track_vz() const 	{return track().isNonnull() ? track()->vz() : -999999;}
  double track_vz(size_t x) const 	{return track(x).isNonnull() ? track(x)->vz() : -999999;}
  double trackerIsoDeposit_candEnergy() const 	{return trackerIsoDeposit() ? trackerIsoDeposit()->candEnergy() : -999999;}
  double trackerIsoDeposit_eta() const 	{return trackerIsoDeposit() ? trackerIsoDeposit()->eta() : -999999;}
  double trackerIsoDeposit_phi() const 	{return trackerIsoDeposit() ? trackerIsoDeposit()->phi() : -999999;}
  bool vertexAssociation_hasErrors(size_t x=0) const 	{return vertexAssociation(x) ? vertexAssociation(x)->hasErrors() : false;}
  bool vertexAssociation_hasTrack(size_t x=0) const 	{return vertexAssociation(x) ? vertexAssociation(x)->hasTrack() : false;}
  bool vertexAssociation_hasTransverseIP(size_t x=0) const 	{return vertexAssociation(x) ? vertexAssociation(x)->hasTransverseIP() : false;}
  bool vertexAssociation_isAvailable(size_t x=0) const 	{return vertexAssociation(x) ? vertexAssociation(x)->isAvailable() : false;}
  bool vertexAssociation_isNonnull(size_t x=0) const 	{return vertexAssociation(x) ? vertexAssociation(x)->isNonnull() : false;}
  bool vertexAssociation_isNull(size_t x=0) const 	{return vertexAssociation(x) ? vertexAssociation(x)->isNull() : false;}
};
#endif

