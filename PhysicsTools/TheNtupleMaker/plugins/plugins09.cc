// -------------------------------------------------------------------------
// File::   plugins09.cc
// Created: Tue Apr  3 21:40:06 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "DataFormats/CastorReco/interface/CastorCell.h"
#include "DataFormats/CastorReco/interface/CastorCluster.h"
#include "DataFormats/CastorReco/interface/CastorEgamma.h"
#include "DataFormats/CastorReco/interface/CastorJet.h"
#include "DataFormats/CastorReco/interface/CastorTower.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CastorJetID.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/MuonReco/interface/CaloMuon.h"
#include "DataFormats/ParticleFlowReco/interface/Calibratable.h"
#include "DataFormats/ParticleFlowReco/interface/ConvBremSeed.h"
#include "DataFormats/ParticleFlowReco/interface/ParticleFiltrationDecision.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/TriggerAlgorithm.h"
#include "DataFormats/PatCandidates/interface/TriggerCondition.h"
#include "DataFormats/PatCandidates/interface/TriggerFilter.h"
#include "DataFormats/PatCandidates/interface/TriggerObject.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/TriggerPath.h"
#include "DataFormats/TauReco/interface/BaseTau.h"
#include "DataFormats/TauReco/interface/BaseTauTagInfo.h"
#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/TauReco/interface/CaloTauTagInfo.h"
// -------------------------------------------------------------------------

typedef Buffer<pat::Tau, false>
patTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTau_t,
                  "patTau");
				  
typedef Buffer<pat::TriggerAlgorithm, false>
patTriggerAlgorithm_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerAlgorithm_t,
                  "patTriggerAlgorithm");
				  
typedef Buffer<pat::TriggerCondition, false>
patTriggerCondition_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerCondition_t,
                  "patTriggerCondition");
				  
typedef Buffer<pat::TriggerFilter, false>
patTriggerFilter_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerFilter_t,
                  "patTriggerFilter");
				  
typedef Buffer<pat::TriggerObject, false>
patTriggerObject_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerObject_t,
                  "patTriggerObject");
				  
typedef Buffer<pat::TriggerObjectStandAlone, false>
patTriggerObjectStandAlone_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerObjectStandAlone_t,
                  "patTriggerObjectStandAlone");
				  
typedef Buffer<pat::TriggerPath, false>
patTriggerPath_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerPath_t,
                  "patTriggerPath");
				  
typedef Buffer<pftools::Calibratable, false>
pftoolsCalibratable_t;
DEFINE_EDM_PLUGIN(BufferFactory, pftoolsCalibratable_t,
                  "pftoolsCalibratable");
				  
typedef Buffer<pftools::ParticleFiltrationDecision, false>
pftoolsParticleFiltrationDecision_t;
DEFINE_EDM_PLUGIN(BufferFactory, pftoolsParticleFiltrationDecision_t,
                  "pftoolsParticleFiltrationDecision");
				  
typedef Buffer<reco::BaseTau, false>
recoBaseTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBaseTau_t,
                  "recoBaseTau");
				  
typedef Buffer<reco::BaseTauTagInfo, false>
recoBaseTauTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBaseTauTagInfo_t,
                  "recoBaseTauTagInfo");
				  
typedef Buffer<reco::BasicCluster, false>
recoBasicCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBasicCluster_t,
                  "recoBasicCluster");
				  
typedef Buffer<reco::BasicJet, false>
recoBasicJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBasicJet_t,
                  "recoBasicJet");
				  
typedef Buffer<reco::CaloCluster, false>
recoCaloCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloCluster_t,
                  "recoCaloCluster");
				  
typedef Buffer<reco::CaloJet, false>
recoCaloJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloJet_t,
                  "recoCaloJet");
				  
typedef Buffer<reco::CaloMET, false>
recoCaloMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloMET_t,
                  "recoCaloMET");
				  
typedef Buffer<reco::CaloMuon, false>
recoCaloMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloMuon_t,
                  "recoCaloMuon");
				  
typedef Buffer<reco::CaloTau, false>
recoCaloTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloTau_t,
                  "recoCaloTau");
				  
typedef Buffer<reco::CaloTauTagInfo, false>
recoCaloTauTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloTauTagInfo_t,
                  "recoCaloTauTagInfo");
				  
typedef Buffer<reco::CastorCell, false>
recoCastorCell_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorCell_t,
                  "recoCastorCell");
				  
typedef Buffer<reco::CastorCluster, false>
recoCastorCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorCluster_t,
                  "recoCastorCluster");
				  
typedef Buffer<reco::CastorEgamma, false>
recoCastorEgamma_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorEgamma_t,
                  "recoCastorEgamma");
				  
typedef Buffer<reco::CastorJet, false>
recoCastorJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorJet_t,
                  "recoCastorJet");
				  
typedef Buffer<reco::CastorJetID, false>
recoCastorJetID_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorJetID_t,
                  "recoCastorJetID");
				  
typedef Buffer<reco::CastorTower, false>
recoCastorTower_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorTower_t,
                  "recoCastorTower");
				  
typedef Buffer<reco::ClusterShape, false>
recoClusterShape_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoClusterShape_t,
                  "recoClusterShape");
				  
typedef Buffer<reco::ConvBremSeed, false>
recoConvBremSeed_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoConvBremSeed_t,
                  "recoConvBremSeed");
				  
typedef Buffer<reco::Conversion, false>
recoConversion_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoConversion_t,
                  "recoConversion");
				  