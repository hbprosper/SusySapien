// -------------------------------------------------------------------------
// File::   plugin_patMHT.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/PatCandidates/interface/MHT.h"

typedef Buffer<pat::MHT, false> patMHT_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMHT_t,
                  "patMHT");
