//-----------------------------------------------------------------------------
// File: testreflex.cc
// Created: 03-Nov-2009 Harrison B. Prosper
//-----------------------------------------------------------------------------
#include <boost/regex.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "TROOT.h"
#include "TString.h"
#include "TSystem.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/LiteAnalysis/interface/ClassMembers.h"
//-----------------------------------------------------------------------------
using namespace std;

void fatal(string message)
{
  cout << "** error ** " << message << endl;
  exit(0);
}

//-----------------------------------------------------------------------------
int 
main(int argc, char** argv)
{
  cout << "enabling autoloader...";
  AutoLibraryLoader::enable();
  cout << "done!" << endl;

  ClassMembers patmuon("pat::Muon", 3);
  
  vector<string>& getters = patmuon.getters();
  ofstream fout("patmuon_getters.txt");
  for(unsigned int i=0; i < getters.size(); i++) fout << getters[i] << endl;
  fout.close();

  return 0;
}

