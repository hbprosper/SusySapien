//-----------------------------------------------------------------------------
// File: testAOD.cc
// Created: 24-Jun-2007 Harrison B. Prosper
// Updated: 01-Nov-2007 HBP
//-----------------------------------------------------------------------------
#include "PhysicsTools/LiteAnalysis/interface/EventStream.h"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
#include <stdio.h>
//-----------------------------------------------------------------------------
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
//-----------------------------------------------------------------------------
#include <TROOT.h>
#include <TSystem.h>
#include <TLorentzVector.h>
#include <TApplication.h>
//-----------------------------------------------------------------------------
#include <string>
#include <vector>
#include <fstream>
using namespace std;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void fatal(string message)
{
  cout << "** error ** " << message << endl;
  exit(0);
}
//-----------------------------------------------------------------------------
int 
main(int argc, char** argv)
{
  cout << "\n\ttestAOD\n" << endl;

  AutoLibraryLoader::enable();
 
  string filename("zee.root");

  EventStream stream(filename);
  if ( !stream.good() ) fatal("unable to open file");
    
  int nevent = stream.size();
  cout << "\tnumber of events: " << nevent << endl;

  /*
  vector<reco::GsfElectron> electrons;    
  stream.select(electrons);
  if ( !stream.good() ) fatal("problem getting branclsh");
  */
  vector<reco::CaloJet> jets;
  stream.select(jets, "iterative");
  if ( !stream.good() ) fatal("problem getting branch");
    
  vector<reco::GenParticle> genparticles;    
  stream.select(genparticles);
  if ( !stream.good() ) fatal("problem getting branch");
    
  char record[80];
  sprintf(record, "%4s\t%10s%10s%10s", "Row", "Npart", "Nele", "Njet");
  cout << record << endl;

  for(int row=0; row < 10; row++)
    {
      stream.read(row);

      sprintf(record, "%4d\t%10d%10d%10d", 
              row, 
              genparticles.size(), 0,
              //electrons.size(), 
              jets.size());
      cout << record << endl;
    }

  //cout << endl << kit::table(genparticles) << endl;
}




