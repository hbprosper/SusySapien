//-----------------------------------------------------------------------------
// File: readntuple.cc
//       Example of how to read ntuples created by Mkntuple
// Created: 14-Feb-2010 Harrison B. Prosper
//$Revision:$
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
#include "PhysicsTools/LiteAnalysis/interface/treestream.hpp"
#include "TApplication.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TH1F.h"
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
const int MAXTRACK=500;
const int MAXMUON=50;
const int MAXJET=50;
void error(string message)
{
  cout << "** error ** " << message << endl;
  exit(0);
}
//-----------------------------------------------------------------------------
void readntuple(string filename="ntuple.root")
{
  cout << "enabling autoloader..." << endl;
  AutoLibraryLoader::enable();
  cout << "done!" << endl;

  // Open file
  
  itreestream stream(filename, "Events");
  if ( !stream.good() )error("unable to read " + filename);
      
  // Define variables to be read in
      
  vector<int>   mucharge(MAXMUON, 0);
  vector<float> mupt(MAXMUON, 0);
  vector<float> mueta(MAXMUON, 0);
  vector<float> muphi(MAXMUON, 0);
  vector<int>   isStandAlone(MAXMUON, 0);

  // Note: we can use wildcards
  
  stream.select("*Muon_muons.charge", mucharge);
  stream.select("*Muon_muons.pt",  mupt);
  stream.select("*Muon_muons.eta", mueta);
  stream.select("*Muon_muons.phi", muphi);
  stream.select("*Muon_muons.isStand*", isStandAlone);

  // Book a few histograms

  kit::setStyle();

  TCanvas* cch  = kit::canvas("ch",  0, 500, 500);
  TH1F*    hch  = kit::histogram("hch", "charge", "", 40, -2, 2);

  TCanvas* cpt  = kit::canvas("pt",  4, 500, 500);
  TH1F*    hpt  = kit::histogram("hpt", "p_{T} [GeV/c]", "", 50, 0, 100);

  TCanvas* ceta = kit::canvas("eta", 8, 500, 500);
  TH1F*    heta = kit::histogram("heta", "#eta", "", 50, -2.5, 2.5);

  TCanvas* cphi = kit::canvas("phi", 12, 500, 500);
  TH1F*    hphi = kit::histogram("hphi", "#phi", "", 50, -5, 5);

  TCanvas* cn   = kit::canvas("n",   16, 500, 500);
  TH1F*    hn   = kit::histogram("hn", "muon multiplicity", "", 10, 0, 10);

  // Loop over events

  int nentries = stream.entries();

  cout << "Number of entries: " << nentries << endl;

  int count = 0;
  for(int entry=0; entry < nentries; entry++)
    {
      stream.read(entry);

      int nmu = mupt.size();
      if ( nmu < 1 ) continue;
      count++;

      // Fill histograms

      int n = 0;
      for(int i=0; i < nmu; i++)
        {
          if ( isStandAlone[i] < 1 ) continue;
          n++;
          hch->Fill(mucharge[i]);
          hpt->Fill(mupt[i]);
          heta->Fill(mueta[i]);
          hphi->Fill(muphi[i]);
        }
      hn->Fill(n);

      if ( count % 10 == 0 )
        {
          kit::plot(cch, hch);
          kit::plot(cpt, hpt);
          kit::plot(ceta, heta);
          kit::plot(cphi, hphi);
          kit::plot(cn, hn);
        }
    }
  stream.close();

  kit::plot(cch, hch, "s");
  kit::plot(cpt, hpt, "s");
  kit::plot(ceta, heta, "s");
  kit::plot(cphi, hphi, "s");
  kit::plot(cn, hn, "s");

}
//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  string filename("ntuple.root");
  if ( argc > 1 ) filename = string(argv[1]);

  TApplication app("app", &argc, argv);

  readntuple(filename);

  app.Run();
  return 0;
}

