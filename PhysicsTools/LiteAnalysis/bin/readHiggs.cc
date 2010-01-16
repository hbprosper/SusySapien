//-----------------------------------------------------------------------------
// File: readHiggs.cc
// Created: 16-Sep-2009 Harrison B. Prosper
//-----------------------------------------------------------------------------
#include "PhysicsTools/LiteAnalysis/interface/EventStream.h"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
#include <stdio.h>
//-----------------------------------------------------------------------------
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
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
  cout << "\n\treadHiggs\n" << endl;

  AutoLibraryLoader::enable();

  string filename("higgs.root");

  EventStream stream(filename);
  if ( !stream.good() ) fatal("unable to open file");
    
  int nevent = stream.size();
  cout << "\tnumber of events: " << nevent << endl;

  // Select branches to be read

  vector<pat::Electron> electrons;    
  stream.select(electrons);
  if ( !stream.good() ) fatal("problem getting Electrons");

  vector<pat::Muon> muons;    
  stream.select(muons);
  if ( !stream.good() ) fatal("problem getting Muons");

  vector<pat::Jet> jets;    
  stream.select(jets);
  if ( !stream.good() ) fatal("problem getting Jets");

  vector<pat::MET> mets;    
  stream.select(mets);
  if ( !stream.good() ) fatal("problem getting MET");
    
  char record[80];
  sprintf(record, "%4s\t%4s\t%4s\t%4s", "Row", "Njet", "Nele", "Nmu");
  cout << record << endl;

  // Loop over first 100 events
  int count = 0;
  while ( stream.read() )
    {
      sprintf(record, "%4d\t%4d\t%4d\t%4d", count, 
              jets.size(), electrons.size(), muons.size());
      cout << record << endl;

      count++;
      if ( count >= 100 ) break;
    }
}




