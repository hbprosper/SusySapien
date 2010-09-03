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
#include "PhysicsTools/LiteAnalysis/interface/Method.h"
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
  void* address = (void*)(&c);

  vector<string> mname;
  mname.push_back("ptrToA()->value()");
  mname.push_back("refToA().value()");
  mname.push_back("toA().value()");
  mname.push_back("avalue");
  mname.push_back("ptrToA()->avalue");

  vector<Method> method;
  for(int i=0; i < (int)mname.size(); i++)
    {
      method.push_back( Method("ATest", mname[i]) );
    }

  for(int i=0; i < 1000; i++)
    {
      cout << endl << i 
           << " -----------------------------------------"   << endl;
      c.refToA().set(random.Uniform(10));

      cout << "direct - " 
           << mname.front() 
           << "\t= " << c.ptrToA()->value() << endl;

      c.avalue = random.Uniform(100);
      cout << "direct - " 
           << mname[mname.size()-2]  
           << "\t= " << c.avalue   << endl;

      c.ptrToA()->avalue = random.Uniform(100);
      cout << "direct - " 
           << mname.back()  << "\t= " 
           << c.ptrToA()->avalue  << endl;

      for(int i=0; i < (int)mname.size(); i++)
        {
          cout << " Method - " << mname[i] 
               << "\t= " << method[i](address) << endl;
        }
    }

  return 0;
}

