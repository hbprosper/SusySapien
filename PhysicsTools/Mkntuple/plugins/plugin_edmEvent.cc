// -------------------------------------------------------------------------
// File::   plugin_edmEvent.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/BufferEvent.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "FWCore/Framework/interface/Event.h"

typedef Buffer<edm::Event, true> edmEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmEvent_t,
                  "edmEvent");
