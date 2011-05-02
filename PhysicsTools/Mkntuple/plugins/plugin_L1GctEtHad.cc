// -------------------------------------------------------------------------
// File::   plugin_L1GctEtHad.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtHad.h"

typedef Buffer<L1GctEtHad, false> L1GctEtHad_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtHad_t,
                  "L1GctEtHad");
