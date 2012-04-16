// -------------------------------------------------------------------------
// File::   plugins11.cc
// Created: Sun Apr 15 22:17:19 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonCore.h"
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
// -------------------------------------------------------------------------

typedef Buffer<reco::PFBlock, false>
recoPFBlock_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFBlock_t,
                  "recoPFBlock");
				  
typedef Buffer<reco::PFBlockElementSuperCluster, false>
recoPFBlockElementSuperCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFBlockElementSuperCluster_t,
                  "recoPFBlockElementSuperCluster");
				  
typedef Buffer<reco::PFCandidate, false>
recoPFCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFCandidate_t,
                  "recoPFCandidate");
				  
typedef Buffer<reco::PFCandidateElectronExtra, false>
recoPFCandidateElectronExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFCandidateElectronExtra_t,
                  "recoPFCandidateElectronExtra");
				  
typedef Buffer<reco::PFCandidatePhotonExtra, false>
recoPFCandidatePhotonExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFCandidatePhotonExtra_t,
                  "recoPFCandidatePhotonExtra");
				  
typedef Buffer<reco::PFCluster, false>
recoPFCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFCluster_t,
                  "recoPFCluster");
				  
typedef Buffer<reco::PFClusterJet, false>
recoPFClusterJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFClusterJet_t,
                  "recoPFClusterJet");
				  
typedef Buffer<reco::PFClusterMET, false>
recoPFClusterMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFClusterMET_t,
                  "recoPFClusterMET");
				  
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
				  