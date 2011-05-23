//$Revision: 1.1.1.1 $
#include "PhysicsTools/TheNtupleMaker/interface/Connection.h"
#include "PhysicsTools/TheNtupleMaker/interface/Slot.h"
#include "PhysicsTools/TheNtupleMaker/interface/pdg.h"
#include "PhysicsTools/TheNtupleMaker/interface/kit.h"
#include "PhysicsTools/TheNtupleMaker/interface/rfx.h"
#include "PhysicsTools/TheNtupleMaker/interface/root.h"
#include "PhysicsTools/TheNtupleMaker/interface/treestream.h"
#include "PhysicsTools/TheNtupleMaker/interface/HelperFor.h"
#include "PhysicsTools/TheNtupleMaker/interface/user.h"
#include "PhysicsTools/TheNtupleMaker/interface/GenParticleHelper.h"
#include "PhysicsTools/TheNtupleMaker/interface/GEvent.h"
#include "PhysicsTools/TheNtupleMaker/interface/HistogramCache.h"
#include "PhysicsTools/TheNtupleMaker/interface/CanvasScribe.h"
#include "PhysicsTools/TheNtupleMaker/interface/ClassFunction.h"
#include "PhysicsTools/TheNtupleMaker/interface/ClassMembers.h"
#include "PhysicsTools/TheNtupleMaker/interface/PoissonGammaFit.h"
#include "PhysicsTools/TheNtupleMaker/interface/RGS.h"
#include "PhysicsTools/TheNtupleMaker/interface/jetnet.h"
#include "PhysicsTools/TheNtupleMaker/interface/KDE.h"
#include "PhysicsTools/TheNtupleMaker/interface/AdBayes.h"
#include "PhysicsTools/TheNtupleMaker/interface/Bayespack.h"
//-----------------------------------------------------------------------------
#include "TLorentzVector.h"
#include "TH1F.h"
//-----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <fstream>
#include "PhysicsTools/TheNtupleMaker/interface/L1GlobalTriggerReadoutRecordHelper.h"
#include "PhysicsTools/TheNtupleMaker/interface/GenEventInfoProductHelper.h"


//-----------------------------------------------------------------------------

// Need explicit instantiations of template classes and functions

namespace {
  std::vector<TVector3>       c1;
  std::vector<TLorentzVector> c2;
  std::vector<kit::MatchedPair>    c3;
  std::vector<std::vector<double> > c4;
  std::vector<TH1F*> c5;
  std::vector<TH1*> c51;
  std::vector<kit::PtThing>    c6;
  std::map<std::string, std::vector<float>* > c7;
  std::map<std::string, std::vector<float> >  c8;
  std::map<std::string, std::vector<double> > c9;
  std::map<std::string, std::vector<double>* > c10;
  std::vector<bool>    c11;

  HelperFor<reco::GenParticle> h1;
  HelperFor<reco::HcalNoiseRBX> h2;
  HelperFor<edm::TriggerResults> h3;
  HelperFor<edm::Event> h4;
  HelperFor<L1GlobalTriggerReadoutRecord> t_L1GlobalTriggerReadoutRecordHelper;
  HelperFor<GenEventInfoProduct> t_GenEventInfoProductHelper;
}

