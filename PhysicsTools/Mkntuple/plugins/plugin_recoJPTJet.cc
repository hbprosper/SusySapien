// -------------------------------------------------------------------------
// File::   plugin_recoJPTJet.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/JetReco/interface/JPTJet.h"

typedef Buffer<reco::JPTJet, false> recoJPTJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoJPTJet_t,
                  "recoJPTJet");
