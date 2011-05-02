// -------------------------------------------------------------------------
// File::   plugin_recoSecondaryVertexTagInfo.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"

typedef Buffer<reco::SecondaryVertexTagInfo, false> recoSecondaryVertexTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSecondaryVertexTagInfo_t,
                  "recoSecondaryVertexTagInfo");
