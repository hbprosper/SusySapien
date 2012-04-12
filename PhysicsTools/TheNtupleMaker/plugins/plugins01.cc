// -------------------------------------------------------------------------
// File::   plugins01.cc
// Created: Thu Apr 12 04:32:19 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "DataFormats/EcalDigi/interface/EcalTrigPrimCompactColl.h"
#include "DataFormats/HLTReco/interface/HLTPerformanceInfo.h"
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
#include "DataFormats/L1GlobalTrigger/interface/L1GtTechnicalTriggerRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtTriggerMenuLite.h"
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

typedef Buffer<AnomalousECALVariables, true>
AnomalousECALVariables_t;
DEFINE_EDM_PLUGIN(BufferFactory, AnomalousECALVariables_t,
                  "AnomalousECALVariables");
				  
typedef Buffer<BaseMETv0, true>
BaseMETv0_t;
DEFINE_EDM_PLUGIN(BufferFactory, BaseMETv0_t,
                  "BaseMETv0");
				  
typedef Buffer<CSCTriggerContainer<csctf::TrackStub>, true>
CSCTriggerContainercsctfTrackStub_t;
DEFINE_EDM_PLUGIN(BufferFactory, CSCTriggerContainercsctfTrackStub_t,
                  "CSCTriggerContainercsctfTrackStub");
				  
typedef Buffer<ClusterSummary, true>
ClusterSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, ClusterSummary_t,
                  "ClusterSummary");
				  
typedef Buffer<CrossingFrame<PCaloHit>, true>
CrossingFramePCaloHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFramePCaloHit_t,
                  "CrossingFramePCaloHit");
				  
typedef Buffer<CrossingFrame<PSimHit>, true>
CrossingFramePSimHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFramePSimHit_t,
                  "CrossingFramePSimHit");
				  
typedef Buffer<CrossingFrame<SimTrack>, true>
CrossingFrameSimTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFrameSimTrack_t,
                  "CrossingFrameSimTrack");
				  
typedef Buffer<CrossingFrame<SimVertex>, true>
CrossingFrameSimVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFrameSimVertex_t,
                  "CrossingFrameSimVertex");
				  
typedef Buffer<CrossingFrame<edm::HepMCProduct>, true>
CrossingFrameedmHepMCProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFrameedmHepMCProduct_t,
                  "CrossingFrameedmHepMCProduct");
				  
typedef Buffer<CrossingFramePlaybackInfo, true>
CrossingFramePlaybackInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFramePlaybackInfo_t,
                  "CrossingFramePlaybackInfo");
				  
typedef Buffer<CrossingFramePlaybackInfoExtended, true>
CrossingFramePlaybackInfoExtended_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFramePlaybackInfoExtended_t,
                  "CrossingFramePlaybackInfoExtended");
				  
typedef Buffer<EcalTrigPrimCompactColl, true>
EcalTrigPrimCompactColl_t;
DEFINE_EDM_PLUGIN(BufferFactory, EcalTrigPrimCompactColl_t,
                  "EcalTrigPrimCompactColl");
				  
typedef Buffer<GenEventInfoProduct, true>
GenEventInfoProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenEventInfoProduct_t,
                  "GenEventInfoProduct");
				  
typedef Buffer<GenFilterInfo, true>
GenFilterInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenFilterInfo_t,
                  "GenFilterInfo");
				  
typedef Buffer<GenRunInfoProduct, true>
GenRunInfoProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenRunInfoProduct_t,
                  "GenRunInfoProduct");
				  
typedef Buffer<HLTPerformanceInfo, true>
HLTPerformanceInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, HLTPerformanceInfo_t,
                  "HLTPerformanceInfo");
				  
typedef Buffer<HcalLaserDigi, true>
HcalLaserDigi_t;
DEFINE_EDM_PLUGIN(BufferFactory, HcalLaserDigi_t,
                  "HcalLaserDigi");
				  
typedef Buffer<HcalNoiseSummary, true>
HcalNoiseSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, HcalNoiseSummary_t,
                  "HcalNoiseSummary");
				  
typedef Buffer<HcalSourcePositionData, true>
HcalSourcePositionData_t;
DEFINE_EDM_PLUGIN(BufferFactory, HcalSourcePositionData_t,
                  "HcalSourcePositionData");
				  
typedef Buffer<HcalTB02HistoClass, true>
HcalTB02HistoClass_t;
DEFINE_EDM_PLUGIN(BufferFactory, HcalTB02HistoClass_t,
                  "HcalTB02HistoClass");
				  
typedef Buffer<HcalUnpackerReport, true>
HcalUnpackerReport_t;
DEFINE_EDM_PLUGIN(BufferFactory, HcalUnpackerReport_t,
                  "HcalUnpackerReport");
				  
typedef Buffer<L1DataEmulRecord, true>
L1DataEmulRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1DataEmulRecord_t,
                  "L1DataEmulRecord");
				  
typedef Buffer<L1GlobalTriggerEvmReadoutRecord, true>
L1GlobalTriggerEvmReadoutRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerEvmReadoutRecord_t,
                  "L1GlobalTriggerEvmReadoutRecord");
				  
typedef Buffer<L1GlobalTriggerObjectMapRecord, true>
L1GlobalTriggerObjectMapRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerObjectMapRecord_t,
                  "L1GlobalTriggerObjectMapRecord");
				  
typedef Buffer<L1GlobalTriggerObjectMaps, true>
L1GlobalTriggerObjectMaps_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerObjectMaps_t,
                  "L1GlobalTriggerObjectMaps");
				  
typedef Buffer<L1GlobalTriggerReadoutRecord, true>
L1GlobalTriggerReadoutRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerReadoutRecord_t,
                  "L1GlobalTriggerReadoutRecord");
				  
typedef Buffer<L1GlobalTriggerReadoutSetup, true>
L1GlobalTriggerReadoutSetup_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerReadoutSetup_t,
                  "L1GlobalTriggerReadoutSetup");
				  
typedef Buffer<L1GlobalTriggerRecord, true>
L1GlobalTriggerRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerRecord_t,
                  "L1GlobalTriggerRecord");
				  
typedef Buffer<L1GtTechnicalTriggerRecord, true>
L1GtTechnicalTriggerRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GtTechnicalTriggerRecord_t,
                  "L1GtTechnicalTriggerRecord");
				  
typedef Buffer<L1GtTriggerMenuLite, true>
L1GtTriggerMenuLite_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GtTriggerMenuLite_t,
                  "L1GtTriggerMenuLite");
				  