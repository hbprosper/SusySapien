#ifndef ADBAYES_HPP
#define ADBAYES_HPP
//-----------------------------------------------------------------------------
// File:	AdBayes.cpp
//
// Description:	This is a C++ wrapper around the ADBAYS program by
//              Alan Genz, Washington State University 
//              http://www.math.wsu.edu/math/faculty/genz/genzhome/links.html
//
// Created:    May 2001  Harrison B. Prosper
// Updated: 28-Aug-2006  HBP
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
typedef void   FUNSUB(int* ndim, double* point, int* numfun, double* value);

/// Wrapper of ADBAYS program by Alan Genz
class AdBayes
{
public:

  ///
  AdBayes();

  ///
  ~AdBayes();

  ///
  vdouble& integrate(vdouble& a,
		     vdouble& b,
		     FUNSUB*  fun,
		     int      numfun=1);

  ///
  void restart(){_restart = 1;}
  void reset()  {_restart = 0;}

  void setMaxpoints(int maxpts){_maxpts=maxpts;}  // Max. points
  void setAccuracy(double relerr){_relerr=relerr;}// Rel. error of estimates
  void setRule  (int    key)   {_key=key;}        // Rule type

  // Getters
  //////////

  double   accuracy() {return _relerr;}
  int      maxpoints(){return _maxpts;}
  int      rule()     {return _key;} 
  int      neval()    {return _neval;}

  vdouble  results()  {return _result;}
  vdouble  errors()   {return _error;}
  //const double* derrors(){return (const double*)(&_error[0]);}
  int      status()   {return _status;}

private:
  int     _key;
  int     _restart;
  int     _maxpts;

  double  _abserr;
  double  _relerr;
  
  vdouble _result;
  vdouble _error;
  vdouble _work;
  vdouble _val;
  
  int     _neval;
  int     _status;

  int     _num(int ndim);

#ifdef __WITH_CINT__
 public:
  ClassDef(AdBayes, 1)
#endif
};

#endif
