//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Add user-defined methods
// Created:     Tue Jan 19, 2010 HBP
// Updated:     Mon Mar 08, 2010 Sezen & HBP - add triggerBits class
//              Thu Apr 08, 2010 Sezen & HBP - add GParticle class
//              Thu Aug 25, 2010 HBP - rename classes
//$Revision: 1.6 $
//-----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/user.h"
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/BufferEventAddon.h"
//-----------------------------------------------------------------------------
typedef BufferAddon<reco::GenParticle, 
                    reco::GenParticleAddon, false> recoGenParticleAddon_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGenParticleAddon_t, 
                  "recoGenParticleAddon");

typedef BufferHelper<reco::HcalNoiseRBX, 
                     reco::HcalNoiseRBXCaloTower, false> 
recoHcalNoiseRBXCaloTower_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHcalNoiseRBXCaloTower_t, 
                  "recoHcalNoiseRBXCaloTower");

typedef BufferAddon<edm::TriggerResults, 
                    edm::TriggerResultsAddon, true> edmTriggerResultsAddon_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmTriggerResultsAddon_t, 
                  "edmTriggerResultsAddon");

typedef BufferAddon<edm::Event, edm::EventAddon, true> edmEventAddon_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmEventAddon_t, "edmEventAddon");

// Deprecated:

typedef BufferAddon<edm::TriggerResults, triggerBits, true> triggerBits_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerBits_t, "triggerBits");

typedef BufferAddon<reco::GenParticle, GParticle, false> GParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, GParticle_t, "GParticle");
