// -------------------------------------------------------------------------
// File::   plugins3.cc
// Created: Thu Jul 28 02:06:20 2011 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"

#include "DataFormats/EgammaReco/interface/PreshowerClusterShape.h"
typedef Buffer<reco::PreshowerClusterShape, false> recoPreshowerClusterShape_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreshowerClusterShape_t,
                  "recoPreshowerClusterShape");

#include "DataFormats/RecoCandidate/interface/RecoChargedRefCandidate.h"
typedef Buffer<reco::RecoChargedRefCandidate, false> recoRecoChargedRefCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoChargedRefCandidate_t,
                  "recoRecoChargedRefCandidate");

#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
typedef Buffer<reco::RecoEcalCandidate, false> recoRecoEcalCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoEcalCandidate_t,
                  "recoRecoEcalCandidate");

#include "DataFormats/EgammaReco/interface/SuperCluster.h"
typedef Buffer<reco::SuperCluster, false> recoSuperCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSuperCluster_t,
                  "recoSuperCluster");

#include "DataFormats/TrackReco/interface/Track.h"
typedef Buffer<reco::Track, false> recoTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrack_t,
                  "recoTrack");

#include "DataFormats/TrackReco/interface/TrackExtra.h"
typedef Buffer<reco::TrackExtra, false> recoTrackExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackExtra_t,
                  "recoTrackExtra");

#include "DataFormats/JetReco/interface/TrackExtrapolation.h"
typedef Buffer<reco::TrackExtrapolation, false> recoTrackExtrapolation_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackExtrapolation_t,
                  "recoTrackExtrapolation");

#include "DataFormats/JetReco/interface/TrackJet.h"
typedef Buffer<reco::TrackJet, false> recoTrackJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackJet_t,
                  "recoTrackJet");

#include "DataFormats/VertexReco/interface/Vertex.h"
typedef Buffer<reco::Vertex, false> recoVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoVertex_t,
                  "recoVertex");

#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
typedef Buffer<reco::VertexCompositeCandidate, false> recoVertexCompositeCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoVertexCompositeCandidate_t,
                  "recoVertexCompositeCandidate");

#include "AnalysisDataFormats/SUSYBSMObjects/interface/HSCPCaloInfo.h"
typedef Buffer<susybsm::HSCPCaloInfo, false> susybsmHSCPCaloInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, susybsmHSCPCaloInfo_t,
                  "susybsmHSCPCaloInfo");

#include "AnalysisDataFormats/SUSYBSMObjects/interface/HSCParticle.h"
typedef Buffer<susybsm::HSCParticle, false> susybsmHSCParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, susybsmHSCParticle_t,
                  "susybsmHSCParticle");

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
typedef Buffer<GenEventInfoProduct, true> GenEventInfoProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenEventInfoProduct_t,
                  "GenEventInfoProduct");

#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
typedef Buffer<GenRunInfoProduct, true> GenRunInfoProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenRunInfoProduct_t,
                  "GenRunInfoProduct");

#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
typedef Buffer<HcalNoiseSummary, true> HcalNoiseSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, HcalNoiseSummary_t,
                  "HcalNoiseSummary");

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
typedef Buffer<L1GlobalTriggerObjectMapRecord, true> L1GlobalTriggerObjectMapRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerObjectMapRecord_t,
                  "L1GlobalTriggerObjectMapRecord");

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
typedef Buffer<L1GlobalTriggerReadoutRecord, true> L1GlobalTriggerReadoutRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerReadoutRecord_t,
                  "L1GlobalTriggerReadoutRecord");

#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
typedef Buffer<LHEEventProduct, true> LHEEventProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, LHEEventProduct_t,
                  "LHEEventProduct");

#include "DataFormats/Common/interface/ConditionsInEdm.h"
typedef Buffer<edm::ConditionsInEventBlock, true> edmConditionsInEventBlock_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmConditionsInEventBlock_t,
                  "edmConditionsInEventBlock");

#include "PhysicsTools/TheNtupleMaker/interface/BufferEvent.h"
#include "FWCore/Framework/interface/Event.h"
typedef Buffer<edm::Event, true> edmEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmEvent_t,
                  "edmEvent");

#include "DataFormats/Common/interface/TriggerResults.h"
typedef Buffer<edm::TriggerResults, true> edmTriggerResults_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmTriggerResults_t,
                  "edmTriggerResults");
