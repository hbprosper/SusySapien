// -------------------------------------------------------------------------
// File::   plugin_edmConditionsInEventBlock.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/Common/interface/ConditionsInEdm.h"

typedef Buffer<edm::ConditionsInEventBlock, true> edmConditionsInEventBlock_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmConditionsInEventBlock_t,
                  "edmConditionsInEventBlock");
