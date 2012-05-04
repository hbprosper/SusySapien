// -------------------------------------------------------------------------
// File::   plugins11.cc
// Created: Thu Apr 26 00:40:14 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/TopObjects/interface/StGenEvent.h"
#include "AnalysisDataFormats/TopObjects/interface/TopGenEvent.h"
#include "AnalysisDataFormats/TopObjects/interface/TtFullHadronicEvent.h"
#include "AnalysisDataFormats/TopObjects/interface/TtFullLeptonicEvent.h"
#include "AnalysisDataFormats/TopObjects/interface/TtGenEvent.h"
#include "AnalysisDataFormats/TopObjects/interface/TtSemiLeptonicEvent.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Common/interface/ConditionsInEdm.h"
#include "DataFormats/Common/interface/DataFrameContainer.h"
#include "DataFormats/Common/interface/HLTGlobalStatus.h"
#include "DataFormats/Common/interface/MergeableCounter.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/ModuleTiming.h"
#include "DataFormats/HeavyIonEvent/interface/HeavyIon.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/METReco/interface/BeamHaloSummary.h"
#include "DataFormats/METReco/interface/CSCHaloData.h"
#include "DataFormats/PatCandidates/interface/StringMap.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/RPCDigi/interface/RPCRawDataCounts.h"
#include "DataFormats/SiPixelDigi/interface/SiPixelCalibDigiError.h"
#include "DataFormats/SiStripCommon/interface/SiStripEventSummary.h"
#include "DataFormats/TauReco/interface/CaloTauDiscriminator.h"
#include "SimDataFormats/Forward/interface/TotemTestHistoClass.h"
#include "SimDataFormats/HiGenData/interface/GenHIEvent.h"
#include "SimDataFormats/ValidationFormats/interface/PValidationFormats.h"
// -------------------------------------------------------------------------

std::string PHcalValidInfoJets_n("PHcalValidInfoJets");
typedef Buffer<PHcalValidInfoJets,
               &PHcalValidInfoJets_n, SINGLETON>
PHcalValidInfoJets_t;
DEFINE_EDM_PLUGIN(BufferFactory, PHcalValidInfoJets_t,
                  "PHcalValidInfoJets");
				  
std::string PHcalValidInfoLayer_n("PHcalValidInfoLayer");
typedef Buffer<PHcalValidInfoLayer,
               &PHcalValidInfoLayer_n, SINGLETON>
PHcalValidInfoLayer_t;
DEFINE_EDM_PLUGIN(BufferFactory, PHcalValidInfoLayer_t,
                  "PHcalValidInfoLayer");
				  
std::string PHcalValidInfoNxN_n("PHcalValidInfoNxN");
typedef Buffer<PHcalValidInfoNxN,
               &PHcalValidInfoNxN_n, SINGLETON>
PHcalValidInfoNxN_t;
DEFINE_EDM_PLUGIN(BufferFactory, PHcalValidInfoNxN_t,
                  "PHcalValidInfoNxN");
				  
std::string PMuonSimHit_n("PMuonSimHit");
typedef Buffer<PMuonSimHit,
               &PMuonSimHit_n, SINGLETON>
PMuonSimHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, PMuonSimHit_t,
                  "PMuonSimHit");
				  
std::string PTrackerSimHit_n("PTrackerSimHit");
typedef Buffer<PTrackerSimHit,
               &PTrackerSimHit_n, SINGLETON>
PTrackerSimHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, PTrackerSimHit_t,
                  "PTrackerSimHit");
				  
std::string RPCRawDataCounts_n("RPCRawDataCounts");
typedef Buffer<RPCRawDataCounts,
               &RPCRawDataCounts_n, SINGLETON>
RPCRawDataCounts_t;
DEFINE_EDM_PLUGIN(BufferFactory, RPCRawDataCounts_t,
                  "RPCRawDataCounts");
				  
