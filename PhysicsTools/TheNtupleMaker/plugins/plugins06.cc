// -------------------------------------------------------------------------
// File::   plugins06.cc
// Created: Sat May  5 16:09:43 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/JTATagInfo.h"
#include "DataFormats/BTauReco/interface/JetCrystalsAssociation.h"
#include "DataFormats/BTauReco/interface/JetTagInfo.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCore.h"
#include "DataFormats/EgammaReco/interface/HFEMClusterShape.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackExtra.h"
#include "DataFormats/HcalIsolatedTrack/interface/IsolatedPixelTrackCandidate.h"
#include "DataFormats/HepMCCandidate/interface/FlavorHistory.h"
#include "DataFormats/HepMCCandidate/interface/FlavorHistoryEvent.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/FFTAnyJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/TrackJet.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/HcalNoiseHPD.h"
#include "DataFormats/METReco/interface/HcalNoiseRBX.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonCosmicCompatibility.h"
#include "DataFormats/MuonReco/interface/MuonMETCorrectionData.h"
#include "DataFormats/ParticleFlowCandidate/interface/IsolatedPFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/GsfPFRecTrack.h"
#include "DataFormats/TauReco/interface/HLTTau.h"
// -------------------------------------------------------------------------

std::string recoFFTAnyJetrecoJPTJet_n("reco::FFTAnyJet<reco::JPTJet>");
typedef Buffer<reco::FFTAnyJet<reco::JPTJet>,
               &recoFFTAnyJetrecoJPTJet_n, COLLECTION>
recoFFTAnyJetrecoJPTJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoFFTAnyJetrecoJPTJet_t,
                  "recoFFTAnyJetrecoJPTJet");
				  
std::string recoFFTAnyJetrecoPFJet_n("reco::FFTAnyJet<reco::PFJet>");
typedef Buffer<reco::FFTAnyJet<reco::PFJet>,
               &recoFFTAnyJetrecoPFJet_n, COLLECTION>
recoFFTAnyJetrecoPFJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoFFTAnyJetrecoPFJet_t,
                  "recoFFTAnyJetrecoPFJet");
				  
std::string recoFFTAnyJetrecoTrackJet_n("reco::FFTAnyJet<reco::TrackJet>");
typedef Buffer<reco::FFTAnyJet<reco::TrackJet>,
               &recoFFTAnyJetrecoTrackJet_n, COLLECTION>
recoFFTAnyJetrecoTrackJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoFFTAnyJetrecoTrackJet_t,
                  "recoFFTAnyJetrecoTrackJet");
				  
std::string recoFlavorHistory_n("reco::FlavorHistory");
typedef Buffer<reco::FlavorHistory,
               &recoFlavorHistory_n, COLLECTION>
recoFlavorHistory_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoFlavorHistory_t,
                  "recoFlavorHistory");
				  
std::string recoFlavorHistoryEvent_n("reco::FlavorHistoryEvent");
typedef Buffer<reco::FlavorHistoryEvent,
               &recoFlavorHistoryEvent_n, COLLECTION>
recoFlavorHistoryEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoFlavorHistoryEvent_t,
                  "recoFlavorHistoryEvent");
				  
std::string recoGenJet_n("reco::GenJet");
typedef Buffer<reco::GenJet,
               &recoGenJet_n, COLLECTION>
recoGenJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGenJet_t,
                  "recoGenJet");
				  
std::string recoGenMET_n("reco::GenMET");
typedef Buffer<reco::GenMET,
               &recoGenMET_n, COLLECTION>
recoGenMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGenMET_t,
                  "recoGenMET");
				  
std::string recoGenParticle_n("reco::GenParticle");
typedef Buffer<reco::GenParticle,
               &recoGenParticle_n, COLLECTION>
recoGenParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGenParticle_t,
                  "recoGenParticle");
				  
std::string recoGsfElectron_n("reco::GsfElectron");
typedef Buffer<reco::GsfElectron,
               &recoGsfElectron_n, COLLECTION>
recoGsfElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGsfElectron_t,
                  "recoGsfElectron");
				  
std::string recoGsfElectronCore_n("reco::GsfElectronCore");
typedef Buffer<reco::GsfElectronCore,
               &recoGsfElectronCore_n, COLLECTION>
recoGsfElectronCore_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGsfElectronCore_t,
                  "recoGsfElectronCore");
				  
std::string recoGsfPFRecTrack_n("reco::GsfPFRecTrack");
typedef Buffer<reco::GsfPFRecTrack,
               &recoGsfPFRecTrack_n, COLLECTION>
recoGsfPFRecTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGsfPFRecTrack_t,
                  "recoGsfPFRecTrack");
				  
std::string recoGsfTrack_n("reco::GsfTrack");
typedef Buffer<reco::GsfTrack,
               &recoGsfTrack_n, COLLECTION>
recoGsfTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGsfTrack_t,
                  "recoGsfTrack");
				  
std::string recoGsfTrackExtra_n("reco::GsfTrackExtra");
typedef Buffer<reco::GsfTrackExtra,
               &recoGsfTrackExtra_n, COLLECTION>
recoGsfTrackExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGsfTrackExtra_t,
                  "recoGsfTrackExtra");
				  
std::string recoHFEMClusterShape_n("reco::HFEMClusterShape");
typedef Buffer<reco::HFEMClusterShape,
               &recoHFEMClusterShape_n, COLLECTION>
recoHFEMClusterShape_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHFEMClusterShape_t,
                  "recoHFEMClusterShape");
				  
std::string recoHLTTau_n("reco::HLTTau");
typedef Buffer<reco::HLTTau,
               &recoHLTTau_n, COLLECTION>
recoHLTTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHLTTau_t,
                  "recoHLTTau");
				  
std::string recoHcalNoiseHPD_n("reco::HcalNoiseHPD");
typedef Buffer<reco::HcalNoiseHPD,
               &recoHcalNoiseHPD_n, COLLECTION>
recoHcalNoiseHPD_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHcalNoiseHPD_t,
                  "recoHcalNoiseHPD");
				  
std::string recoHcalNoiseRBX_n("reco::HcalNoiseRBX");
typedef Buffer<reco::HcalNoiseRBX,
               &recoHcalNoiseRBX_n, COLLECTION>
recoHcalNoiseRBX_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHcalNoiseRBX_t,
                  "recoHcalNoiseRBX");
				  
std::string recoIsolatedPFCandidate_n("reco::IsolatedPFCandidate");
typedef Buffer<reco::IsolatedPFCandidate,
               &recoIsolatedPFCandidate_n, COLLECTION>
recoIsolatedPFCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoIsolatedPFCandidate_t,
                  "recoIsolatedPFCandidate");
				  
std::string recoIsolatedPixelTrackCandidate_n("reco::IsolatedPixelTrackCandidate");
typedef Buffer<reco::IsolatedPixelTrackCandidate,
               &recoIsolatedPixelTrackCandidate_n, COLLECTION>
recoIsolatedPixelTrackCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoIsolatedPixelTrackCandidate_t,
                  "recoIsolatedPixelTrackCandidate");
				  
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
				  