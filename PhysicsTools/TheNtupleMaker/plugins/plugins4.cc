// -------------------------------------------------------------------------
// File::   plugins4.cc
// Created: Thu Jul 28 02:06:20 2011 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"

#include "DataFormats/METReco/interface/BeamHaloSummary.h"
typedef Buffer<reco::BeamHaloSummary, true> recoBeamHaloSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBeamHaloSummary_t,
                  "recoBeamHaloSummary");

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
typedef Buffer<reco::BeamSpot, true> recoBeamSpot_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBeamSpot_t,
                  "recoBeamSpot");

#include "DataFormats/TauReco/interface/CaloTauDiscriminator.h"
typedef Buffer<reco::CaloTauDiscriminator, true> recoCaloTauDiscriminator_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloTauDiscriminator_t,
                  "recoCaloTauDiscriminator");

#include "DataFormats/METReco/interface/GlobalHaloData.h"
typedef Buffer<reco::GlobalHaloData, true> recoGlobalHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGlobalHaloData_t,
                  "recoGlobalHaloData");

#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
typedef Buffer<reco::PFTauDiscriminator, true> recoPFTauDiscriminator_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTauDiscriminator_t,
                  "recoPFTauDiscriminator");

#include "DataFormats/HLTReco/interface/TriggerEvent.h"
typedef Buffer<trigger::TriggerEvent, true> triggerTriggerEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerTriggerEvent_t,
                  "triggerTriggerEvent");
