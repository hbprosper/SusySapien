// -------------------------------------------------------------------------
// File::   plugin_recoVertex.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

typedef Buffer<reco::Vertex, false> recoVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoVertex_t,
                  "recoVertex");
