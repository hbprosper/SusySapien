// -------------------------------------------------------------------------
// File::   plugins09.cc
// Created: Sat May  5 17:06:51 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/EWK/interface/WMuNuCandidate.h"
#include "AnalysisDataFormats/EWK/interface/WMuNuCandidatePtr.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/EcalDigi/interface/EcalTrigPrimCompactColl.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/HLTReco/interface/HLTPerformanceInfo.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HcalDigi/interface/HcalLaserDigi.h"
#include "DataFormats/HcalDigi/interface/HcalUnpackerReport.h"
#include "DataFormats/HcalRecHit/interface/HcalSourcePositionData.h"
#include "DataFormats/L1CSCTrackFinder/interface/CSCTriggerContainer.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1Track.h"
#include "DataFormats/L1CSCTrackFinder/interface/TrackStub.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerEvmReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1Trigger/interface/L1DataEmulRecord.h"
#include "DataFormats/METObjects/interface/BaseMET.h"
#include "DataFormats/METReco/interface/AnomalousECALVariables.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/MuonData/interface/MuonDigiCollection.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/TrackerCommon/interface/ClusterSummary.h"
#include "DataFormats/V0Candidate/interface/V0Candidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
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
				  
std::string triggerTriggerObject_n("trigger::TriggerObject");
typedef Buffer<trigger::TriggerObject,
               &triggerTriggerObject_n, COLLECTION>
triggerTriggerObject_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerTriggerObject_t,
                  "triggerTriggerObject");
				  
std::string AnomalousECALVariables_n("AnomalousECALVariables");
typedef Buffer<AnomalousECALVariables,
               &AnomalousECALVariables_n, SINGLETON>
AnomalousECALVariables_t;
DEFINE_EDM_PLUGIN(BufferFactory, AnomalousECALVariables_t,
                  "AnomalousECALVariables");
				  
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
				  
std::string ClusterSummary_n("ClusterSummary");
typedef Buffer<ClusterSummary,
               &ClusterSummary_n, SINGLETON>
ClusterSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, ClusterSummary_t,
                  "ClusterSummary");
				  
std::string CrossingFramePCaloHit_n("CrossingFrame<PCaloHit>");
typedef Buffer<CrossingFrame<PCaloHit>,
               &CrossingFramePCaloHit_n, SINGLETON>
CrossingFramePCaloHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFramePCaloHit_t,
                  "CrossingFramePCaloHit");
				  
std::string CrossingFramePSimHit_n("CrossingFrame<PSimHit>");
typedef Buffer<CrossingFrame<PSimHit>,
               &CrossingFramePSimHit_n, SINGLETON>
CrossingFramePSimHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFramePSimHit_t,
                  "CrossingFramePSimHit");
				  
std::string CrossingFrameSimTrack_n("CrossingFrame<SimTrack>");
typedef Buffer<CrossingFrame<SimTrack>,
               &CrossingFrameSimTrack_n, SINGLETON>
CrossingFrameSimTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFrameSimTrack_t,
                  "CrossingFrameSimTrack");
				  
std::string CrossingFrameSimVertex_n("CrossingFrame<SimVertex>");
typedef Buffer<CrossingFrame<SimVertex>,
               &CrossingFrameSimVertex_n, SINGLETON>
CrossingFrameSimVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFrameSimVertex_t,
                  "CrossingFrameSimVertex");
				  
std::string CrossingFrameedmHepMCProduct_n("CrossingFrame<edm::HepMCProduct>");
typedef Buffer<CrossingFrame<edm::HepMCProduct>,
               &CrossingFrameedmHepMCProduct_n, SINGLETON>
CrossingFrameedmHepMCProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, CrossingFrameedmHepMCProduct_t,
                  "CrossingFrameedmHepMCProduct");
				  
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
				  