#ifndef BAYESPACK_H
#define BAYESPACK_H
//-----------------------------------------------------------------------------
// File:	Bayespack.h
//
// Description:	This is a C++ wrapper around the BAYESPACK package by
//              Alan Genz, Washington State University 
//              http://www.math.wsu.edu/math/faculty/genz/genzhome/links.html
//
// Created:    May 2001	 Harrison B. Prosper
// Updated: 28-Aug-2006  HBP Make callable from Root
// $Revision:$
//-----------------------------------------------------------------------------
#include <cmath>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

#ifdef __WITH_CINT__
#include "TObject.h"
#endif

typedef std::vector<int> vint;
typedef std::vector<float> vfloat;
typedef std::vector<double> vdouble;
typedef double USRLGP(double* point);
typedef void   USRMNS(double* point, double* means);

/// Wrapper of BAYESPACK by Alan Genz
class Bayespack
{
public:
  ///
  Bayespack(std::string title="BayesPack",       
            std::string filename=".Bayespack.out");
  ///
  ~Bayespack();

  ///
  void setAccuracy(double relreq) {_relreq =relreq;} // Rel. error of estimates

  ///
  void setMaxpoints(int   maxvls) {_maxvls = maxvls;}// Max. func. evaluations

  ///
  void setTitle (std::string problem){_problem=problem;}

  ///
  void setTransform(int   numtrn) {_numtrn = numtrn;} // Transformation type

  ///
  void setMethod(int      method) {_method = method;} 

  ///
  double accuracy() {return _relreq;}
  
  ///
  int    maxpoints(){return _maxvls;}

  ///
  std::string  title ()   {return _problem;}

  ///
  int    transform(){return _numtrn;}

  ///
  int    method()   {return _method;} 

  ///
  vdouble& mode()     {return _mu;}

  ///
  vdouble& cholesky() {return _c;}

  ///
  void restart(){_rs = 1;}

  ///
  void reset()  {_rs = 0;}

  ///
  double   integrate(vdouble& mode, 
		     int m,     USRLGP* flogp, 
		     int n = 0, USRMNS* fmean = 0);

  ///
  vdouble& results()   {return _means;}

  ///
  vdouble& errors()    {return _errors;}

  ///
  vdouble& covariance(){return _covrnc;}

  ///
  int      status()    {return _inform;}

private:
  int     _m;
  int     _n;
  double  _relreq;
  int     _maxvls;
  int     _rs;
  int     _mn;

  std::string _problem;

  vdouble _mu;
  vdouble _c;

  int     _pu;
  int     _numtrn;
  int     _method;
  double  _nrmcon;

  vdouble _means;
  vdouble _errors;
  vdouble _covrnc;
  int     _inform;

#ifdef __WITH_CINT__
 public:
  ClassDef(Bayespack, 1)
#endif
};


#endif
