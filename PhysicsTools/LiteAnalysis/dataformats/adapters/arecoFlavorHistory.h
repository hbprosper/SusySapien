#ifndef ARECOFLAVORHISTORY_H
#define ARECOFLAVORHISTORY_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       arecoFlavorHistory
// Description: Simple adapter for:
//              class:   reco::FlavorHistory
//              header:  DataFormats/HepMCCandidate/interface/FlavorHistory.h
//              
// Created:     Fri Jan 15 21:58:03 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/HepMCCandidate/interface/FlavorHistory.h"
//-----------------------------------------------------------------------------
class arecoFlavorHistory : public reco::FlavorHistory
{
 public: 
  arecoFlavorHistory() {}
  arecoFlavorHistory(const reco::FlavorHistory& o)
    : reco::FlavorHistory(o) {}
  ~arecoFlavorHistory() {}

  int matchedJet_charge() const 	{return matchedJet().charge();}
  double matchedJet_energy() const 	{return matchedJet().energy();}
  double matchedJet_et() const 	{return matchedJet().et();}
  double matchedJet_eta() const 	{return matchedJet().eta();}
  bool matchedJet_longLived() const 	{return matchedJet().longLived();}
  double matchedJet_mass() const 	{return matchedJet().mass();}
  bool matchedJet_massConstraint() const 	{return matchedJet().massConstraint();}
  double matchedJet_massSqr() const 	{return matchedJet().massSqr();}
  double matchedJet_mt() const 	{return matchedJet().mt();}
  double matchedJet_mtSqr() const 	{return matchedJet().mtSqr();}
  double matchedJet_p() const 	{return matchedJet().p();}
  int matchedJet_pdgId() const 	{return matchedJet().pdgId();}
  double matchedJet_phi() const 	{return matchedJet().phi();}
  double matchedJet_pt() const 	{return matchedJet().pt();}
  double matchedJet_px() const 	{return matchedJet().px();}
  double matchedJet_py() const 	{return matchedJet().py();}
  double matchedJet_pz() const 	{return matchedJet().pz();}
  double matchedJet_rapidity() const 	{return matchedJet().rapidity();}
  int matchedJet_status() const 	{return matchedJet().status();}
  double matchedJet_theta() const 	{return matchedJet().theta();}
  int matchedJet_threeCharge() const 	{return matchedJet().threeCharge();}
  double matchedJet_vx() const 	{return matchedJet().vx();}
  double matchedJet_vy() const 	{return matchedJet().vy();}
  double matchedJet_vz() const 	{return matchedJet().vz();}
  double matchedJet_y() const 	{return matchedJet().y();}
  int sisterJet_charge() const 	{return sisterJet().charge();}
  double sisterJet_energy() const 	{return sisterJet().energy();}
  double sisterJet_et() const 	{return sisterJet().et();}
  double sisterJet_eta() const 	{return sisterJet().eta();}
  bool sisterJet_longLived() const 	{return sisterJet().longLived();}
  double sisterJet_mass() const 	{return sisterJet().mass();}
  bool sisterJet_massConstraint() const 	{return sisterJet().massConstraint();}
  double sisterJet_massSqr() const 	{return sisterJet().massSqr();}
  double sisterJet_mt() const 	{return sisterJet().mt();}
  double sisterJet_mtSqr() const 	{return sisterJet().mtSqr();}
  double sisterJet_p() const 	{return sisterJet().p();}
  int sisterJet_pdgId() const 	{return sisterJet().pdgId();}
  double sisterJet_phi() const 	{return sisterJet().phi();}
  double sisterJet_pt() const 	{return sisterJet().pt();}
  double sisterJet_px() const 	{return sisterJet().px();}
  double sisterJet_py() const 	{return sisterJet().py();}
  double sisterJet_pz() const 	{return sisterJet().pz();}
  double sisterJet_rapidity() const 	{return sisterJet().rapidity();}
  int sisterJet_status() const 	{return sisterJet().status();}
  double sisterJet_theta() const 	{return sisterJet().theta();}
  int sisterJet_threeCharge() const 	{return sisterJet().threeCharge();}
  double sisterJet_vx() const 	{return sisterJet().vx();}
  double sisterJet_vy() const 	{return sisterJet().vy();}
  double sisterJet_vz() const 	{return sisterJet().vz();}
  double sisterJet_y() const 	{return sisterJet().y();}
};
#endif

