//-----------------------------------------------------------------------------
// File: effStudy.cc
// Description: Compare generated and reconstructed electrons in the
//              region |eta| < 1.48 (the CMS barrel region). This code
//              assumes that the link zee.root pointing to an actual
//              Z -> ee file exists.
//
// Created: 24-Jun-2007 Harrison B. Prosper
// Updated: 01-Nov-2007 HBP
// $Revision:$
//-----------------------------------------------------------------------------
#include "PhysicsTools/LiteAnalysis/interface/EventStream.h"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
#include <string>
#include <vector>
#include <fstream>
//-----------------------------------------------------------------------------
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
//-----------------------------------------------------------------------------
#include <TROOT.h>
#include <TSystem.h>
#include <TLorentzVector.h>
#include <TApplication.h>
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------
string FILENAME("zee.root");
float MATCHCUT = 0.05;
float JETETCUT = 10.0;
float ETACUT   = 1.48;
int SECONDS    = 10000;
//-----------------------------------------------------------------------------
void fatal(string message)
{
  cout << "** error ** " << message << endl;
  exit(0);
}
//-----------------------------------------------------------------------------
// Selectors and mappers
//-----------------------------------------------------------------------------
void
filterCaloJets(vector<reco::CaloJet>& p,
	       vector<reco::CaloJet>& q)
{
  for(unsigned i=0; i < p.size(); i++)
    {
      if ( (p[i].et() > JETETCUT) && (fabs(p[i].eta()) < ETACUT) )
	q.push_back(p[i]);
    }
}

void
filterGenElectrons(vector<reco::GenParticle*>& p,
		   vector<reco::GenParticle*>& q)
{
  for(unsigned i=0; i < p.size(); i++)
    {
      int id  = (int)fabs(p[i]->pdgId());
      float eta = fabs(p[i]->eta());
      if ( (id == kit::ELECTRON) && (eta < ETACUT) )
	q.push_back(p[i]);
    }
}

void
mapGenParticles(vector<reco::GenParticle*>& p,
		vector<TLorentzVector>& v)
{
  for(unsigned i=0; i < p.size(); i++) v.push_back(kit::lorentzVector(p[i]));
}

void
mapRecoElectrons(vector<reco::GsfElectron>& p,
		 vector<TLorentzVector>& v)
{
  for(unsigned i=0; i < p.size(); i++)
    {
      math::XYZVector q = p[i].trackMomentumAtVtx();
      v.push_back(kit::lorentzVector( q ));
    }
}

