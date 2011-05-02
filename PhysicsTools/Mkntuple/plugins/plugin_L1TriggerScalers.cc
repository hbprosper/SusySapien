// -------------------------------------------------------------------------
// File::   plugin_L1TriggerScalers.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/Scalers/interface/L1TriggerScalers.h"

typedef Buffer<L1TriggerScalers, false> L1TriggerScalers_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1TriggerScalers_t,
                  "L1TriggerScalers");
