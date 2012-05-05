// -------------------------------------------------------------------------
// File::   plugins09.cc
// Created: Sat May  5 16:09:43 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/EWK/interface/WMuNuCandidatePtr.h"
#include "AnalysisDataFormats/SUSYBSMObjects/interface/HSCPCaloInfo.h"
#include "AnalysisDataFormats/SUSYBSMObjects/interface/HSCPIsolation.h"
#include "AnalysisDataFormats/SUSYBSMObjects/interface/HSCParticle.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/EcalDigi/interface/EcalTrigPrimCompactColl.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/HLTReco/interface/HLTPerformanceInfo.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HcalDigi/interface/HcalLaserDigi.h"
#include "DataFormats/HcalDigi/interface/HcalUnpackerReport.h"
#include "DataFormats/HcalRecHit/interface/HcalSourcePositionData.h"
#include "DataFormats/L1CSCTrackFinder/interface/CSCTriggerContainer.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1Track.h"
#include "DataFormats/L1CSCTrackFinder/interface/TrackStub.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerEvmReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtTechnicalTriggerRecord.h"
#include "DataFormats/L1Trigger/interface/L1DataEmulRecord.h"
#include "DataFormats/METObjects/interface/BaseMET.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/MuonData/interface/MuonDigiCollection.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFramePlaybackInfo.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFramePlaybackInfoExtended.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/HcalTestBeam/interface/HcalTB02HistoClass.h"
// -------------------------------------------------------------------------

std::string recoWMuNuCandidatePtr_n("reco::WMuNuCandidatePtr");
typedef Buffer<reco::WMuNuCandidatePtr,
               &recoWMuNuCandidatePtr_n, COLLECTION>
recoWMuNuCandidatePtr_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoWMuNuCandidatePtr_t,
                  "recoWMuNuCandidatePtr");
				  
std::string stdpaircscL1TrackMuonDigiCollectionCSCDetIdCSCCorrelatedLCTDigi_n("std::pair<csc::L1Track,MuonDigiCollection<CSCDetId,CSCCorrelatedLCTDigi> >");
typedef Buffer<std::pair<csc::L1Track,MuonDigiCollection<CSCDetId,CSCCorrelatedLCTDigi> >,
               &stdpaircscL1TrackMuonDigiCollectionCSCDetIdCSCCorrelatedLCTDigi_n, COLLECTION>
stdpaircscL1TrackMuonDigiCollectionCSCDetIdCSCCorrelatedLCTDigi_t;
DEFINE_EDM_PLUGIN(BufferFactory, stdpaircscL1TrackMuonDigiCollectionCSCDetIdCSCCorrelatedLCTDigi_t,
                  "stdpaircscL1TrackMuonDigiCollectionCSCDetIdCSCCorrelatedLCTDigi");
				  
std::string stdpairrecoCompositeCandidatestdvectorint_n("std::pair<reco::CompositeCandidate,std::vector<int> >");
typedef Buffer<std::pair<reco::CompositeCandidate,std::vector<int> >,
               &stdpairrecoCompositeCandidatestdvectorint_n, COLLECTION>
stdpairrecoCompositeCandidatestdvectorint_t;
DEFINE_EDM_PLUGIN(BufferFactory, stdpairrecoCompositeCandidatestdvectorint_t,
                  "stdpairrecoCompositeCandidatestdvectorint");
				  
std::string stdvectoredmDetSetEcalRecHit_n("std::vector<edm::DetSet<EcalRecHit> >");
typedef Buffer<std::vector<edm::DetSet<EcalRecHit> >,
               &stdvectoredmDetSetEcalRecHit_n, COLLECTION>
stdvectoredmDetSetEcalRecHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, stdvectoredmDetSetEcalRecHit_t,
                  "stdvectoredmDetSetEcalRecHit");
				  
std::string stdvectoredmDetSetSiStripCluster_n("std::vector<edm::DetSet<SiStripCluster> >");
typedef Buffer<std::vector<edm::DetSet<SiStripCluster> >,
               &stdvectoredmDetSetSiStripCluster_n, COLLECTION>
stdvectoredmDetSetSiStripCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, stdvectoredmDetSetSiStripCluster_t,
                  "stdvectoredmDetSetSiStripCluster");
				  
