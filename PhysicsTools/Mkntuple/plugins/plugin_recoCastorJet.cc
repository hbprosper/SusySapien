// -------------------------------------------------------------------------
// File::   plugin_recoCastorJet.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/CastorReco/interface/CastorJet.h"

typedef Buffer<reco::CastorJet, false> recoCastorJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorJet_t,
                  "recoCastorJet");
