// -------------------------------------------------------------------------
// File::   plugins1.cc
// Created: Thu May 19 10:06:12 2011 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"

#include "DataFormats/Scalers/interface/BeamSpotOnline.h"
typedef Buffer<BeamSpotOnline, false> BeamSpotOnline_t;
DEFINE_EDM_PLUGIN(BufferFactory, BeamSpotOnline_t,
                  "BeamSpotOnline");

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

#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
typedef Buffer<TrajectorySeed, false> TrajectorySeed_t;
DEFINE_EDM_PLUGIN(BufferFactory, TrajectorySeed_t,
                  "TrajectorySeed");

#include "FWCore/MessageLogger/interface/ErrorSummaryEntry.h"
typedef Buffer<edm::ErrorSummaryEntry, false> edmErrorSummaryEntry_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmErrorSummaryEntry_t,
                  "edmErrorSummaryEntry");

#include "DataFormats/PatCandidates/interface/Electron.h"
typedef Buffer<pat::Electron, false> patElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, patElectron_t,
                  "patElectron");

#include "DataFormats/PatCandidates/interface/Hemisphere.h"
typedef Buffer<pat::Hemisphere, false> patHemisphere_t;
DEFINE_EDM_PLUGIN(BufferFactory, patHemisphere_t,
                  "patHemisphere");

#include "DataFormats/PatCandidates/interface/Jet.h"
typedef Buffer<pat::Jet, false> patJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, patJet_t,
                  "patJet");

#include "DataFormats/PatCandidates/interface/MET.h"
typedef Buffer<pat::MET, false> patMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMET_t,
                  "patMET");

#include "DataFormats/PatCandidates/interface/MHT.h"
typedef Buffer<pat::MHT, false> patMHT_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMHT_t,
                  "patMHT");

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

#include "DataFormats/MuonReco/interface/CaloMuon.h"
typedef Buffer<reco::CaloMuon, false> recoCaloMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloMuon_t,
                  "recoCaloMuon");
