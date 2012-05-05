// -------------------------------------------------------------------------
// File::   plugins08.cc
// Created: Sat May  5 17:06:51 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/TrackInfo/interface/TrackInfo.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "DataFormats/BTauReco/interface/SoftLeptonTagInfo.h"
#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include "DataFormats/BTauReco/interface/TauImpactParameterInfo.h"
#include "DataFormats/BTauReco/interface/TauMassTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackProbabilityTagInfo.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonCore.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
#include "DataFormats/EgammaReco/interface/PreshowerClusterShape.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/JetReco/interface/TrackExtrapolation.h"
#include "DataFormats/JetReco/interface/TrackJet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PileUpPFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticle.h"
#include "DataFormats/ParticleFlowReco/interface/PFTrajectoryPoint.h"
#include "DataFormats/ParticleFlowReco/interface/PFV0.h"
#include "DataFormats/ParticleFlowReco/interface/PreId.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedRefCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoPFClusterRefCandidate.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauDecayMode.h"
#include "DataFormats/TauReco/interface/PFTauTagInfo.h"
#include "DataFormats/TauReco/interface/RecoTauPiZero.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
// -------------------------------------------------------------------------

std::string recoPFRecTrack_n("reco::PFRecTrack");
typedef Buffer<reco::PFRecTrack,
               &recoPFRecTrack_n, COLLECTION>
recoPFRecTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFRecTrack_t,
                  "recoPFRecTrack");
				  
std::string recoPFSimParticle_n("reco::PFSimParticle");
typedef Buffer<reco::PFSimParticle,
               &recoPFSimParticle_n, COLLECTION>
recoPFSimParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFSimParticle_t,
                  "recoPFSimParticle");
				  
std::string recoPFTau_n("reco::PFTau");
typedef Buffer<reco::PFTau,
               &recoPFTau_n, COLLECTION>
recoPFTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTau_t,
                  "recoPFTau");
				  
std::string recoPFTauDecayMode_n("reco::PFTauDecayMode");
typedef Buffer<reco::PFTauDecayMode,
               &recoPFTauDecayMode_n, COLLECTION>
recoPFTauDecayMode_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTauDecayMode_t,
                  "recoPFTauDecayMode");
				  
std::string recoPFTauTagInfo_n("reco::PFTauTagInfo");
typedef Buffer<reco::PFTauTagInfo,
               &recoPFTauTagInfo_n, COLLECTION>
recoPFTauTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTauTagInfo_t,
                  "recoPFTauTagInfo");
				  
std::string recoPFTrajectoryPoint_n("reco::PFTrajectoryPoint");
typedef Buffer<reco::PFTrajectoryPoint,
               &recoPFTrajectoryPoint_n, COLLECTION>
recoPFTrajectoryPoint_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTrajectoryPoint_t,
                  "recoPFTrajectoryPoint");
				  
std::string recoPFV0_n("reco::PFV0");
typedef Buffer<reco::PFV0,
               &recoPFV0_n, COLLECTION>
recoPFV0_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFV0_t,
                  "recoPFV0");
				  
std::string recoParticle_n("reco::Particle");
typedef Buffer<reco::Particle,
               &recoParticle_n, COLLECTION>
recoParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoParticle_t,
                  "recoParticle");
				  
std::string recoPhoton_n("reco::Photon");
typedef Buffer<reco::Photon,
               &recoPhoton_n, COLLECTION>
recoPhoton_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPhoton_t,
                  "recoPhoton");
				  
std::string recoPhotonCore_n("reco::PhotonCore");
typedef Buffer<reco::PhotonCore,
               &recoPhotonCore_n, COLLECTION>
recoPhotonCore_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPhotonCore_t,
                  "recoPhotonCore");
				  
std::string recoPileUpPFCandidate_n("reco::PileUpPFCandidate");
typedef Buffer<reco::PileUpPFCandidate,
               &recoPileUpPFCandidate_n, COLLECTION>
recoPileUpPFCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPileUpPFCandidate_t,
                  "recoPileUpPFCandidate");
				  
std::string recoPreId_n("reco::PreId");
typedef Buffer<reco::PreId,
               &recoPreId_n, COLLECTION>
recoPreId_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreId_t,
                  "recoPreId");
				  
std::string recoPreshowerCluster_n("reco::PreshowerCluster");
typedef Buffer<reco::PreshowerCluster,
               &recoPreshowerCluster_n, COLLECTION>
recoPreshowerCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreshowerCluster_t,
                  "recoPreshowerCluster");
				  
std::string recoPreshowerClusterShape_n("reco::PreshowerClusterShape");
typedef Buffer<reco::PreshowerClusterShape,
               &recoPreshowerClusterShape_n, COLLECTION>
recoPreshowerClusterShape_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreshowerClusterShape_t,
                  "recoPreshowerClusterShape");
				  
std::string recoRecoChargedCandidate_n("reco::RecoChargedCandidate");
typedef Buffer<reco::RecoChargedCandidate,
               &recoRecoChargedCandidate_n, COLLECTION>
recoRecoChargedCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoChargedCandidate_t,
                  "recoRecoChargedCandidate");
				  
std::string recoRecoChargedRefCandidate_n("reco::RecoChargedRefCandidate");
typedef Buffer<reco::RecoChargedRefCandidate,
               &recoRecoChargedRefCandidate_n, COLLECTION>
recoRecoChargedRefCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoChargedRefCandidate_t,
                  "recoRecoChargedRefCandidate");
				  
std::string recoRecoEcalCandidate_n("reco::RecoEcalCandidate");
typedef Buffer<reco::RecoEcalCandidate,
               &recoRecoEcalCandidate_n, COLLECTION>
recoRecoEcalCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoEcalCandidate_t,
                  "recoRecoEcalCandidate");
				  
std::string recoRecoPFClusterRefCandidate_n("reco::RecoPFClusterRefCandidate");
typedef Buffer<reco::RecoPFClusterRefCandidate,
               &recoRecoPFClusterRefCandidate_n, COLLECTION>
recoRecoPFClusterRefCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoPFClusterRefCandidate_t,
                  "recoRecoPFClusterRefCandidate");
				  
std::string recoRecoTauPiZero_n("reco::RecoTauPiZero");
typedef Buffer<reco::RecoTauPiZero,
               &recoRecoTauPiZero_n, COLLECTION>
recoRecoTauPiZero_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoTauPiZero_t,
                  "recoRecoTauPiZero");
				  
std::string recoSecondaryVertexTagInfo_n("reco::SecondaryVertexTagInfo");
typedef Buffer<reco::SecondaryVertexTagInfo,
               &recoSecondaryVertexTagInfo_n, COLLECTION>
recoSecondaryVertexTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSecondaryVertexTagInfo_t,
                  "recoSecondaryVertexTagInfo");
				  
std::string recoSiStripElectron_n("reco::SiStripElectron");
typedef Buffer<reco::SiStripElectron,
               &recoSiStripElectron_n, COLLECTION>
recoSiStripElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSiStripElectron_t,
                  "recoSiStripElectron");
				  
std::string recoSoftLeptonTagInfo_n("reco::SoftLeptonTagInfo");
typedef Buffer<reco::SoftLeptonTagInfo,
               &recoSoftLeptonTagInfo_n, COLLECTION>
recoSoftLeptonTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSoftLeptonTagInfo_t,
                  "recoSoftLeptonTagInfo");
				  
std::string recoSuperCluster_n("reco::SuperCluster");
typedef Buffer<reco::SuperCluster,
               &recoSuperCluster_n, COLLECTION>
recoSuperCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSuperCluster_t,
                  "recoSuperCluster");
				  
std::string recoTaggingVariableList_n("reco::TaggingVariableList");
typedef Buffer<reco::TaggingVariableList,
               &recoTaggingVariableList_n, COLLECTION>
recoTaggingVariableList_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTaggingVariableList_t,
                  "recoTaggingVariableList");
				  
std::string recoTauImpactParameterInfo_n("reco::TauImpactParameterInfo");
typedef Buffer<reco::TauImpactParameterInfo,
               &recoTauImpactParameterInfo_n, COLLECTION>
recoTauImpactParameterInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTauImpactParameterInfo_t,
                  "recoTauImpactParameterInfo");
				  
std::string recoTauMassTagInfo_n("reco::TauMassTagInfo");
typedef Buffer<reco::TauMassTagInfo,
               &recoTauMassTagInfo_n, COLLECTION>
recoTauMassTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTauMassTagInfo_t,
                  "recoTauMassTagInfo");
				  
std::string recoTrack_n("reco::Track");
typedef Buffer<reco::Track,
               &recoTrack_n, COLLECTION>
recoTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrack_t,
                  "recoTrack");
				  
std::string recoTrackCountingTagInfo_n("reco::TrackCountingTagInfo");
typedef Buffer<reco::TrackCountingTagInfo,
               &recoTrackCountingTagInfo_n, COLLECTION>
recoTrackCountingTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackCountingTagInfo_t,
                  "recoTrackCountingTagInfo");
				  
std::string recoTrackExtra_n("reco::TrackExtra");
typedef Buffer<reco::TrackExtra,
               &recoTrackExtra_n, COLLECTION>
recoTrackExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackExtra_t,
                  "recoTrackExtra");
				  
std::string recoTrackExtrapolation_n("reco::TrackExtrapolation");
typedef Buffer<reco::TrackExtrapolation,
               &recoTrackExtrapolation_n, COLLECTION>
recoTrackExtrapolation_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackExtrapolation_t,
                  "recoTrackExtrapolation");
				  
std::string recoTrackIPTagInfo_n("reco::TrackIPTagInfo");
typedef Buffer<reco::TrackIPTagInfo,
               &recoTrackIPTagInfo_n, COLLECTION>
recoTrackIPTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackIPTagInfo_t,
                  "recoTrackIPTagInfo");
				  
std::string recoTrackInfo_n("reco::TrackInfo");
typedef Buffer<reco::TrackInfo,
               &recoTrackInfo_n, COLLECTION>
recoTrackInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackInfo_t,
                  "recoTrackInfo");
				  
std::string recoTrackJet_n("reco::TrackJet");
typedef Buffer<reco::TrackJet,
               &recoTrackJet_n, COLLECTION>
recoTrackJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackJet_t,
                  "recoTrackJet");
				  
std::string recoTrackProbabilityTagInfo_n("reco::TrackProbabilityTagInfo");
typedef Buffer<reco::TrackProbabilityTagInfo,
               &recoTrackProbabilityTagInfo_n, COLLECTION>
recoTrackProbabilityTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackProbabilityTagInfo_t,
                  "recoTrackProbabilityTagInfo");
				  