// -------------------------------------------------------------------------
// File::   plugin_L1GctHFBitCounts.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFBitCounts.h"

typedef Buffer<L1GctHFBitCounts, false> L1GctHFBitCounts_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHFBitCounts_t,
                  "L1GctHFBitCounts");
