// -------------------------------------------------------------------------
// File::   plugins07.cc
// Created: Mon May  7 06:32:21 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "DataFormats/Candidate/interface/NamedCompositeCandidate.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonCore.h"
#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
#include "DataFormats/JetReco/interface/PFClusterJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/METReco/interface/PFClusterMET.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateElectronExtra.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidatePhotonExtra.h"
#include "DataFormats/ParticleFlowCandidate/interface/PileUpPFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementSuperCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
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
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauDecayMode.h"
#include "DataFormats/TauReco/interface/PFTauTagInfo.h"
#include "DataFormats/VertexReco/interface/NuclearInteraction.h"
// -------------------------------------------------------------------------

std::string recoNamedCompositeCandidate_n("reco::NamedCompositeCandidate");
typedef Buffer<reco::NamedCompositeCandidate,
               &recoNamedCompositeCandidate_n, COLLECTION>
recoNamedCompositeCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoNamedCompositeCandidate_t,
                  "recoNamedCompositeCandidate");
				  
std::string recoNuclearInteraction_n("reco::NuclearInteraction");
typedef Buffer<reco::NuclearInteraction,
               &recoNuclearInteraction_n, COLLECTION>
recoNuclearInteraction_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoNuclearInteraction_t,
                  "recoNuclearInteraction");
				  
std::string recoPFBlock_n("reco::PFBlock");
typedef Buffer<reco::PFBlock,
               &recoPFBlock_n, COLLECTION>
recoPFBlock_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFBlock_t,
                  "recoPFBlock");
				  
std::string recoPFBlockElementSuperCluster_n("reco::PFBlockElementSuperCluster");
typedef Buffer<reco::PFBlockElementSuperCluster,
               &recoPFBlockElementSuperCluster_n, COLLECTION>
recoPFBlockElementSuperCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFBlockElementSuperCluster_t,
                  "recoPFBlockElementSuperCluster");
				  
std::string recoPFCandidate_n("reco::PFCandidate");
typedef Buffer<reco::PFCandidate,
               &recoPFCandidate_n, COLLECTION>
recoPFCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFCandidate_t,
                  "recoPFCandidate");
				  
std::string recoPFCandidateElectronExtra_n("reco::PFCandidateElectronExtra");
typedef Buffer<reco::PFCandidateElectronExtra,
               &recoPFCandidateElectronExtra_n, COLLECTION>
recoPFCandidateElectronExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFCandidateElectronExtra_t,
                  "recoPFCandidateElectronExtra");
				  
std::string recoPFCandidatePhotonExtra_n("reco::PFCandidatePhotonExtra");
typedef Buffer<reco::PFCandidatePhotonExtra,
               &recoPFCandidatePhotonExtra_n, COLLECTION>
recoPFCandidatePhotonExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFCandidatePhotonExtra_t,
                  "recoPFCandidatePhotonExtra");
				  
std::string recoPFCluster_n("reco::PFCluster");
typedef Buffer<reco::PFCluster,
               &recoPFCluster_n, COLLECTION>
recoPFCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFCluster_t,
                  "recoPFCluster");
				  
std::string recoPFClusterJet_n("reco::PFClusterJet");
typedef Buffer<reco::PFClusterJet,
               &recoPFClusterJet_n, COLLECTION>
recoPFClusterJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFClusterJet_t,
                  "recoPFClusterJet");
				  
std::string recoPFClusterMET_n("reco::PFClusterMET");
typedef Buffer<reco::PFClusterMET,
               &recoPFClusterMET_n, COLLECTION>
recoPFClusterMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFClusterMET_t,
                  "recoPFClusterMET");
				  
std::string recoPFConversion_n("reco::PFConversion");
typedef Buffer<reco::PFConversion,
               &recoPFConversion_n, COLLECTION>
recoPFConversion_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFConversion_t,
                  "recoPFConversion");
				  
std::string recoPFDisplacedTrackerVertex_n("reco::PFDisplacedTrackerVertex");
typedef Buffer<reco::PFDisplacedTrackerVertex,
               &recoPFDisplacedTrackerVertex_n, COLLECTION>
recoPFDisplacedTrackerVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFDisplacedTrackerVertex_t,
                  "recoPFDisplacedTrackerVertex");
				  
std::string recoPFDisplacedVertex_n("reco::PFDisplacedVertex");
typedef Buffer<reco::PFDisplacedVertex,
               &recoPFDisplacedVertex_n, COLLECTION>
recoPFDisplacedVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFDisplacedVertex_t,
                  "recoPFDisplacedVertex");
				  
std::string recoPFDisplacedVertexCandidate_n("reco::PFDisplacedVertexCandidate");
typedef Buffer<reco::PFDisplacedVertexCandidate,
               &recoPFDisplacedVertexCandidate_n, COLLECTION>
recoPFDisplacedVertexCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFDisplacedVertexCandidate_t,
                  "recoPFDisplacedVertexCandidate");
				  
std::string recoPFJet_n("reco::PFJet");
typedef Buffer<reco::PFJet,
               &recoPFJet_n, COLLECTION>
recoPFJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFJet_t,
                  "recoPFJet");
				  
std::string recoPFMET_n("reco::PFMET");
typedef Buffer<reco::PFMET,
               &recoPFMET_n, COLLECTION>
recoPFMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFMET_t,
                  "recoPFMET");
				  
std::string recoPFNuclearInteraction_n("reco::PFNuclearInteraction");
typedef Buffer<reco::PFNuclearInteraction,
               &recoPFNuclearInteraction_n, COLLECTION>
recoPFNuclearInteraction_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFNuclearInteraction_t,
                  "recoPFNuclearInteraction");
				  
std::string recoPFRecHit_n("reco::PFRecHit");
typedef Buffer<reco::PFRecHit,
               &recoPFRecHit_n, COLLECTION>
recoPFRecHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFRecHit_t,
                  "recoPFRecHit");
				  
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
				  