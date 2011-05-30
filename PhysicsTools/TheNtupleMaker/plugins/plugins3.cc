// -------------------------------------------------------------------------
// File::   plugins3.cc
// Created: Mon May 30 06:05:59 2011 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"

#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
typedef Buffer<reco::PFCluster, false> recoPFCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFCluster_t,
                  "recoPFCluster");

#include "DataFormats/ParticleFlowReco/interface/PFDisplacedVertex.h"
typedef Buffer<reco::PFDisplacedVertex, false> recoPFDisplacedVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFDisplacedVertex_t,
                  "recoPFDisplacedVertex");

#include "DataFormats/JetReco/interface/PFJet.h"
typedef Buffer<reco::PFJet, false> recoPFJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFJet_t,
                  "recoPFJet");

#include "DataFormats/METReco/interface/PFMET.h"
typedef Buffer<reco::PFMET, false> recoPFMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFMET_t,
                  "recoPFMET");

#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
typedef Buffer<reco::PFRecHit, false> recoPFRecHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFRecHit_t,
                  "recoPFRecHit");

#include "DataFormats/TauReco/interface/PFTau.h"
typedef Buffer<reco::PFTau, false> recoPFTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTau_t,
                  "recoPFTau");

#include "DataFormats/EgammaCandidates/interface/Photon.h"
typedef Buffer<reco::Photon, false> recoPhoton_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPhoton_t,
                  "recoPhoton");

#include "DataFormats/EgammaCandidates/interface/PhotonCore.h"
typedef Buffer<reco::PhotonCore, false> recoPhotonCore_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPhotonCore_t,
                  "recoPhotonCore");

#include "DataFormats/ParticleFlowReco/interface/PreId.h"
typedef Buffer<reco::PreId, false> recoPreId_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreId_t,
                  "recoPreId");

#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
typedef Buffer<reco::PreshowerCluster, false> recoPreshowerCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreshowerCluster_t,
                  "recoPreshowerCluster");

#include "DataFormats/EgammaReco/interface/PreshowerClusterShape.h"
typedef Buffer<reco::PreshowerClusterShape, false> recoPreshowerClusterShape_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreshowerClusterShape_t,
                  "recoPreshowerClusterShape");

#include "DataFormats/RecoCandidate/interface/RecoChargedRefCandidate.h"
typedef Buffer<reco::RecoChargedRefCandidate, false> recoRecoChargedRefCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoChargedRefCandidate_t,
                  "recoRecoChargedRefCandidate");

#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
typedef Buffer<reco::RecoEcalCandidate, false> recoRecoEcalCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoEcalCandidate_t,
                  "recoRecoEcalCandidate");

#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
typedef Buffer<reco::SecondaryVertexTagInfo, false> recoSecondaryVertexTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSecondaryVertexTagInfo_t,
                  "recoSecondaryVertexTagInfo");

#include "DataFormats/BTauReco/interface/SoftLeptonTagInfo.h"
typedef Buffer<reco::SoftLeptonTagInfo, false> recoSoftLeptonTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSoftLeptonTagInfo_t,
                  "recoSoftLeptonTagInfo");

#include "DataFormats/EgammaReco/interface/SuperCluster.h"
typedef Buffer<reco::SuperCluster, false> recoSuperCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSuperCluster_t,
                  "recoSuperCluster");

#include "DataFormats/TrackReco/interface/Track.h"
typedef Buffer<reco::Track, false> recoTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrack_t,
                  "recoTrack");

#include "DataFormats/TrackReco/interface/TrackExtra.h"
typedef Buffer<reco::TrackExtra, false> recoTrackExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackExtra_t,
                  "recoTrackExtra");

#include "DataFormats/JetReco/interface/TrackExtrapolation.h"
typedef Buffer<reco::TrackExtrapolation, false> recoTrackExtrapolation_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackExtrapolation_t,
                  "recoTrackExtrapolation");

#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
typedef Buffer<reco::TrackIPTagInfo, false> recoTrackIPTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackIPTagInfo_t,
                  "recoTrackIPTagInfo");

#include "DataFormats/JetReco/interface/TrackJet.h"
typedef Buffer<reco::TrackJet, false> recoTrackJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackJet_t,
                  "recoTrackJet");

#include "DataFormats/VertexReco/interface/Vertex.h"
typedef Buffer<reco::Vertex, false> recoVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoVertex_t,
                  "recoVertex");

#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
typedef Buffer<reco::VertexCompositeCandidate, false> recoVertexCompositeCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoVertexCompositeCandidate_t,
                  "recoVertexCompositeCandidate");

#include "DataFormats/EcalDigi/interface/EcalTrigPrimCompactColl.h"
typedef Buffer<EcalTrigPrimCompactColl, true> EcalTrigPrimCompactColl_t;
DEFINE_EDM_PLUGIN(BufferFactory, EcalTrigPrimCompactColl_t,
                  "EcalTrigPrimCompactColl");

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
typedef Buffer<GenEventInfoProduct, true> GenEventInfoProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenEventInfoProduct_t,
                  "GenEventInfoProduct");

#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
typedef Buffer<GenRunInfoProduct, true> GenRunInfoProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenRunInfoProduct_t,
                  "GenRunInfoProduct");
