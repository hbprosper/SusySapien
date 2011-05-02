// -------------------------------------------------------------------------
// File::   plugin_recoTrackExtra.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"

typedef Buffer<reco::TrackExtra, false> recoTrackExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackExtra_t,
                  "recoTrackExtra");
