// -------------------------------------------------------------------------
// File::   plugins11.cc
// Created: Thu Apr 12 04:32:19 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonCore.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
#include "DataFormats/EgammaReco/interface/PreshowerClusterShape.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/ParticleFlowCandidate/interface/PileUpPFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFConversion.h"
#include "DataFormats/ParticleFlowReco/interface/PFDisplacedTrackerVertex.h"
#include "DataFormats/ParticleFlowReco/interface/PFDisplacedVertex.h"
#include "DataFormats/ParticleFlowReco/interface/PFDisplacedVertexCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFNuclearInteraction.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
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
// -------------------------------------------------------------------------

typedef Buffer<reco::PFConversion, false>
recoPFConversion_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFConversion_t,
                  "recoPFConversion");
				  
typedef Buffer<reco::PFDisplacedTrackerVertex, false>
recoPFDisplacedTrackerVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFDisplacedTrackerVertex_t,
                  "recoPFDisplacedTrackerVertex");
				  
typedef Buffer<reco::PFDisplacedVertex, false>
recoPFDisplacedVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFDisplacedVertex_t,
                  "recoPFDisplacedVertex");
				  
typedef Buffer<reco::PFDisplacedVertexCandidate, false>
recoPFDisplacedVertexCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFDisplacedVertexCandidate_t,
                  "recoPFDisplacedVertexCandidate");
				  
typedef Buffer<reco::PFJet, false>
recoPFJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFJet_t,
                  "recoPFJet");
				  
typedef Buffer<reco::PFMET, false>
recoPFMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFMET_t,
                  "recoPFMET");
				  
typedef Buffer<reco::PFNuclearInteraction, false>
recoPFNuclearInteraction_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFNuclearInteraction_t,
                  "recoPFNuclearInteraction");
				  
typedef Buffer<reco::PFRecHit, false>
recoPFRecHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFRecHit_t,
                  "recoPFRecHit");
				  
typedef Buffer<reco::PFRecTrack, false>
recoPFRecTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFRecTrack_t,
                  "recoPFRecTrack");
				  
typedef Buffer<reco::PFSimParticle, false>
recoPFSimParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFSimParticle_t,
                  "recoPFSimParticle");
				  
typedef Buffer<reco::PFTau, false>
recoPFTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTau_t,
                  "recoPFTau");
				  
typedef Buffer<reco::PFTauDecayMode, false>
recoPFTauDecayMode_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTauDecayMode_t,
                  "recoPFTauDecayMode");
				  
typedef Buffer<reco::PFTauTagInfo, false>
recoPFTauTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTauTagInfo_t,
                  "recoPFTauTagInfo");
				  
typedef Buffer<reco::PFTrajectoryPoint, false>
recoPFTrajectoryPoint_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTrajectoryPoint_t,
                  "recoPFTrajectoryPoint");
				  
typedef Buffer<reco::PFV0, false>
recoPFV0_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFV0_t,
                  "recoPFV0");
				  
typedef Buffer<reco::Particle, false>
recoParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoParticle_t,
                  "recoParticle");
				  
typedef Buffer<reco::Photon, false>
recoPhoton_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPhoton_t,
                  "recoPhoton");
				  
typedef Buffer<reco::PhotonCore, false>
recoPhotonCore_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPhotonCore_t,
                  "recoPhotonCore");
				  
typedef Buffer<reco::PileUpPFCandidate, false>
recoPileUpPFCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPileUpPFCandidate_t,
                  "recoPileUpPFCandidate");
				  
typedef Buffer<reco::PreId, false>
recoPreId_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreId_t,
                  "recoPreId");
				  
typedef Buffer<reco::PreshowerCluster, false>
recoPreshowerCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreshowerCluster_t,
                  "recoPreshowerCluster");
				  
typedef Buffer<reco::PreshowerClusterShape, false>
recoPreshowerClusterShape_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreshowerClusterShape_t,
                  "recoPreshowerClusterShape");
				  
typedef Buffer<reco::RecoChargedCandidate, false>
recoRecoChargedCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoChargedCandidate_t,
                  "recoRecoChargedCandidate");
				  
typedef Buffer<reco::RecoChargedRefCandidate, false>
recoRecoChargedRefCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoChargedRefCandidate_t,
                  "recoRecoChargedRefCandidate");
				  
typedef Buffer<reco::RecoEcalCandidate, false>
recoRecoEcalCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoEcalCandidate_t,
                  "recoRecoEcalCandidate");
				  
typedef Buffer<reco::RecoPFClusterRefCandidate, false>
recoRecoPFClusterRefCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoPFClusterRefCandidate_t,
                  "recoRecoPFClusterRefCandidate");
				  
typedef Buffer<reco::RecoTauPiZero, false>
recoRecoTauPiZero_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoTauPiZero_t,
                  "recoRecoTauPiZero");
				  
typedef Buffer<reco::SiStripElectron, false>
recoSiStripElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSiStripElectron_t,
                  "recoSiStripElectron");
				  
typedef Buffer<reco::SuperCluster, false>
recoSuperCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSuperCluster_t,
                  "recoSuperCluster");
				  
typedef Buffer<reco::Track, false>
recoTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrack_t,
                  "recoTrack");
				  