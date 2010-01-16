#ifndef POISSONGAMMAFIT_HPP
#define POISSONGAMMAFIT_HPP
///////////////////////////////////////////////////////////////////////////////
// File:    PoissonGammaFit.hpp
// Description:	
//
//              Model:
//
//              d_i = Sum p_j * a_ji,  i=1..M,   j=1..N
// 
//              D_i  is count corresponding to true mean d_i
//              A_ji is count corresponding to true mean a_ji
//
//              D is a vector of M observed counts (that is, over M bins)
//              p is a vector of N parameters (that is, over N sources)
//              A is a vector of vectors of size N x M counts.
//
// Created: 27-Jun-2005 Harrison B. Prosper
///////////////////////////////////////////////////////////////////////////////
#include <vector>

typedef std::vector<double>  vdouble;
typedef std::vector<std::vector<double> > vvdouble;

#ifdef __WITH_CINT__
#include "TObject.h"
#endif

/// Fit Poisson/Gamma model to data
class PoissonGammaFit
{
 public: 

  /// Status codes for fit
  enum Status
    {
      kSUCCESS  = 0,
      kNMISMATCH=-1,
      kMMISMATCH=-2,
      kZEROBINS =-3,
      kZEROSRCS =-4,
      kTOOMANYSRCS=-5,
      kNOTCONVERGED=-6
    };

  ///
  PoissonGammaFit(vvdouble&	A,
		  vdouble&	D,
		  bool scale=true,  // Scale so that "p_j" are counts
		  int  verbosity=0);
  ///
  virtual ~PoissonGammaFit();

  /// True if all is well.
  bool       good();

  ///
  bool       fit(std::vector<double>& guess);

  /// Return status code.
  Status     status();

  /// Return mode of posterior density
  vdouble&   mode();

  /// Return mean of posterior density
  vdouble&   mean();
    
  /// Return width of posterior density. 
  vdouble&   width();

  /// Return error matrix of parameters
  vvdouble&  errorMatrix();

  /// Return logarithm of evidence.
  double     logEvidence();

  /// Return logarithm of likelihood at its mode.
  double     logLikelihoodMax();

  ///
  double     logLikelihood(vdouble& x);
  double     logLikelihood(double* x);

  //double     integrate();

 private:

  vvdouble _A;
  vdouble  _D;
  bool     _scale;
  int      _verbosity;

  vvdouble _a;
  
  int      _N;
  int      _M;
  double   _nd;

  double   _loglikemax;
  double   _logmax;
  double   _logevidence;

  vdouble  _ns;
  vdouble  _mode;
  vdouble  _mean;
  vdouble  _width;
  vvdouble _cov;

  Status   _status;

  bool     _findmode(std::vector<double>& guess);

#ifdef __WITH_CINT__
 public:
  ClassDef(PoissonGammaFit, 1)
#endif

};

#endif
