#ifndef THINGFACTORY_H
#define THINGFACTORY_H
#include "FWCore/PluginManager/interface/PluginFactory.h"
//-----------------------------------------------------------------------------
struct ThingBase
{
  virtual ~ThingBase();
  virtual void say() = 0;
};
typedef edmplugin::PluginFactory<ThingBase*(void)> ThingFactory;

#endif
