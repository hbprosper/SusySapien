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
#include "PhysicsTools/LiteAnalysis/interface/FunctionMember.h"
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

  vector<string> mname;
  mname.push_back("ptrToA()->value()");
  mname.push_back("refToA().value()");
  mname.push_back("toA().value()");
  mname.push_back("avalue");
  mname.push_back("ptrToA()->avalue");
  mname.push_back("ptrToB->value()");

  vector<Method<ATest> > method;

  for(int i=0; i < (int)mname.size(); i++)
    {
      method.push_back( Method<ATest>(mname[i]) );
    }

  char record[256];

  for(int i=0; i < 1000; i++)
    {
      cout << endl << i 
           << " ------------------------------------------------"   << endl;
      //--------------------------------------------------------------------
      c.ptrToA()->set(random.Uniform(100));
      sprintf(record, "Direct         - %-20s\t= %10.4f", 
              mname[0].c_str(), c.ptrToA()->value());
      cout << record << endl;

      sprintf(record, "Method         - %-20s\t= %10.4f", 
              mname[0].c_str(), method[0](c)); 
      cout << record << endl;
      cout << endl;
      //--------------------------------------------------------------------
      c.refToA().set(random.Uniform(100));
      sprintf(record, "Direct         - %-20s\t= %10.4f", 
              mname[1].c_str(), c.refToA().value());
      cout << record << endl;

      sprintf(record, "Method         - %-20s\t= %10.4f", 
              mname[1].c_str(), method[1]((const ATest)c)); 
      cout << record << endl;
      cout << endl;
      //--------------------------------------------------------------------
      a.set(random.Uniform(100));
      sprintf(record, "Direct         - %-20s\t= %10.4f", 
              mname[2].c_str(), c.toA().value());
      cout << record << endl;

      sprintf(record, "Method         - %-20s\t= %10.4f", 
              mname[2].c_str(), method[2](c)); 
      cout << record << endl;
      cout << endl;
      //--------------------------------------------------------------------
      c.avalue = random.Uniform(100);
      sprintf(record, "Direct         - %-20s\t= %10.4f", 
              mname[3].c_str(), c.avalue);
      cout << record << endl;

      sprintf(record, "Method         - %-20s\t= %10.4f", 
              mname[3].c_str(), method[3]((const ATest)c)); 
      cout << record << endl;
      cout << endl;
      //--------------------------------------------------------------------
      c.ptrToA()->avalue = random.Uniform(100);
      sprintf(record, "Direct         - %-20s\t= %10.4f", 
              mname[4].c_str(), c.ptrToA()->avalue);
      cout << record << endl;

      sprintf(record, "Method         - %-20s\t= %10.4f", 
              mname[4].c_str(), method[4](c)); 
      cout << record << endl;
      cout << endl;
      //--------------------------------------------------------------------
      c.ptrToB->set(random.Uniform(100));
      sprintf(record, "Direct         - %-20s\t= %10.4f", 
              mname[5].c_str(), c.ptrToB->value());
      cout << record << endl;

      sprintf(record, "Method         - %-20s\t= %10.4f", 
              mname[5].c_str(), method[5]((const ATest)c)); 
      cout << record << endl;
    }

  return 0;
}

