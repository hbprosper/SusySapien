// -*- C++ -*-
//
// Description: Test FunctionMember, a method calling system based on Reflex.
//
// Original Author:  Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
//         Updated:  Sun Jan 17 HBP - add log file
// $Id: TestMethod.cc,v 1.3 2010/10/11 02:15:31 prosper Exp $
// $Revision: 1.3 $
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
 
#include "PhysicsTools/TheNtupleMaker/interface/Method.h"
#include "PhysicsTools/TheNtupleMaker/interface/MethodT.h"
#include "PhysicsTools/TheNtupleMaker/interface/CurrentEvent.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Event.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include "TROOT.h"

using namespace std;
using namespace edm;
using namespace reco;

class TestMethod : public edm::EDAnalyzer 
{
public:
  explicit TestMethod(const edm::ParameterSet&);
  ~TestMethod();

private:
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  std::string label;
  const edm::ParameterSet config;
  int count;
};


TestMethod::TestMethod(const edm::ParameterSet& iConfig) 
  : config(iConfig)
{}

TestMethod::~TestMethod() {}

void printit(string object, int i)
{
  cout << object << "[" << i << "]" << endl;
}

void printit(string method, double x, double y, double z)
{
  char record[1024];
  sprintf(record, " %-40s: %f %f %f", method.c_str(), x, y, z);
  cout << record << endl;
}

