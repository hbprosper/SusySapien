// -------------------------------------------------------------------------
// File::   plugin_edmEvent.cc
// Created: Tue May  3 03:49:10 2011 by mkplugins.py
// $Revision: 1.2 $
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/BufferEvent.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
#include "FWCore/Framework/interface/Event.h"

typedef Buffer<edm::Event, true> edmEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmEvent_t,
                  "edmEvent");