std::string susybsmHSCPCaloInfo_n("susybsm::HSCPCaloInfo");
typedef Buffer<susybsm::HSCPCaloInfo,
               &susybsmHSCPCaloInfo_n, COLLECTION>
susybsmHSCPCaloInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, susybsmHSCPCaloInfo_t,
                  "susybsmHSCPCaloInfo");
				  
std::string susybsmHSCPIsolation_n("susybsm::HSCPIsolation");
typedef Buffer<susybsm::HSCPIsolation,
               &susybsmHSCPIsolation_n, COLLECTION>
susybsmHSCPIsolation_t;
DEFINE_EDM_PLUGIN(BufferFactory, susybsmHSCPIsolation_t,
                  "susybsmHSCPIsolation");
				  
std::string susybsmHSCParticle_n("susybsm::HSCParticle");
typedef Buffer<susybsm::HSCParticle,
               &susybsmHSCParticle_n, COLLECTION>
susybsmHSCParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, susybsmHSCParticle_t,
                  "susybsmHSCParticle");
				  
std::string triggerTriggerObject_n("trigger::TriggerObject");
typedef Buffer<trigger::TriggerObject,
               &triggerTriggerObject_n, COLLECTION>
triggerTriggerObject_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerTriggerObject_t,
                  "triggerTriggerObject");
				  
std::string BaseMETv0_n("BaseMETv0");
typedef Buffer<BaseMETv0,
               &BaseMETv0_n, SINGLETON>
BaseMETv0_t;
DEFINE_EDM_PLUGIN(BufferFactory, BaseMETv0_t,
                  "BaseMETv0");
				  
std::string CSCTriggerContainercsctfTrackStub_n("CSCTriggerContainer<csctf::TrackStub>");
typedef Buffer<CSCTriggerContainer<csctf::TrackStub>,
               &CSCTriggerContainercsctfTrackStub_n, SINGLETON>
CSCTriggerContainercsctfTrackStub_t;
DEFINE_EDM_PLUGIN(BufferFactory, CSCTriggerContainercsctfTrackStub_t,
                  "CSCTriggerContainercsctfTrackStub");
				  
std::string CrossingFramePlaybackInfo_n("CrossingFramePlaybackInfo");
typedef Buffer<CrossingFramePlaybackInfo,
               &CrossingFramePlaybackInfo_n, SINGLETON>
CrossingFramePlaybackInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFramePlaybackInfo_t,
                  "CrossingFramePlaybackInfo");
				  
std::string CrossingFramePlaybackInfoExtended_n("CrossingFramePlaybackInfoExtended");
typedef Buffer<CrossingFramePlaybackInfoExtended,
               &CrossingFramePlaybackInfoExtended_n, SINGLETON>
CrossingFramePlaybackInfoExtended_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFramePlaybackInfoExtended_t,
                  "CrossingFramePlaybackInfoExtended");
				  
std::string EcalTrigPrimCompactColl_n("EcalTrigPrimCompactColl");
typedef Buffer<EcalTrigPrimCompactColl,
               &EcalTrigPrimCompactColl_n, SINGLETON>
EcalTrigPrimCompactColl_t;
DEFINE_EDM_PLUGIN(BufferFactory, EcalTrigPrimCompactColl_t,
                  "EcalTrigPrimCompactColl");
				  
std::string GenEventInfoProduct_n("GenEventInfoProduct");
typedef Buffer<GenEventInfoProduct,
               &GenEventInfoProduct_n, SINGLETON>
GenEventInfoProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenEventInfoProduct_t,
                  "GenEventInfoProduct");
				  
std::string GenFilterInfo_n("GenFilterInfo");
typedef Buffer<GenFilterInfo,
               &GenFilterInfo_n, SINGLETON>
GenFilterInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenFilterInfo_t,
                  "GenFilterInfo");
				  
std::string GenRunInfoProduct_n("GenRunInfoProduct");
typedef Buffer<GenRunInfoProduct,
               &GenRunInfoProduct_n, SINGLETON>
GenRunInfoProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenRunInfoProduct_t,
                  "GenRunInfoProduct");
				  
std::string HLTPerformanceInfo_n("HLTPerformanceInfo");
typedef Buffer<HLTPerformanceInfo,
               &HLTPerformanceInfo_n, SINGLETON>
HLTPerformanceInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, HLTPerformanceInfo_t,
                  "HLTPerformanceInfo");
				  
