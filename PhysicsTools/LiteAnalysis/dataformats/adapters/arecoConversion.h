#ifndef ARECOCONVERSION_H
#define ARECOCONVERSION_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       arecoConversion
// Description: Simple adapter for:
//              class:   reco::Conversion
//              header:  DataFormats/EgammaCandidates/interface/Conversion.h
//              
// Created:     Thu Jan 14 23:47:54 2010 by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
//-----------------------------------------------------------------------------
class arecoConversion : public reco::Conversion
{
 public: 
  arecoConversion() {}
  arecoConversion(const reco::Conversion& o)
    : reco::Conversion(o) {}
  ~arecoConversion() {}

  double conversionVertex_chi2() const 	{return conversionVertex().chi2();}
  bool conversionVertex_hasRefittedTracks() const 	{return conversionVertex().hasRefittedTracks();}
  bool conversionVertex_isFake() const 	{return conversionVertex().isFake();}
  bool conversionVertex_isValid() const 	{return conversionVertex().isValid();}
  double conversionVertex_ndof() const 	{return conversionVertex().ndof();}
  double conversionVertex_normalizedChi2() const 	{return conversionVertex().normalizedChi2();}
  size_t conversionVertex_tracksSize() const 	{return conversionVertex().tracksSize();}
  double conversionVertex_x() const 	{return conversionVertex().x();}
  double conversionVertex_xError() const 	{return conversionVertex().xError();}
  double conversionVertex_y() const 	{return conversionVertex().y();}
  double conversionVertex_yError() const 	{return conversionVertex().yError();}
  double conversionVertex_z() const 	{return conversionVertex().z();}
  double conversionVertex_zError() const 	{return conversionVertex().zError();}
};
#endif

