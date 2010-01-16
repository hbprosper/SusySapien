//-----------------------------------------------------------------------------
// File: testplugins.cc
// Created: 03-Nov-2009 Harrison B. Prosper
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PluginManager/interface/PluginFactory.h"
#include "PhysicsTools/LiteAnalysis/interface/ATest.h"
#include "PhysicsTools/LiteAnalysis/interface/ThingFactory.h"
//-----------------------------------------------------------------------------
using namespace std;
using namespace edmplugin;

void fatal(string message)
{
  cout << "** error ** " << message << endl;
  exit(0);
}

using namespace std;

//-----------------------------------------------------------------------------
int 
main(int argc, char** argv)
{
  cout << "testplugins" << endl;

  AutoLibraryLoader::enable();

  std::auto_ptr<ThingBase> thing1(ThingFactory::get()->create("ThingOne"));
  assert(thing1.get() != 0);
  thing1->say();

  std::auto_ptr<ThingBase> thing2(ThingFactory::get()->create("ThingTwo"));
  assert(thing2.get() != 0);
  thing2->say();

  std::auto_ptr<ThingBase> 
    thing3(ThingFactory::get()->create("Thing<Aclass>"));
  assert(thing3.get() != 0);
  thing3->say();

  return 0;
}