std::string HcalLaserDigi_n("HcalLaserDigi");
typedef Buffer<HcalLaserDigi,
               &HcalLaserDigi_n, SINGLETON>
HcalLaserDigi_t;
DEFINE_EDM_PLUGIN(BufferFactory, HcalLaserDigi_t,
                  "HcalLaserDigi");
				  
std::string HcalNoiseSummary_n("HcalNoiseSummary");
typedef Buffer<HcalNoiseSummary,
               &HcalNoiseSummary_n, SINGLETON>
HcalNoiseSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, HcalNoiseSummary_t,
                  "HcalNoiseSummary");
				  
std::string HcalSourcePositionData_n("HcalSourcePositionData");
typedef Buffer<HcalSourcePositionData,
               &HcalSourcePositionData_n, SINGLETON>
HcalSourcePositionData_t;
DEFINE_EDM_PLUGIN(BufferFactory, HcalSourcePositionData_t,
                  "HcalSourcePositionData");
				  
std::string HcalTB02HistoClass_n("HcalTB02HistoClass");
typedef Buffer<HcalTB02HistoClass,
               &HcalTB02HistoClass_n, SINGLETON>
HcalTB02HistoClass_t;
DEFINE_EDM_PLUGIN(BufferFactory, HcalTB02HistoClass_t,
                  "HcalTB02HistoClass");
				  
std::string HcalUnpackerReport_n("HcalUnpackerReport");
typedef Buffer<HcalUnpackerReport,
               &HcalUnpackerReport_n, SINGLETON>
HcalUnpackerReport_t;
DEFINE_EDM_PLUGIN(BufferFactory, HcalUnpackerReport_t,
                  "HcalUnpackerReport");
				  
std::string L1DataEmulRecord_n("L1DataEmulRecord");
typedef Buffer<L1DataEmulRecord,
               &L1DataEmulRecord_n, SINGLETON>
L1DataEmulRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1DataEmulRecord_t,
                  "L1DataEmulRecord");
				  
std::string L1GlobalTriggerEvmReadoutRecord_n("L1GlobalTriggerEvmReadoutRecord");
typedef Buffer<L1GlobalTriggerEvmReadoutRecord,
               &L1GlobalTriggerEvmReadoutRecord_n, SINGLETON>
L1GlobalTriggerEvmReadoutRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerEvmReadoutRecord_t,
                  "L1GlobalTriggerEvmReadoutRecord");
				  
std::string L1GlobalTriggerObjectMapRecord_n("L1GlobalTriggerObjectMapRecord");
typedef Buffer<L1GlobalTriggerObjectMapRecord,
               &L1GlobalTriggerObjectMapRecord_n, SINGLETON>
L1GlobalTriggerObjectMapRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerObjectMapRecord_t,
                  "L1GlobalTriggerObjectMapRecord");
				  
std::string L1GlobalTriggerReadoutRecord_n("L1GlobalTriggerReadoutRecord");
typedef Buffer<L1GlobalTriggerReadoutRecord,
               &L1GlobalTriggerReadoutRecord_n, SINGLETON>
L1GlobalTriggerReadoutRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerReadoutRecord_t,
                  "L1GlobalTriggerReadoutRecord");
				  
std::string L1GlobalTriggerReadoutSetup_n("L1GlobalTriggerReadoutSetup");
typedef Buffer<L1GlobalTriggerReadoutSetup,
               &L1GlobalTriggerReadoutSetup_n, SINGLETON>
L1GlobalTriggerReadoutSetup_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerReadoutSetup_t,
                  "L1GlobalTriggerReadoutSetup");
				  
std::string L1GlobalTriggerRecord_n("L1GlobalTriggerRecord");
typedef Buffer<L1GlobalTriggerRecord,
               &L1GlobalTriggerRecord_n, SINGLETON>
L1GlobalTriggerRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerRecord_t,
                  "L1GlobalTriggerRecord");
				  
std::string L1GtTechnicalTriggerRecord_n("L1GtTechnicalTriggerRecord");
typedef Buffer<L1GtTechnicalTriggerRecord,
               &L1GtTechnicalTriggerRecord_n, SINGLETON>
L1GtTechnicalTriggerRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GtTechnicalTriggerRecord_t,
                  "L1GtTechnicalTriggerRecord");
				  