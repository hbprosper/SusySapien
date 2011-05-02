// -------------------------------------------------------------------------
// File::   plugin_recoCaloMuon.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/MuonReco/interface/CaloMuon.h"

typedef Buffer<reco::CaloMuon, false> recoCaloMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloMuon_t,
                  "recoCaloMuon");
