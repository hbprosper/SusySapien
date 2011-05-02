// -------------------------------------------------------------------------
// File::   plugin_recoCastorEgamma.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/CastorReco/interface/CastorEgamma.h"

typedef Buffer<reco::CastorEgamma, false> recoCastorEgamma_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorEgamma_t,
                  "recoCastorEgamma");
