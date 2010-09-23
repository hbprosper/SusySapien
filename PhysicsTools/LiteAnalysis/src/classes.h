//-----------------------------------------------------------------------------
#include "PhysicsTools/LiteAnalysis/interface/Connection.hpp"
#include "PhysicsTools/LiteAnalysis/interface/Slot.hpp"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
#include "PhysicsTools/LiteAnalysis/interface/pdg.h"
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
#include "PhysicsTools/LiteAnalysis/interface/FunctionMember.h"
#include "PhysicsTools/LiteAnalysis/interface/ClassMembers.h"
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
    std::vector<TH1*> c51;
    std::vector<kit::PtThing>    c6;
    std::map<std::string, std::vector<float>* > c7;
    std::map<std::string, std::vector<float> >  c8;
    std::map<std::string, std::vector<double> > c9;
    std::vector<bool>    c10;
    Atemp<Bclass> c11;
  }
}

//-----------------------------------------------------------------------------
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

