// -*- C++ -*-
//
// Original Author:  Sezen SEKMEN & Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
//         Updated:  Sun Jan 17 HBP - add log file
// $Id: TestBed.cc,v 1.3 2010/02/19 05:09:41 prosper Exp $
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
#include "TROOT.h"

using namespace std;

class TestBed : public edm::EDAnalyzer 
{
public:
  explicit TestBed(const edm::ParameterSet&);
  ~TestBed();


private:
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();
};


TestBed::TestBed(const edm::ParameterSet& iConfig)
{
  cout << GREEN << "BEGIN TestBed" << BLACK << endl;
  //gROOT->ProcessLine(".> /dev/null");
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
  edm::Handle< edm::View<reco::GenParticle> > handle;
  iEvent.getByLabel("genParticles", handle);
  if ( !handle.isValid() )
    throw edm::Exception(edm::errors::Configuration,
                         "\nBuffer - " + 
                         RED +
                         "getByLabel failed" +
                         BLACK);

  counter++;

  if ( handle->size() < 1 ) return;


  map<string, vector<int> > particleMap;

  int nhep = handle->size();

  for (int i=0; i < nhep; i++)
    {
      const reco::GenParticle& p = ((*handle)[i]);
      char key[512];
      sprintf(key, "%-16s %d %10.3e %10.3e %10.3e %10.3e",
              kit::particleName(p.pdgId()).c_str(), 
              p.status(), 
              p.energy(),  p.px(), p.py(), p.pz());
      particleMap[key] = vector<int>();
    }

  for (int i=0; i < nhep; i++)
    {
      const reco::GenParticle& p = ((*handle)[i]);
      if ( p.numberOfMothers() < 1 ) continue;

      char key[512];
      sprintf(key, "%-16s %d %10.3e %10.3e %10.3e %10.3e",
              kit::particleName(p.mother(0)->pdgId()).c_str(), 
              p.mother(0)->status(), 
              p.mother(0)->energy(),  
              p.mother(0)->px(),
              p.mother(0)->py(),
              p.mother(0)->pz());

      if ( particleMap.find(key) == particleMap.end() )
        {
          cout << " ** error ** can't find key: " << key << endl;
          exit(0);
        }
      particleMap[key].push_back(i);
    }

  for (int i=0; i < nhep; i++)
    {
      const reco::GenParticle& p = ((*handle)[i]);
      char key[512];
      sprintf(key, "%-16s %d %10.3e %10.3e %10.3e %10.3e",
              kit::particleName(p.pdgId()).c_str(), 
              p.status(), 
              p.energy(),  p.px(), p.py(), p.pz());

      if ( particleMap.find(key) == particleMap.end() )
        {
          cout << " ** error ** can't find key: " << key << endl;
          continue;
        }

      string name = kit::particleName(p.pdgId());
      char record[512];
      sprintf(record, "%4d %-16s\t%d\t%d",i,name.c_str(),
              particleMap[key].size(), p.status());
      cout << record << endl;

      for(unsigned j=0; j < particleMap[key].size(); j++)
        {
          int k = particleMap[key][j];
          const reco::GenParticle& q = ((*handle)[k]);
          string name = kit::particleName(q.pdgId());
          sprintf(record, "\t%4d %-16s\t%d", k, name.c_str(), p.status());
          cout << record << endl;
        }
    }

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
