// -*- C++ -*-
//
// Original Author:  Sezen SEKMEN & Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
//         Updated:  Sun Jan 17 HBP - add log file
// $Id: TestBed.cc,v 1.4 2010/03/10 15:09:32 prosper Exp $
//
//
// ---------------------------------------------------------------------------
#include <boost/regex.hpp>
#include <memory>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>
#include <map>
#include <time.h>
 
#include "PhysicsTools/LiteAnalysis/interface/treestream.hpp"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
#include "PhysicsTools/LiteAnalysis/interface/Method.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "TROOT.h"
#include "TSystem.h"

using namespace std;
using namespace reco;

class TestBed : public edm::EDAnalyzer 
{
public:
  explicit TestBed(const edm::ParameterSet&);
  ~TestBed();


private:
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  otreestream output;
  std::vector<double> pt;
  int nhep;

};


TestBed::TestBed(const edm::ParameterSet& iConfig)
  : output(otreestream("testbed.root", "Events", "test")),
    pt(std::vector<double>(4000,0))
{
  cout << GREEN << "BEGIN TestBed" << BLACK << endl;
  gSystem->Load("selector_C.so");
  output.add(string("nhep"), nhep);
  output.add(string("pt[nhep]"), pt);
}


TestBed::~TestBed()
{
}


//
// member functions
//
 
static int counter=-1;

// ------------ method called to for each event  ------------
void
TestBed::analyze(const edm::Event& iEvent, 
                 const edm::EventSetup& iSetup)
{

  counter++;
  cout << "Event count: " << counter << endl;

  // Get genparticles:
  edm::Handle<GenParticleCollection> genParticles;
  iEvent.getByLabel("genParticles", genParticles);
  
  nhep = genParticles->size();
  for(int i = 0; i < nhep; i++) 
    {
      const GenParticle* p = &((*genParticles)[i]);
      pt[i] = sqrt(p->px()*p->px()+p->py()*p->py());
    }

  cout << "\tnhep  = " << nhep  << endl;
  cout << "\tpt[3] = " << pt[3] << endl;

  output.store();
  gROOT->ProcessLine("gROOT->Reset(); selector()");
  output.save();
}

// --- method called once each job just before starting event loop  -----------
void 
TestBed::beginJob()
{
}

// --- method called once each job just after ending the event loop  ----------
void 
TestBed::endJob() 
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(TestBed);