std::string SiPixelCalibDigiError_n("SiPixelCalibDigiError");
typedef Buffer<SiPixelCalibDigiError,
               &SiPixelCalibDigiError_n, SINGLETON>
SiPixelCalibDigiError_t;
DEFINE_EDM_PLUGIN(BufferFactory, SiPixelCalibDigiError_t,
                  "SiPixelCalibDigiError");
				  
std::string SiStripEventSummary_n("SiStripEventSummary");
typedef Buffer<SiStripEventSummary,
               &SiStripEventSummary_n, SINGLETON>
SiStripEventSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, SiStripEventSummary_t,
                  "SiStripEventSummary");
				  
std::string StGenEvent_n("StGenEvent");
typedef Buffer<StGenEvent,
               &StGenEvent_n, SINGLETON>
StGenEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, StGenEvent_t,
                  "StGenEvent");
				  
std::string StringMap_n("StringMap");
typedef Buffer<StringMap,
               &StringMap_n, SINGLETON>
StringMap_t;
DEFINE_EDM_PLUGIN(BufferFactory, StringMap_t,
                  "StringMap");
				  
std::string TopGenEvent_n("TopGenEvent");
typedef Buffer<TopGenEvent,
               &TopGenEvent_n, SINGLETON>
TopGenEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, TopGenEvent_t,
                  "TopGenEvent");
				  
std::string TotemTestHistoClass_n("TotemTestHistoClass");
typedef Buffer<TotemTestHistoClass,
               &TotemTestHistoClass_n, SINGLETON>
TotemTestHistoClass_t;
DEFINE_EDM_PLUGIN(BufferFactory, TotemTestHistoClass_t,
                  "TotemTestHistoClass");
				  
std::string TtFullHadronicEvent_n("TtFullHadronicEvent");
typedef Buffer<TtFullHadronicEvent,
               &TtFullHadronicEvent_n, SINGLETON>
TtFullHadronicEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, TtFullHadronicEvent_t,
                  "TtFullHadronicEvent");
				  
std::string TtFullLeptonicEvent_n("TtFullLeptonicEvent");
typedef Buffer<TtFullLeptonicEvent,
               &TtFullLeptonicEvent_n, SINGLETON>
TtFullLeptonicEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, TtFullLeptonicEvent_t,
                  "TtFullLeptonicEvent");
				  
std::string TtGenEvent_n("TtGenEvent");
typedef Buffer<TtGenEvent,
               &TtGenEvent_n, SINGLETON>
TtGenEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, TtGenEvent_t,
                  "TtGenEvent");
				  
std::string TtSemiLeptonicEvent_n("TtSemiLeptonicEvent");
typedef Buffer<TtSemiLeptonicEvent,
               &TtSemiLeptonicEvent_n, SINGLETON>
TtSemiLeptonicEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, TtSemiLeptonicEvent_t,
                  "TtSemiLeptonicEvent");
				  
std::string edmConditionsInEventBlock_n("edm::ConditionsInEventBlock");
typedef Buffer<edm::ConditionsInEventBlock,
               &edmConditionsInEventBlock_n, SINGLETON>
edmConditionsInEventBlock_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmConditionsInEventBlock_t,
                  "edmConditionsInEventBlock");
				  
std::string edmConditionsInLumiBlock_n("edm::ConditionsInLumiBlock");
typedef Buffer<edm::ConditionsInLumiBlock,
               &edmConditionsInLumiBlock_n, SINGLETON>
edmConditionsInLumiBlock_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmConditionsInLumiBlock_t,
                  "edmConditionsInLumiBlock");
				  
std::string edmConditionsInRunBlock_n("edm::ConditionsInRunBlock");
typedef Buffer<edm::ConditionsInRunBlock,
               &edmConditionsInRunBlock_n, SINGLETON>
edmConditionsInRunBlock_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmConditionsInRunBlock_t,
                  "edmConditionsInRunBlock");
				  
