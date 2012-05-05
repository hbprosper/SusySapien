// -------------------------------------------------------------------------
// File::   plugins04.cc
// Created: Sun May  6 00:03:31 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/TopObjects/interface/CATopJetTagInfo.h"
#include "DataFormats/BTauReco/interface/BaseTagInfo.h"
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"
#include "DataFormats/ParticleFlowReco/interface/Calibratable.h"
#include "DataFormats/ParticleFlowReco/interface/ParticleFiltrationDecision.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/EventHypothesis.h"
#include "DataFormats/PatCandidates/interface/GenericParticle.h"
#include "DataFormats/PatCandidates/interface/Hemisphere.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/MHT.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/PFParticle.h"
#include "DataFormats/PatCandidates/interface/Particle.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/TauJetCorrFactors.h"
#include "DataFormats/PatCandidates/interface/TriggerAlgorithm.h"
#include "DataFormats/PatCandidates/interface/TriggerCondition.h"
#include "DataFormats/PatCandidates/interface/TriggerFilter.h"
#include "DataFormats/PatCandidates/interface/TriggerObject.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/TriggerPath.h"
#include "DataFormats/TauReco/interface/BaseTau.h"
#include "DataFormats/TauReco/interface/BaseTauTagInfo.h"
// -------------------------------------------------------------------------

std::string l1extraL1MuonParticle_n("l1extra::L1MuonParticle");
typedef Buffer<l1extra::L1MuonParticle,
               &l1extraL1MuonParticle_n, COLLECTION>
l1extraL1MuonParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, l1extraL1MuonParticle_t,
                  "l1extraL1MuonParticle");
				  
std::string l1extraL1ParticleMap_n("l1extra::L1ParticleMap");
typedef Buffer<l1extra::L1ParticleMap,
               &l1extraL1ParticleMap_n, COLLECTION>
l1extraL1ParticleMap_t;
DEFINE_EDM_PLUGIN(BufferFactory, l1extraL1ParticleMap_t,
                  "l1extraL1ParticleMap");
				  
std::string patCompositeCandidate_n("pat::CompositeCandidate");
typedef Buffer<pat::CompositeCandidate,
               &patCompositeCandidate_n, COLLECTION>
patCompositeCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, patCompositeCandidate_t,
                  "patCompositeCandidate");
				  
std::string patElectron_n("pat::Electron");
typedef Buffer<pat::Electron,
               &patElectron_n, COLLECTION>
patElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, patElectron_t,
                  "patElectron");
				  
std::string patEventHypothesis_n("pat::EventHypothesis");
typedef Buffer<pat::EventHypothesis,
               &patEventHypothesis_n, COLLECTION>
patEventHypothesis_t;
DEFINE_EDM_PLUGIN(BufferFactory, patEventHypothesis_t,
                  "patEventHypothesis");
				  
std::string patGenericParticle_n("pat::GenericParticle");
typedef Buffer<pat::GenericParticle,
               &patGenericParticle_n, COLLECTION>
patGenericParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, patGenericParticle_t,
                  "patGenericParticle");
				  
std::string patHemisphere_n("pat::Hemisphere");
typedef Buffer<pat::Hemisphere,
               &patHemisphere_n, COLLECTION>
patHemisphere_t;
DEFINE_EDM_PLUGIN(BufferFactory, patHemisphere_t,
                  "patHemisphere");
				  
std::string patJet_n("pat::Jet");
typedef Buffer<pat::Jet,
               &patJet_n, COLLECTION>
patJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, patJet_t,
                  "patJet");
				  
std::string patMET_n("pat::MET");
typedef Buffer<pat::MET,
               &patMET_n, COLLECTION>
patMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMET_t,
                  "patMET");
				  
std::string patMHT_n("pat::MHT");
typedef Buffer<pat::MHT,
               &patMHT_n, COLLECTION>
patMHT_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMHT_t,
                  "patMHT");
				  
std::string patMuon_n("pat::Muon");
typedef Buffer<pat::Muon,
               &patMuon_n, COLLECTION>
patMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMuon_t,
                  "patMuon");
				  
std::string patPFParticle_n("pat::PFParticle");
typedef Buffer<pat::PFParticle,
               &patPFParticle_n, COLLECTION>
patPFParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, patPFParticle_t,
                  "patPFParticle");
				  
std::string patParticle_n("pat::Particle");
typedef Buffer<pat::Particle,
               &patParticle_n, COLLECTION>
patParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, patParticle_t,
                  "patParticle");
				  
std::string patPhoton_n("pat::Photon");
typedef Buffer<pat::Photon,
               &patPhoton_n, COLLECTION>
patPhoton_t;
DEFINE_EDM_PLUGIN(BufferFactory, patPhoton_t,
                  "patPhoton");
				  
std::string patTau_n("pat::Tau");
typedef Buffer<pat::Tau,
               &patTau_n, COLLECTION>
patTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTau_t,
                  "patTau");
				  
std::string patTauJetCorrFactorsCorrectionFactor_n("pat::TauJetCorrFactors::CorrectionFactor");
typedef Buffer<pat::TauJetCorrFactors::CorrectionFactor,
               &patTauJetCorrFactorsCorrectionFactor_n, COLLECTION>
patTauJetCorrFactorsCorrectionFactor_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTauJetCorrFactorsCorrectionFactor_t,
                  "patTauJetCorrFactorsCorrectionFactor");
				  
std::string patTriggerAlgorithm_n("pat::TriggerAlgorithm");
typedef Buffer<pat::TriggerAlgorithm,
               &patTriggerAlgorithm_n, COLLECTION>
patTriggerAlgorithm_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerAlgorithm_t,
                  "patTriggerAlgorithm");
				  
std::string patTriggerCondition_n("pat::TriggerCondition");
typedef Buffer<pat::TriggerCondition,
               &patTriggerCondition_n, COLLECTION>
patTriggerCondition_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerCondition_t,
                  "patTriggerCondition");
				  
std::string patTriggerFilter_n("pat::TriggerFilter");
typedef Buffer<pat::TriggerFilter,
               &patTriggerFilter_n, COLLECTION>
patTriggerFilter_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerFilter_t,
                  "patTriggerFilter");
				  
std::string patTriggerObject_n("pat::TriggerObject");
typedef Buffer<pat::TriggerObject,
               &patTriggerObject_n, COLLECTION>
patTriggerObject_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerObject_t,
                  "patTriggerObject");
				  
std::string patTriggerObjectStandAlone_n("pat::TriggerObjectStandAlone");
typedef Buffer<pat::TriggerObjectStandAlone,
               &patTriggerObjectStandAlone_n, COLLECTION>
patTriggerObjectStandAlone_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerObjectStandAlone_t,
                  "patTriggerObjectStandAlone");
				  
std::string patTriggerPath_n("pat::TriggerPath");
typedef Buffer<pat::TriggerPath,
               &patTriggerPath_n, COLLECTION>
patTriggerPath_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerPath_t,
                  "patTriggerPath");
				  
std::string pftoolsCalibratable_n("pftools::Calibratable");
typedef Buffer<pftools::Calibratable,
               &pftoolsCalibratable_n, COLLECTION>
pftoolsCalibratable_t;
DEFINE_EDM_PLUGIN(BufferFactory, pftoolsCalibratable_t,
                  "pftoolsCalibratable");
				  
std::string pftoolsParticleFiltrationDecision_n("pftools::ParticleFiltrationDecision");
typedef Buffer<pftools::ParticleFiltrationDecision,
               &pftoolsParticleFiltrationDecision_n, COLLECTION>
pftoolsParticleFiltrationDecision_t;
DEFINE_EDM_PLUGIN(BufferFactory, pftoolsParticleFiltrationDecision_t,
                  "pftoolsParticleFiltrationDecision");
				  
std::string recoBaseTagInfo_n("reco::BaseTagInfo");
typedef Buffer<reco::BaseTagInfo,
               &recoBaseTagInfo_n, COLLECTION>
recoBaseTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBaseTagInfo_t,
                  "recoBaseTagInfo");
				  
std::string recoBaseTau_n("reco::BaseTau");
typedef Buffer<reco::BaseTau,
               &recoBaseTau_n, COLLECTION>
recoBaseTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBaseTau_t,
                  "recoBaseTau");
				  
std::string recoBaseTauTagInfo_n("reco::BaseTauTagInfo");
typedef Buffer<reco::BaseTauTagInfo,
               &recoBaseTauTagInfo_n, COLLECTION>
recoBaseTauTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBaseTauTagInfo_t,
                  "recoBaseTauTagInfo");
				  
std::string recoBasicCluster_n("reco::BasicCluster");
typedef Buffer<reco::BasicCluster,
               &recoBasicCluster_n, COLLECTION>
recoBasicCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBasicCluster_t,
                  "recoBasicCluster");
				  
std::string recoBasicJet_n("reco::BasicJet");
typedef Buffer<reco::BasicJet,
               &recoBasicJet_n, COLLECTION>
recoBasicJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBasicJet_t,
                  "recoBasicJet");
				  
std::string recoCATopJetTagInfo_n("reco::CATopJetTagInfo");
typedef Buffer<reco::CATopJetTagInfo,
               &recoCATopJetTagInfo_n, COLLECTION>
recoCATopJetTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCATopJetTagInfo_t,
                  "recoCATopJetTagInfo");
				  
std::string recoCaloCluster_n("reco::CaloCluster");
typedef Buffer<reco::CaloCluster,
               &recoCaloCluster_n, COLLECTION>
recoCaloCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloCluster_t,
                  "recoCaloCluster");
				  