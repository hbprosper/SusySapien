//-----------------------------------------------------------------------------
// File:	PoissonGammaFit.cc
// Description:	Fit up to 4 sources to data.
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
//              Method:
//
//              The integration over source priors is done exactly, while the
//              source counts and errors are approximated by the mode of the
//              posterior density. The logarithm of the evidence is 
//              approximated by the expression (see for example, 
//              Probability Theory: The Logic of Science, page 2404, by the
//              late Prof. Edwin Jaynes)
//
//              log(p(D|M,I)) ~ log(Lmax) + (N/2)*log(2*pi) 
//                            + (1/2) log(det(Sigma))
//              For large counts, it is better to set the exact flag to false.
//              In this case, the calculations are done using MC integration.
//
// Created:     27-Jun-2005 Harrison B. Prosper
// Updated:     06-Feb-2006 HBP Adapt for use in Root
//-----------------------------------------------------------------------------
#include <iostream>
#include <cmath>
#include "PhysicsTools/Mkntuple/interface/PoissonGammaFit.h"
#include "PhysicsTools/Mkntuple/interface/kit.h"
#include <TMinuit.h>
#include <TMatrix.h>

#ifdef __WITH_CINT__
  ClassImp(PoissonGammaFit)
#endif

using namespace std;

//-----------------------------------------------------------------------------

typedef std::vector<float> vfloat;

static PoissonGammaFit* fitobject=0;
namespace
{

  void logLike(int&    npar, 
	       double* grad, 
	       double& fval,
	       double* xval,
	       int    iflag)
  {
    vector<double> p(npar);
    copy(xval, xval + npar, p.begin());
    fval = -fitobject->logLikelihood(p);
  }

  double flogp(double* point) 
  { 
    return fitobject->logLikelihood(point);
  }
  const int MAXITER=10000;
  const int MAXSRCS=10;
}

//-----------------------------------------------------------------------------
// Class to perform Bayesian fit
//-----------------------------------------------------------------------------
PoissonGammaFit::PoissonGammaFit(vvdouble&	A,  // Counts  "A_ji"
				 vdouble&	D,  // Counts  "D_i" for data.
				 bool scale,
				 int verbosity)
  : _A(A), _D(D), _scale(scale), _verbosity(verbosity), _a(A)
{
  if ( _verbosity > 0 )
    cout << "PoissonGammaFit - Started" << endl;

  _status = kSUCCESS;
  
  _N = _A.size();  // Number of sources (N)
  _M = _D.size();  // Number of bins    (M)

  if ( _M < 1 )
    {
      std::cout 
	<< "**Error - PoissonGammaFit - bin count is zero"
	<< std::endl;
      _status = kZEROBINS;
      return;      
    }
  
  if ( _N < 1 )
    {
      std::cout 
	<< "**Error - PoissonGammaFit - zero sources"
	<< std::endl;
      _status = kZEROSRCS;
      return;
    }

  if ( _N > MAXSRCS )
    {
      std::cout 
	<< "**Error - PoissonGammaFit - too many sources: " << _N 
	<< std::endl;
      _status = kTOOMANYSRCS;
      return;
    }

  if ( _A[0].size() != (unsigned int)_M )
    {
      std::cout 
	<< "**Error - PoissonGammaFit - mis-match in number of sources\n"
	<< "size(D): " << _M << " differs from size(A[0]) = " 
	<< _A[0].size()
	<< std::endl;
      _status = kMMISMATCH;
      return;
    }

  // Get total counts per source
  for ( int j = 0; j < _N; j++ )
    {
      _cov.push_back(vector<double>(_N));
      _mode.push_back(0);
      _mean.push_back(0);
      _width.push_back(0);
      _ns.push_back(0);
      for ( int i = 0; i < _M; i++ ) _ns[j] += _A[j][i];
    }
  
  // Get data count

  _nd = 0.0; 					
  for ( int i = 0; i < _M; i++ ) _nd += _D[i];
}


bool
PoissonGammaFit::execute(vector<double>& guess)
{
  return _findmode(guess);
}

