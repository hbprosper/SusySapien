// -------------------------------------------------------------------------
// File::   plugins06.cc
// Created: Sun May  6 00:40:38 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "DataFormats/BTauReco/interface/CombinedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/EMIsolatedTauTagInfo.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/CastorReco/interface/CastorCluster.h"
#include "DataFormats/CastorReco/interface/CastorEgamma.h"
#include "DataFormats/CastorReco/interface/CastorJet.h"
#include "DataFormats/CastorReco/interface/CastorTower.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCore.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/EgammaReco/interface/ElectronSeed.h"
#include "DataFormats/EgammaTrackReco/interface/ConversionTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/HcalIsolatedTrack/interface/EcalIsolatedParticleCandidate.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "DataFormats/HeavyIonEvent/interface/EvtPlane.h"
#include "DataFormats/HepMCCandidate/interface/FlavorHistory.h"
#include "DataFormats/HepMCCandidate/interface/FlavorHistoryEvent.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CastorJetID.h"
#include "DataFormats/JetReco/interface/FFTAnyJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/TrackJet.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/ParticleFlowReco/interface/ConvBremSeed.h"
#include "DataFormats/ParticleFlowReco/interface/GsfPFRecTrack.h"
#include "DataFormats/TrackReco/interface/DeDxData.h"
// -------------------------------------------------------------------------

std::string recoCastorCluster_n("reco::CastorCluster");
typedef Buffer<reco::CastorCluster,
               &recoCastorCluster_n, COLLECTION>
recoCastorCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorCluster_t,
                  "recoCastorCluster");
				  
std::string recoCastorEgamma_n("reco::CastorEgamma");
typedef Buffer<reco::CastorEgamma,
               &recoCastorEgamma_n, COLLECTION>
recoCastorEgamma_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorEgamma_t,
                  "recoCastorEgamma");
				  
std::string recoCastorJet_n("reco::CastorJet");
typedef Buffer<reco::CastorJet,
               &recoCastorJet_n, COLLECTION>
recoCastorJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorJet_t,
                  "recoCastorJet");
				  
std::string recoCastorJetID_n("reco::CastorJetID");
typedef Buffer<reco::CastorJetID,
               &recoCastorJetID_n, COLLECTION>
recoCastorJetID_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorJetID_t,
                  "recoCastorJetID");
				  
std::string recoCastorTower_n("reco::CastorTower");
typedef Buffer<reco::CastorTower,
               &recoCastorTower_n, COLLECTION>
recoCastorTower_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorTower_t,
                  "recoCastorTower");
				  
std::string recoCentrality_n("reco::Centrality");
typedef Buffer<reco::Centrality,
               &recoCentrality_n, COLLECTION>
recoCentrality_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCentrality_t,
                  "recoCentrality");
				  
std::string recoClusterShape_n("reco::ClusterShape");
typedef Buffer<reco::ClusterShape,
               &recoClusterShape_n, COLLECTION>
recoClusterShape_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoClusterShape_t,
                  "recoClusterShape");
				  
std::string recoCombinedTauTagInfo_n("reco::CombinedTauTagInfo");
typedef Buffer<reco::CombinedTauTagInfo,
               &recoCombinedTauTagInfo_n, COLLECTION>
recoCombinedTauTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCombinedTauTagInfo_t,
                  "recoCombinedTauTagInfo");
				  
std::string recoCompositeCandidate_n("reco::CompositeCandidate");
typedef Buffer<reco::CompositeCandidate,
               &recoCompositeCandidate_n, COLLECTION>
recoCompositeCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCompositeCandidate_t,
                  "recoCompositeCandidate");
				  
std::string recoConvBremSeed_n("reco::ConvBremSeed");
typedef Buffer<reco::ConvBremSeed,
               &recoConvBremSeed_n, COLLECTION>
recoConvBremSeed_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoConvBremSeed_t,
                  "recoConvBremSeed");
				  
std::string recoConversion_n("reco::Conversion");
typedef Buffer<reco::Conversion,
               &recoConversion_n, COLLECTION>
recoConversion_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoConversion_t,
                  "recoConversion");
				  
std::string recoConversionTrack_n("reco::ConversionTrack");
typedef Buffer<reco::ConversionTrack,
               &recoConversionTrack_n, COLLECTION>
recoConversionTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoConversionTrack_t,
                  "recoConversionTrack");
				  
std::string recoDeDxData_n("reco::DeDxData");
typedef Buffer<reco::DeDxData,
               &recoDeDxData_n, COLLECTION>
recoDeDxData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoDeDxData_t,
                  "recoDeDxData");
				  
std::string recoEMIsolatedTauTagInfo_n("reco::EMIsolatedTauTagInfo");
typedef Buffer<reco::EMIsolatedTauTagInfo,
               &recoEMIsolatedTauTagInfo_n, COLLECTION>
recoEMIsolatedTauTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoEMIsolatedTauTagInfo_t,
                  "recoEMIsolatedTauTagInfo");
				  
std::string recoEcalIsolatedParticleCandidate_n("reco::EcalIsolatedParticleCandidate");
typedef Buffer<reco::EcalIsolatedParticleCandidate,
               &recoEcalIsolatedParticleCandidate_n, COLLECTION>
recoEcalIsolatedParticleCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoEcalIsolatedParticleCandidate_t,
                  "recoEcalIsolatedParticleCandidate");
				  
std::string recoElectron_n("reco::Electron");
typedef Buffer<reco::Electron,
               &recoElectron_n, COLLECTION>
recoElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoElectron_t,
                  "recoElectron");
				  
std::string recoElectronID_n("reco::ElectronID");
typedef Buffer<reco::ElectronID,
               &recoElectronID_n, COLLECTION>
recoElectronID_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoElectronID_t,
                  "recoElectronID");
				  
std::string recoElectronSeed_n("reco::ElectronSeed");
typedef Buffer<reco::ElectronSeed,
               &recoElectronSeed_n, COLLECTION>
recoElectronSeed_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoElectronSeed_t,
                  "recoElectronSeed");
				  
std::string recoEvtPlane_n("reco::EvtPlane");
typedef Buffer<reco::EvtPlane,
               &recoEvtPlane_n, COLLECTION>
recoEvtPlane_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoEvtPlane_t,
                  "recoEvtPlane");
				  
std::string recoFFTAnyJetrecoBasicJet_n("reco::FFTAnyJet<reco::BasicJet>");
typedef Buffer<reco::FFTAnyJet<reco::BasicJet>,
               &recoFFTAnyJetrecoBasicJet_n, COLLECTION>
recoFFTAnyJetrecoBasicJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoFFTAnyJetrecoBasicJet_t,
                  "recoFFTAnyJetrecoBasicJet");
				  
std::string recoFFTAnyJetrecoCaloJet_n("reco::FFTAnyJet<reco::CaloJet>");
typedef Buffer<reco::FFTAnyJet<reco::CaloJet>,
               &recoFFTAnyJetrecoCaloJet_n, COLLECTION>
recoFFTAnyJetrecoCaloJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoFFTAnyJetrecoCaloJet_t,
                  "recoFFTAnyJetrecoCaloJet");
				  
std::string recoFFTAnyJetrecoGenJet_n("reco::FFTAnyJet<reco::GenJet>");
typedef Buffer<reco::FFTAnyJet<reco::GenJet>,
               &recoFFTAnyJetrecoGenJet_n, COLLECTION>
recoFFTAnyJetrecoGenJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoFFTAnyJetrecoGenJet_t,
                  "recoFFTAnyJetrecoGenJet");
				  
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
				  