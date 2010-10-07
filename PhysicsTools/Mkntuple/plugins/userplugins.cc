//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Add user-defined methods
// Created:     Tue Jan 19, 2010 HBP
// Updated:     Mon Mar 08, 2010 Sezen & HBP - add triggerBits class
//              Thu Apr 08, 2010 Sezen & HBP - add GParticle class
//              Thu Aug 25, 2010 HBP - rename classes
//$Revision: 1.9 $
//-----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "PhysicsTools/Mkntuple/interface/user.h"
#include "PhysicsTools/Mkntuple/interface/BufferEventHelper.h"
//-----------------------------------------------------------------------------
typedef UserBuffer<reco::GenParticle, 
                   reco::GenParticleHelper, false> recoGenParticleHelper_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGenParticleHelper_t, 
                  "recoGenParticleHelper");

typedef UserBuffer<reco::HcalNoiseRBX, 
                   reco::HcalNoiseRBXCaloTower, false> 
recoHcalNoiseRBXCaloTower_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHcalNoiseRBXCaloTower_t, 
                  "recoHcalNoiseRBXCaloTower");

typedef UserBuffer<edm::TriggerResults, 
                   edm::TriggerResultsHelper, true> edmTriggerResultsHelper_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmTriggerResultsHelper_t, 
                  "edmTriggerResultsHelper");

typedef UserBuffer<edm::Event, edm::EventHelper, true> edmEventHelper_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmEventHelper_t, "edmEventHelper");

// Synomyms:

typedef UserBuffer<edm::TriggerResults, triggerBits, true> triggerBits_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerBits_t, "triggerBits");

typedef UserBuffer<reco::GenParticle, GParticle, false> GParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, GParticle_t, "GParticle");

