#ifndef ARECOPFBLOCKELEMENTTRACK_H
#define ARECOPFBLOCKELEMENTTRACK_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       arecoPFBlockElementTrack
// Description: Simple adapter for:
//              class:   reco::PFBlockElementTrack
//              header:  DataFormats/ParticleFlowReco/interface/PFBlockElementTrack.h
//              
// Created:     Fri Jan 15 21:58:16 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementTrack.h"
//-----------------------------------------------------------------------------
class arecoPFBlockElementTrack : public reco::PFBlockElementTrack
{
 public: 
  arecoPFBlockElementTrack() {}
  arecoPFBlockElementTrack(const reco::PFBlockElementTrack& o)
    : reco::PFBlockElementTrack(o) {}
  ~arecoPFBlockElementTrack() {}

  int trackRef_charge() const 	{return trackRef().isNonnull() ? trackRef()->charge() : -999999;}
  double trackRef_chi2() const 	{return trackRef().isNonnull() ? trackRef()->chi2() : -999999;}
  double trackRef_d0() const 	{return trackRef().isNonnull() ? trackRef()->d0() : -999999;}
  double trackRef_d0Error() const 	{return trackRef().isNonnull() ? trackRef()->d0Error() : -999999;}
  double trackRef_dsz() const 	{return trackRef().isNonnull() ? trackRef()->dsz() : -999999;}
  double trackRef_dszError() const 	{return trackRef().isNonnull() ? trackRef()->dszError() : -999999;}
  double trackRef_dxy() const 	{return trackRef().isNonnull() ? trackRef()->dxy() : -999999;}
  double trackRef_dxyError() const 	{return trackRef().isNonnull() ? trackRef()->dxyError() : -999999;}
  double trackRef_dz() const 	{return trackRef().isNonnull() ? trackRef()->dz() : -999999;}
  double trackRef_dzError() const 	{return trackRef().isNonnull() ? trackRef()->dzError() : -999999;}
  double trackRef_eta() const 	{return trackRef().isNonnull() ? trackRef()->eta() : -999999;}
  double trackRef_etaError() const 	{return trackRef().isNonnull() ? trackRef()->etaError() : -999999;}
  unsigned short trackRef_found() const 	{return trackRef().isNonnull() ? trackRef()->found() : 9999;}
  unsigned int trackRef_innerDetId() const 	{return trackRef().isNonnull() ? trackRef()->innerDetId() : 999999;}
  bool trackRef_innerOk() const 	{return trackRef().isNonnull() ? trackRef()->innerOk() : false;}
  double trackRef_lambda() const 	{return trackRef().isNonnull() ? trackRef()->lambda() : -999999;}
  double trackRef_lambdaError() const 	{return trackRef().isNonnull() ? trackRef()->lambdaError() : -999999;}
  unsigned short trackRef_lost() const 	{return trackRef().isNonnull() ? trackRef()->lost() : 9999;}
  double trackRef_ndof() const 	{return trackRef().isNonnull() ? trackRef()->ndof() : -999999;}
  double trackRef_normalizedChi2() const 	{return trackRef().isNonnull() ? trackRef()->normalizedChi2() : -999999;}
  unsigned short trackRef_numberOfLostHits() const 	{return trackRef().isNonnull() ? trackRef()->numberOfLostHits() : 9999;}
  unsigned short trackRef_numberOfValidHits() const 	{return trackRef().isNonnull() ? trackRef()->numberOfValidHits() : 9999;}
  unsigned int trackRef_outerDetId() const 	{return trackRef().isNonnull() ? trackRef()->outerDetId() : 999999;}
  double trackRef_outerEta() const 	{return trackRef().isNonnull() ? trackRef()->outerEta() : -999999;}
  bool trackRef_outerOk() const 	{return trackRef().isNonnull() ? trackRef()->outerOk() : false;}
  double trackRef_outerP() const 	{return trackRef().isNonnull() ? trackRef()->outerP() : -999999;}
  double trackRef_outerPhi() const 	{return trackRef().isNonnull() ? trackRef()->outerPhi() : -999999;}
  double trackRef_outerPt() const 	{return trackRef().isNonnull() ? trackRef()->outerPt() : -999999;}
  double trackRef_outerPx() const 	{return trackRef().isNonnull() ? trackRef()->outerPx() : -999999;}
  double trackRef_outerPy() const 	{return trackRef().isNonnull() ? trackRef()->outerPy() : -999999;}
  double trackRef_outerPz() const 	{return trackRef().isNonnull() ? trackRef()->outerPz() : -999999;}
  double trackRef_outerRadius() const 	{return trackRef().isNonnull() ? trackRef()->outerRadius() : -999999;}
  double trackRef_outerTheta() const 	{return trackRef().isNonnull() ? trackRef()->outerTheta() : -999999;}
  double trackRef_outerX() const 	{return trackRef().isNonnull() ? trackRef()->outerX() : -999999;}
  double trackRef_outerY() const 	{return trackRef().isNonnull() ? trackRef()->outerY() : -999999;}
  double trackRef_outerZ() const 	{return trackRef().isNonnull() ? trackRef()->outerZ() : -999999;}
  double trackRef_p() const 	{return trackRef().isNonnull() ? trackRef()->p() : -999999;}
  double trackRef_phi() const 	{return trackRef().isNonnull() ? trackRef()->phi() : -999999;}
  double trackRef_phiError() const 	{return trackRef().isNonnull() ? trackRef()->phiError() : -999999;}
  double trackRef_pt() const 	{return trackRef().isNonnull() ? trackRef()->pt() : -999999;}
  double trackRef_ptError() const 	{return trackRef().isNonnull() ? trackRef()->ptError() : -999999;}
  double trackRef_px() const 	{return trackRef().isNonnull() ? trackRef()->px() : -999999;}
  double trackRef_py() const 	{return trackRef().isNonnull() ? trackRef()->py() : -999999;}
  double trackRef_pz() const 	{return trackRef().isNonnull() ? trackRef()->pz() : -999999;}
  double trackRef_qoverp() const 	{return trackRef().isNonnull() ? trackRef()->qoverp() : -999999;}
  double trackRef_qoverpError() const 	{return trackRef().isNonnull() ? trackRef()->qoverpError() : -999999;}
  int trackRef_qualityMask() const 	{return trackRef().isNonnull() ? trackRef()->qualityMask() : -999999;}
  size_t trackRef_recHitsSize() const 	{return trackRef().isNonnull() ? trackRef()->recHitsSize() : 999999;}
  double trackRef_theta() const 	{return trackRef().isNonnull() ? trackRef()->theta() : -999999;}
  double trackRef_thetaError() const 	{return trackRef().isNonnull() ? trackRef()->thetaError() : -999999;}
  double trackRef_vx() const 	{return trackRef().isNonnull() ? trackRef()->vx() : -999999;}
  double trackRef_vy() const 	{return trackRef().isNonnull() ? trackRef()->vy() : -999999;}
  double trackRef_vz() const 	{return trackRef().isNonnull() ? trackRef()->vz() : -999999;}
};
#endif

