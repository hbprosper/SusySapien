//-----------------------------------------------------------------------------
// Package:     SO10
// Sub-Package: Analysis
//$Revision: 1.3 $
//-----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "SO10/Analysis/interface/myplugins.h"
//-----------------------------------------------------------------------------
typedef UserBuffer<pat::Muon, patMuonExtra> patMuonExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMuonExtra_t, "patMuonExtra");

