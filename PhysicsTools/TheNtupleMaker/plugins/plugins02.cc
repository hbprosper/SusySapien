// -------------------------------------------------------------------------
// File::   plugins02.cc
// Created: Thu Apr 12 04:32:19 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/TopObjects/interface/StGenEvent.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTTrackContainer.h"
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

typedef Buffer<L1GtfeExtWord, true>
L1GtfeExtWord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GtfeExtWord_t,
                  "L1GtfeExtWord");
				  
typedef Buffer<L1GtfeWord, true>
L1GtfeWord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GtfeWord_t,
                  "L1GtfeWord");
				  
typedef Buffer<L1MuDTChambPhContainer, true>
L1MuDTChambPhContainer_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1MuDTChambPhContainer_t,
                  "L1MuDTChambPhContainer");
				  
typedef Buffer<L1MuDTChambThContainer, true>
L1MuDTChambThContainer_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1MuDTChambThContainer_t,
                  "L1MuDTChambThContainer");
				  
typedef Buffer<L1MuDTTrackContainer, true>
L1MuDTTrackContainer_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1MuDTTrackContainer_t,
                  "L1MuDTTrackContainer");
				  
typedef Buffer<L1TcsWord, true>
L1TcsWord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1TcsWord_t,
                  "L1TcsWord");
				  
typedef Buffer<LHEEventProduct, true>
LHEEventProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, LHEEventProduct_t,
                  "LHEEventProduct");
				  
typedef Buffer<LHERunInfoProduct, true>
LHERunInfoProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, LHERunInfoProduct_t,
                  "LHERunInfoProduct");
				  
typedef Buffer<PCrossingFrame<PCaloHit>, true>
PCrossingFramePCaloHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, PCrossingFramePCaloHit_t,
                  "PCrossingFramePCaloHit");
				  
typedef Buffer<PCrossingFrame<PSimHit>, true>
PCrossingFramePSimHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, PCrossingFramePSimHit_t,
                  "PCrossingFramePSimHit");
				  
typedef Buffer<PCrossingFrame<SimTrack>, true>
PCrossingFrameSimTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, PCrossingFrameSimTrack_t,
                  "PCrossingFrameSimTrack");
				  
typedef Buffer<PCrossingFrame<SimVertex>, true>
PCrossingFrameSimVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, PCrossingFrameSimVertex_t,
                  "PCrossingFrameSimVertex");
				  
typedef Buffer<PCrossingFrame<edm::HepMCProduct>, true>
PCrossingFrameedmHepMCProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, PCrossingFrameedmHepMCProduct_t,
                  "PCrossingFrameedmHepMCProduct");
				  
typedef Buffer<PEcalTBInfo, true>
PEcalTBInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, PEcalTBInfo_t,
                  "PEcalTBInfo");
				  
typedef Buffer<PEcalValidInfo, true>
PEcalValidInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, PEcalValidInfo_t,
                  "PEcalValidInfo");
				  
typedef Buffer<PGlobalDigi, true>
PGlobalDigi_t;
DEFINE_EDM_PLUGIN(BufferFactory, PGlobalDigi_t,
                  "PGlobalDigi");
				  
typedef Buffer<PGlobalRecHit, true>
PGlobalRecHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, PGlobalRecHit_t,
                  "PGlobalRecHit");
				  
typedef Buffer<PGlobalSimHit, true>
PGlobalSimHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, PGlobalSimHit_t,
                  "PGlobalSimHit");
				  
typedef Buffer<PHcalTB04Info, true>
PHcalTB04Info_t;
DEFINE_EDM_PLUGIN(BufferFactory, PHcalTB04Info_t,
                  "PHcalTB04Info");
				  
typedef Buffer<PHcalTB06Info, true>
PHcalTB06Info_t;
DEFINE_EDM_PLUGIN(BufferFactory, PHcalTB06Info_t,
                  "PHcalTB06Info");
				  
typedef Buffer<PHcalValidInfoJets, true>
PHcalValidInfoJets_t;
DEFINE_EDM_PLUGIN(BufferFactory, PHcalValidInfoJets_t,
                  "PHcalValidInfoJets");
				  
typedef Buffer<PHcalValidInfoLayer, true>
PHcalValidInfoLayer_t;
DEFINE_EDM_PLUGIN(BufferFactory, PHcalValidInfoLayer_t,
                  "PHcalValidInfoLayer");
				  
typedef Buffer<PHcalValidInfoNxN, true>
PHcalValidInfoNxN_t;
DEFINE_EDM_PLUGIN(BufferFactory, PHcalValidInfoNxN_t,
                  "PHcalValidInfoNxN");
				  
typedef Buffer<PMuonSimHit, true>
PMuonSimHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, PMuonSimHit_t,
                  "PMuonSimHit");
				  
typedef Buffer<PTrackerSimHit, true>
PTrackerSimHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, PTrackerSimHit_t,
                  "PTrackerSimHit");
				  
typedef Buffer<RPCRawDataCounts, true>
RPCRawDataCounts_t;
DEFINE_EDM_PLUGIN(BufferFactory, RPCRawDataCounts_t,
                  "RPCRawDataCounts");
				  
typedef Buffer<SiPixelCalibDigiError, true>
SiPixelCalibDigiError_t;
DEFINE_EDM_PLUGIN(BufferFactory, SiPixelCalibDigiError_t,
                  "SiPixelCalibDigiError");
				  
typedef Buffer<SiStripEventSummary, true>
SiStripEventSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, SiStripEventSummary_t,
                  "SiStripEventSummary");
				  
typedef Buffer<StGenEvent, true>
StGenEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, StGenEvent_t,
                  "StGenEvent");
				  