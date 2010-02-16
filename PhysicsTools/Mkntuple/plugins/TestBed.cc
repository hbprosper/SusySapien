// -*- C++ -*-
//
// Original Author:  Sezen SEKMEN & Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
//         Updated:  Sun Jan 17 HBP - add log file
// $Id: TestBed.cc,v 1.1 2010/02/08 03:21:10 prosper Exp $
//
//
// ---------------------------------------------------------------------------
#include <boost/regex.hpp>
#include <memory>
#include <iostream>
#include <fstream>
#include <cassert>
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
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
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

  const reco::GenParticle& p = ((*handle)[0]);
  int n = p.numberOfMothers();
  cout << "Number of mothers: " << n << endl;
  if ( p.motherRef(0).get() )
    {
      cout << " found mother" << endl;
    }
}

// --- method called once each job just before starting event loop  -----------
void 
TestBed::beginJob(const edm::EventSetup&)
{
}

// --- method called once each job just after ending the event loop  ----------
void 
TestBed::endJob() 
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(TestBed);
