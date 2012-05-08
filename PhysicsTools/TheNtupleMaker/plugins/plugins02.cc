// -------------------------------------------------------------------------
// File::   plugins02.cc
// Created: Tue May  8 03:57:32 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/TopObjects/interface/StEvtSolution.h"
#include "AnalysisDataFormats/TrackInfo/interface/RecoTracktoTP.h"
#include "AnalysisDataFormats/TrackInfo/interface/TPtoRecoTrack.h"
#include "DataFormats/GeometryVector/interface/GlobalTag.h"
#include "DataFormats/GeometryVector/interface/Point3DBase.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1Trigger/interface/L1TriggerError.h"
#include "DataFormats/LTCDigi/interface/LTCDigi.h"
#include "DataFormats/METObjects/interface/MET.h"
#include "DataFormats/METObjects/interface/TowerMET.h"
#include "DataFormats/MuonSeed/interface/L2MuonTrajectorySeed.h"
#include "DataFormats/MuonSeed/interface/L3MuonTrajectorySeed.h"
#include "DataFormats/RPCDigi/interface/RPCDigiL1Link.h"
#include "DataFormats/Scalers/interface/L1TriggerRates.h"
#include "DataFormats/Scalers/interface/L1TriggerScalers.h"
#include "DataFormats/Scalers/interface/Level1TriggerRates.h"
#include "DataFormats/Scalers/interface/Level1TriggerScalers.h"
#include "DataFormats/Scalers/interface/LumiScalers.h"
#include "DataFormats/SiPixelRawData/interface/SiPixelRawDataError.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/Forward/interface/LHCTransportLink.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupMixingContent.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/TrackingAnalysis/interface/ParticleBase.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/ValidationFormats/interface/MaterialAccountingDetector.h"
#include "SimDataFormats/ValidationFormats/interface/MaterialAccountingStep.h"
#include "SimDataFormats/ValidationFormats/interface/MaterialAccountingTrack.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
// -------------------------------------------------------------------------

std::string L1MuGMTCand_n("L1MuGMTCand");
typedef Buffer<L1MuGMTCand,
               &L1MuGMTCand_n, COLLECTION>
L1MuGMTCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1MuGMTCand_t,
                  "L1MuGMTCand");
				  
std::string L1MuRegionalCand_n("L1MuRegionalCand");
typedef Buffer<L1MuRegionalCand,
               &L1MuRegionalCand_n, COLLECTION>
L1MuRegionalCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1MuRegionalCand_t,
                  "L1MuRegionalCand");
				  
std::string L1TriggerError_n("L1TriggerError");
typedef Buffer<L1TriggerError,
               &L1TriggerError_n, COLLECTION>
L1TriggerError_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1TriggerError_t,
                  "L1TriggerError");
				  
std::string L1TriggerRates_n("L1TriggerRates");
typedef Buffer<L1TriggerRates,
               &L1TriggerRates_n, COLLECTION>
L1TriggerRates_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1TriggerRates_t,
                  "L1TriggerRates");
				  
std::string L1TriggerScalers_n("L1TriggerScalers");
typedef Buffer<L1TriggerScalers,
               &L1TriggerScalers_n, COLLECTION>
L1TriggerScalers_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1TriggerScalers_t,
                  "L1TriggerScalers");
				  
std::string L2MuonTrajectorySeed_n("L2MuonTrajectorySeed");
typedef Buffer<L2MuonTrajectorySeed,
               &L2MuonTrajectorySeed_n, COLLECTION>
L2MuonTrajectorySeed_t;
DEFINE_EDM_PLUGIN(BufferFactory, L2MuonTrajectorySeed_t,
                  "L2MuonTrajectorySeed");
				  
std::string L3MuonTrajectorySeed_n("L3MuonTrajectorySeed");
typedef Buffer<L3MuonTrajectorySeed,
               &L3MuonTrajectorySeed_n, COLLECTION>
L3MuonTrajectorySeed_t;
DEFINE_EDM_PLUGIN(BufferFactory, L3MuonTrajectorySeed_t,
                  "L3MuonTrajectorySeed");
				  
std::string LHCTransportLink_n("LHCTransportLink");
typedef Buffer<LHCTransportLink,
               &LHCTransportLink_n, COLLECTION>
LHCTransportLink_t;
DEFINE_EDM_PLUGIN(BufferFactory, LHCTransportLink_t,
                  "LHCTransportLink");
				  
std::string LTCDigi_n("LTCDigi");
typedef Buffer<LTCDigi,
               &LTCDigi_n, COLLECTION>
LTCDigi_t;
DEFINE_EDM_PLUGIN(BufferFactory, LTCDigi_t,
                  "LTCDigi");
				  
std::string Level1TriggerRates_n("Level1TriggerRates");
typedef Buffer<Level1TriggerRates,
               &Level1TriggerRates_n, COLLECTION>
Level1TriggerRates_t;
DEFINE_EDM_PLUGIN(BufferFactory, Level1TriggerRates_t,
                  "Level1TriggerRates");
				  
std::string Level1TriggerScalers_n("Level1TriggerScalers");
typedef Buffer<Level1TriggerScalers,
               &Level1TriggerScalers_n, COLLECTION>
Level1TriggerScalers_t;
DEFINE_EDM_PLUGIN(BufferFactory, Level1TriggerScalers_t,
                  "Level1TriggerScalers");
				  
std::string LumiScalers_n("LumiScalers");
typedef Buffer<LumiScalers,
               &LumiScalers_n, COLLECTION>
LumiScalers_t;
DEFINE_EDM_PLUGIN(BufferFactory, LumiScalers_t,
                  "LumiScalers");
				  
