// -------------------------------------------------------------------------
// File::   plugin_L1GctEtMiss.cc
// Created: Tue May  3 03:49:10 2011 by mkplugins.py
// $Revision: 1.2 $
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtMiss.h"

typedef Buffer<L1GctEtMiss, false> L1GctEtMiss_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtMiss_t,
                  "L1GctEtMiss");
