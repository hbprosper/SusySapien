// -------------------------------------------------------------------------
// File::   plugin_edmEvent.cc
// Created: Tue May  3 03:49:10 2011 by mkplugins.py
// $Revision: 1.15 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/BufferEvent.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "FWCore/Framework/interface/Event.h"

typedef Buffer<edm::Event, true> edmEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmEvent_t,
                  "edmEvent");
