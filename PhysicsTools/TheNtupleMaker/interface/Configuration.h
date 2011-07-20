#ifndef CONFIGURATION_H
#define CONFIGURATION_H
//
// Package:    PhysicsTools/TheNtupleMaker
//             Configuration.h
//
//             A singleton object to make configuration info available
//             to whoever needs it
//
// Original Author:  Harrison B. Prosper
//         Created:  Wed Sep 01, 2010
//
// $Id: Configuration.h,v 1.1.1.1 2011/05/04 13:04:28 prosper Exp $

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

/// A singleton class to cache global objects.
class Configuration
{
public:
  ///
  static Configuration& instance()
  {
    static Configuration singleton;
    return singleton;
  }

  ///
  void set(const edm::ParameterSet& config, 
           HLTConfigProvider& hltconfig) 
  { 
    config_ = &config;
    hltconfig_ = &hltconfig;
  }

  ///
  const edm::ParameterSet* getConfig() const { return config_; }

  ///
  const HLTConfigProvider* getHLTconfig() const { return hltconfig_; }

//   ///
//   void setLocal(const edm::ParameterSet& config) 
//   { 
//     localconfig_ = &config;
//   }

//   ///
//   const edm::ParameterSet*  getLocal() const { return localconfig_; }

private:
  Configuration() {}                               // prevent explicit creation
  ~Configuration() {}                  
  Configuration(const Configuration&);             // prevent copy
  Configuration& operator=(const Configuration&);  // prevent assignment
  
  const edm::ParameterSet* config_;
  const HLTConfigProvider* hltconfig_;
  //const edm::ParameterSet* localconfig_;
};

#endif
