//-----------------------------------------------------------------------------
#include "PhysicsTools/LiteAnalysis/interface/Connection.hpp"
#include "PhysicsTools/LiteAnalysis/interface/Slot.hpp"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
#include "PhysicsTools/LiteAnalysis/interface/root.h"
#include "PhysicsTools/LiteAnalysis/interface/treestream.hpp"
#include "PhysicsTools/LiteAnalysis/interface/KDE.hpp"
#include "PhysicsTools/LiteAnalysis/interface/Slurper.hpp"
#include "PhysicsTools/LiteAnalysis/interface/jetnet.hpp"
#include "PhysicsTools/LiteAnalysis/interface/PoissonGamma.hpp"
#include "PhysicsTools/LiteAnalysis/interface/PoissonGammaFit.hpp"
#include "PhysicsTools/LiteAnalysis/interface/HistogramCache.hpp"
#include "PhysicsTools/LiteAnalysis/interface/CanvasScribe.hpp"
#include "PhysicsTools/LiteAnalysis/interface/AdBayes.hpp"
#include "PhysicsTools/LiteAnalysis/interface/Bayespack.hpp"
#include "PhysicsTools/LiteAnalysis/interface/Slurper.hpp"
#include "PhysicsTools/LiteAnalysis/interface/ATest.h"
//-----------------------------------------------------------------------------
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/MuonReco/interface/Muon.h"
//-----------------------------------------------------------------------------
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Hemisphere.h"
#include "DataFormats/PatCandidates/interface/MET.h"
//-----------------------------------------------------------------------------
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"
//-----------------------------------------------------------------------------
#include "TLorentzVector.h"
#include "TH1F.h"
//-----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <fstream>
//-----------------------------------------------------------------------------

// Need explicit instantiations of template classes and functions

namespace {
  namespace {
    std::vector<TVector3>       c1;
    std::vector<TLorentzVector> c2;
    std::vector<kit::MatchedPair>    c3;
    std::vector<std::vector<double> > c4;
    std::vector<TH1F*> c5;
    std::vector<kit::PtThing>    c6;
    std::map<std::string, std::vector<float>* > c7;
    std::map<std::string, std::vector<float> >  c8;
    std::map<std::string, std::vector<double> > c9;
  }
}

//-------------------------------------------------------------------------

template
double ATest::method<Aclass>() const;

template
double ATest::method<Bclass>() const;

template
double ATest::method<Aclass, Bclass>() const;

template
void kit::sort(std::vector<int>&);

template
void kit::sort(std::vector<float>&);

template
void kit::sort(std::vector<double>&);

template
void kit::sort(std::vector<std::string>&);

template
void kit::sort(std::vector<PtThing>&);

template
bool kit::hasKey(std::map<std::string, int>&, std::string);

template
bool kit::hasKey(std::map<std::string, float>&, std::string);

template
bool kit::hasKey(std::map<std::string, double>&, std::string);

template
bool kit::hasKey(std::map<std::string, std::vector<int> >&, std::string);

template
bool kit::hasKey(std::map<std::string, std::vector<float> >&, std::string);

template
bool kit::hasKey(std::map<std::string, std::vector<double> >&, std::string);
//-------------------------------------------------------------------------
/*
template 
bool EventStream::select(double&, std::string);

template 
bool EventStream::select(long&, std::string);

template 
bool EventStream::select(int&, std::string);

template 
bool EventStream::select(std::vector<double>&, std::string);

template 
bool EventStream::select(std::vector<float>*&, std::string);

template 
bool EventStream::select(std::vector<long>&, std::string);

template 
bool EventStream::select(std::vector<int>&, std::string);

//-------------------------------------------------------------------------

template 
bool EventStream::select(std::vector<CaloTower>&, std::string);

template 
bool EventStream::select(std::vector<reco::BasicCluster>&, std::string);

template 
bool EventStream::select(std::vector<reco::SuperCluster>&, std::string);

template 
bool EventStream::select(std::vector<reco::PreshowerCluster>&, 
                         std::string);

template 
bool EventStream::select(std::vector<reco::Track>&, std::string);

template 
bool EventStream::select(std::vector<reco::JetTag>&, std::string);

template 
bool EventStream::select(std::vector<reco::CaloJet>&, std::string);

template
bool EventStream::select(std::vector<reco::Photon>&, std::string);
   
template
bool EventStream::select(std::vector<reco::MET>&, std::string); 

template 
bool EventStream::select(std::vector<reco::GsfElectron>&, std::string);

template
bool EventStream::select(std::vector<reco::Muon>&, std::string);

template 
bool EventStream::select(std::vector<reco::GenParticle>&, std::string="");

template 
bool EventStream::select(std::vector<reco::GenJet>&, std::string="");

template 
bool EventStream::select(std::vector<reco::GenMET>&, std::string="");

//-------------------------------------------------------------------------

template 
bool EventStream::select(edm::EventAuxiliary&, std::string="");

template 
bool EventStream::select(edm::TriggerResults&, std::string="");
    
//-------------------------------------------------------------------------
// PAT
//-------------------------------------------------------------------------

template 
bool EventStream::select(std::vector<pat::Electron>&, std::string="");

template 
bool EventStream::select(std::vector<pat::Muon>&, std::string="");

template 
bool EventStream::select(std::vector<pat::Tau>&, std::string="");

template 
bool EventStream::select(std::vector<pat::Jet>&, std::string="");

template 
bool EventStream::select(std::vector<pat::Photon>&, std::string="");

template 
bool EventStream::select(std::vector<pat::Hemisphere>&, std::string="");

template 
bool EventStream::select(std::vector<pat::MET>&, std::string="");

*/
