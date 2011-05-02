// -------------------------------------------------------------------------
// File::   plugin_recoCSCHaloData.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/METReco/interface/CSCHaloData.h"

typedef Buffer<reco::CSCHaloData, true> recoCSCHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCSCHaloData_t,
                  "recoCSCHaloData");
