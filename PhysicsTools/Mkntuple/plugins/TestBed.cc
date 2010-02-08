// -*- C++ -*-
//
// Original Author:  Sezen SEKMEN & Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
//         Updated:  Sun Jan 17 HBP - add log file
// $Id: Mkntuple.cc,v 1.3 2010/01/20 04:57:23 prosper Exp $
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
  edm::Handle< edm::View<pat::Muon> > handle;
  iEvent.getByLabel("cleanLayer1Muons", handle);
  if ( !handle.isValid() )
    throw edm::Exception(edm::errors::Configuration,
                         "\nBuffer - " + 
                         RED +
                         "getByLabel failed" +
                         BLACK);

  counter++;

  if ( handle->size() < 1 ) return;

  Method<pat::Muon> method("track()->pt()");

  const pat::Muon muon = (*handle)[0];

  //bool isnull = false;
  gROOT->ProcessLine(Form("pat::Muon* o = (pat::Muon*)0x%x", &muon));

 //  reco::GsfTrackRef gsftrack = muon->gsfTrack();
//   bool isnull = (bool)gROOT->ProcessLineFast("o->gsfTrack().isNull()");
//   //cout << "gstTrack().isNull() = " << RED << isnull << BLACK << endl;
//   if ( ! isnull )
//     {
//       double chi2 = gsftrack->chi2();
//       cout << "\tchi2: " << chi2 << endl;
//     }

  reco::TrackRef track = muon.track();
  bool isnull = (bool)gROOT->ProcessLineFast("o->track().isNull()");
  //cout << "track().isNull() = " << RED << isnull << BLACK << endl;
  if ( ! isnull )
    {
      double pt1 = 0;
      gROOT->ProcessLine(Form("double* x = (double*)0x%x", &pt1));
      gROOT->ProcessLineFast("kit::set(o->track()->pt(),x)");
      int charge = muon.charge();
      double pt  = track->pt();
      double pt2 = method(muon);

      cout << counter 
           << "  charge[0]: " << charge 
           << "\tpt[0]:  " << pt 
           << "\tpt1[0]: " << pt1
           << "\tpt2[0]: " << pt2 
           << endl;
    }

//   void* addr = (void*)gROOT->ProcessLineFast("o->gsfTrack().get()");
//   cout << "gstTrack().get() = " 
//        << RED 
//        << addr 
//        << BLACK
//        << endl;

//   addr = (void*)gROOT->ProcessLineFast("o->track().get()");
//   reco::Track* trk = static_cast<reco::Track*>(addr);
//   cout << "track().get() = " 
//        << RED 
//        << addr 
//        << BLACK << " " << trk->pt() 
//        << endl;
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
