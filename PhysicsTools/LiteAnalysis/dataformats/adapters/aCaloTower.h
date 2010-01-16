#ifndef ACALOTOWER_H
#define ACALOTOWER_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       aCaloTower
// Description: Simple adapter for:
//              class:   CaloTower
//              header:  DataFormats/CaloTowers/interface/CaloTower.h
//              
// Created:     Thu Jan 14 23:47:24 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/CaloTowers/interface/CaloTower.h"
//-----------------------------------------------------------------------------
class aCaloTower : public CaloTower
{
 public: 
  aCaloTower() {}
  aCaloTower(const CaloTower& o)
    : CaloTower(o) {}
  ~aCaloTower() {}

  bool constituent_null(size_t x) const 	{return constituent(x).null();}
  int constituent_subdetId(size_t x) const 	{return constituent(x).subdetId();}
  int id_ieta() const 	{return id().ieta();}
  int id_ietaAbs() const 	{return id().ietaAbs();}
  int id_iphi() const 	{return id().iphi();}
  bool id_null() const 	{return id().null();}
  int id_subdetId() const 	{return id().subdetId();}
  int id_zside() const 	{return id().zside();}
};
#endif

