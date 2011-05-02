// -------------------------------------------------------------------------
// File::   plugin_BeamSpotOnline.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/Scalers/interface/BeamSpotOnline.h"

typedef Buffer<BeamSpotOnline, false> BeamSpotOnline_t;
DEFINE_EDM_PLUGIN(BufferFactory, BeamSpotOnline_t,
                  "BeamSpotOnline");
