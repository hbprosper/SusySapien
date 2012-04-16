// -------------------------------------------------------------------------
// File::   plugins02.cc
// Created: Sun Apr 15 22:17:19 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/TopObjects/interface/StGenEvent.h"
#include "AnalysisDataFormats/TopObjects/interface/TopGenEvent.h"
#include "AnalysisDataFormats/TopObjects/interface/TtFullHadronicEvent.h"
#include "DataFormats/PatCandidates/interface/StringMap.h"
#include "DataFormats/RPCDigi/interface/RPCRawDataCounts.h"
#include "DataFormats/SiPixelDigi/interface/SiPixelCalibDigiError.h"
#include "DataFormats/SiStripCommon/interface/SiStripEventSummary.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CrossingFrame/interface/PCrossingFrame.h"
#include "SimDataFormats/EcalTestBeam/interface/PEcalTBInfo.h"
#include "SimDataFormats/Forward/interface/TotemTestHistoClass.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/HcalTestBeam/interface/PHcalTB04Info.h"
#include "SimDataFormats/HcalTestBeam/interface/PHcalTB06Info.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/ValidationFormats/interface/PValidationFormats.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
// -------------------------------------------------------------------------

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
				  
typedef Buffer<StringMap, true>
StringMap_t;
DEFINE_EDM_PLUGIN(BufferFactory, StringMap_t,
                  "StringMap");
				  
typedef Buffer<TopGenEvent, true>
TopGenEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, TopGenEvent_t,
                  "TopGenEvent");
				  
typedef Buffer<TotemTestHistoClass, true>
TotemTestHistoClass_t;
DEFINE_EDM_PLUGIN(BufferFactory, TotemTestHistoClass_t,
                  "TotemTestHistoClass");
				  
typedef Buffer<TtFullHadronicEvent, true>
TtFullHadronicEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, TtFullHadronicEvent_t,
                  "TtFullHadronicEvent");
				  