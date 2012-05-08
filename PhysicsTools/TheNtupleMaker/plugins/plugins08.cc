// -------------------------------------------------------------------------
// File::   plugins08.cc
// Created: Tue May  8 03:57:32 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/EWK/interface/WMuNuCandidate.h"
#include "AnalysisDataFormats/EWK/interface/WMuNuCandidatePtr.h"
#include "AnalysisDataFormats/SUSYBSMObjects/interface/HSCPCaloInfo.h"
#include "AnalysisDataFormats/TrackInfo/interface/TrackInfo.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "DataFormats/BTauReco/interface/SoftLeptonTagInfo.h"
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
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "DataFormats/EgammaReco/interface/PreshowerClusterShape.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/JetReco/interface/TrackExtrapolation.h"
#include "DataFormats/JetReco/interface/TrackJet.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1Track.h"
#include "DataFormats/MuonData/interface/MuonDigiCollection.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedRefCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoPFClusterRefCandidate.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/TauReco/interface/RecoTauPiZero.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/V0Candidate/interface/V0Candidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
// -------------------------------------------------------------------------

std::string recoPreshowerClusterShape_n("reco::PreshowerClusterShape");
typedef Buffer<reco::PreshowerClusterShape,
               &recoPreshowerClusterShape_n, COLLECTION>
recoPreshowerClusterShape_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreshowerClusterShape_t,
                  "recoPreshowerClusterShape");
				  
std::string recoRecoChargedCandidate_n("reco::RecoChargedCandidate");
typedef Buffer<reco::RecoChargedCandidate,
               &recoRecoChargedCandidate_n, COLLECTION>
recoRecoChargedCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoChargedCandidate_t,
                  "recoRecoChargedCandidate");
				  
std::string recoRecoChargedRefCandidate_n("reco::RecoChargedRefCandidate");
typedef Buffer<reco::RecoChargedRefCandidate,
               &recoRecoChargedRefCandidate_n, COLLECTION>
recoRecoChargedRefCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoChargedRefCandidate_t,
                  "recoRecoChargedRefCandidate");
				  
std::string recoRecoEcalCandidate_n("reco::RecoEcalCandidate");
typedef Buffer<reco::RecoEcalCandidate,
               &recoRecoEcalCandidate_n, COLLECTION>
recoRecoEcalCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoEcalCandidate_t,
                  "recoRecoEcalCandidate");
				  
std::string recoRecoPFClusterRefCandidate_n("reco::RecoPFClusterRefCandidate");
typedef Buffer<reco::RecoPFClusterRefCandidate,
               &recoRecoPFClusterRefCandidate_n, COLLECTION>
recoRecoPFClusterRefCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoPFClusterRefCandidate_t,
                  "recoRecoPFClusterRefCandidate");
				  
std::string recoRecoTauPiZero_n("reco::RecoTauPiZero");
typedef Buffer<reco::RecoTauPiZero,
               &recoRecoTauPiZero_n, COLLECTION>
recoRecoTauPiZero_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoTauPiZero_t,
                  "recoRecoTauPiZero");
				  
std::string recoSecondaryVertexTagInfo_n("reco::SecondaryVertexTagInfo");
typedef Buffer<reco::SecondaryVertexTagInfo,
               &recoSecondaryVertexTagInfo_n, COLLECTION>
recoSecondaryVertexTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSecondaryVertexTagInfo_t,
                  "recoSecondaryVertexTagInfo");
				  
std::string recoSiStripElectron_n("reco::SiStripElectron");
typedef Buffer<reco::SiStripElectron,
               &recoSiStripElectron_n, COLLECTION>
recoSiStripElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSiStripElectron_t,
                  "recoSiStripElectron");
				  
std::string recoSoftLeptonTagInfo_n("reco::SoftLeptonTagInfo");
typedef Buffer<reco::SoftLeptonTagInfo,
               &recoSoftLeptonTagInfo_n, COLLECTION>
recoSoftLeptonTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSoftLeptonTagInfo_t,
                  "recoSoftLeptonTagInfo");
				  
std::string recoSuperCluster_n("reco::SuperCluster");
typedef Buffer<reco::SuperCluster,
               &recoSuperCluster_n, COLLECTION>
recoSuperCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSuperCluster_t,
                  "recoSuperCluster");
				  
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
				  