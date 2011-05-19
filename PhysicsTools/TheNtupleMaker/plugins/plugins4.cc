// -------------------------------------------------------------------------
// File::   plugins4.cc
// Created: Thu May 19 10:06:12 2011 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"

#include "DataFormats/METReco/interface/GlobalHaloData.h"
typedef Buffer<reco::GlobalHaloData, true> recoGlobalHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGlobalHaloData_t,
                  "recoGlobalHaloData");

#include "DataFormats/METReco/interface/HcalHaloData.h"
typedef Buffer<reco::HcalHaloData, true> recoHcalHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHcalHaloData_t,
                  "recoHcalHaloData");

#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
typedef Buffer<reco::PFTauDiscriminator, true> recoPFTauDiscriminator_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTauDiscriminator_t,
                  "recoPFTauDiscriminator");

#include "DataFormats/HLTReco/interface/TriggerEvent.h"
typedef Buffer<trigger::TriggerEvent, true> triggerTriggerEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerTriggerEvent_t,
                  "triggerTriggerEvent");
