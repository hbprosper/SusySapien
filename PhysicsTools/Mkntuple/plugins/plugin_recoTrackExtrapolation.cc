// -------------------------------------------------------------------------
// File::   plugin_recoTrackExtrapolation.cc
// Created: Tue May  3 03:49:10 2011 by mkplugins.py
// $Revision: 1.15 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/JetReco/interface/TrackExtrapolation.h"

typedef Buffer<reco::TrackExtrapolation, false> recoTrackExtrapolation_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackExtrapolation_t,
                  "recoTrackExtrapolation");
