#ifndef PLUGINFACTORY_H
#define PLUGINFACTORY_H
/*
 Description: model a factory that creates a buffer for caching variables that
              are to be written to a simple ntuple

 Implementation:
     A sense of beauty and common sense
*/
//
// Original Author:  HBP
// $Id: pluginfactory.h,v 1.7 2010/08/27 01:34:53 prosper Exp $
//
//

#include "FWCore/PluginManager/interface/PluginFactory.h"
#include "PhysicsTools/Mkntuple/interface/BufferUtil.h"
  
// Give this ugly thing a simpler name!
typedef edmplugin::PluginFactory<BufferThing*(void)> BufferFactory;

#endif
