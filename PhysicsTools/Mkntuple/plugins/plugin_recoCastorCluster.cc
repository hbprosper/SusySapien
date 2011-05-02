// -------------------------------------------------------------------------
// File::   plugin_recoCastorCluster.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/CastorReco/interface/CastorCluster.h"

typedef Buffer<reco::CastorCluster, false> recoCastorCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorCluster_t,
                  "recoCastorCluster");
