//$Revision: 1.10 $
#include "PhysicsTools/Mkntuple/interface/Connection.h"
#include "PhysicsTools/Mkntuple/interface/Slot.h"
#include "PhysicsTools/Mkntuple/interface/pdg.h"
#include "PhysicsTools/Mkntuple/interface/kit.h"
#include "PhysicsTools/Mkntuple/interface/rfx.h"
#include "PhysicsTools/Mkntuple/interface/root.h"
#include "PhysicsTools/Mkntuple/interface/treestream.h"
#include "PhysicsTools/Mkntuple/interface/HelperFor.h"
#include "PhysicsTools/Mkntuple/interface/user.h"
#include "PhysicsTools/Mkntuple/interface/GenParticleHelper.h"
#include "PhysicsTools/Mkntuple/interface/GEvent.h"
#include "PhysicsTools/Mkntuple/interface/HistogramCache.h"
#include "PhysicsTools/Mkntuple/interface/CanvasScribe.h"
#include "PhysicsTools/Mkntuple/interface/FunctionMember.h"
#include "PhysicsTools/Mkntuple/interface/ClassMembers.h"
#include "PhysicsTools/Mkntuple/interface/PoissonGammaFit.h"
#include "PhysicsTools/Mkntuple/interface/RGS.h"
#include "PhysicsTools/Mkntuple/interface/jetnet.h"
#include "PhysicsTools/Mkntuple/interface/KDE.h"
#include "PhysicsTools/Mkntuple/interface/AdBayes.h"
#include "PhysicsTools/Mkntuple/interface/Bayespack.h"
//-----------------------------------------------------------------------------
#include "TLorentzVector.h"
#include "TH1F.h"
//-----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <fstream>
#include "PhysicsTools/Mkntuple/interface/L1GlobalTriggerReadoutRecordHelper.h"

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
}

