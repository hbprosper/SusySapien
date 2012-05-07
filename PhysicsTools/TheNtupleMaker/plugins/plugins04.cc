// -------------------------------------------------------------------------
// File::   plugins04.cc
// Created: Sun May  6 00:40:38 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/VBFEventT1T2.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/HLTPathStatus.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/FP420Cluster/interface/ClusterFP420.h"
#include "DataFormats/FP420Cluster/interface/RecoFP420.h"
#include "DataFormats/FP420Cluster/interface/TrackFP420.h"
#include "DataFormats/FP420Digi/interface/HDigiFP420.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1Track.h"
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
#include "DataFormats/L1Trigger/interface/L1HFRings.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/SiPixelDigi/interface/SiPixelCalibDigi.h"
#include "DataFormats/SiPixelDigi/interface/SiPixelCalibDigiError.h"
#include "DataFormats/SiPixelRawData/interface/SiPixelRawDataError.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripProcessedRawDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h"
#include "FWCore/MessageLogger/interface/ErrorSummaryEntry.h"
#include "SimDataFormats/RPCDigiSimLink/interface/RPCDigiSimLink.h"
#include "SimDataFormats/TrackerDigiSimLink/interface/PixelDigiSimLink.h"
#include "SimDataFormats/TrackerDigiSimLink/interface/StripDigiSimLink.h"
// -------------------------------------------------------------------------

std::string cmgVBFEventT1T2patJetrecoPFCandidate_n("cmg::VBFEventT1T2<pat::Jet,reco::PFCandidate>");
typedef Buffer<cmg::VBFEventT1T2<pat::Jet,reco::PFCandidate>,
               &cmgVBFEventT1T2patJetrecoPFCandidate_n, COLLECTION>
cmgVBFEventT1T2patJetrecoPFCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgVBFEventT1T2patJetrecoPFCandidate_t,
                  "cmgVBFEventT1T2patJetrecoPFCandidate");
				  
std::string cmgVBFEventT1T2recoGenJetrecoGenJet_n("cmg::VBFEventT1T2<reco::GenJet,reco::GenJet>");
typedef Buffer<cmg::VBFEventT1T2<reco::GenJet,reco::GenJet>,
               &cmgVBFEventT1T2recoGenJetrecoGenJet_n, COLLECTION>
cmgVBFEventT1T2recoGenJetrecoGenJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgVBFEventT1T2recoGenJetrecoGenJet_t,
                  "cmgVBFEventT1T2recoGenJetrecoGenJet");
				  
std::string cmgVBFEventT1T2recoPFJetrecoPFCandidate_n("cmg::VBFEventT1T2<reco::PFJet,reco::PFCandidate>");
typedef Buffer<cmg::VBFEventT1T2<reco::PFJet,reco::PFCandidate>,
               &cmgVBFEventT1T2recoPFJetrecoPFCandidate_n, COLLECTION>
cmgVBFEventT1T2recoPFJetrecoPFCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgVBFEventT1T2recoPFJetrecoPFCandidate_t,
                  "cmgVBFEventT1T2recoPFJetrecoPFCandidate");
				  
std::string cmgVBFEventT1T2recoPFJetrecoPFJet_n("cmg::VBFEventT1T2<reco::PFJet,reco::PFJet>");
typedef Buffer<cmg::VBFEventT1T2<reco::PFJet,reco::PFJet>,
               &cmgVBFEventT1T2recoPFJetrecoPFJet_n, COLLECTION>
cmgVBFEventT1T2recoPFJetrecoPFJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgVBFEventT1T2recoPFJetrecoPFJet_t,
                  "cmgVBFEventT1T2recoPFJetrecoPFJet");
				  
std::string cmgWENu_n("cmg::WENu");
typedef Buffer<cmg::WENu,
               &cmgWENu_n, COLLECTION>
cmgWENu_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgWENu_t,
                  "cmgWENu");
				  
std::string cmgWMuNu_n("cmg::WMuNu");
typedef Buffer<cmg::WMuNu,
               &cmgWMuNu_n, COLLECTION>
cmgWMuNu_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgWMuNu_t,
                  "cmgWMuNu");
				  
std::string cmgWTauNu_n("cmg::WTauNu");
typedef Buffer<cmg::WTauNu,
               &cmgWTauNu_n, COLLECTION>
cmgWTauNu_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgWTauNu_t,
                  "cmgWTauNu");
				  
std::string cscL1Track_n("csc::L1Track");
typedef Buffer<csc::L1Track,
               &cscL1Track_n, COLLECTION>
cscL1Track_t;
DEFINE_EDM_PLUGIN(BufferFactory, cscL1Track_t,
                  "cscL1Track");
				  
