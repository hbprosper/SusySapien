// -------------------------------------------------------------------------
// File::   plugin_recoHcalHaloData.cc
// Created: Tue May  3 03:49:10 2011 by mkplugins.py
// $Revision: 1.15 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/METReco/interface/HcalHaloData.h"

typedef Buffer<reco::HcalHaloData, true> recoHcalHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHcalHaloData_t,
                  "recoHcalHaloData");
