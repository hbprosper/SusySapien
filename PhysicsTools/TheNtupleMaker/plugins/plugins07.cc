// -------------------------------------------------------------------------
// File::   plugins07.cc
// Created: Tue Apr  3 21:16:04 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/TopObjects/interface/StEvtSolution.h"
#include "AnalysisDataFormats/TrackInfo/interface/RecoTracktoTP.h"
#include "AnalysisDataFormats/TrackInfo/interface/TPtoRecoTrack.h"
#include "DataFormats/LTCDigi/interface/LTCDigi.h"
#include "DataFormats/METObjects/interface/MET.h"
#include "DataFormats/METObjects/interface/TowerMET.h"
#include "DataFormats/MuonSeed/interface/L3MuonTrajectorySeed.h"
#include "DataFormats/RPCDigi/interface/RPCDigiL1Link.h"
#include "DataFormats/Scalers/interface/Level1TriggerRates.h"
#include "DataFormats/Scalers/interface/Level1TriggerScalers.h"
#include "DataFormats/Scalers/interface/LumiScalers.h"
#include "DataFormats/SiPixelRawData/interface/SiPixelRawDataError.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "DataFormats/TrackingSeed/interface/TrackingSeed.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/Forward/interface/LHCTransportLink.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupMixingContent.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/TrackingAnalysis/interface/ParticleBase.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/ValidationFormats/interface/MaterialAccountingDetector.h"
#include "SimDataFormats/ValidationFormats/interface/MaterialAccountingStep.h"
#include "SimDataFormats/ValidationFormats/interface/MaterialAccountingTrack.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
// -------------------------------------------------------------------------

typedef Buffer<L3MuonTrajectorySeed, false>
L3MuonTrajectorySeed_t;
DEFINE_EDM_PLUGIN(BufferFactory, L3MuonTrajectorySeed_t,
                  "L3MuonTrajectorySeed");
				  
typedef Buffer<LHCTransportLink, false>
LHCTransportLink_t;
DEFINE_EDM_PLUGIN(BufferFactory, LHCTransportLink_t,
                  "LHCTransportLink");
				  
typedef Buffer<LTCDigi, false>
LTCDigi_t;
DEFINE_EDM_PLUGIN(BufferFactory, LTCDigi_t,
                  "LTCDigi");
				  
typedef Buffer<Level1TriggerRates, false>
Level1TriggerRates_t;
DEFINE_EDM_PLUGIN(BufferFactory, Level1TriggerRates_t,
                  "Level1TriggerRates");
				  
typedef Buffer<Level1TriggerScalers, false>
Level1TriggerScalers_t;
DEFINE_EDM_PLUGIN(BufferFactory, Level1TriggerScalers_t,
                  "Level1TriggerScalers");
				  
typedef Buffer<LumiScalers, false>
LumiScalers_t;
DEFINE_EDM_PLUGIN(BufferFactory, LumiScalers_t,
                  "LumiScalers");
				  
typedef Buffer<METv0, false>
METv0_t;
DEFINE_EDM_PLUGIN(BufferFactory, METv0_t,
                  "METv0");
				  
typedef Buffer<MaterialAccountingDetector, false>
MaterialAccountingDetector_t;
DEFINE_EDM_PLUGIN(BufferFactory, MaterialAccountingDetector_t,
                  "MaterialAccountingDetector");
				  
typedef Buffer<MaterialAccountingStep, false>
MaterialAccountingStep_t;
DEFINE_EDM_PLUGIN(BufferFactory, MaterialAccountingStep_t,
                  "MaterialAccountingStep");
				  
typedef Buffer<MaterialAccountingTrack, false>
MaterialAccountingTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, MaterialAccountingTrack_t,
                  "MaterialAccountingTrack");
				  
typedef Buffer<PCaloHit, false>
PCaloHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, PCaloHit_t,
                  "PCaloHit");
				  
typedef Buffer<PSimHit, false>
PSimHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, PSimHit_t,
                  "PSimHit");
				  
typedef Buffer<ParticleBase, false>
ParticleBase_t;
DEFINE_EDM_PLUGIN(BufferFactory, ParticleBase_t,
                  "ParticleBase");
				  
typedef Buffer<PileupMixingContent, false>
PileupMixingContent_t;
DEFINE_EDM_PLUGIN(BufferFactory, PileupMixingContent_t,
                  "PileupMixingContent");
				  
typedef Buffer<PileupSummaryInfo, false>
PileupSummaryInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, PileupSummaryInfo_t,
                  "PileupSummaryInfo");
				  
typedef Buffer<RPCDigiL1Link, false>
RPCDigiL1Link_t;
DEFINE_EDM_PLUGIN(BufferFactory, RPCDigiL1Link_t,
                  "RPCDigiL1Link");
				  
typedef Buffer<RecoTracktoTP, false>
RecoTracktoTP_t;
DEFINE_EDM_PLUGIN(BufferFactory, RecoTracktoTP_t,
                  "RecoTracktoTP");
				  
typedef Buffer<SiPixelRawDataError, false>
SiPixelRawDataError_t;
DEFINE_EDM_PLUGIN(BufferFactory, SiPixelRawDataError_t,
                  "SiPixelRawDataError");
				  
typedef Buffer<SimTrack, false>
SimTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, SimTrack_t,
                  "SimTrack");
				  
typedef Buffer<SimVertex, false>
SimVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, SimVertex_t,
                  "SimVertex");
				  
typedef Buffer<StEvtSolution, false>
StEvtSolution_t;
DEFINE_EDM_PLUGIN(BufferFactory, StEvtSolution_t,
                  "StEvtSolution");
				  
typedef Buffer<TPtoRecoTrack, false>
TPtoRecoTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, TPtoRecoTrack_t,
                  "TPtoRecoTrack");
				  
typedef Buffer<TowerMETv0, false>
TowerMETv0_t;
DEFINE_EDM_PLUGIN(BufferFactory, TowerMETv0_t,
                  "TowerMETv0");
				  
typedef Buffer<TrackCandidate, false>
TrackCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, TrackCandidate_t,
                  "TrackCandidate");
				  
typedef Buffer<TrackingParticle, false>
TrackingParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, TrackingParticle_t,
                  "TrackingParticle");
				  
typedef Buffer<TrackingSeed, false>
TrackingSeed_t;
DEFINE_EDM_PLUGIN(BufferFactory, TrackingSeed_t,
                  "TrackingSeed");
				  
typedef Buffer<TrackingVertex, false>
TrackingVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, TrackingVertex_t,
                  "TrackingVertex");
				  