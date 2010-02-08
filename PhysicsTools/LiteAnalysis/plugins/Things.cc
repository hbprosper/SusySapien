#include <iostream>
#include "PhysicsTools/LiteAnalysis/interface/ThingFactory.h"
#include "PhysicsTools/LiteAnalysis/interface/ATest.h"
//-----------------------------------------------------------------------------
struct ThingOne: public ThingBase
{
  ThingOne() {}
  ~ThingOne() {}
  void say() {std::cout << "I'm a ThingOne" << std::endl;}
};

struct ThingTwo: public ThingBase
{
  ThingTwo() {}
  ~ThingTwo() {}
  void say() {std::cout << "I'm a ThingTwo" << std::endl;}
};

template <typename X>
struct Thing : public ThingBase
{
  Thing() {}
  ~Thing() {}
  void say() { o.say(); }
  X o;
};

DEFINE_EDM_PLUGIN(ThingFactory, ThingOne, "ThingOne");
DEFINE_EDM_PLUGIN(ThingFactory, ThingTwo, "ThingTwo");
DEFINE_EDM_PLUGIN(ThingFactory, Thing<Aclass>, "Thing<Aclass>");
DEFINE_EDM_PLUGIN(ThingFactory, Thing<Bclass>, "Thing<Bclass>");

