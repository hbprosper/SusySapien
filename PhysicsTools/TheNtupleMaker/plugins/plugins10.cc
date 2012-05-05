// -------------------------------------------------------------------------
// File::   plugins10.cc
// Created: Sat May  5 16:09:43 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/TopObjects/interface/StGenEvent.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTTrackContainer.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtTriggerMenuLite.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtfeExtWord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtfeWord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1TcsWord.h"
#include "DataFormats/RPCDigi/interface/RPCRawDataCounts.h"
#include "DataFormats/SiPixelDigi/interface/SiPixelCalibDigiError.h"
#include "DataFormats/SiStripCommon/interface/SiStripEventSummary.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CrossingFrame/interface/PCrossingFrame.h"
#include "SimDataFormats/EcalTestBeam/interface/PEcalTBInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/HcalTestBeam/interface/PHcalTB04Info.h"
#include "SimDataFormats/HcalTestBeam/interface/PHcalTB06Info.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/ValidationFormats/interface/PValidationFormats.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
// -------------------------------------------------------------------------

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
				  