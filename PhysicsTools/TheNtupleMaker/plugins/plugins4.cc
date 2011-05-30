// -------------------------------------------------------------------------
// File::   plugins4.cc
// Created: Mon May 30 06:05:59 2011 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"

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

#include "DataFormats/METReco/interface/BeamHaloSummary.h"
typedef Buffer<reco::BeamHaloSummary, true> recoBeamHaloSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBeamHaloSummary_t,
                  "recoBeamHaloSummary");

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
typedef Buffer<reco::BeamSpot, true> recoBeamSpot_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBeamSpot_t,
                  "recoBeamSpot");

#include "DataFormats/METReco/interface/CSCHaloData.h"
typedef Buffer<reco::CSCHaloData, true> recoCSCHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCSCHaloData_t,
                  "recoCSCHaloData");

#include "DataFormats/TauReco/interface/CaloTauDiscriminator.h"
typedef Buffer<reco::CaloTauDiscriminator, true> recoCaloTauDiscriminator_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloTauDiscriminator_t,
                  "recoCaloTauDiscriminator");

#include "DataFormats/METReco/interface/EcalHaloData.h"
typedef Buffer<reco::EcalHaloData, true> recoEcalHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoEcalHaloData_t,
                  "recoEcalHaloData");

#include "DataFormats/METReco/interface/GlobalHaloData.h"
typedef Buffer<reco::GlobalHaloData, true> recoGlobalHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGlobalHaloData_t,
                  "recoGlobalHaloData");

#include "DataFormats/METReco/interface/HcalHaloData.h"
typedef Buffer<reco::HcalHaloData, true> recoHcalHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHcalHaloData_t,
                  "recoHcalHaloData");

#include "DataFormats/TauReco/interface/JetPiZeroAssociation.h"
typedef Buffer<reco::JetPiZeroAssociation, true> recoJetPiZeroAssociation_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoJetPiZeroAssociation_t,
                  "recoJetPiZeroAssociation");

#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
typedef Buffer<reco::PFTauDiscriminator, true> recoPFTauDiscriminator_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTauDiscriminator_t,
                  "recoPFTauDiscriminator");

#include "DataFormats/HLTReco/interface/TriggerEvent.h"
typedef Buffer<trigger::TriggerEvent, true> triggerTriggerEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerTriggerEvent_t,
                  "triggerTriggerEvent");