void
TestMethod::analyze(const edm::Event& iEvent, 
                    const edm::EventSetup& iSetup)
{
  double x = 0;
  double y = 0;
  double z = 0;
  string method("");

  CurrentEvent::instance().set(iEvent, iSetup);

  { // Electrons

    label = config.getUntrackedParameter<string>("patElectronLabel");

    edm::Handle<edm::View<pat::Electron> > handle;
    iEvent.getByLabel(label, handle);
    if ( !handle.isValid() )
      throw edm::Exception(edm::errors::Configuration,
                           "getByLabel failed on label: " + label);
    
    if ( handle->size() > 0 )
      {
        for(unsigned int i=0; i < handle->size(); i++)
          {      
            printit("pat::Electron", i);
            const pat::Electron& object = (*handle)[i];

            {
              x = y = z = 0;
              x = object.pt();
              method = string("pt()");
              Method<pat::Electron> f(method);
              y = f(object);
              MethodT<pat::Electron> g(method);
              z = g(object);
              printit(method, x, y, z);
            }

            {
              x = y = z = 0;
              x = object.eta();
              method = string("eta()");
              Method<pat::Electron> f(method);
              y = f(object);
              MethodT<pat::Electron> g(method);
              z = g(object);
              printit(method, x, y, z);
            }

            {
              x = y = z = 0;
              x = object.phi();
              method = string("phi()");
              Method<pat::Electron> f(method);
              y = f(object);
              MethodT<pat::Electron> g(method);
              z = g(object);
              printit(method, x, y, z);
            }

            {
              x = y = z = 0;
              x  = object.trackIso();
              method = string("trackIso()");
              Method<pat::Electron> f(method);
              y = f(object);
              MethodT<pat::Electron> g(method);
              z = g(object);
              printit(method, x, y, z);
            }
            
            {
              x = y = z = 0;
              if ( object.gsfTrack().isAvailable() &&
                   object.gsfTrack().isNonnull() ) 
                x  = object.gsfTrack()->numberOfValidHits();
              method = string("gsfTrack()->numberOfValidHits()");
              Method<pat::Electron> f(method);
              y = f(object);
              MethodT<pat::Electron> g(method);
              z = g(object);
              printit(method, x, y, z);
            }

            {
              x = y = z = 0;
              if ( object.track().isAvailable() &&
                   object.track().isNonnull() )
                x  = object.track()->d0();
              method = string("track()->d0()");
              Method<pat::Electron> f(method);
              y = f(object);
              MethodT<pat::Electron> g(method);
              z = g(object);
              printit(method, x, y, z);
            }

          }
      }
  }


  { // Muons

    label = config.getUntrackedParameter<string>("patMuonLabel");

    edm::Handle<edm::View<pat::Muon> > handle;
    iEvent.getByLabel(label, handle);
    if ( !handle.isValid() )
      throw edm::Exception(edm::errors::Configuration,
                           "getByLabel failed on label: " + label);

    if ( handle->size() > 0 )
      {
        for(unsigned int i=0; i < handle->size(); i++)
          {      
            printit("pat::Muon", i);
            const pat::Muon& object = (*handle)[i];
                        
            {
              x = y = z = 0;
              x = object.pt();
              method = string("pt()");
              Method<pat::Muon> f(method);
              y = f(object);
              MethodT<pat::Muon> g(method);
              z = g(object);
              printit(method, x, y, z);
             }

            {
              x = y = z = 0;
              x = object.eta();
              method = string("eta()");
              Method<pat::Muon> f(method);
              y = f(object);
              MethodT<pat::Muon> g(method);
              z = g(object);
              printit(method, x, y, z);
            }

            {
              x = y = z = 0;
              x = object.phi();
              method = string("phi()");
              Method<pat::Muon> f(method);
              y = f(object);
              MethodT<pat::Muon> g(method);
              z = g(object);
              printit(method, x, y, z);
            }

            {
              x = y = z = 0;
              x = object.trackIso();
              method = string("trackIso()");
              Method<pat::Muon> f(method);
              y = f(object);
              MethodT<pat::Muon> g(method);
              z = g(object);
              printit(method, x, y, z);
            }

            {
              x = y = z = 0;
              x = object.isGlobalMuon();
              method = string("isGlobalMuon()");
              Method<pat::Muon> f(method);
              y = f(object);
              MethodT<pat::Muon> g(method);
              z = g(object);
              printit(method, x, y, z);
            }
               
            {
              x = y = z = 0;
              if ( object.track().isAvailable() &&
                   object.track().isNonnull() ) 
                x  = object.track()->hitPattern().numberOfValidMuonHits();
              method = string("track()->hitPattern().numberOfValidMuonHits()");
              Method<pat::Muon> f(method);
              y = f(object);
              MethodT<pat::Muon> g(method);
              z = g(object);
              printit(method, x, y, z);
            }

            {
              x = y = z = 0;
              if ( object.track().isAvailable() &&
                   object.track().isNonnull() )
                x  = object.track()->d0();
              method = string("track()->d0()");
              Method<pat::Muon> f(method);
              y = f(object);
              MethodT<pat::Muon> g(method);
              z = g(object);
              printit(method, x, y, z);
            }

          }
       }
  }

  { // Jets

    label = config.getUntrackedParameter<string>("patJetLabel");

    edm::Handle<edm::View<pat::Jet> > handle;
    iEvent.getByLabel(label, handle);
    if ( !handle.isValid() )
      throw edm::Exception(edm::errors::Configuration,
                           "getByLabel failed on label: " + label);

    if ( handle->size() > 0 )
      {
        for(unsigned int i=0; i < handle->size(); i++)
          {      
            printit("pat::Jet", i);
            const pat::Jet& object = (*handle)[i];

            {
              x = y = z = 0;
              x = object.pt();
              method = string("pt()");
              Method<pat::Jet> f(method);
              y = f(object);
              MethodT<pat::Jet> g(method);
              z = g(object);
              printit(method, x, y, z);
            }

            {
              x = y = z = 0;
              x = object.eta();
              method = string("eta()");
              Method<pat::Jet> f(method);
              y = f(object);
              MethodT<pat::Jet> g(method);
              z = g(object);
              printit(method, x, y, z);
            }

            {
              x = y = z = 0;
              x = object.phi();
              method = string("phi()");
              Method<pat::Jet> f(method);
              y = f(object);
              MethodT<pat::Jet> g(method);
              z = g(object);
              printit(method, x, y, z);
            }

            {
              x = y = z = 0;
              x = object.jetID().fHPD;
              method = string("jetID().fHPD");
              Method<pat::Jet> f(method);
              y = f(object);
              MethodT<pat::Jet> g(method);
              z = g(object);
              printit(method, x, y, z);
            }

            {
              x = y = z = 0;
              x = object.jetID().n90Hits;
              method = string("jetID().n90Hits");
              Method<pat::Jet> f(method);
              y = f(object);
              MethodT<pat::Jet> g(method);
              z = g(object);
              printit(method, x, y, z);
            }

            {
              string b("combinedSecondaryVertexBJetTags");
              x = y = z = 0;
              x = object.bDiscriminator(b);
              method = string("bDiscriminator(\"") + b + string("\")");
              Method<pat::Jet> f(method);
              y = f(object);
              MethodT<pat::Jet> g(method);
              z = g(object);
              printit(method, x, y, z);
            }
            
          }
      }
  }
}


// --- method called once each job just before starting event loop  -----------
void 
TestMethod::beginJob() {}

// --- method called once each job just after ending the event loop  ----------
void 
TestMethod::endJob() {}

//define this as a plug-in
DEFINE_FWK_MODULE(TestMethod);
