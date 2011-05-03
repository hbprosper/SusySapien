// -------------------------------------------------------------------------
// File::   plugin_recoSecondaryVertexTagInfo.cc
// Created: Tue May  3 03:49:10 2011 by mkplugins.py
// $Revision: 1.15 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"

typedef Buffer<reco::SecondaryVertexTagInfo, false> recoSecondaryVertexTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSecondaryVertexTagInfo_t,
                  "recoSecondaryVertexTagInfo");
