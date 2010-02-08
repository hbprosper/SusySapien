//-----------------------------------------------------------------------------
// File: testreestream.cc
// Created: 24-Jun-2007 Harrison B. Prosper
// Updated: 01-Nov-2007 HBP
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "Math/Random.h"
#include "Math/GSLRndmEngines.h"
#include "PhysicsTools/LiteAnalysis/interface/treestream.hpp"
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
const int MAXJET=50;
void fatal(string message)
{
  cout << "** error ** " << message << endl;
  exit(0);
}
//-----------------------------------------------------------------------------
int 
main(int argc, char** argv)
{
  ROOT::Math::Random<ROOT::Math::GSLRngMT> random;
  char record[80];
  string filename("simpletree.root");

  cout << endl << "test otreestream" << endl;
  otreestream oustream(filename, "Analysis", "Analysis");
  //oustream.autosave(50);

  // Define output buffers

  vector<float> jetet(MAXJET, 0);
  int njet = 0;
  float ht = 0;

  // Define a variable length vector
  oustream.add("njet", njet);
  oustream.add("jetet[njet]",jetet);

  oustream.add("ht",   ht);

  // Loop over events to be written out

  int entries = 100;
  for(int entry=0; entry < entries; entry++)
    {
      njet = random.Poisson(10);
      for(int i=0; i < njet; i++) jetet[i] = random.Exp(50.0);
      ht = 0.0; for(int i=0; i < njet; i++) ht += jetet[i];

      // Commit data to output

      oustream.commit();

      if ( entry % 10 == 0 )
	{
	  float et = njet > 0 ? jetet[0] : -1;
	  sprintf(record, "%4d\t%4d\t%10.2f%10.2f", 
		  entry, njet, et, ht);
	  cout << record << endl;
	}
    }
  oustream.close();

  // Now read it in

  cout << endl << "test itreestream" << endl;

  // Open file

  itreestream stream(filename);

  // Define variables to be read in
 
  int nJet = 0;
  float Ht = 0;
  vector<float> JetEt(MAXJET, -1);

  stream.select("njet", nJet);
  stream.select("jetet",JetEt);
  stream.select("ht",   Ht);

  // Loop over events

  int nentries = stream.entries();
  for(int entry=0; entry < nentries; entry++)
    {
      stream.read(entry);

      if ( entry % 10 == 0 )
	{
	  float et = JetEt.size() > 0 ? JetEt[0] : -1;
	  sprintf(record, "%4d\t%4d\t%10.2f%10.2f", 
		  entry, JetEt.size(), et, Ht);
	  cout << record << endl;
	}
    }
  stream.close();
  return 0;
}

