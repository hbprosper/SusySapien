// -------------------------------------------------------------------------
// File::   plugin_L1GctHtMiss.cc
// Created: Tue May  3 03:49:10 2011 by mkplugins.py
// $Revision: 1.15 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHtMiss.h"

typedef Buffer<L1GctHtMiss, false> L1GctHtMiss_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHtMiss_t,
                  "L1GctHtMiss");
