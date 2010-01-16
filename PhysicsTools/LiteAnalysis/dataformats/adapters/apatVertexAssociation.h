#ifndef APATVERTEXASSOCIATION_H
#define APATVERTEXASSOCIATION_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       apatVertexAssociation
// Description: Simple adapter for:
//              class:   pat::VertexAssociation
//              header:  DataFormats/PatCandidates/interface/Vertexing.h
//              
// Created:     Fri Jan 15 21:58:27 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/PatCandidates/interface/Vertexing.h"
//-----------------------------------------------------------------------------
class apatVertexAssociation : public pat::VertexAssociation
{
 public: 
  apatVertexAssociation() {}
  apatVertexAssociation(const pat::VertexAssociation& o)
    : pat::VertexAssociation(o) {}
  ~apatVertexAssociation() {}

  int track_charge() const 	{return track() ? track()->charge() : -999999;}
  double track_chi2() const 	{return track() ? track()->chi2() : -999999;}
  double track_d0() const 	{return track() ? track()->d0() : -999999;}
  double track_d0Error() const 	{return track() ? track()->d0Error() : -999999;}
  double track_dsz() const 	{return track() ? track()->dsz() : -999999;}
  double track_dszError() const 	{return track() ? track()->dszError() : -999999;}
  double track_dxy() const 	{return track() ? track()->dxy() : -999999;}
  double track_dxyError() const 	{return track() ? track()->dxyError() : -999999;}
  double track_dz() const 	{return track() ? track()->dz() : -999999;}
  double track_dzError() const 	{return track() ? track()->dzError() : -999999;}
  double track_eta() const 	{return track() ? track()->eta() : -999999;}
  double track_etaError() const 	{return track() ? track()->etaError() : -999999;}
  unsigned short track_found() const 	{return track() ? track()->found() : 9999;}
  unsigned int track_innerDetId() const 	{return track() ? track()->innerDetId() : 999999;}
  bool track_innerOk() const 	{return track() ? track()->innerOk() : false;}
  double track_lambda() const 	{return track() ? track()->lambda() : -999999;}
  double track_lambdaError() const 	{return track() ? track()->lambdaError() : -999999;}
  unsigned short track_lost() const 	{return track() ? track()->lost() : 9999;}
  double track_ndof() const 	{return track() ? track()->ndof() : -999999;}
  double track_normalizedChi2() const 	{return track() ? track()->normalizedChi2() : -999999;}
  unsigned short track_numberOfLostHits() const 	{return track() ? track()->numberOfLostHits() : 9999;}
  unsigned short track_numberOfValidHits() const 	{return track() ? track()->numberOfValidHits() : 9999;}
  unsigned int track_outerDetId() const 	{return track() ? track()->outerDetId() : 999999;}
  double track_outerEta() const 	{return track() ? track()->outerEta() : -999999;}
  bool track_outerOk() const 	{return track() ? track()->outerOk() : false;}
  double track_outerP() const 	{return track() ? track()->outerP() : -999999;}
  double track_outerPhi() const 	{return track() ? track()->outerPhi() : -999999;}
  double track_outerPt() const 	{return track() ? track()->outerPt() : -999999;}
  double track_outerPx() const 	{return track() ? track()->outerPx() : -999999;}
  double track_outerPy() const 	{return track() ? track()->outerPy() : -999999;}
  double track_outerPz() const 	{return track() ? track()->outerPz() : -999999;}
  double track_outerRadius() const 	{return track() ? track()->outerRadius() : -999999;}
  double track_outerTheta() const 	{return track() ? track()->outerTheta() : -999999;}
  double track_outerX() const 	{return track() ? track()->outerX() : -999999;}
  double track_outerY() const 	{return track() ? track()->outerY() : -999999;}
  double track_outerZ() const 	{return track() ? track()->outerZ() : -999999;}
  double track_p() const 	{return track() ? track()->p() : -999999;}
  double track_phi() const 	{return track() ? track()->phi() : -999999;}
  double track_phiError() const 	{return track() ? track()->phiError() : -999999;}
  double track_pt() const 	{return track() ? track()->pt() : -999999;}
  double track_ptError() const 	{return track() ? track()->ptError() : -999999;}
  double track_px() const 	{return track() ? track()->px() : -999999;}
  double track_py() const 	{return track() ? track()->py() : -999999;}
  double track_pz() const 	{return track() ? track()->pz() : -999999;}
  double track_qoverp() const 	{return track() ? track()->qoverp() : -999999;}
  double track_qoverpError() const 	{return track() ? track()->qoverpError() : -999999;}
  int track_qualityMask() const 	{return track() ? track()->qualityMask() : -999999;}
  size_t track_recHitsSize() const 	{return track() ? track()->recHitsSize() : 999999;}
  double track_theta() const 	{return track() ? track()->theta() : -999999;}
  double track_thetaError() const 	{return track() ? track()->thetaError() : -999999;}
  double track_vx() const 	{return track() ? track()->vx() : -999999;}
  double track_vy() const 	{return track() ? track()->vy() : -999999;}
  double track_vz() const 	{return track() ? track()->vz() : -999999;}
  double vertex_chi2() const 	{return vertex() ? vertex()->chi2() : -999999;}
  bool vertex_hasRefittedTracks() const 	{return vertex() ? vertex()->hasRefittedTracks() : false;}
  bool vertex_isFake() const 	{return vertex() ? vertex()->isFake() : false;}
  bool vertex_isValid() const 	{return vertex() ? vertex()->isValid() : false;}
  double vertex_ndof() const 	{return vertex() ? vertex()->ndof() : -999999;}
  double vertex_normalizedChi2() const 	{return vertex() ? vertex()->normalizedChi2() : -999999;}
  size_t vertex_tracksSize() const 	{return vertex() ? vertex()->tracksSize() : 999999;}
  double vertex_x() const 	{return vertex() ? vertex()->x() : -999999;}
  double vertex_xError() const 	{return vertex() ? vertex()->xError() : -999999;}
  double vertex_y() const 	{return vertex() ? vertex()->y() : -999999;}
  double vertex_yError() const 	{return vertex() ? vertex()->yError() : -999999;}
  double vertex_z() const 	{return vertex() ? vertex()->z() : -999999;}
  double vertex_zError() const 	{return vertex() ? vertex()->zError() : -999999;}
};
#endif

