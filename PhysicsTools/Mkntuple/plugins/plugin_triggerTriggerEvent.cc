// -------------------------------------------------------------------------
// File::   plugin_triggerTriggerEvent.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"

typedef Buffer<trigger::TriggerEvent, true> triggerTriggerEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerTriggerEvent_t,
                  "triggerTriggerEvent");
