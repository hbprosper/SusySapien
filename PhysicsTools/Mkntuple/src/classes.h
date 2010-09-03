//$Revision: 1.3 $
#include "PhysicsTools/Mkntuple/interface/user.h"
#include "PhysicsTools/Mkntuple/interface/GEvent.h"
#include "PhysicsTools/Mkntuple/interface/HelperFor.h"

namespace 
{
  HelperFor<reco::GenParticle> c1;
  HelperFor<reco::HcalNoiseRBXCaloTower> c2;
  HelperFor<edm::TriggerResultsAddon> c3;
  HelperFor<edm::EventAddon> c4;
}

