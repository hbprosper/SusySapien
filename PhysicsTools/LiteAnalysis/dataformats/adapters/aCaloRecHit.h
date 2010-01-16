#ifndef ACALORECHIT_H
#define ACALORECHIT_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       aCaloRecHit
// Description: Simple adapter for:
//              class:   CaloRecHit
//              header:  DataFormats/CaloRecHit/interface/CaloRecHit.h
//              
// Created:     Thu Jan 14 23:47:24 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/CaloRecHit/interface/CaloRecHit.h"
//-----------------------------------------------------------------------------
class aCaloRecHit : public CaloRecHit
{
 public: 
  aCaloRecHit() {}
  aCaloRecHit(const CaloRecHit& o)
    : CaloRecHit(o) {}
  ~aCaloRecHit() {}

  bool detid_null() const 	{return detid().null();}
  int detid_subdetId() const 	{return detid().subdetId();}
};
#endif

