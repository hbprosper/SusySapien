#ifndef AL1DATAEMULDIGI_H
#define AL1DATAEMULDIGI_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       aL1DataEmulDigi
// Description: Simple adapter for:
//              class:   L1DataEmulDigi
//              header:  DataFormats/L1Trigger/interface/L1DataEmulDigi.h
//              
// Created:     Thu Jan 14 23:48:04 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/L1Trigger/interface/L1DataEmulDigi.h"
//-----------------------------------------------------------------------------
class aL1DataEmulDigi : public L1DataEmulDigi
{
 public: 
  aL1DataEmulDigi() {}
  aL1DataEmulDigi(const L1DataEmulDigi& o)
    : L1DataEmulDigi(o) {}
  ~aL1DataEmulDigi() {}

  unsigned data_cid() const 	{return data().cid();}
  bool data_empty() const 	{return data().empty();}
  unsigned data_raw() const 	{return data().raw();}
  unsigned data_reset() const 	{return data().reset();}
  unsigned data_sid() const 	{return data().sid();}
  unsigned data_value() const 	{return data().value();}
  unsigned data_x1() const 	{return data().x1();}
  unsigned data_x2() const 	{return data().x2();}
  unsigned data_x3() const 	{return data().x3();}
  unsigned emul_cid() const 	{return emul().cid();}
  bool emul_empty() const 	{return emul().empty();}
  unsigned emul_raw() const 	{return emul().raw();}
  unsigned emul_reset() const 	{return emul().reset();}
  unsigned emul_sid() const 	{return emul().sid();}
  unsigned emul_value() const 	{return emul().value();}
  unsigned emul_x1() const 	{return emul().x1();}
  unsigned emul_x2() const 	{return emul().x2();}
  unsigned emul_x3() const 	{return emul().x3();}
};
#endif