std::string edmDetSetClusterFP420_n("edm::DetSet<ClusterFP420>");
typedef Buffer<edm::DetSet<ClusterFP420>,
               &edmDetSetClusterFP420_n, COLLECTION>
edmDetSetClusterFP420_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDetSetClusterFP420_t,
                  "edmDetSetClusterFP420");
				  
std::string edmDetSetEcalRecHit_n("edm::DetSet<EcalRecHit>");
typedef Buffer<edm::DetSet<EcalRecHit>,
               &edmDetSetEcalRecHit_n, COLLECTION>
edmDetSetEcalRecHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDetSetEcalRecHit_t,
                  "edmDetSetEcalRecHit");
				  
std::string edmDetSetHDigiFP420_n("edm::DetSet<HDigiFP420>");
typedef Buffer<edm::DetSet<HDigiFP420>,
               &edmDetSetHDigiFP420_n, COLLECTION>
edmDetSetHDigiFP420_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDetSetHDigiFP420_t,
                  "edmDetSetHDigiFP420");
				  
std::string edmDetSetPixelDigi_n("edm::DetSet<PixelDigi>");
typedef Buffer<edm::DetSet<PixelDigi>,
               &edmDetSetPixelDigi_n, COLLECTION>
edmDetSetPixelDigi_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDetSetPixelDigi_t,
                  "edmDetSetPixelDigi");
				  
std::string edmDetSetPixelDigiSimLink_n("edm::DetSet<PixelDigiSimLink>");
typedef Buffer<edm::DetSet<PixelDigiSimLink>,
               &edmDetSetPixelDigiSimLink_n, COLLECTION>
edmDetSetPixelDigiSimLink_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDetSetPixelDigiSimLink_t,
                  "edmDetSetPixelDigiSimLink");
				  
std::string edmDetSetRPCDigiSimLink_n("edm::DetSet<RPCDigiSimLink>");
typedef Buffer<edm::DetSet<RPCDigiSimLink>,
               &edmDetSetRPCDigiSimLink_n, COLLECTION>
edmDetSetRPCDigiSimLink_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDetSetRPCDigiSimLink_t,
                  "edmDetSetRPCDigiSimLink");
				  
std::string edmDetSetRecoFP420_n("edm::DetSet<RecoFP420>");
typedef Buffer<edm::DetSet<RecoFP420>,
               &edmDetSetRecoFP420_n, COLLECTION>
edmDetSetRecoFP420_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDetSetRecoFP420_t,
                  "edmDetSetRecoFP420");
				  
std::string edmDetSetSiPixelCalibDigi_n("edm::DetSet<SiPixelCalibDigi>");
typedef Buffer<edm::DetSet<SiPixelCalibDigi>,
               &edmDetSetSiPixelCalibDigi_n, COLLECTION>
edmDetSetSiPixelCalibDigi_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDetSetSiPixelCalibDigi_t,
                  "edmDetSetSiPixelCalibDigi");
				  
std::string edmDetSetSiPixelCalibDigiError_n("edm::DetSet<SiPixelCalibDigiError>");
typedef Buffer<edm::DetSet<SiPixelCalibDigiError>,
               &edmDetSetSiPixelCalibDigiError_n, COLLECTION>
edmDetSetSiPixelCalibDigiError_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDetSetSiPixelCalibDigiError_t,
                  "edmDetSetSiPixelCalibDigiError");
				  
std::string edmDetSetSiPixelRawDataError_n("edm::DetSet<SiPixelRawDataError>");
typedef Buffer<edm::DetSet<SiPixelRawDataError>,
               &edmDetSetSiPixelRawDataError_n, COLLECTION>
edmDetSetSiPixelRawDataError_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDetSetSiPixelRawDataError_t,
                  "edmDetSetSiPixelRawDataError");
				  
std::string edmDetSetSiStripCluster_n("edm::DetSet<SiStripCluster>");
typedef Buffer<edm::DetSet<SiStripCluster>,
               &edmDetSetSiStripCluster_n, COLLECTION>
edmDetSetSiStripCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDetSetSiStripCluster_t,
                  "edmDetSetSiStripCluster");
				  
std::string edmDetSetSiStripDigi_n("edm::DetSet<SiStripDigi>");
typedef Buffer<edm::DetSet<SiStripDigi>,
               &edmDetSetSiStripDigi_n, COLLECTION>
edmDetSetSiStripDigi_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDetSetSiStripDigi_t,
                  "edmDetSetSiStripDigi");
				  
std::string edmDetSetSiStripProcessedRawDigi_n("edm::DetSet<SiStripProcessedRawDigi>");
typedef Buffer<edm::DetSet<SiStripProcessedRawDigi>,
               &edmDetSetSiStripProcessedRawDigi_n, COLLECTION>
