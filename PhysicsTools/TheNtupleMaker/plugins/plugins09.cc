// -------------------------------------------------------------------------
// File::   plugins09.cc
// Created: Thu Apr 26 00:40:14 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/EWK/interface/WMuNuCandidate.h"
#include "AnalysisDataFormats/EWK/interface/WMuNuCandidatePtr.h"
#include "AnalysisDataFormats/SUSYBSMObjects/interface/HSCPCaloInfo.h"
#include "AnalysisDataFormats/SUSYBSMObjects/interface/HSCPIsolation.h"
#include "AnalysisDataFormats/SUSYBSMObjects/interface/HSCParticle.h"
#include "AnalysisDataFormats/TrackInfo/interface/TrackInfo.h"
#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include "DataFormats/BTauReco/interface/TauImpactParameterInfo.h"
#include "DataFormats/BTauReco/interface/TauMassTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackProbabilityTagInfo.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/EcalDigi/interface/EcalTrigPrimCompactColl.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/HLTReco/interface/HLTPerformanceInfo.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/JetReco/interface/TrackExtrapolation.h"
#include "DataFormats/JetReco/interface/TrackJet.h"
#include "DataFormats/L1CSCTrackFinder/interface/CSCTriggerContainer.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1Track.h"
#include "DataFormats/L1CSCTrackFinder/interface/TrackStub.h"
#include "DataFormats/METObjects/interface/BaseMET.h"
#include "DataFormats/MuonData/interface/MuonDigiCollection.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/V0Candidate/interface/V0Candidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFramePlaybackInfo.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFramePlaybackInfoExtended.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
// -------------------------------------------------------------------------

std::string recoTaggingVariableList_n("reco::TaggingVariableList");
typedef Buffer<reco::TaggingVariableList,
               &recoTaggingVariableList_n, COLLECTION>
recoTaggingVariableList_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTaggingVariableList_t,
                  "recoTaggingVariableList");
				  
std::string recoTauImpactParameterInfo_n("reco::TauImpactParameterInfo");
typedef Buffer<reco::TauImpactParameterInfo,
               &recoTauImpactParameterInfo_n, COLLECTION>
recoTauImpactParameterInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTauImpactParameterInfo_t,
                  "recoTauImpactParameterInfo");
				  
std::string recoTauMassTagInfo_n("reco::TauMassTagInfo");
typedef Buffer<reco::TauMassTagInfo,
               &recoTauMassTagInfo_n, COLLECTION>
recoTauMassTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTauMassTagInfo_t,
                  "recoTauMassTagInfo");
				  
std::string recoTrack_n("reco::Track");
typedef Buffer<reco::Track,
               &recoTrack_n, COLLECTION>
recoTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrack_t,
                  "recoTrack");
				  
std::string recoTrackCountingTagInfo_n("reco::TrackCountingTagInfo");
typedef Buffer<reco::TrackCountingTagInfo,
               &recoTrackCountingTagInfo_n, COLLECTION>
recoTrackCountingTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackCountingTagInfo_t,
                  "recoTrackCountingTagInfo");
				  
std::string recoTrackExtra_n("reco::TrackExtra");
typedef Buffer<reco::TrackExtra,
               &recoTrackExtra_n, COLLECTION>
recoTrackExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackExtra_t,
                  "recoTrackExtra");
				  
std::string recoTrackExtrapolation_n("reco::TrackExtrapolation");
typedef Buffer<reco::TrackExtrapolation,
               &recoTrackExtrapolation_n, COLLECTION>
recoTrackExtrapolation_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackExtrapolation_t,
                  "recoTrackExtrapolation");
				  
std::string recoTrackIPTagInfo_n("reco::TrackIPTagInfo");
typedef Buffer<reco::TrackIPTagInfo,
               &recoTrackIPTagInfo_n, COLLECTION>
recoTrackIPTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackIPTagInfo_t,
                  "recoTrackIPTagInfo");
				  
std::string recoTrackInfo_n("reco::TrackInfo");
typedef Buffer<reco::TrackInfo,
               &recoTrackInfo_n, COLLECTION>
recoTrackInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackInfo_t,
                  "recoTrackInfo");
				  
std::string recoTrackJet_n("reco::TrackJet");
typedef Buffer<reco::TrackJet,
               &recoTrackJet_n, COLLECTION>
recoTrackJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackJet_t,
                  "recoTrackJet");
				  
std::string recoTrackProbabilityTagInfo_n("reco::TrackProbabilityTagInfo");
typedef Buffer<reco::TrackProbabilityTagInfo,
               &recoTrackProbabilityTagInfo_n, COLLECTION>
recoTrackProbabilityTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackProbabilityTagInfo_t,
                  "recoTrackProbabilityTagInfo");
				  
std::string recoV0Candidate_n("reco::V0Candidate");
typedef Buffer<reco::V0Candidate,
               &recoV0Candidate_n, COLLECTION>
recoV0Candidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoV0Candidate_t,
                  "recoV0Candidate");
				  
std::string recoVertex_n("reco::Vertex");
typedef Buffer<reco::Vertex,
               &recoVertex_n, COLLECTION>
recoVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoVertex_t,
                  "recoVertex");
				  
std::string recoVertexCompositeCandidate_n("reco::VertexCompositeCandidate");
typedef Buffer<reco::VertexCompositeCandidate,
               &recoVertexCompositeCandidate_n, COLLECTION>
recoVertexCompositeCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoVertexCompositeCandidate_t,
                  "recoVertexCompositeCandidate");
				  
std::string recoWMuNuCandidate_n("reco::WMuNuCandidate");
typedef Buffer<reco::WMuNuCandidate,
               &recoWMuNuCandidate_n, COLLECTION>
recoWMuNuCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoWMuNuCandidate_t,
                  "recoWMuNuCandidate");
				  
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
				  