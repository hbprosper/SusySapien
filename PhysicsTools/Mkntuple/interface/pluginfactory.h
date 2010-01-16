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
// $Id$
//
//

#include <vector>
#include "FWCore/PluginManager/interface/PluginFactory.h"
#include "FWCore/Framework/interface/Event.h"
#include "PhysicsTools/LiteAnalysis/interface/treestream.hpp"
  
// Model a buffer as a thing with
// 1. a maximum count
// 2. a count of the number of values per variable
// 3. a vector of variables, each with the same maxcount and count
//
// The name of the ith ntuple variable is constructed as follows:
// name = prefix + "_" + var[i].second
//
// where var[i] is a pair of strings with
// var[i].first the name of the (simple) method to be called
// var[i].second is the desired name of the variable
//
// // We use a base class to allow for polymorthic behavior. See call
// to fill() method in plugins.cc.
//

typedef std::pair<std::string, std::string> VariableDescriptor;

///
struct BufferThing
{
  virtual ~BufferThing();
      
  /// Initialize the buffer. 
  virtual void init(otreestream& out,
                    std::string label, 
                    std::string prefix,
                    std::vector<VariableDescriptor>& var,
                    int maxcount)=0;
  
  /// Call requested methods of selected objects and fill buffer.
  virtual void fill(const edm::Event& event)=0;
};


// Give this ugly thing a simpler name!
typedef edmplugin::PluginFactory<BufferThing*(void)> BufferFactory;
#endif
