// -------------------------------------------------------------------------
// File::   plugin_L1GctHFRingEtSums.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFRingEtSums.h"

typedef Buffer<L1GctHFRingEtSums, false> L1GctHFRingEtSums_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHFRingEtSums_t,
                  "L1GctHFRingEtSums");