std::string edmDataFrameContainer_n("edm::DataFrameContainer");
typedef Buffer<edm::DataFrameContainer,
               &edmDataFrameContainer_n, SINGLETON>
edmDataFrameContainer_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDataFrameContainer_t,
                  "edmDataFrameContainer");
				  
std::string edmEventTime_n("edm::EventTime");
typedef Buffer<edm::EventTime,
               &edmEventTime_n, SINGLETON>
edmEventTime_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmEventTime_t,
                  "edmEventTime");
				  
std::string edmGenHIEvent_n("edm::GenHIEvent");
typedef Buffer<edm::GenHIEvent,
               &edmGenHIEvent_n, SINGLETON>
edmGenHIEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmGenHIEvent_t,
                  "edmGenHIEvent");
				  
std::string edmHLTGlobalStatus_n("edm::HLTGlobalStatus");
typedef Buffer<edm::HLTGlobalStatus,
               &edmHLTGlobalStatus_n, SINGLETON>
edmHLTGlobalStatus_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmHLTGlobalStatus_t,
                  "edmHLTGlobalStatus");
				  
std::string edmMergeableCounter_n("edm::MergeableCounter");
typedef Buffer<edm::MergeableCounter,
               &edmMergeableCounter_n, SINGLETON>
edmMergeableCounter_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmMergeableCounter_t,
                  "edmMergeableCounter");
				  
std::string edmTriggerResults_n("edm::TriggerResults");
typedef Buffer<edm::TriggerResults,
               &edmTriggerResults_n, SINGLETON>
edmTriggerResults_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmTriggerResults_t,
                  "edmTriggerResults");
				  
std::string patHeavyIon_n("pat::HeavyIon");
typedef Buffer<pat::HeavyIon,
               &patHeavyIon_n, SINGLETON>
patHeavyIon_t;
DEFINE_EDM_PLUGIN(BufferFactory, patHeavyIon_t,
                  "patHeavyIon");
				  
std::string patTriggerEvent_n("pat::TriggerEvent");
typedef Buffer<pat::TriggerEvent,
               &patTriggerEvent_n, SINGLETON>
patTriggerEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerEvent_t,
                  "patTriggerEvent");
				  
std::string recoBasicJetRefVector_n("reco::BasicJetRefVector");
typedef Buffer<reco::BasicJetRefVector,
               &recoBasicJetRefVector_n, SINGLETON>
recoBasicJetRefVector_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBasicJetRefVector_t,
                  "recoBasicJetRefVector");
				  
std::string recoBeamHaloSummary_n("reco::BeamHaloSummary");
typedef Buffer<reco::BeamHaloSummary,
               &recoBeamHaloSummary_n, SINGLETON>
recoBeamHaloSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBeamHaloSummary_t,
                  "recoBeamHaloSummary");
				  
std::string recoBeamSpot_n("reco::BeamSpot");
typedef Buffer<reco::BeamSpot,
               &recoBeamSpot_n, SINGLETON>
recoBeamSpot_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBeamSpot_t,
                  "recoBeamSpot");
				  
std::string recoCSCHaloData_n("reco::CSCHaloData");
typedef Buffer<reco::CSCHaloData,
               &recoCSCHaloData_n, SINGLETON>
recoCSCHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCSCHaloData_t,
                  "recoCSCHaloData");
				  
std::string recoCaloJetRefVector_n("reco::CaloJetRefVector");
typedef Buffer<reco::CaloJetRefVector,
               &recoCaloJetRefVector_n, SINGLETON>
recoCaloJetRefVector_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloJetRefVector_t,
                  "recoCaloJetRefVector");
				  
std::string recoCaloTauDiscriminator_n("reco::CaloTauDiscriminator");
typedef Buffer<reco::CaloTauDiscriminator,
               &recoCaloTauDiscriminator_n, SINGLETON>
recoCaloTauDiscriminator_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloTauDiscriminator_t,
                  "recoCaloTauDiscriminator");
				  