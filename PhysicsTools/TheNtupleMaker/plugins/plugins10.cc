// -------------------------------------------------------------------------
// File::   plugins10.cc
// Created: Tue Apr  3 21:16:04 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCore.h"
#include "DataFormats/EgammaReco/interface/ElectronSeed.h"
#include "DataFormats/EgammaReco/interface/HFEMClusterShape.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackExtra.h"
#include "DataFormats/HepMCCandidate/interface/FlavorHistory.h"
#include "DataFormats/HepMCCandidate/interface/FlavorHistoryEvent.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonCosmicCompatibility.h"
#include "DataFormats/MuonReco/interface/MuonMETCorrectionData.h"
#include "DataFormats/MuonReco/interface/MuonQuality.h"
#include "DataFormats/MuonReco/interface/MuonShower.h"
#include "DataFormats/MuonReco/interface/MuonTimeExtra.h"
#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"
#include "DataFormats/ParticleFlowCandidate/interface/IsolatedPFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/GsfPFRecTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/TauReco/interface/HLTTau.h"
#include "DataFormats/VertexReco/interface/NuclearInteraction.h"
// -------------------------------------------------------------------------

typedef Buffer<reco::ElectronID, false>
recoElectronID_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoElectronID_t,
                  "recoElectronID");
				  
typedef Buffer<reco::ElectronSeed, false>
recoElectronSeed_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoElectronSeed_t,
                  "recoElectronSeed");
				  
typedef Buffer<reco::FlavorHistory, false>
recoFlavorHistory_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoFlavorHistory_t,
                  "recoFlavorHistory");
				  
typedef Buffer<reco::FlavorHistoryEvent, false>
recoFlavorHistoryEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoFlavorHistoryEvent_t,
                  "recoFlavorHistoryEvent");
				  
typedef Buffer<reco::GenJet, false>
recoGenJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGenJet_t,
                  "recoGenJet");
				  
typedef Buffer<reco::GenMET, false>
recoGenMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGenMET_t,
                  "recoGenMET");
				  
typedef Buffer<reco::GenParticle, false>
recoGenParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGenParticle_t,
                  "recoGenParticle");
				  
typedef Buffer<reco::GsfElectron, false>
recoGsfElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGsfElectron_t,
                  "recoGsfElectron");
				  
typedef Buffer<reco::GsfElectronCore, false>
recoGsfElectronCore_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGsfElectronCore_t,
                  "recoGsfElectronCore");
				  
typedef Buffer<reco::GsfPFRecTrack, false>
recoGsfPFRecTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGsfPFRecTrack_t,
                  "recoGsfPFRecTrack");
				  
typedef Buffer<reco::GsfTrack, false>
recoGsfTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGsfTrack_t,
                  "recoGsfTrack");
				  
typedef Buffer<reco::GsfTrackExtra, false>
recoGsfTrackExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGsfTrackExtra_t,
                  "recoGsfTrackExtra");
				  
typedef Buffer<reco::HFEMClusterShape, false>
recoHFEMClusterShape_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHFEMClusterShape_t,
                  "recoHFEMClusterShape");
				  
typedef Buffer<reco::HLTTau, false>
recoHLTTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHLTTau_t,
                  "recoHLTTau");
				  
typedef Buffer<reco::IsolatedPFCandidate, false>
recoIsolatedPFCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoIsolatedPFCandidate_t,
                  "recoIsolatedPFCandidate");
				  
typedef Buffer<reco::JPTJet, false>
recoJPTJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoJPTJet_t,
                  "recoJPTJet");
				  
typedef Buffer<reco::JetID, false>
recoJetID_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoJetID_t,
                  "recoJetID");
				  
typedef Buffer<reco::LeafCandidate, false>
recoLeafCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoLeafCandidate_t,
                  "recoLeafCandidate");
				  
typedef Buffer<reco::MET, false>
recoMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMET_t,
                  "recoMET");
				  
typedef Buffer<reco::Muon, false>
recoMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuon_t,
                  "recoMuon");
				  
typedef Buffer<reco::MuonCosmicCompatibility, false>
recoMuonCosmicCompatibility_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuonCosmicCompatibility_t,
                  "recoMuonCosmicCompatibility");
				  
typedef Buffer<reco::MuonMETCorrectionData, false>
recoMuonMETCorrectionData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuonMETCorrectionData_t,
                  "recoMuonMETCorrectionData");
				  
typedef Buffer<reco::MuonQuality, false>
recoMuonQuality_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuonQuality_t,
                  "recoMuonQuality");
				  
typedef Buffer<reco::MuonShower, false>
recoMuonShower_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuonShower_t,
                  "recoMuonShower");
				  
typedef Buffer<reco::MuonTimeExtra, false>
recoMuonTimeExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuonTimeExtra_t,
                  "recoMuonTimeExtra");
				  
typedef Buffer<reco::MuonTrackLinks, false>
recoMuonTrackLinks_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuonTrackLinks_t,
                  "recoMuonTrackLinks");
				  
typedef Buffer<reco::NuclearInteraction, false>
recoNuclearInteraction_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoNuclearInteraction_t,
                  "recoNuclearInteraction");
				  
typedef Buffer<reco::PFBlock, false>
recoPFBlock_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFBlock_t,
                  "recoPFBlock");
				  