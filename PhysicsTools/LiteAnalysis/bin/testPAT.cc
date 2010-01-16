//-----------------------------------------------------------------------------
// File: testPAT.cc
// Created: 24-Jun-2007 Harrison B. Prosper
// Updated: 01-Nov-2007 HBP
//-----------------------------------------------------------------------------
#include "PhysicsTools/LiteAnalysis/interface/EventStream.h"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
#include <stdio.h>
//-----------------------------------------------------------------------------
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "HepMC/GenParticle.h"
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
  cout << "\n\ttestPAT\n" << endl;

  AutoLibraryLoader::enable();

  string filename("ZEE_PAT.root");

  EventStream stream(filename);
  if ( !stream.good() ) fatal("unable to open file");
    
  int nevent = stream.size();
  cout << "\tnumber of events: " << nevent << endl;

  vector<pat::Jet> jets;    
  stream.select(jets);
  if ( !stream.good() ) fatal("problem getting branch");
    
  char record[80];
  sprintf(record, "%4s\t%4s", "Row", "Njet");
  cout << record << endl;

  for(int row=0; row <  10; row++)
    {
      stream.read(row);
      sprintf(record, "%4d\t%4d", row, jets.size());
      cout << record << endl;
    }
}




