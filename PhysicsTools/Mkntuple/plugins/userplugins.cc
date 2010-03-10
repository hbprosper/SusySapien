//-----------------------------------------------------------------------------
//$Revision: 1.1 $
//-----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/userplugins.h"
//-----------------------------------------------------------------------------
typedef UserBuffer<reco::GenParticle, GenMother> GenMother_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenMother_t, "GenMother");

typedef UserBuffer<edm::TriggerResults, triggerBits> triggerBits_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerBits_t, "triggerBits");
