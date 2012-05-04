// -------------------------------------------------------------------------
// File::   plugins10.cc
// Created: Thu Apr 26 00:40:14 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "DataFormats/HcalDigi/interface/HcalLaserDigi.h"
#include "DataFormats/HcalDigi/interface/HcalUnpackerReport.h"
#include "DataFormats/HcalRecHit/interface/HcalSourcePositionData.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTTrackContainer.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerEvmReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtTechnicalTriggerRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtTriggerMenuLite.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtfeExtWord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtfeWord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1TcsWord.h"
#include "DataFormats/L1Trigger/interface/L1DataEmulRecord.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CrossingFrame/interface/PCrossingFrame.h"
#include "SimDataFormats/EcalTestBeam/interface/PEcalTBInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/HcalTestBeam/interface/HcalTB02HistoClass.h"
#include "SimDataFormats/HcalTestBeam/interface/PHcalTB04Info.h"
#include "SimDataFormats/HcalTestBeam/interface/PHcalTB06Info.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/ValidationFormats/interface/PValidationFormats.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
// -------------------------------------------------------------------------

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
				  
std::string L1GtTriggerMenuLite_n("L1GtTriggerMenuLite");
typedef Buffer<L1GtTriggerMenuLite,
               &L1GtTriggerMenuLite_n, SINGLETON>
L1GtTriggerMenuLite_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GtTriggerMenuLite_t,
                  "L1GtTriggerMenuLite");
				  
std::string L1GtfeExtWord_n("L1GtfeExtWord");
typedef Buffer<L1GtfeExtWord,
               &L1GtfeExtWord_n, SINGLETON>
L1GtfeExtWord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GtfeExtWord_t,
                  "L1GtfeExtWord");
				  
std::string L1GtfeWord_n("L1GtfeWord");
typedef Buffer<L1GtfeWord,
               &L1GtfeWord_n, SINGLETON>
L1GtfeWord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GtfeWord_t,
                  "L1GtfeWord");
				  
std::string L1MuDTChambPhContainer_n("L1MuDTChambPhContainer");
typedef Buffer<L1MuDTChambPhContainer,
               &L1MuDTChambPhContainer_n, SINGLETON>
L1MuDTChambPhContainer_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1MuDTChambPhContainer_t,
                  "L1MuDTChambPhContainer");
				  
std::string L1MuDTChambThContainer_n("L1MuDTChambThContainer");
typedef Buffer<L1MuDTChambThContainer,
               &L1MuDTChambThContainer_n, SINGLETON>
L1MuDTChambThContainer_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1MuDTChambThContainer_t,
                  "L1MuDTChambThContainer");
				  
std::string L1MuDTTrackContainer_n("L1MuDTTrackContainer");
typedef Buffer<L1MuDTTrackContainer,
               &L1MuDTTrackContainer_n, SINGLETON>
L1MuDTTrackContainer_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1MuDTTrackContainer_t,
                  "L1MuDTTrackContainer");
				  
std::string L1TcsWord_n("L1TcsWord");
typedef Buffer<L1TcsWord,
               &L1TcsWord_n, SINGLETON>
L1TcsWord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1TcsWord_t,
                  "L1TcsWord");
				  
std::string LHEEventProduct_n("LHEEventProduct");
typedef Buffer<LHEEventProduct,
               &LHEEventProduct_n, SINGLETON>
LHEEventProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, LHEEventProduct_t,
                  "LHEEventProduct");
				  
std::string LHERunInfoProduct_n("LHERunInfoProduct");
typedef Buffer<LHERunInfoProduct,
               &LHERunInfoProduct_n, SINGLETON>
LHERunInfoProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, LHERunInfoProduct_t,
                  "LHERunInfoProduct");
				  
std::string PCrossingFramePCaloHit_n("PCrossingFrame<PCaloHit>");
typedef Buffer<PCrossingFrame<PCaloHit>,
               &PCrossingFramePCaloHit_n, SINGLETON>
PCrossingFramePCaloHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, PCrossingFramePCaloHit_t,
                  "PCrossingFramePCaloHit");
				  
std::string PCrossingFramePSimHit_n("PCrossingFrame<PSimHit>");
typedef Buffer<PCrossingFrame<PSimHit>,
               &PCrossingFramePSimHit_n, SINGLETON>
PCrossingFramePSimHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, PCrossingFramePSimHit_t,
                  "PCrossingFramePSimHit");
				  
std::string PCrossingFrameSimTrack_n("PCrossingFrame<SimTrack>");
typedef Buffer<PCrossingFrame<SimTrack>,
               &PCrossingFrameSimTrack_n, SINGLETON>
PCrossingFrameSimTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, PCrossingFrameSimTrack_t,
                  "PCrossingFrameSimTrack");
				  
std::string PCrossingFrameSimVertex_n("PCrossingFrame<SimVertex>");
typedef Buffer<PCrossingFrame<SimVertex>,
               &PCrossingFrameSimVertex_n, SINGLETON>
PCrossingFrameSimVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, PCrossingFrameSimVertex_t,
                  "PCrossingFrameSimVertex");
				  
std::string PCrossingFrameedmHepMCProduct_n("PCrossingFrame<edm::HepMCProduct>");
typedef Buffer<PCrossingFrame<edm::HepMCProduct>,
               &PCrossingFrameedmHepMCProduct_n, SINGLETON>
PCrossingFrameedmHepMCProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, PCrossingFrameedmHepMCProduct_t,
                  "PCrossingFrameedmHepMCProduct");
				  
std::string PEcalTBInfo_n("PEcalTBInfo");
typedef Buffer<PEcalTBInfo,
               &PEcalTBInfo_n, SINGLETON>
PEcalTBInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, PEcalTBInfo_t,
                  "PEcalTBInfo");
				  
std::string PEcalValidInfo_n("PEcalValidInfo");
typedef Buffer<PEcalValidInfo,
               &PEcalValidInfo_n, SINGLETON>
PEcalValidInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, PEcalValidInfo_t,
                  "PEcalValidInfo");
				  
std::string PGlobalDigi_n("PGlobalDigi");
typedef Buffer<PGlobalDigi,
               &PGlobalDigi_n, SINGLETON>
PGlobalDigi_t;
DEFINE_EDM_PLUGIN(BufferFactory, PGlobalDigi_t,
                  "PGlobalDigi");
				  
std::string PGlobalRecHit_n("PGlobalRecHit");
typedef Buffer<PGlobalRecHit,
               &PGlobalRecHit_n, SINGLETON>
PGlobalRecHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, PGlobalRecHit_t,
                  "PGlobalRecHit");
				  
std::string PGlobalSimHit_n("PGlobalSimHit");
typedef Buffer<PGlobalSimHit,
               &PGlobalSimHit_n, SINGLETON>
PGlobalSimHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, PGlobalSimHit_t,
                  "PGlobalSimHit");
				  
std::string PHcalTB04Info_n("PHcalTB04Info");
typedef Buffer<PHcalTB04Info,
               &PHcalTB04Info_n, SINGLETON>
PHcalTB04Info_t;
DEFINE_EDM_PLUGIN(BufferFactory, PHcalTB04Info_t,
                  "PHcalTB04Info");
				  
std::string PHcalTB06Info_n("PHcalTB06Info");
typedef Buffer<PHcalTB06Info,
               &PHcalTB06Info_n, SINGLETON>
PHcalTB06Info_t;
DEFINE_EDM_PLUGIN(BufferFactory, PHcalTB06Info_t,
                  "PHcalTB06Info");
				  