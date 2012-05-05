// -------------------------------------------------------------------------
// File::   plugins07.cc
// Created: Sat May  5 17:06:51 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/JTATagInfo.h"
#include "DataFormats/BTauReco/interface/JetCrystalsAssociation.h"
#include "DataFormats/BTauReco/interface/JetTagInfo.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/Candidate/interface/NamedCompositeCandidate.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/JetReco/interface/PFClusterJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/PFClusterMET.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonCosmicCompatibility.h"
#include "DataFormats/MuonReco/interface/MuonMETCorrectionData.h"
#include "DataFormats/MuonReco/interface/MuonQuality.h"
#include "DataFormats/MuonReco/interface/MuonShower.h"
#include "DataFormats/MuonReco/interface/MuonTimeExtra.h"
#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"
#include "DataFormats/ParticleFlowCandidate/interface/IsolatedPFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateElectronExtra.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidatePhotonExtra.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementSuperCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFConversion.h"
#include "DataFormats/ParticleFlowReco/interface/PFDisplacedTrackerVertex.h"
#include "DataFormats/ParticleFlowReco/interface/PFDisplacedVertex.h"
#include "DataFormats/ParticleFlowReco/interface/PFDisplacedVertexCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFNuclearInteraction.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include "DataFormats/VertexReco/interface/NuclearInteraction.h"
// -------------------------------------------------------------------------

std::string recoIsolatedPFCandidate_n("reco::IsolatedPFCandidate");
typedef Buffer<reco::IsolatedPFCandidate,
               &recoIsolatedPFCandidate_n, COLLECTION>
recoIsolatedPFCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoIsolatedPFCandidate_t,
                  "recoIsolatedPFCandidate");
				  
std::string recoIsolatedTauTagInfo_n("reco::IsolatedTauTagInfo");
typedef Buffer<reco::IsolatedTauTagInfo,
               &recoIsolatedTauTagInfo_n, COLLECTION>
recoIsolatedTauTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoIsolatedTauTagInfo_t,
                  "recoIsolatedTauTagInfo");
				  
std::string recoJPTJet_n("reco::JPTJet");
typedef Buffer<reco::JPTJet,
               &recoJPTJet_n, COLLECTION>
recoJPTJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoJPTJet_t,
                  "recoJPTJet");
				  
std::string recoJTATagInfo_n("reco::JTATagInfo");
typedef Buffer<reco::JTATagInfo,
               &recoJTATagInfo_n, COLLECTION>
recoJTATagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoJTATagInfo_t,
                  "recoJTATagInfo");
				  
std::string recoJetCrystalsAssociation_n("reco::JetCrystalsAssociation");
typedef Buffer<reco::JetCrystalsAssociation,
               &recoJetCrystalsAssociation_n, COLLECTION>
recoJetCrystalsAssociation_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoJetCrystalsAssociation_t,
                  "recoJetCrystalsAssociation");
				  
std::string recoJetID_n("reco::JetID");
typedef Buffer<reco::JetID,
               &recoJetID_n, COLLECTION>
recoJetID_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoJetID_t,
                  "recoJetID");
				  
std::string recoJetTagInfo_n("reco::JetTagInfo");
typedef Buffer<reco::JetTagInfo,
               &recoJetTagInfo_n, COLLECTION>
recoJetTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoJetTagInfo_t,
                  "recoJetTagInfo");
				  
std::string recoLeafCandidate_n("reco::LeafCandidate");
typedef Buffer<reco::LeafCandidate,
               &recoLeafCandidate_n, COLLECTION>
recoLeafCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoLeafCandidate_t,
                  "recoLeafCandidate");
				  
std::string recoMET_n("reco::MET");
typedef Buffer<reco::MET,
               &recoMET_n, COLLECTION>
recoMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMET_t,
                  "recoMET");
				  
std::string recoMuon_n("reco::Muon");
typedef Buffer<reco::Muon,
               &recoMuon_n, COLLECTION>
recoMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuon_t,
                  "recoMuon");
				  
std::string recoMuonCosmicCompatibility_n("reco::MuonCosmicCompatibility");
typedef Buffer<reco::MuonCosmicCompatibility,
               &recoMuonCosmicCompatibility_n, COLLECTION>
recoMuonCosmicCompatibility_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuonCosmicCompatibility_t,
                  "recoMuonCosmicCompatibility");
				  
std::string recoMuonMETCorrectionData_n("reco::MuonMETCorrectionData");
typedef Buffer<reco::MuonMETCorrectionData,
               &recoMuonMETCorrectionData_n, COLLECTION>
recoMuonMETCorrectionData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuonMETCorrectionData_t,
                  "recoMuonMETCorrectionData");
				  
std::string recoMuonQuality_n("reco::MuonQuality");
typedef Buffer<reco::MuonQuality,
               &recoMuonQuality_n, COLLECTION>
recoMuonQuality_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuonQuality_t,
                  "recoMuonQuality");
				  
std::string recoMuonShower_n("reco::MuonShower");
typedef Buffer<reco::MuonShower,
               &recoMuonShower_n, COLLECTION>
recoMuonShower_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuonShower_t,
                  "recoMuonShower");
				  
std::string recoMuonTimeExtra_n("reco::MuonTimeExtra");
typedef Buffer<reco::MuonTimeExtra,
               &recoMuonTimeExtra_n, COLLECTION>
recoMuonTimeExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuonTimeExtra_t,
                  "recoMuonTimeExtra");
				  
std::string recoMuonTrackLinks_n("reco::MuonTrackLinks");
typedef Buffer<reco::MuonTrackLinks,
               &recoMuonTrackLinks_n, COLLECTION>
recoMuonTrackLinks_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuonTrackLinks_t,
                  "recoMuonTrackLinks");
				  
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
				  