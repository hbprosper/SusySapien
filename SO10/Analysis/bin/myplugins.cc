//-----------------------------------------------------------------------------
// Package:     SO10
// Sub-Package: Analysis
//$Revision: 1.2 $
//-----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "SO10/Analysis/interface/myplugins.h"
//-----------------------------------------------------------------------------
typedef Buffer<pat::Muon, patMuonExtra> patMuonExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMuonExtra_t, "patMuonExtra");

typedef Buffer<reco::Muon, recoMuonIsol> recoMuonIsol_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuonIsol_t, "recoMuonIsol");
