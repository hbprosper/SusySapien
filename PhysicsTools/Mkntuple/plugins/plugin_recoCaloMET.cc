// -------------------------------------------------------------------------
// File::   plugin_recoCaloMET.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/METReco/interface/CaloMET.h"

typedef Buffer<reco::CaloMET, false> recoCaloMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloMET_t,
                  "recoCaloMET");