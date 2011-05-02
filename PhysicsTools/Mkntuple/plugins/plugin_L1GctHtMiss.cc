// -------------------------------------------------------------------------
// File::   plugin_L1GctHtMiss.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHtMiss.h"

typedef Buffer<L1GctHtMiss, false> L1GctHtMiss_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHtMiss_t,
                  "L1GctHtMiss");
