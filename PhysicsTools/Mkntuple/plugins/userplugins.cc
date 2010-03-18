//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Add user-defined methods
// Created:     Tue Jan 19, 2010 HBP
// Updated:     Mon Mar 08, 2010 Sezen & HBP - add triggerBits class
//$Revision: 1.2 $
//-----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/user.h"
//-----------------------------------------------------------------------------
typedef UserBuffer<reco::GenParticle, GenMother> GenMother_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenMother_t, "GenMother");

typedef UserBuffer<edm::TriggerResults, triggerBits> triggerBits_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerBits_t, "triggerBits");