edmDetSetSiStripProcessedRawDigi_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDetSetSiStripProcessedRawDigi_t,
                  "edmDetSetSiStripProcessedRawDigi");
				  
std::string edmDetSetSiStripRawDigi_n("edm::DetSet<SiStripRawDigi>");
typedef Buffer<edm::DetSet<SiStripRawDigi>,
               &edmDetSetSiStripRawDigi_n, COLLECTION>
edmDetSetSiStripRawDigi_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDetSetSiStripRawDigi_t,
                  "edmDetSetSiStripRawDigi");
				  
std::string edmDetSetStripDigiSimLink_n("edm::DetSet<StripDigiSimLink>");
typedef Buffer<edm::DetSet<StripDigiSimLink>,
               &edmDetSetStripDigiSimLink_n, COLLECTION>
edmDetSetStripDigiSimLink_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDetSetStripDigiSimLink_t,
                  "edmDetSetStripDigiSimLink");
				  
std::string edmDetSetTrackFP420_n("edm::DetSet<TrackFP420>");
typedef Buffer<edm::DetSet<TrackFP420>,
               &edmDetSetTrackFP420_n, COLLECTION>
edmDetSetTrackFP420_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmDetSetTrackFP420_t,
                  "edmDetSetTrackFP420");
				  
std::string edmErrorSummaryEntry_n("edm::ErrorSummaryEntry");
typedef Buffer<edm::ErrorSummaryEntry,
               &edmErrorSummaryEntry_n, COLLECTION>
edmErrorSummaryEntry_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmErrorSummaryEntry_t,
                  "edmErrorSummaryEntry");
				  
std::string edmEventAuxiliary_n("edm::EventAuxiliary");
typedef Buffer<edm::EventAuxiliary,
               &edmEventAuxiliary_n, COLLECTION>
edmEventAuxiliary_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmEventAuxiliary_t,
                  "edmEventAuxiliary");
				  
std::string edmHLTPathStatus_n("edm::HLTPathStatus");
typedef Buffer<edm::HLTPathStatus,
               &edmHLTPathStatus_n, COLLECTION>
edmHLTPathStatus_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmHLTPathStatus_t,
                  "edmHLTPathStatus");
				  
std::string l1extraL1EmParticle_n("l1extra::L1EmParticle");
typedef Buffer<l1extra::L1EmParticle,
               &l1extraL1EmParticle_n, COLLECTION>
l1extraL1EmParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, l1extraL1EmParticle_t,
                  "l1extraL1EmParticle");
				  
std::string l1extraL1EtMissParticle_n("l1extra::L1EtMissParticle");
typedef Buffer<l1extra::L1EtMissParticle,
               &l1extraL1EtMissParticle_n, COLLECTION>
l1extraL1EtMissParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, l1extraL1EtMissParticle_t,
                  "l1extraL1EtMissParticle");
				  
std::string l1extraL1HFRings_n("l1extra::L1HFRings");
typedef Buffer<l1extra::L1HFRings,
               &l1extraL1HFRings_n, COLLECTION>
l1extraL1HFRings_t;
DEFINE_EDM_PLUGIN(BufferFactory, l1extraL1HFRings_t,
                  "l1extraL1HFRings");
				  
std::string l1extraL1JetParticle_n("l1extra::L1JetParticle");
typedef Buffer<l1extra::L1JetParticle,
               &l1extraL1JetParticle_n, COLLECTION>
l1extraL1JetParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, l1extraL1JetParticle_t,
                  "l1extraL1JetParticle");
				  
std::string l1extraL1MuonParticle_n("l1extra::L1MuonParticle");
typedef Buffer<l1extra::L1MuonParticle,
               &l1extraL1MuonParticle_n, COLLECTION>
l1extraL1MuonParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, l1extraL1MuonParticle_t,
                  "l1extraL1MuonParticle");
				  
std::string l1extraL1ParticleMap_n("l1extra::L1ParticleMap");
typedef Buffer<l1extra::L1ParticleMap,
               &l1extraL1ParticleMap_n, COLLECTION>
l1extraL1ParticleMap_t;
DEFINE_EDM_PLUGIN(BufferFactory, l1extraL1ParticleMap_t,
                  "l1extraL1ParticleMap");
				  
std::string patCompositeCandidate_n("pat::CompositeCandidate");
typedef Buffer<pat::CompositeCandidate,
               &patCompositeCandidate_n, COLLECTION>
patCompositeCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, patCompositeCandidate_t,
                  "patCompositeCandidate");
				  