PoissonGammaFit::~PoissonGammaFit() {}

double 
PoissonGammaFit::logLikelihood(vdouble& p)
{
  for(int i=0; i < _N; i++) if (p[i] < 0) return 0;
  return kit::poissongamma(p, _A, _D, 1);
}

double 
PoissonGammaFit::logLikelihood(double* point)
{
  vdouble p(_N);
  copy(point, point+_N, p.begin());
  return kit::poissongamma(p, _A, _D, 1);
}

bool 
PoissonGammaFit::good() { return _status == kSUCCESS; }

PoissonGammaFit::Status
PoissonGammaFit::status() { return _status; }

vdouble&
PoissonGammaFit::mode() { return _mode; }

vdouble&
PoissonGammaFit::mean() { return _mean; }

vdouble&
PoissonGammaFit::width() { return _width; }

vvdouble&
PoissonGammaFit::errorMatrix() { return _cov; }

double 
PoissonGammaFit::logEvidence() { return _logevidence; }

double 
PoissonGammaFit::logLikelihoodMax() { return _loglikemax; }

bool
PoissonGammaFit::_findmode(vector<double>& guess)
{
  fitobject = this; // Update global pointer

  _status = kSUCCESS;

  if ( _verbosity > 0 )
    cout << "\tFind mode" << endl;

  // Find mode of likelihood function

  TMinuit minuit(_N);
  minuit.SetFCN(logLike);

  double x    = _nd / _N;
  double step = x / 1000;
  double minx = 0;
  double maxx = x * 10;
  
  int ierflag;
  for (int i=0; i < _N; i++)
    {
      char name[80]; sprintf(name,"x%d", i);
      if ( (int)guess.size() == _N )
	{
	  x    = guess[i];
	  step = x / 1000;
	  minx = 0;
	  maxx = 10 * x;
	}
      minuit.mnparm(i, name, x, step, minx, maxx, ierflag);
    }

  // Do fit

  minuit.SetErrorDef(0.5);
  minuit.SetMaxIterations(MAXITER);
  minuit.Migrad();

  // Check for convergence

  double fmin, fedm, errdef;
  int nvpar, nparx, status;
  minuit.mnstat(fmin, fedm, errdef, nvpar, nparx, status);
  if ( status == 3 )
    {
      // Get parameters
      _loglikemax = -fmin;
      for (int i=0; i < _N; i++) minuit.GetParameter(i, _mode[i], _width[i]);
    }
  else
    {
      if ( _verbosity > 0 )
	  cout << "\tMode finding FAILED" << endl;
      _status = kNOTCONVERGED;
      return false;
    }

  if ( _verbosity > 0 )
    cout << "\tMode FOUND" << endl;
  
  // Compute estimate of log(evidence)
  // log p(D|M,I) ~ ln Lmax + (N/2) ln 2*pi + (1/2) ln det(Sigma)
  
  int ndim = _N *_N;
  double errmat[2500];
  minuit.mnemat(&errmat[0], ndim);

  TMatrix mat(_N, _N);
  for(int i=0; i < _N; i++)
    for(int j=0; j < _N; j++)
      { 
	_cov[i][j] = errmat[i*ndim+j];
	mat[i][j] = _cov[i][j];
      }
  _logevidence = _loglikemax + 0.5*_N * log(2*M_PI);
  double det = mat.Determinant();
  if ( det > 0 ) _logevidence += log(det);

  return true;
}

/*
double 
PoissonGammaFit::integrate() 
{ 
  Bayespack bayes;
  double y = bayes.integrate(_mode, _N, flogp);

  _mean  = bayes.results();
  vdouble c = bayes.covariance();
  int k=0;
  for(int i=0; i < _N; i++)
    for(int j=0; j <= i; j++)
      {
	_cov[i][j] = c[k];
	_cov[j][i] = c[k];
	k++;
      }
  for(int i=0; i < _N; i++) _width[i] = sqrt(_cov[i][i]);

  if ( bayes.status() == 0 )
    _status = kSUCCESS;
  else
    _status = kNOTCONVERGED;
}
*/
