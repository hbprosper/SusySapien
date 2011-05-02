// -------------------------------------------------------------------------
// File::   plugin_L1GctJetCand.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCand.h"

typedef Buffer<L1GctJetCand, false> L1GctJetCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctJetCand_t,
                  "L1GctJetCand");
