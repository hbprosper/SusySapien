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
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
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
  cout << endl << "test otreestream" << endl;

  // Define output buffers

  vector<float> jetet(MAXJET, 0);
  int njet = 0;
  float ht = 0;

  string filename1("simpletree1.root");
  otreestream oustream1(filename1, "Events", "Events");

  oustream1.add("njet", njet);
  oustream1.add("jetet[njet]",jetet);
  oustream1.add("ht",   ht);

  // Loop over events to be written out

  int entries = 100;
  for(int entry=0; entry < entries; entry++)
    {
      njet = random.Poisson(10);
      for(int i=0; i < njet; i++) jetet[i] = random.Exp(50.0);
      ht = 0.0; for(int i=0; i < njet; i++) ht += jetet[i];

      if ( entry % 10 == 0 )
        {
          float et = njet > 0 ? jetet[0] : -1;
          sprintf(record, "%4d\t%4d\t%10.2f%10.2f", 
                  entry, njet, et, ht);
          cout << record << endl;
        }
      oustream1.commit();
    }
  oustream1.close();

  // Second ntuple

  string filename2("simpletree2.root");
  cout << endl << "test otreestream" << endl;
  otreestream oustream2(filename2, "Events", "Events");

  oustream2.add("njet", njet);
  oustream2.add("jetet[njet]",jetet);
  oustream2.add("ht",   ht);

  // Loop over events to be written out

  for(int entry=0; entry < entries; entry++)
    {
      njet = random.Poisson(10);
      for(int i=0; i < njet; i++) jetet[i] = random.Exp(50.0);
      ht = 0.0; for(int i=0; i < njet; i++) ht += jetet[i];

      int nn = entry + entries;
      if ( nn % 10 == 0 )
        {
          float et = njet > 0 ? jetet[0] : -1;
          sprintf(record, "%4d\t%4d\t%10.2f%10.2f", 
                  nn, njet, et, ht);
          cout << record << endl;
        }
      oustream2.commit();
    }
  oustream2.close();


  // Now read it in

  cout << endl << "test itreestream" << endl;

  // Open files

  vector<string> filenames;
  filenames.push_back(filename1);
  filenames.push_back(filename2);
  itreestream stream(filenames, "Events");

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

