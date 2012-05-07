// -------------------------------------------------------------------------
// File::   plugins09.cc
// Created: Mon May  7 16:53:25 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/SUSYBSMObjects/interface/HSCPIsolation.h"
#include "AnalysisDataFormats/SUSYBSMObjects/interface/HSCParticle.h"
#include "DataFormats/EcalDigi/interface/EcalTrigPrimCompactColl.h"
#include "DataFormats/HLTReco/interface/HLTPerformanceInfo.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HcalDigi/interface/HcalLaserDigi.h"
#include "DataFormats/HcalDigi/interface/HcalUnpackerReport.h"
#include "DataFormats/HcalRecHit/interface/HcalSourcePositionData.h"
#include "DataFormats/L1CSCTrackFinder/interface/CSCTriggerContainer.h"
#include "DataFormats/L1CSCTrackFinder/interface/TrackStub.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerEvmReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMaps.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerRecord.h"
#include "DataFormats/L1Trigger/interface/L1DataEmulRecord.h"
#include "DataFormats/METObjects/interface/BaseMET.h"
#include "DataFormats/METReco/interface/AnomalousECALVariables.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/TrackerCommon/interface/ClusterSummary.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFramePlaybackInfo.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFramePlaybackInfoExtended.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/HcalTestBeam/interface/HcalTB02HistoClass.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
// -------------------------------------------------------------------------

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
				  
std::string AnomalousECALVariables_n("AnomalousECALVariables");
typedef Buffer<AnomalousECALVariables,
               &AnomalousECALVariables_n, SINGLETON>
AnomalousECALVariables_t;
DEFINE_EDM_PLUGIN(BufferFactory, AnomalousECALVariables_t,
                  "AnomalousECALVariables");
				  
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
				  
std::string ClusterSummary_n("ClusterSummary");
typedef Buffer<ClusterSummary,
               &ClusterSummary_n, SINGLETON>
ClusterSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, ClusterSummary_t,
                  "ClusterSummary");
				  
std::string CrossingFramePCaloHit_n("CrossingFrame<PCaloHit>");
typedef Buffer<CrossingFrame<PCaloHit>,
               &CrossingFramePCaloHit_n, SINGLETON>
CrossingFramePCaloHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFramePCaloHit_t,
                  "CrossingFramePCaloHit");
				  
std::string CrossingFramePSimHit_n("CrossingFrame<PSimHit>");
typedef Buffer<CrossingFrame<PSimHit>,
               &CrossingFramePSimHit_n, SINGLETON>
CrossingFramePSimHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFramePSimHit_t,
                  "CrossingFramePSimHit");
				  
std::string CrossingFrameSimTrack_n("CrossingFrame<SimTrack>");
typedef Buffer<CrossingFrame<SimTrack>,
               &CrossingFrameSimTrack_n, SINGLETON>
CrossingFrameSimTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFrameSimTrack_t,
                  "CrossingFrameSimTrack");
				  
std::string CrossingFrameSimVertex_n("CrossingFrame<SimVertex>");
typedef Buffer<CrossingFrame<SimVertex>,
               &CrossingFrameSimVertex_n, SINGLETON>
CrossingFrameSimVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFrameSimVertex_t,
                  "CrossingFrameSimVertex");
				  
std::string CrossingFrameedmHepMCProduct_n("CrossingFrame<edm::HepMCProduct>");
typedef Buffer<CrossingFrame<edm::HepMCProduct>,
               &CrossingFrameedmHepMCProduct_n, SINGLETON>
CrossingFrameedmHepMCProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFrameedmHepMCProduct_t,
                  "CrossingFrameedmHepMCProduct");
				  
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
				  
std::string L1GlobalTriggerObjectMaps_n("L1GlobalTriggerObjectMaps");
typedef Buffer<L1GlobalTriggerObjectMaps,
               &L1GlobalTriggerObjectMaps_n, SINGLETON>
L1GlobalTriggerObjectMaps_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerObjectMaps_t,
                  "L1GlobalTriggerObjectMaps");
				  
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
				  