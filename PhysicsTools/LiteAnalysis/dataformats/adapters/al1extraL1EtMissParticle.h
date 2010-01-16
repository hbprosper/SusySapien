#ifndef AL1EXTRAL1ETMISSPARTICLE_H
#define AL1EXTRAL1ETMISSPARTICLE_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       al1extraL1EtMissParticle
// Description: Simple adapter for:
//              class:   l1extra::L1EtMissParticle
//              header:  DataFormats/L1Trigger/interface/L1EtMissParticle.h
//              
// Created:     Thu Jan 14 23:48:05 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
//-----------------------------------------------------------------------------
class al1extraL1EtMissParticle : public l1extra::L1EtMissParticle
{
 public: 
  al1extraL1EtMissParticle() {}
  al1extraL1EtMissParticle(const l1extra::L1EtMissParticle& o)
    : l1extra::L1EtMissParticle(o) {}
  ~al1extraL1EtMissParticle() {}

  int16_t gctEtHad_bx() const 	{return gctEtHad() ? gctEtHad()->bx() : -999999;}
  bool gctEtHad_empty() const 	{return gctEtHad() ? gctEtHad()->empty() : false;}
  unsigned gctEtHad_et() const 	{return gctEtHad() ? gctEtHad()->et() : 999999;}
  bool gctEtHad_overFlow() const 	{return gctEtHad() ? gctEtHad()->overFlow() : false;}
  int16_t gctEtMiss_bx() const 	{return gctEtMiss() ? gctEtMiss()->bx() : -999999;}
  bool gctEtMiss_empty() const 	{return gctEtMiss() ? gctEtMiss()->empty() : false;}
  unsigned gctEtMiss_et() const 	{return gctEtMiss() ? gctEtMiss()->et() : 999999;}
  bool gctEtMiss_overFlow() const 	{return gctEtMiss() ? gctEtMiss()->overFlow() : false;}
  unsigned gctEtMiss_phi() const 	{return gctEtMiss() ? gctEtMiss()->phi() : 999999;}
  int16_t gctEtTotal_bx() const 	{return gctEtTotal() ? gctEtTotal()->bx() : -999999;}
  bool gctEtTotal_empty() const 	{return gctEtTotal() ? gctEtTotal()->empty() : false;}
  unsigned gctEtTotal_et() const 	{return gctEtTotal() ? gctEtTotal()->et() : 999999;}
  bool gctEtTotal_overFlow() const 	{return gctEtTotal() ? gctEtTotal()->overFlow() : false;}
};
#endif

