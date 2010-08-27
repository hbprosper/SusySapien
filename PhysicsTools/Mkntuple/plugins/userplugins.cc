//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Add user-defined methods
// Created:     Tue Jan 19, 2010 HBP
// Updated:     Mon Mar 08, 2010 Sezen & HBP - add triggerBits class
//              Thu Apr 08, 2010 Sezen & HBP - add GParticle class
//              Thu Aug 25, 2010 HBP - rename classes
//$Revision: 1.4 $
//-----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/user.h"
//-----------------------------------------------------------------------------
typedef BufferAddon<reco::GenParticle, 
                    GenParticleAddon, false> GenParticleAddon_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenParticleAddon_t, 
                  "GenParticleAddon");

typedef BufferHelper<reco::HcalNoiseRBX, 
                     HcalNoiseRBXCaloTower, false> HcalNoiseRBXCaloTower_t;
DEFINE_EDM_PLUGIN(BufferFactory, HcalNoiseRBXCaloTower_t, 
                  "HcalNoiseRBXCaloTower");

typedef BufferAddon<edm::TriggerResults, 
                    TriggerResultsAddon, true> TriggerResultsAddon_t;
DEFINE_EDM_PLUGIN(BufferFactory, TriggerResultsAddon_t, 
                  "TriggerResultsAddon");
