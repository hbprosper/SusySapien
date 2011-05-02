// -------------------------------------------------------------------------
// File::   plugin_patHemisphere.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/PatCandidates/interface/Hemisphere.h"

typedef Buffer<pat::Hemisphere, false> patHemisphere_t;
DEFINE_EDM_PLUGIN(BufferFactory, patHemisphere_t,
                  "patHemisphere");
