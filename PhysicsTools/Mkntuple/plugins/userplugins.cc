//-----------------------------------------------------------------------------
//$Revision: 1.1 $
//-----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/userplugins.h"
//-----------------------------------------------------------------------------
typedef UserBuffer<reco::Muon, recoMuonExtra> recoMuonExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuonExtra_t, "recoMuonExtra");

typedef UserBuffer<reco::GenParticle, recoGenMother> recoGenMother_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGenMother_t, "recoGenMother");

