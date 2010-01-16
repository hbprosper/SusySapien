//-----------------------------------------------------------------------------
// File: testhcache.cc
// Created: 24-Jun-2007 Harrison B. Prosper
// Updated: 01-Nov-2007 HBP
//-----------------------------------------------------------------------------
#include <iostream>
#include "PhysicsTools/LiteAnalysis/interface/HistogramCache.hpp"
//-----------------------------------------------------------------------------
using namespace std;
int 
main(int argc, char** argv)
{
  cout << "\ttesthcache\n" << endl;
  HistogramCache hcache("hist.root");
  //hcache.ls();
  cout << "\tdone!\n" << endl;
  return 0;
}

