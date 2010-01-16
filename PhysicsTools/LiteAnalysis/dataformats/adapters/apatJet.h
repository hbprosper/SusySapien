#ifndef APATJET_H
#define APATJET_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       apatJet
// Description: Simple adapter for:
//              class:   pat::Jet
//              header:  DataFormats/PatCandidates/interface/Jet.h
//              
// Created:     Thu Jan 14 23:48:21 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/PatCandidates/interface/Jet.h"
//-----------------------------------------------------------------------------
class apatJet : public pat::Jet
{
 public: 
  apatJet() {}
  apatJet(const pat::Jet& o)
    : pat::Jet(o) {}
  ~apatJet() {}

  int daughter_charge(size_t x) const 	{return daughter(x) ? daughter(x)->charge() : -999999;}
  double daughter_energy(size_t x) const 	{return daughter(x) ? daughter(x)->energy() : -999999;}
  double daughter_et(size_t x) const 	{return daughter(x) ? daughter(x)->et() : -999999;}
  double daughter_eta(size_t x) const 	{return daughter(x) ? daughter(x)->eta() : -999999;}
  bool daughter_hasMasterClone(size_t x) const 	{return daughter(x) ? daughter(x)->hasMasterClone() : false;}
  bool daughter_hasMasterClonePtr(size_t x) const 	{return daughter(x) ? daughter(x)->hasMasterClonePtr() : false;}
  bool daughter_isCaloMuon(size_t x) const 	{return daughter(x) ? daughter(x)->isCaloMuon() : false;}
  bool daughter_isConvertedPhoton(size_t x) const 	{return daughter(x) ? daughter(x)->isConvertedPhoton() : false;}
  bool daughter_isElectron(size_t x) const 	{return daughter(x) ? daughter(x)->isElectron() : false;}
  bool daughter_isGlobalMuon(size_t x) const 	{return daughter(x) ? daughter(x)->isGlobalMuon() : false;}
  bool daughter_isJet(size_t x) const 	{return daughter(x) ? daughter(x)->isJet() : false;}
  bool daughter_isMuon(size_t x) const 	{return daughter(x) ? daughter(x)->isMuon() : false;}
  bool daughter_isPhoton(size_t x) const 	{return daughter(x) ? daughter(x)->isPhoton() : false;}
  bool daughter_isStandAloneMuon(size_t x) const 	{return daughter(x) ? daughter(x)->isStandAloneMuon() : false;}
  bool daughter_isTrackerMuon(size_t x) const 	{return daughter(x) ? daughter(x)->isTrackerMuon() : false;}
  bool daughter_longLived(size_t x) const 	{return daughter(x) ? daughter(x)->longLived() : false;}
  double daughter_mass(size_t x) const 	{return daughter(x) ? daughter(x)->mass() : -999999;}
  double daughter_massSqr(size_t x) const 	{return daughter(x) ? daughter(x)->massSqr() : -999999;}
  double daughter_mt(size_t x) const 	{return daughter(x) ? daughter(x)->mt() : -999999;}
  double daughter_mtSqr(size_t x) const 	{return daughter(x) ? daughter(x)->mtSqr() : -999999;}
  size_t daughter_numberOfDaughters(size_t x) const 	{return daughter(x) ? daughter(x)->numberOfDaughters() : 999999;}
  size_t daughter_numberOfMothers(size_t x) const 	{return daughter(x) ? daughter(x)->numberOfMothers() : 999999;}
  size_t daughter_numberOfSourceCandidatePtrs(size_t x) const 	{return daughter(x) ? daughter(x)->numberOfSourceCandidatePtrs() : 999999;}
  double daughter_p(size_t x) const 	{return daughter(x) ? daughter(x)->p() : -999999;}
  int daughter_pdgId(size_t x) const 	{return daughter(x) ? daughter(x)->pdgId() : -999999;}
  double daughter_phi(size_t x) const 	{return daughter(x) ? daughter(x)->phi() : -999999;}
  double daughter_pt(size_t x) const 	{return daughter(x) ? daughter(x)->pt() : -999999;}
  double daughter_px(size_t x) const 	{return daughter(x) ? daughter(x)->px() : -999999;}
  double daughter_py(size_t x) const 	{return daughter(x) ? daughter(x)->py() : -999999;}
  double daughter_pz(size_t x) const 	{return daughter(x) ? daughter(x)->pz() : -999999;}
  double daughter_rapidity(size_t x) const 	{return daughter(x) ? daughter(x)->rapidity() : -999999;}
  int daughter_status(size_t x) const 	{return daughter(x) ? daughter(x)->status() : -999999;}
  double daughter_theta(size_t x) const 	{return daughter(x) ? daughter(x)->theta() : -999999;}
  int daughter_threeCharge(size_t x) const 	{return daughter(x) ? daughter(x)->threeCharge() : -999999;}
  double daughter_vertexChi2(size_t x) const 	{return daughter(x) ? daughter(x)->vertexChi2() : -999999;}
  double daughter_vertexNdof(size_t x) const 	{return daughter(x) ? daughter(x)->vertexNdof() : -999999;}
  double daughter_vertexNormalizedChi2(size_t x) const 	{return daughter(x) ? daughter(x)->vertexNormalizedChi2() : -999999;}
  double daughter_vx(size_t x) const 	{return daughter(x) ? daughter(x)->vx() : -999999;}
  double daughter_vy(size_t x) const 	{return daughter(x) ? daughter(x)->vy() : -999999;}
  double daughter_vz(size_t x) const 	{return daughter(x) ? daughter(x)->vz() : -999999;}
  double daughter_y(size_t x) const 	{return daughter(x) ? daughter(x)->y() : -999999;}
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
  int genParton_charge() const 	{return genParton() ? genParton()->charge() : -999999;}
  double genParton_energy() const 	{return genParton() ? genParton()->energy() : -999999;}
  double genParton_et() const 	{return genParton() ? genParton()->et() : -999999;}
  double genParton_eta() const 	{return genParton() ? genParton()->eta() : -999999;}
  bool genParton_longLived() const 	{return genParton() ? genParton()->longLived() : false;}
  double genParton_mass() const 	{return genParton() ? genParton()->mass() : -999999;}
  double genParton_massSqr() const 	{return genParton() ? genParton()->massSqr() : -999999;}
  double genParton_mt() const 	{return genParton() ? genParton()->mt() : -999999;}
  double genParton_mtSqr() const 	{return genParton() ? genParton()->mtSqr() : -999999;}
  size_t genParton_numberOfDaughters() const 	{return genParton() ? genParton()->numberOfDaughters() : 999999;}
  size_t genParton_numberOfMothers() const 	{return genParton() ? genParton()->numberOfMothers() : 999999;}
  double genParton_p() const 	{return genParton() ? genParton()->p() : -999999;}
  int genParton_pdgId() const 	{return genParton() ? genParton()->pdgId() : -999999;}
  double genParton_phi() const 	{return genParton() ? genParton()->phi() : -999999;}
  double genParton_pt() const 	{return genParton() ? genParton()->pt() : -999999;}
  double genParton_px() const 	{return genParton() ? genParton()->px() : -999999;}
  double genParton_py() const 	{return genParton() ? genParton()->py() : -999999;}
  double genParton_pz() const 	{return genParton() ? genParton()->pz() : -999999;}
  double genParton_rapidity() const 	{return genParton() ? genParton()->rapidity() : -999999;}
  int genParton_status() const 	{return genParton() ? genParton()->status() : -999999;}
  double genParton_theta() const 	{return genParton() ? genParton()->theta() : -999999;}
  int genParton_threeCharge() const 	{return genParton() ? genParton()->threeCharge() : -999999;}
  double genParton_vx() const 	{return genParton() ? genParton()->vx() : -999999;}
  double genParton_vy() const 	{return genParton() ? genParton()->vy() : -999999;}
  double genParton_vz() const 	{return genParton() ? genParton()->vz() : -999999;}
  double genParton_y() const 	{return genParton() ? genParton()->y() : -999999;}
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
  bool tagInfo_hasTracks(const std::string x) const 	{return tagInfo(x) ? tagInfo(x)->hasTracks() : false;}
  unsigned int tagInfoSecondaryVertex_nSelectedTracks(const std::string& x="") const 	{return tagInfoSecondaryVertex(x) ? tagInfoSecondaryVertex(x)->nSelectedTracks() : 999999;}
  unsigned int tagInfoSecondaryVertex_nVertexCandidates(const std::string& x="") const 	{return tagInfoSecondaryVertex(x) ? tagInfoSecondaryVertex(x)->nVertexCandidates() : 999999;}
  unsigned int tagInfoSecondaryVertex_nVertexTracks(const std::string& x="") const 	{return tagInfoSecondaryVertex(x) ? tagInfoSecondaryVertex(x)->nVertexTracks() : 999999;}
  unsigned int tagInfoSecondaryVertex_nVertices(const std::string& x="") const 	{return tagInfoSecondaryVertex(x) ? tagInfoSecondaryVertex(x)->nVertices() : 999999;}
  bool tagInfoSoftLepton_hasTracks(const std::string& x="") const 	{return tagInfoSoftLepton(x) ? tagInfoSoftLepton(x)->hasTracks() : false;}
  unsigned int tagInfoSoftLepton_leptons(const std::string& x="") const 	{return tagInfoSoftLepton(x) ? tagInfoSoftLepton(x)->leptons() : 999999;}
  bool tagInfoTrackIP_hasProbabilities(const std::string& x="") const 	{return tagInfoTrackIP(x) ? tagInfoTrackIP(x)->hasProbabilities() : false;}
  bool tagInfoTrackIP_hasTracks(const std::string& x="") const 	{return tagInfoTrackIP(x) ? tagInfoTrackIP(x)->hasTracks() : false;}
};
#endif