void 
fillPt(TH1F* h1, TH1F* h2, 
       vector<TLorentzVector>& p, 
       vector<kit::MatchedPair>& matches)
{
  for (unsigned i=0; i < p.size(); i++) 
    {
      h1->Fill(p[i].Pt());        
      if (matches[i].distance < MATCHCUT) h2->Fill(p[i].Pt());
    }
}
//-----------------------------------------------------------------------------
int 
main(int argc, char** argv)
{
  AutoLibraryLoader::enable();

  TApplication app("App", &argc, argv);

  cout << "\n\tElectron Efficiency Study" << endl;

  //--------------------------------------
  // Open file or chain of CMS root files
  //--------------------------------------
  cout << "\n\topening file: " << FILENAME << endl;
  EventStream stream(FILENAME);
  if (!stream.good()) fatal("unable to open file: " + FILENAME);
  cout << "Number of events: " << stream.size() << endl << endl;

  //--------------------------------------
  // Select objects to be accessed
  //--------------------------------------
  vector<reco::GenParticle> particles;
  if (!stream.select(particles)) fatal("GenParticle select failed");
        
  vector<reco::GsfElectron> electrons;
  if (!stream.select(electrons)) fatal("electron select failed");

  vector<reco::CaloJet> jets;
  if (!stream.select(jets, "iterative")) fatal("jet select failed");
  
  //--------------------------------------
  // Setup canvases etc.
  //--------------------------------------
  kit::setStyle();
    
  TCanvas* cdeltar= kit::canvas("deltaR",1);
  TH1F* hdeltar   = kit::histogram("hdeltaR","min //DeltaR","",50,0,0.05);

  TCanvas* czmass = kit::canvas("zmass",2);
  TH1F* hzmass    = kit::histogram("hzmass", "m_{Z} [GeV/c^{2}]","",50,0,200);
  TH1F* hzmassr   = kit::histogram("hzmassr","m_{Z} [GeV/c^{2}]","",50,0,200, 
				   kBlue);

  TCanvas* cpte   = kit::canvas("ept",3);
  TH1F* hpte1     = kit::histogram("hpte1","p_{T}(e) [GeV/c]","",20,0,200);
  TH1F* hpte2     = kit::histogram("hpte2","p_{T}(e) [GeV/c]","",20,0,200);

  TCanvas* cptjet = kit::canvas("jetet",5);
  TH1F* hptjet    = kit::histogram("hptjet","p_{T}(jet) [GeV/c]","",40,0,400);

  TCanvas* cnjet  = kit::canvas("njet",6);
  TH1F* hnjet     = kit::histogram("hnjet","n_{jet}","",20,0,20);

  TCanvas* czpt   = kit::canvas("zpt",7);
  TH1F* hzpt      = kit::histogram("hzpt","p_{T}(Z)","",40,0,400);
        
  //--------------------------------------
  // Loop over events
  //--------------------------------------

  int count = 0;
  while (stream.read())
    {
      // Get Z boson

      int iz = kit::find(particles, kit::ZBOSON);
      if ( iz < 0 ) continue;
      reco::GenParticle& Zboson = particles[iz];

      // Get Z daughters

      vector<reco::GenParticle*> Zd = kit::daughters(Zboson);

      // Select events with both electrons in barrel

      vector<reco::GenParticle*> d;
      filterGenElectrons(Zd, d);
      if ( d.size() == 0 ) continue;
      count++;

      // Select jets

      vector<reco::CaloJet> jet;
      filterCaloJets(jets, jet);

      // Map objects to Lorentz vector

      TLorentzVector Z  = kit::lorentzVector(Zboson);

      vector<TLorentzVector> l;
      mapGenParticles(d, l);

      vector<TLorentzVector> L;
      mapRecoElectrons(electrons, L);

      // Match between truth and reco

      vector<kit::MatchedPair> matches = kit::deltaR(l, L);

      float LLmass=-1;
      if (L.size() > 1)
        {
          TLorentzVector LL = L[0] + L[1];
          LLmass = LL.M();
        }

      //--------------------------------------
      // Fill histograms
      //--------------------------------------
      hzmass->Fill(Z.M());

      hzpt->Fill(Z.Pt());

      if (LLmass > 0) hzmassr->Fill(LLmass);
        
      hnjet->Fill( jet.size() );

      for (unsigned i=0; i < jet.size(); i++) hptjet->Fill(jet[i].et());

      for (unsigned i=0; i < matches.size(); i++) 
        hdeltar->Fill( matches[i].distance ); 

      // Fill pts for barrel leptons

      fillPt(hpte1, hpte2, l, matches);

      //--------------------------------------
      // Plot histograms
      //--------------------------------------
      if (count % 50 == 0)
	{
	  cout << count << endl;

	  kit::plot(cdeltar, hdeltar);
	  kit::plot(czmass, hzmass);
	  kit::plot(czmass, hzmass);
	  kit::plot(cpte, hpte1);
	  kit::plot(cptjet, hptjet);
	  kit::plot(cnjet, hnjet);
	  kit::plot(czpt, hzpt);
	}
    }

  // Plot everything once more

  kit::plot(cdeltar, hdeltar, "S");
  kit::plot(czmass, hzmass, "S");
  kit::plot(cpte, hpte1, "S");
  kit::plot(cptjet, hptjet, "S");
  kit::plot(cnjet, hnjet, "S");
  kit::plot(czpt, hzpt, "S");

  // Construct efficiency histograms

  TCanvas* ceffe = kit::canvas("eeff", 8);
  TH1F* heffe = kit::divideHistograms(hpte2, hpte1, "Efficiency");
  heffe->SetMinimum(0);
  heffe->SetMaximum(1);
  kit::plot(ceffe, heffe, "ES");

  gSystem->Sleep(SECONDS);
}



