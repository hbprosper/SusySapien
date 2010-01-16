#ifndef AL1EXTRAL1EMPARTICLE_H
#define AL1EXTRAL1EMPARTICLE_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       al1extraL1EmParticle
// Description: Simple adapter for:
//              class:   l1extra::L1EmParticle
//              header:  DataFormats/L1Trigger/interface/L1EmParticle.h
//              
// Created:     Thu Jan 14 23:48:05 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
//-----------------------------------------------------------------------------
class al1extraL1EmParticle : public l1extra::L1EmParticle
{
 public: 
  al1extraL1EmParticle() {}
  al1extraL1EmParticle(const l1extra::L1EmParticle& o)
    : l1extra::L1EmParticle(o) {}
  ~al1extraL1EmParticle() {}

  int16_t gctEmCand_bx() const 	{return gctEmCand() ? gctEmCand()->bx() : -999999;}
  unsigned gctEmCand_capBlock() const 	{return gctEmCand() ? gctEmCand()->capBlock() : 999999;}
  unsigned gctEmCand_capIndex() const 	{return gctEmCand() ? gctEmCand()->capIndex() : 999999;}
  bool gctEmCand_empty() const 	{return gctEmCand() ? gctEmCand()->empty() : false;}
  unsigned gctEmCand_etaIndex() const 	{return gctEmCand() ? gctEmCand()->etaIndex() : 999999;}
  unsigned gctEmCand_etaSign() const 	{return gctEmCand() ? gctEmCand()->etaSign() : 999999;}
  bool gctEmCand_isolated() const 	{return gctEmCand() ? gctEmCand()->isolated() : false;}
  unsigned gctEmCand_phiIndex() const 	{return gctEmCand() ? gctEmCand()->phiIndex() : 999999;}
  unsigned gctEmCand_rank() const 	{return gctEmCand() ? gctEmCand()->rank() : 999999;}
};
#endif

