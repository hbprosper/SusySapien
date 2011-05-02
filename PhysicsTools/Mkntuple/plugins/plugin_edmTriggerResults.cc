// -------------------------------------------------------------------------
// File::   plugin_edmTriggerResults.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/Common/interface/TriggerResults.h"

typedef Buffer<edm::TriggerResults, true> edmTriggerResults_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmTriggerResults_t,
                  "edmTriggerResults");
