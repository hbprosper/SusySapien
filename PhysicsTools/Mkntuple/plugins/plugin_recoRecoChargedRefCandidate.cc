// -------------------------------------------------------------------------
// File::   plugin_recoRecoChargedRefCandidate.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedRefCandidate.h"

typedef Buffer<reco::RecoChargedRefCandidate, false> recoRecoChargedRefCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoChargedRefCandidate_t,
                  "recoRecoChargedRefCandidate");