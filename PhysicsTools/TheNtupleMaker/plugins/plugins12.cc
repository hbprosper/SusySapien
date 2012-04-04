// -------------------------------------------------------------------------
// File::   plugins12.cc
// Created: Tue Apr  3 22:26:00 2012 by mkplugins.py
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
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
#include "DataFormats/EgammaReco/interface/PreshowerClusterShape.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/HLTReco/interface/HLTPrescaleTable.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/JetReco/interface/TrackExtrapolation.h"
#include "DataFormats/JetReco/interface/TrackJet.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedRefCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoPFClusterRefCandidate.h"
#include "DataFormats/TauReco/interface/RecoTauPiZero.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/V0Candidate/interface/V0Candidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
// -------------------------------------------------------------------------

typedef Buffer<reco::PreshowerCluster, false>
recoPreshowerCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreshowerCluster_t,
                  "recoPreshowerCluster");
				  
typedef Buffer<reco::PreshowerClusterShape, false>
recoPreshowerClusterShape_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreshowerClusterShape_t,
                  "recoPreshowerClusterShape");
				  
typedef Buffer<reco::RecoChargedCandidate, false>
recoRecoChargedCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoChargedCandidate_t,
                  "recoRecoChargedCandidate");
				  
typedef Buffer<reco::RecoChargedRefCandidate, false>
recoRecoChargedRefCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoChargedRefCandidate_t,
                  "recoRecoChargedRefCandidate");
				  
typedef Buffer<reco::RecoEcalCandidate, false>
recoRecoEcalCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoEcalCandidate_t,
                  "recoRecoEcalCandidate");
				  
typedef Buffer<reco::RecoPFClusterRefCandidate, false>
recoRecoPFClusterRefCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoPFClusterRefCandidate_t,
                  "recoRecoPFClusterRefCandidate");
				  
typedef Buffer<reco::RecoTauPiZero, false>
recoRecoTauPiZero_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoTauPiZero_t,
                  "recoRecoTauPiZero");
				  
typedef Buffer<reco::SiStripElectron, false>
recoSiStripElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSiStripElectron_t,
                  "recoSiStripElectron");
				  
typedef Buffer<reco::SuperCluster, false>
recoSuperCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSuperCluster_t,
                  "recoSuperCluster");
				  
typedef Buffer<reco::Track, false>
recoTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrack_t,
                  "recoTrack");
				  
typedef Buffer<reco::TrackExtra, false>
recoTrackExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackExtra_t,
                  "recoTrackExtra");
				  
typedef Buffer<reco::TrackExtrapolation, false>
recoTrackExtrapolation_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackExtrapolation_t,
                  "recoTrackExtrapolation");
				  
typedef Buffer<reco::TrackInfo, false>
recoTrackInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackInfo_t,
                  "recoTrackInfo");
				  
typedef Buffer<reco::TrackJet, false>
recoTrackJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackJet_t,
                  "recoTrackJet");
				  
typedef Buffer<reco::V0Candidate, false>
recoV0Candidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoV0Candidate_t,
                  "recoV0Candidate");
				  
typedef Buffer<reco::Vertex, false>
recoVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoVertex_t,
                  "recoVertex");
				  
typedef Buffer<reco::VertexCompositeCandidate, false>
recoVertexCompositeCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoVertexCompositeCandidate_t,
                  "recoVertexCompositeCandidate");
				  
typedef Buffer<reco::WMuNuCandidate, false>
recoWMuNuCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoWMuNuCandidate_t,
                  "recoWMuNuCandidate");
				  
typedef Buffer<reco::WMuNuCandidatePtr, false>
recoWMuNuCandidatePtr_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoWMuNuCandidatePtr_t,
                  "recoWMuNuCandidatePtr");
				  
typedef Buffer<std::pair<reco::CompositeCandidate, std::vector<int> >, false>
stdpairrecoCompositeCandidatestdvectorint_t;
DEFINE_EDM_PLUGIN(BufferFactory, stdpairrecoCompositeCandidatestdvectorint_t,
                  "stdpairrecoCompositeCandidatestdvectorint");
				  
typedef Buffer<susybsm::HSCPCaloInfo, true>
susybsmHSCPCaloInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, susybsmHSCPCaloInfo_t,
                  "susybsmHSCPCaloInfo");
				  
typedef Buffer<susybsm::HSCPIsolation, true>
susybsmHSCPIsolation_t;
DEFINE_EDM_PLUGIN(BufferFactory, susybsmHSCPIsolation_t,
                  "susybsmHSCPIsolation");
				  
typedef Buffer<susybsm::TracksEcalRecHitsMap, true>
susybsmTracksEcalRecHitsMap_t;
DEFINE_EDM_PLUGIN(BufferFactory, susybsmTracksEcalRecHitsMap_t,
                  "susybsmTracksEcalRecHitsMap");
				  
typedef Buffer<trigger::HLTPrescaleTable, true>
triggerHLTPrescaleTable_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerHLTPrescaleTable_t,
                  "triggerHLTPrescaleTable");
				  
typedef Buffer<trigger::TriggerEvent, true>
triggerTriggerEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerTriggerEvent_t,
                  "triggerTriggerEvent");
				  
typedef Buffer<trigger::TriggerEventWithRefs, true>
triggerTriggerEventWithRefs_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerTriggerEventWithRefs_t,
                  "triggerTriggerEventWithRefs");
				  
typedef Buffer<trigger::TriggerFilterObjectWithRefs, true>
triggerTriggerFilterObjectWithRefs_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerTriggerFilterObjectWithRefs_t,
                  "triggerTriggerFilterObjectWithRefs");
				  