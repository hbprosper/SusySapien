//$Revision: 1.4 $
#include "PhysicsTools/Mkntuple/interface/user.h"
#include "PhysicsTools/Mkntuple/interface/GEvent.h"
#include "PhysicsTools/Mkntuple/interface/HelperFor.h"

namespace 
{
  HelperFor<reco::GenParticle> c1;
  HelperFor<reco::HcalNoiseRBXCaloTower> c2;
  HelperFor<edm::TriggerResultsHelper> c3;
  HelperFor<edm::EventHelper> c4;
}

