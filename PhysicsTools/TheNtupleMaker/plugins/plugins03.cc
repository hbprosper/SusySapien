// -------------------------------------------------------------------------
// File::   plugins03.cc
// Created: Tue Apr  3 22:26:00 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/TopObjects/interface/TtFullLeptonicEvent.h"
#include "AnalysisDataFormats/TopObjects/interface/TtGenEvent.h"
#include "AnalysisDataFormats/TopObjects/interface/TtSemiLeptonicEvent.h"
#include "DataFormats/Common/interface/Association.h"
#include "DataFormats/Common/interface/ConditionsInEdm.h"
#include "DataFormats/Common/interface/DataFrameContainer.h"
#include "DataFormats/Common/interface/HLTGlobalStatus.h"
#include "DataFormats/Common/interface/MergeableCounter.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/HLTReco/interface/ModuleTiming.h"
#include "DataFormats/HeavyIonEvent/interface/HeavyIon.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/TriggerAlgorithm.h"
#include "DataFormats/PatCandidates/interface/TriggerCondition.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/TriggerFilter.h"
#include "DataFormats/PatCandidates/interface/TriggerObject.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/TriggerPath.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauDecayMode.h"
#include "SimDataFormats/HiGenData/interface/GenHIEvent.h"
// -------------------------------------------------------------------------

typedef Buffer<TtFullLeptonicEvent, true>
TtFullLeptonicEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, TtFullLeptonicEvent_t,
                  "TtFullLeptonicEvent");
				  
typedef Buffer<TtGenEvent, true>
TtGenEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, TtGenEvent_t,
                  "TtGenEvent");
				  
typedef Buffer<TtSemiLeptonicEvent, true>
TtSemiLeptonicEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, TtSemiLeptonicEvent_t,
                  "TtSemiLeptonicEvent");
				  
typedef Buffer<edm::Association<std::vector<pat::TriggerObject> >, true>
edmAssociationstdvectorpatTriggerObject_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmAssociationstdvectorpatTriggerObject_t,
                  "edmAssociationstdvectorpatTriggerObject");
				  
typedef Buffer<edm::Association<std::vector<pat::TriggerObjectStandAlone> >, true>
edmAssociationstdvectorpatTriggerObjectStandAlone_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmAssociationstdvectorpatTriggerObjectStandAlone_t,
                  "edmAssociationstdvectorpatTriggerObjectStandAlone");
				  
typedef Buffer<edm::Association<std::vector<reco::GenParticle> >, true>
edmAssociationstdvectorrecoGenParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmAssociationstdvectorrecoGenParticle_t,
                  "edmAssociationstdvectorrecoGenParticle");
				  
typedef Buffer<edm::Association<std::vector<reco::PFTau> >, true>
edmAssociationstdvectorrecoPFTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmAssociationstdvectorrecoPFTau_t,
                  "edmAssociationstdvectorrecoPFTau");
				  
typedef Buffer<edm::Association<std::vector<reco::PFTauDecayMode> >, true>
edmAssociationstdvectorrecoPFTauDecayMode_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmAssociationstdvectorrecoPFTauDecayMode_t,
                  "edmAssociationstdvectorrecoPFTauDecayMode");
				  
typedef Buffer<edm::ConditionsInEventBlock, true>
edmConditionsInEventBlock_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmConditionsInEventBlock_t,
                  "edmConditionsInEventBlock");
				  
typedef Buffer<edm::ConditionsInLumiBlock, true>
edmConditionsInLumiBlock_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmConditionsInLumiBlock_t,
                  "edmConditionsInLumiBlock");
				  
typedef Buffer<edm::ConditionsInRunBlock, true>
edmConditionsInRunBlock_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmConditionsInRunBlock_t,
                  "edmConditionsInRunBlock");
				  
typedef Buffer<edm::DataFrameContainer, true>
edmDataFrameContainer_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDataFrameContainer_t,
                  "edmDataFrameContainer");
				  
typedef Buffer<edm::EventTime, true>
edmEventTime_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmEventTime_t,
                  "edmEventTime");
				  
typedef Buffer<edm::GenHIEvent, true>
edmGenHIEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmGenHIEvent_t,
                  "edmGenHIEvent");
				  
typedef Buffer<edm::HLTGlobalStatus, true>
edmHLTGlobalStatus_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmHLTGlobalStatus_t,
                  "edmHLTGlobalStatus");
				  
typedef Buffer<edm::MergeableCounter, true>
edmMergeableCounter_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmMergeableCounter_t,
                  "edmMergeableCounter");
				  
typedef Buffer<edm::Ptr<reco::GsfElectron>, true>
edmPtrrecoGsfElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmPtrrecoGsfElectron_t,
                  "edmPtrrecoGsfElectron");
				  
typedef Buffer<edm::Ptr<reco::Muon>, true>
edmPtrrecoMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmPtrrecoMuon_t,
                  "edmPtrrecoMuon");
				  
typedef Buffer<edm::TriggerResults, true>
edmTriggerResults_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmTriggerResults_t,
                  "edmTriggerResults");
				  
typedef Buffer<pat::HeavyIon, true>
patHeavyIon_t;
DEFINE_EDM_PLUGIN(BufferFactory, patHeavyIon_t,
                  "patHeavyIon");
				  
typedef Buffer<pat::TriggerAlgorithmRefProd, true>
patTriggerAlgorithmRefProd_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerAlgorithmRefProd_t,
                  "patTriggerAlgorithmRefProd");
				  
typedef Buffer<pat::TriggerConditionRefProd, true>
patTriggerConditionRefProd_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerConditionRefProd_t,
                  "patTriggerConditionRefProd");
				  
typedef Buffer<pat::TriggerEvent, true>
patTriggerEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerEvent_t,
                  "patTriggerEvent");
				  
typedef Buffer<pat::TriggerFilterRefProd, true>
patTriggerFilterRefProd_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerFilterRefProd_t,
                  "patTriggerFilterRefProd");
				  
typedef Buffer<pat::TriggerObjectRefProd, true>
patTriggerObjectRefProd_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerObjectRefProd_t,
                  "patTriggerObjectRefProd");
				  
typedef Buffer<pat::TriggerObjectStandAloneRefProd, true>
patTriggerObjectStandAloneRefProd_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerObjectStandAloneRefProd_t,
                  "patTriggerObjectStandAloneRefProd");
				  
typedef Buffer<pat::TriggerPathRefProd, true>
patTriggerPathRefProd_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerPathRefProd_t,
                  "patTriggerPathRefProd");
				  