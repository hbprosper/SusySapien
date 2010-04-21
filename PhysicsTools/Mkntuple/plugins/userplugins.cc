//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Add user-defined methods
// Created:     Tue Jan 19, 2010 HBP
// Updated:     Mon Mar 08, 2010 Sezen & HBP - add triggerBits class
//              Thu Apr 08, 2010 Sezen & HBP - add GParticle class
//$Revision: 1.3 $
//-----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/user.h"
//-----------------------------------------------------------------------------
typedef UserBuffer<reco::GenParticle, GParticle> GParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, GParticle_t, "GParticle");

typedef UserBuffer<edm::TriggerResults, triggerBits> triggerBits_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerBits_t, "triggerBits");
