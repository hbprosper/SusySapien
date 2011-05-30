// -------------------------------------------------------------------------
// File::   plugins1.cc
// Created: Mon May 30 06:05:59 2011 by mkplugins.py
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

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEmCand.h"
typedef Buffer<L1GctEmCand, false> L1GctEmCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEmCand_t,
                  "L1GctEmCand");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtHad.h"
typedef Buffer<L1GctEtHad, false> L1GctEtHad_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtHad_t,
                  "L1GctEtHad");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtMiss.h"
typedef Buffer<L1GctEtMiss, false> L1GctEtMiss_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtMiss_t,
                  "L1GctEtMiss");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtTotal.h"
typedef Buffer<L1GctEtTotal, false> L1GctEtTotal_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtTotal_t,
                  "L1GctEtTotal");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFBitCounts.h"
typedef Buffer<L1GctHFBitCounts, false> L1GctHFBitCounts_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHFBitCounts_t,
                  "L1GctHFBitCounts");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFRingEtSums.h"
typedef Buffer<L1GctHFRingEtSums, false> L1GctHFRingEtSums_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHFRingEtSums_t,
                  "L1GctHFRingEtSums");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHtMiss.h"
typedef Buffer<L1GctHtMiss, false> L1GctHtMiss_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHtMiss_t,
                  "L1GctHtMiss");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCand.h"
typedef Buffer<L1GctJetCand, false> L1GctJetCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctJetCand_t,
                  "L1GctJetCand");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCounts.h"
typedef Buffer<L1GctJetCounts, false> L1GctJetCounts_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctJetCounts_t,
                  "L1GctJetCounts");

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

#include "SimDataFormats/Track/interface/SimTrack.h"
typedef Buffer<SimTrack, false> SimTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, SimTrack_t,
                  "SimTrack");

#include "SimDataFormats/Vertex/interface/SimVertex.h"
typedef Buffer<SimVertex, false> SimVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, SimVertex_t,
                  "SimVertex");

#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
typedef Buffer<TrajectorySeed, false> TrajectorySeed_t;
DEFINE_EDM_PLUGIN(BufferFactory, TrajectorySeed_t,
                  "TrajectorySeed");

#include <vector>
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
