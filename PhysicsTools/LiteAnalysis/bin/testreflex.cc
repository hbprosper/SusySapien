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

  kit::Count count;
  count("test");

  Aclass a;
  Bclass b;
  ATest  c(a, b);

  cout << "Address of a: " << &a << endl;
  cout << "Address of b: " << &b << endl;

  for(int i=0; i < 5; i++)
    {
      Member method = kit::getMethod("ATest", "ptrToA");
      void* address = static_cast<void*>(&c);
      void* raddr   = kit::invokeMethod(method, address);
      if ( raddr != 0 )
        {
          string name = kit::getReturnClass(method);
          cout << i << "\treturn class(" << name << ")" << endl;

          Member m = kit::getMethod(name, "value");
          if ( kit::methodValid(m) )
            {
              raddr = kit::invokeMethod(m, raddr);
              double y = *static_cast<double*>(raddr);
              cout << "\t\to->ptrToA().value() = " << y << endl;
            }
          else
            {
              cout << "\t** error ** scope not found" << endl;
            }
        }
    }

  cout << endl << "getB(std::string)" << endl << endl;

  string args = string("(42,\"The time has come\", 3.14159)");
  string argsregex;
  vector<kit::ValueThing*> vals;
  kit::decodeArguments(args, argsregex, vals);

  Member method = kit::getMethod("ATest", "value", argsregex);
  void* address = static_cast<void*>(&c);

  vector<void*> vargs;
  for(unsigned j=0; j < vals.size(); j++)
    vargs.push_back( vals[j]->address() );

//       if ( raddr != 0 )
//         {
//           string name = kit::getReturnClass(method);
//           cout << i << "\treturn class(" << name << ")" << endl;
//           if ( name == "" )
//             {
//               cout << "**** error ** can't get return class" << endl;
//               continue;
//             }
//           Member m = kit::getMethod(name, "value");
//           if ( kit::methodValid(m) )
//             {
//               raddr = kit::invokeMethod(m, raddr);
//               double y = *static_cast<double*>(raddr);
//               cout << "\to->getB(std::string).value() = " << y << endl;
//             }
//           else
//             {
//               cout << "\t** error ** scope not found" << endl;
//             }
//         }
  

  cout << endl << "*** TEST OF Method for simple methods ***\n" << endl;

  kit::Method value("ATest", 
                    "value(42, \"The time has come\", 3.1415926536)");

  cout << endl 
       <<"** PRINT METHOD **" << endl;
  cout << value.str() << endl;

  vector<kit::ValueThing*>& vvals = value.arguments(1);
  vector<void*> vaddr;
  for(unsigned i=0; i < vvals.size(); i++)
    vaddr.push_back( vvals[i]->address() );


  for(int i=0; i < 1000; i++)
    {
      cout << endl;
      void* raddr = kit::invokeMethod(method, address, vargs);
      double y = *static_cast<double*>(raddr);
      cout << "address 0(" << raddr << ") "
           << "value 0(" << y << ")" << endl;

      raddr = kit::invokeMethod(method, address, vaddr);
      y = *static_cast<double*>(raddr);
      cout << "address 1(" << raddr << ") "
           << "value 1(" << y << ")" << endl;

      y = value(address);
      cout << "value 2(" << y << ")" << endl;
    }



  cout << endl << "*** TEST OF Method for compound methods ***\n" << endl;

  kit::Method value2("ATest", 
                     "getB().value(42)");

  double y = value2(address);
  cout << "compound method result: " << y << endl;

  return 0;
}

