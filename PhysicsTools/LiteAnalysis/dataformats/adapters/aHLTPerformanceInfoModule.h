#ifndef AHLTPERFORMANCEINFOMODULE_H
#define AHLTPERFORMANCEINFOMODULE_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       aHLTPerformanceInfoModule
// Description: Simple adapter for:
//              class:   HLTPerformanceInfo::Module
//              header:  DataFormats/HLTReco/interface/HLTPerformanceInfo.h
//              
// Created:     Fri Jan 15 21:58:02 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/HLTReco/interface/HLTPerformanceInfo.h"
//-----------------------------------------------------------------------------
class aHLTPerformanceInfoModule : public HLTPerformanceInfo::Module
{
 public: 
  aHLTPerformanceInfoModule() {}
  aHLTPerformanceInfoModule(const HLTPerformanceInfo::Module& o)
    : HLTPerformanceInfo::Module(o) {}
  ~aHLTPerformanceInfoModule() {}

  bool status_accept() const 	{return status().accept();}
  bool status_error() const 	{return status().error();}
  unsigned int status_index() const 	{return status().index();}
  bool status_wasrun() const 	{return status().wasrun();}
};
#endif

