#ifndef ARECORECOCALOTOWERCANDIDATE_H
#define ARECORECOCALOTOWERCANDIDATE_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       arecoRecoCaloTowerCandidate
// Description: Simple adapter for:
//              class:   reco::RecoCaloTowerCandidate
//              header:  DataFormats/RecoCandidate/interface/RecoCaloTowerCandidate.h
//              
// Created:     Thu Jan 14 23:48:29 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/RecoCandidate/interface/RecoCaloTowerCandidate.h"
//-----------------------------------------------------------------------------
class arecoRecoCaloTowerCandidate : public reco::RecoCaloTowerCandidate
{
 public: 
  arecoRecoCaloTowerCandidate() {}
  arecoRecoCaloTowerCandidate(const reco::RecoCaloTowerCandidate& o)
    : reco::RecoCaloTowerCandidate(o) {}
  ~arecoRecoCaloTowerCandidate() {}

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
};
#endif

