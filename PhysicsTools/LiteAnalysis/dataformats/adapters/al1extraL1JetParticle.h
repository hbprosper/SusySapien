#ifndef AL1EXTRAL1JETPARTICLE_H
#define AL1EXTRAL1JETPARTICLE_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       al1extraL1JetParticle
// Description: Simple adapter for:
//              class:   l1extra::L1JetParticle
//              header:  DataFormats/L1Trigger/interface/L1JetParticle.h
//              
// Created:     Thu Jan 14 23:48:05 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
//-----------------------------------------------------------------------------
class al1extraL1JetParticle : public l1extra::L1JetParticle
{
 public: 
  al1extraL1JetParticle() {}
  al1extraL1JetParticle(const l1extra::L1JetParticle& o)
    : l1extra::L1JetParticle(o) {}
  ~al1extraL1JetParticle() {}

  int16_t gctJetCand_bx() const 	{return gctJetCand() ? gctJetCand()->bx() : -999999;}
  unsigned gctJetCand_capBlock() const 	{return gctJetCand() ? gctJetCand()->capBlock() : 999999;}
  unsigned gctJetCand_capIndex() const 	{return gctJetCand() ? gctJetCand()->capIndex() : 999999;}
  bool gctJetCand_empty() const 	{return gctJetCand() ? gctJetCand()->empty() : false;}
  unsigned gctJetCand_etaIndex() const 	{return gctJetCand() ? gctJetCand()->etaIndex() : 999999;}
  unsigned gctJetCand_etaSign() const 	{return gctJetCand() ? gctJetCand()->etaSign() : 999999;}
  bool gctJetCand_isCentral() const 	{return gctJetCand() ? gctJetCand()->isCentral() : false;}
  bool gctJetCand_isForward() const 	{return gctJetCand() ? gctJetCand()->isForward() : false;}
  bool gctJetCand_isTau() const 	{return gctJetCand() ? gctJetCand()->isTau() : false;}
  unsigned gctJetCand_phiIndex() const 	{return gctJetCand() ? gctJetCand()->phiIndex() : 999999;}
  unsigned gctJetCand_rank() const 	{return gctJetCand() ? gctJetCand()->rank() : 999999;}
};
#endif

