//-----------------------------------------------------------------------------
// File:	AdBayes.cpp
//
// Description:	This is a C++ wrapper around the ADBAYS program by
//              Alan Genz, Washington State University 
//              http://www.math.wsu.edu/math/faculty/genz/genzhome/links.html
//
// Created:    May 2001	 Harrison B. Prosper
// Updated: 28-Aug-2006  HBP
//          01-Aug-2007  HBP switch order of last two arguments in integrate
//          07-May-2008  HBP adapt to CMSSW structure 
//-----------------------------------------------------------------------------
#include "PhysicsTools/LiteAnalysis/interface/AdBayes.hpp"

#ifdef __WITH_CINT__
  ClassImp(AdBayes)
#endif

using namespace std;

extern "C"
{
  void adbays_(int    &ndim,
	       int    &numfun,
	       double &a,
	       double &b,
	       int    &minpts,
	       int    &maxpts,

	       FUNSUB *fun,

	       double &epsabs,
	       double &epsrel,
	       int    &key,
	       int    &nw,
	       int    &restar,

	       double &result,
	       double &abserr,
	       int    &neval,
	       int    &ifail,
	       double &work);
}


// AdBayes Constructor
/////////////////////////////////
AdBayes::AdBayes()
  : _key(1),
    _restart(0),
    _maxpts(250000),
    _abserr(0.0),
    _relerr(4e-3)
{_val.resize(2);}

// AdBayes Destructor
/////////////////////////////////
AdBayes::~AdBayes(){}


// Methods
//////////

vdouble &AdBayes::integrate(vdouble &a,
			    vdouble &b,
			    FUNSUB  *fun,
			    int      numfun)
{
  // Compute sizes of various buffers

  int ndim   = a.size();
  int num    = _num(ndim);
  int minpts = 1;                  // Minimum number of points
  int maxpts = 7 * num +_maxpts;   // Maximum number of points
  int maxsub = (maxpts-num)/(2*num) + 1;
  //int nw     = 2*maxsub*(ndim+numfun+1) + 7*numfun + ndim + 1;
  int nw     = 2 * (maxsub*(2*ndim + 2*numfun+2) + 7*numfun + ndim);

  // Create buffers

  _result.resize(numfun);
  _error.resize(numfun);
  _work.resize(nw);
  
  adbays_(ndim,               // Dimension of space
	  numfun,             // Number of integrands
	  a[0],               // Lower bounds
	  b[0],               // Upper bounds
	  minpts,             // Minimum number of points to compute
	  maxpts,             // Maximum number of points to compute
	  
	  fun,                // Integrands fun(ndim, x, numfun, y)
	  
	  _abserr,            // Absolute error (=0.0)
	  _relerr,            // Relative error
	  _key,               // Integration rule
	  nw,                 // Size of work buffer
	  _restart,           // = 0 first call, 1 otherwise
	  
	  _result[0],  // Vector of numfun results
	  _error[0],   // Vector of error estimates
	  _neval,      // Number of functions evaluations
	  _status,     // Status code

	  _work[0]);          // Work buffer

  _val[0] = _result[0];
  _val[1] = _error[0];
  return _val;
}

// Private methods

int AdBayes::_num(int ndim)
{
  if ( ndim == 1 )
    return 15;
  else if ( _key == 0 )
    {
      if ( ndim < 12 )
	return 1 + 2*ndim*(ndim+3) + (int)pow((float)ndim,2);
      else
	return 1 + 2*ndim*(ndim+4);
    }
  else if ( _key == 1 )
    return 1 + 2*ndim*(ndim+3) + (int)pow((float)ndim,2);
  else if ( _key == 2 )
    return 1 + 4*ndim + 6*ndim*ndim + 
      4*ndim*(ndim-1)*(ndim-2)/3 + (int)pow((float)ndim,2);
  else
    return 1 + 2*ndim*(ndim+4);
}
