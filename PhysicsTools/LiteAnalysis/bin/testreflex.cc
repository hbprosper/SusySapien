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
#include "Reflex/Object.h"
#include "Reflex/Base.h"
#include "Reflex/Type.h"
#include "TROOT.h"
#include "TString.h"
#include "TRandom.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/LiteAnalysis/interface/ATest.h"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
//-----------------------------------------------------------------------------
using namespace std;
using namespace Reflex;

void fatal(string message)
{
  cout << "** error ** " << message << endl;
  exit(0);
}



//-----------------------------------------------------------------------------
int 
main(int argc, char** argv)
{
  cout << "enabling autoloader..." << endl;
  AutoLibraryLoader::enable();
  cout << "\tdone!" << endl;

  TRandom random;
  Aclass a;
  Bclass b;
  ATest  c(a, b);
  int debug = 1;

  vector<string> mname;
  mname.push_back("ptrToA()->value()");
  mname.push_back("refToA().value()");
  mname.push_back("toA().value()");

  vector<kit::Method> method;
  for(int i=0; i < 3; i++)
    {
      method.push_back( kit::Method("ATest", mname[i], debug) );
    }

  for(int i=0; i < 100; i++)
    {
      cout << endl << i 
           << " -----------------------------------------" << endl;
      c.refToA().set(random.Uniform(10));

      cout << mname[0] << " = " <<
        c.ptrToA()->value() << "\tdirect " << endl;

 
      for(int i=0; i < 3; i++)
        {
          cout << " method( " << mname[i] << " ) = " << method[i](&c) << endl;
        }
    }

  return 0;
}

