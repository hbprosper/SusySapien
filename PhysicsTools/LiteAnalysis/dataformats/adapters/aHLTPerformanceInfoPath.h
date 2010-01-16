#ifndef AHLTPERFORMANCEINFOPATH_H
#define AHLTPERFORMANCEINFOPATH_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       aHLTPerformanceInfoPath
// Description: Simple adapter for:
//              class:   HLTPerformanceInfo::Path
//              header:  DataFormats/HLTReco/interface/HLTPerformanceInfo.h
//              
// Created:     Thu Jan 14 23:47:58 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/HLTReco/interface/HLTPerformanceInfo.h"
//-----------------------------------------------------------------------------
class aHLTPerformanceInfoPath : public HLTPerformanceInfo::Path
{
 public: 
  aHLTPerformanceInfoPath() {}
  aHLTPerformanceInfoPath(const HLTPerformanceInfo::Path& o)
    : HLTPerformanceInfo::Path(o) {}
  ~aHLTPerformanceInfoPath() {}

  bool status_accept() const 	{return status().accept();}
  bool status_error() const 	{return status().error();}
  unsigned int status_index() const 	{return status().index();}
  bool status_wasrun() const 	{return status().wasrun();}
};
#endif

