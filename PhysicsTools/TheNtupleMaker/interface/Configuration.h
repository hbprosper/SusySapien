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
// $Id: Configuration.h,v 1.2 2011/07/20 16:19:54 prosper Exp $

#include <string>
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
  void set(std::string& blockname,
           std::string& buffername,
           std::string& labelname)
  { 
    blockname_  = blockname;
    buffername_ = buffername;
    labelname_  = labelname;
  }

  ///
  const edm::ParameterSet* getConfig() const { return config_; }

  ///
  const HLTConfigProvider* getHLTconfig() const { return hltconfig_; }

  ///
  std::string getblockname() const { return blockname_; }

  ///
  std::string getbuffername() const { return buffername_; }

  ///
  std::string getlabelname() const { return labelname_; }

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

  std::string blockname_;
  std::string buffername_;
  std::string labelname_;
};

#endif
