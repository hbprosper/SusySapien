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
#include "FWCore/Framework/interface/Event.h"
#include "TROOT.h"

using namespace std;

class TestMe : public edm::EDAnalyzer 
{
public:
  explicit TestMe(const edm::ParameterSet&);
  ~TestMe();


private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
};


TestMe::TestMe(const edm::ParameterSet& iConfig) {}


TestMe::~TestMe() {}


//
// member functions
//

static int counter=-1;

// ------------ method called to for each event  ------------
void
TestMe::analyze(const edm::Event& iEvent, 
                const edm::EventSetup& iSetup)
{
  edm::Handle<edm::TriggerResults> handle;
  iEvent.getByLabel("TriggerResults", handle);
  if ( !handle.isValid() )
    throw edm::Exception(edm::errors::Configuration,
                         "getByLabel failed on TriggerResults");

  counter++;

  // NB: use a reference to avoid expensive copying
  const edm::TriggerNames& tNames = iEvent.triggerNames(*handle);
  
  // Get bit associated with trigger name
  string tname("HLT_Jet80");
  unsigned int bit = tNames.triggerIndex(tname);
  cout << counter << "\t" << tname << " = " << handle->accept(bit) << endl;
}

// --- method called once each job just before starting event loop  -----------
void 
TestMe::beginJob(const edm::EventSetup&) {}

// --- method called once each job just after ending the event loop  ----------
void 
TestMe::endJob() {}

//define this as a plug-in
DEFINE_FWK_MODULE(TestMe);