std::string METv0_n("METv0");
typedef Buffer<METv0,
               &METv0_n, COLLECTION>
METv0_t;
DEFINE_EDM_PLUGIN(BufferFactory, METv0_t,
                  "METv0");
				  
std::string MaterialAccountingDetector_n("MaterialAccountingDetector");
typedef Buffer<MaterialAccountingDetector,
               &MaterialAccountingDetector_n, COLLECTION>
MaterialAccountingDetector_t;
DEFINE_EDM_PLUGIN(BufferFactory, MaterialAccountingDetector_t,
                  "MaterialAccountingDetector");
				  
std::string MaterialAccountingStep_n("MaterialAccountingStep");
typedef Buffer<MaterialAccountingStep,
               &MaterialAccountingStep_n, COLLECTION>
MaterialAccountingStep_t;
DEFINE_EDM_PLUGIN(BufferFactory, MaterialAccountingStep_t,
                  "MaterialAccountingStep");
				  
std::string MaterialAccountingTrack_n("MaterialAccountingTrack");
typedef Buffer<MaterialAccountingTrack,
               &MaterialAccountingTrack_n, COLLECTION>
MaterialAccountingTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, MaterialAccountingTrack_t,
                  "MaterialAccountingTrack");
				  
std::string PCaloHit_n("PCaloHit");
typedef Buffer<PCaloHit,
               &PCaloHit_n, COLLECTION>
PCaloHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, PCaloHit_t,
                  "PCaloHit");
				  
std::string PSimHit_n("PSimHit");
typedef Buffer<PSimHit,
               &PSimHit_n, COLLECTION>
PSimHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, PSimHit_t,
                  "PSimHit");
				  
std::string ParticleBase_n("ParticleBase");
typedef Buffer<ParticleBase,
               &ParticleBase_n, COLLECTION>
ParticleBase_t;
DEFINE_EDM_PLUGIN(BufferFactory, ParticleBase_t,
                  "ParticleBase");
				  
std::string PileupMixingContent_n("PileupMixingContent");
typedef Buffer<PileupMixingContent,
               &PileupMixingContent_n, COLLECTION>
PileupMixingContent_t;
DEFINE_EDM_PLUGIN(BufferFactory, PileupMixingContent_t,
                  "PileupMixingContent");
				  
std::string PileupSummaryInfo_n("PileupSummaryInfo");
typedef Buffer<PileupSummaryInfo,
               &PileupSummaryInfo_n, COLLECTION>
PileupSummaryInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, PileupSummaryInfo_t,
                  "PileupSummaryInfo");
				  
std::string Point3DBasefloatGlobalTag_n("Point3DBase<float,GlobalTag>");
typedef Buffer<Point3DBase<float,GlobalTag>,
               &Point3DBasefloatGlobalTag_n, COLLECTION>
Point3DBasefloatGlobalTag_t;
DEFINE_EDM_PLUGIN(BufferFactory, Point3DBasefloatGlobalTag_t,
                  "Point3DBasefloatGlobalTag");
				  
std::string RPCDigiL1Link_n("RPCDigiL1Link");
typedef Buffer<RPCDigiL1Link,
               &RPCDigiL1Link_n, COLLECTION>
RPCDigiL1Link_t;
DEFINE_EDM_PLUGIN(BufferFactory, RPCDigiL1Link_t,
                  "RPCDigiL1Link");
				  
std::string RecoTracktoTP_n("RecoTracktoTP");
typedef Buffer<RecoTracktoTP,
               &RecoTracktoTP_n, COLLECTION>
RecoTracktoTP_t;
DEFINE_EDM_PLUGIN(BufferFactory, RecoTracktoTP_t,
                  "RecoTracktoTP");
				  
std::string SiPixelRawDataError_n("SiPixelRawDataError");
typedef Buffer<SiPixelRawDataError,
               &SiPixelRawDataError_n, COLLECTION>
SiPixelRawDataError_t;
DEFINE_EDM_PLUGIN(BufferFactory, SiPixelRawDataError_t,
                  "SiPixelRawDataError");
				  
std::string SimTrack_n("SimTrack");
typedef Buffer<SimTrack,
               &SimTrack_n, COLLECTION>
SimTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, SimTrack_t,
                  "SimTrack");
				  
std::string SimVertex_n("SimVertex");
typedef Buffer<SimVertex,
               &SimVertex_n, COLLECTION>
SimVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, SimVertex_t,
                  "SimVertex");
				  
std::string StEvtSolution_n("StEvtSolution");
typedef Buffer<StEvtSolution,
               &StEvtSolution_n, COLLECTION>
StEvtSolution_t;
DEFINE_EDM_PLUGIN(BufferFactory, StEvtSolution_t,
                  "StEvtSolution");
				  
std::string TPtoRecoTrack_n("TPtoRecoTrack");
typedef Buffer<TPtoRecoTrack,
               &TPtoRecoTrack_n, COLLECTION>
TPtoRecoTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, TPtoRecoTrack_t,
                  "TPtoRecoTrack");
				  
std::string TowerMETv0_n("TowerMETv0");
typedef Buffer<TowerMETv0,
               &TowerMETv0_n, COLLECTION>
TowerMETv0_t;
DEFINE_EDM_PLUGIN(BufferFactory, TowerMETv0_t,
                  "TowerMETv0");
				  
std::string TrackCandidate_n("TrackCandidate");
typedef Buffer<TrackCandidate,
               &TrackCandidate_n, COLLECTION>
TrackCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, TrackCandidate_t,
                  "TrackCandidate");
				  