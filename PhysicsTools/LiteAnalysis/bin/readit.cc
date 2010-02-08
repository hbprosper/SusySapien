//-----------------------------------------------------------------------------
// File: readit.cc
// Created: 03-Nov-2009 Harrison B. Prosper
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "PhysicsTools/LiteAnalysis/interface/treestream.hpp"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "TStopwatch.h"
//-----------------------------------------------------------------------------
using namespace std;

string FILENAME("zee_pat.root");
string GENPT("GenParticle*pt");

void fatal(string message)
{
  cout << "** error ** " << message << endl;
  exit(0);
}
//-----------------------------------------------------------------------------
int 
main(int argc, char** argv)
{
  AutoLibraryLoader::enable();

  cout << endl << "\t\t*** readit ***" << endl << endl;

  // Open file

  itreestream stream(FILENAME, "Events");
  if ( ! stream.good() ) fatal("unable to open file: " + FILENAME);

  int nevent = stream.size();
  cout << endl << endl << "Number of events: " << nevent << endl;
  
  vector<double> gpt(2000);
  stream.select(GENPT, gpt);
  if ( ! stream.good() ) fatal("unable to access branch: " + GENPT);

  cout << "enter event loop" << endl;

  // Time loop

  TStopwatch stopwatch;
  double count=0;
  stopwatch.Start();
  for(int entry=0; entry < nevent; entry++)
    {
      stream.read(entry);
      count += gpt.size();
    }
  stopwatch.Stop();
  double dt = stopwatch.RealTime();
  double rate = (float)nevent/dt;
  count /= nevent;

  char record[80];
  sprintf(record,
          "Read rate:              %10.1f events/sec\n"
          "Average # GenParticles: %10.1f", rate, count);
  cout << record << endl;
  return 0;
}

