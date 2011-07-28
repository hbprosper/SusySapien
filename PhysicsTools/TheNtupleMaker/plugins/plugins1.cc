// -------------------------------------------------------------------------
// File::   plugins1.cc
// Created: Thu Jul 28 02:06:20 2011 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"

#include "DataFormats/Scalers/interface/BeamSpotOnline.h"
typedef Buffer<BeamSpotOnline, false> BeamSpotOnline_t;
DEFINE_EDM_PLUGIN(BufferFactory, BeamSpotOnline_t,
                  "BeamSpotOnline");

#include "DataFormats/CaloTowers/interface/CaloTower.h"
typedef Buffer<CaloTower, false> CaloTower_t;
DEFINE_EDM_PLUGIN(BufferFactory, CaloTower_t,
                  "CaloTower");

#include "DataFormats/Scalers/interface/DcsStatus.h"
typedef Buffer<DcsStatus, false> DcsStatus_t;
DEFINE_EDM_PLUGIN(BufferFactory, DcsStatus_t,
                  "DcsStatus");

#include "DataFormats/Scalers/interface/L1AcceptBunchCrossing.h"
typedef Buffer<L1AcceptBunchCrossing, false> L1AcceptBunchCrossing_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1AcceptBunchCrossing_t,
                  "L1AcceptBunchCrossing");

#include "DataFormats/Scalers/interface/L1TriggerScalers.h"
typedef Buffer<L1TriggerScalers, false> L1TriggerScalers_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1TriggerScalers_t,
                  "L1TriggerScalers");

#include "DataFormats/Scalers/interface/Level1TriggerScalers.h"
typedef Buffer<Level1TriggerScalers, false> Level1TriggerScalers_t;
DEFINE_EDM_PLUGIN(BufferFactory, Level1TriggerScalers_t,
                  "Level1TriggerScalers");

#include "DataFormats/Scalers/interface/LumiScalers.h"
typedef Buffer<LumiScalers, false> LumiScalers_t;
DEFINE_EDM_PLUGIN(BufferFactory, LumiScalers_t,
                  "LumiScalers");

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
typedef Buffer<PileupSummaryInfo, false> PileupSummaryInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, PileupSummaryInfo_t,
                  "PileupSummaryInfo");

#include "DataFormats/Common/interface/AssociationVector.h"
#include "DataFormats/JetReco/interface/Jet.h"
typedef Buffer<edm::AssociationVector<edm::RefToBaseProd<reco::Jet>,std::vector<float>,edm::RefToBase<reco::Jet>,unsigned int,edm::helper::AssociationIdenticalKeyReference>, false> edmAssociationVectorrecoJetfloat_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmAssociationVectorrecoJetfloat_t,
                  "edmAssociationVectorrecoJetfloat");

#include "FWCore/MessageLogger/interface/ErrorSummaryEntry.h"
typedef Buffer<edm::ErrorSummaryEntry, false> edmErrorSummaryEntry_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmErrorSummaryEntry_t,
                  "edmErrorSummaryEntry");

#include "DataFormats/PatCandidates/interface/Electron.h"
typedef Buffer<pat::Electron, false> patElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, patElectron_t,
                  "patElectron");

#include "DataFormats/PatCandidates/interface/Jet.h"
typedef Buffer<pat::Jet, false> patJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, patJet_t,
                  "patJet");

#include "DataFormats/PatCandidates/interface/MET.h"
typedef Buffer<pat::MET, false> patMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMET_t,
                  "patMET");

#include "DataFormats/PatCandidates/interface/Muon.h"
typedef Buffer<pat::Muon, false> patMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMuon_t,
                  "patMuon");

#include "DataFormats/PatCandidates/interface/Photon.h"
typedef Buffer<pat::Photon, false> patPhoton_t;
DEFINE_EDM_PLUGIN(BufferFactory, patPhoton_t,
                  "patPhoton");

#include "DataFormats/PatCandidates/interface/Tau.h"
typedef Buffer<pat::Tau, false> patTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTau_t,
                  "patTau");

#include "DataFormats/JetReco/interface/BasicJet.h"
typedef Buffer<reco::BasicJet, false> recoBasicJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBasicJet_t,
                  "recoBasicJet");

#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
typedef Buffer<reco::CaloCluster, false> recoCaloCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloCluster_t,
                  "recoCaloCluster");

#include "DataFormats/JetReco/interface/CaloJet.h"
typedef Buffer<reco::CaloJet, false> recoCaloJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloJet_t,
                  "recoCaloJet");

#include "DataFormats/METReco/interface/CaloMET.h"
typedef Buffer<reco::CaloMET, false> recoCaloMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloMET_t,
                  "recoCaloMET");

#include "DataFormats/TauReco/interface/CaloTau.h"
typedef Buffer<reco::CaloTau, false> recoCaloTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloTau_t,
                  "recoCaloTau");
