#ifndef CONFIGURATION_H
#define CONFIGURATION_H
//
// Package:    PhysicsTools/Mkntuple
//             Configuration.h
//
//             A singleton object to make configuration info available
//             to whoever needs it
//
// Original Author:  Harrison B. Prosper
//         Created:  Wed Sep 01, 2010
//
// $Id: Configuration.h,v 1.1 2010/09/03 01:54:13 prosper Exp $

#include "FWCore/ParameterSet/interface/ParameterSet.h"

/// A singleton class to cache ParameterSet object.
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
  void set(const edm::ParameterSet& config) 
  { 
    config_ = &config;
  }

  ///
  const edm::ParameterSet* get() const { return config_; }

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
  //const edm::ParameterSet* localconfig_;
};

#endif
