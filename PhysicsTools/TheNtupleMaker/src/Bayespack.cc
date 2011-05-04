//-----------------------------------------------------------------------------
// File:	Bayespack.cc
//
// Description:	This is a C++ wrapper around the BAYESPACK package by
//              Alan Genz, Washington State University 
//              http://www.math.wsu.edu/math/faculty/genz/genzhome/links.html
//
// Created:    May 2001	 Harrison B. Prosper
// Updated: 28-Aug-2006  HBP Make callable from Root
//          07-May-2008  HBP - Adapt to CMSSW structure
// $Revision: 1.1 $
//-----------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Bayespack.h"

#ifdef __WITH_CINT__
  ClassImp(Bayespack)
#endif

using namespace std;

extern "C"
{
  void banint_(int    &m,
               double &relreq,
               int    &maxvls,
               int    &rs,
               USRLGP *lf,
               int    &mn,
               USRMNS *lm,
               const char   *problm,
               double &mu,
               double &c,
               int    &pu,
               int    &numtrn,
               int    &method,
               double &nrmcon,
               double &means,
               double &errors,
               double &covrnc,
               int    &inform,
               int);	  

  void bayespackopen_  (const char *filename, int);
  void bayespackclose_();
 
}


// Bayespack Constructor
/////////////////////////////////
Bayespack::Bayespack(string title, string filename)
  : _relreq(1.0e-2),
    _maxvls(100000),
    _rs(0),
    _problem(title),
    _pu(-55),          // Must match lun in bayespackf.f
    _numtrn(20),
    _method(10)
{  
  bayespackopen_(filename.c_str(), filename.size());
}

// Bayespack Destructor
/////////////////////////////////
Bayespack::~Bayespack(){bayespackclose_();}


// Methods
//////////

double Bayespack::integrate(vdouble &mode,
			    int m, USRLGP *flogp, 
			    int n, USRMNS *fmean)
{
  int mn = m + n;

  // Make sure vectors are the right size
  //////////////////////////////////////////
  _mu.resize(mode.size());
  copy(mode.begin(), mode.end(), _mu.begin());

  _c.resize(m*(m+1)/2);
  _means.resize(mn);
  _errors.resize(mn);
  _covrnc.resize(m*(m+1)/2);

  banint_(m,                        // Dimension of space
          _relreq,                  // Relative error
          _maxvls,                  // Maximum number of function evaluations
          _rs,                      // rs = 0 first call, rs > 0 otherwise
          flogp,                    // log[Post(theta|d)]
          mn,                       // Number of means mn >= m
          fmean,                    // Additional means to compute
          _problem.c_str(),         // Problem title
          _mu[0],                   // Modes
          _c[0],                    // Cholesky matrix
          _pu,                      // Print level
          _numtrn,                  // Transform
          _method,                  // Method
          _nrmcon,                  // Normalization (integral)
          _means[0],                // Means
          _errors[0],               // Errors on means
          _covrnc[0],               // Covariance matrix
          _inform,                  // 0 = Ok, 1 = Not Ok
          _problem.size());
  return _nrmcon;
}
