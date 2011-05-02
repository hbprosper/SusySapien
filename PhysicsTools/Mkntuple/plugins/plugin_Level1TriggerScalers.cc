// -------------------------------------------------------------------------
// File::   plugin_Level1TriggerScalers.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/Scalers/interface/Level1TriggerScalers.h"

typedef Buffer<Level1TriggerScalers, false> Level1TriggerScalers_t;
DEFINE_EDM_PLUGIN(BufferFactory, Level1TriggerScalers_t,
                  "Level1TriggerScalers");
