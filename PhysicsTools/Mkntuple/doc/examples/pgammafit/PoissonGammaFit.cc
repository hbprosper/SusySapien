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
#include <stdlib.h>
#ifdef PROJECT_NAME
#include "PhysicsTools/Mkntuple/interface/PoissonGammaFit.h"
#else
#include "PoissonGammaFit.h"
#endif

#include <TMinuit.h>
#include <TMatrix.h>

#ifdef __WITH_CINT__
  ClassImp(PoissonGammaFit)
#endif

using namespace std;

//-----------------------------------------------------------------------------

typedef std::vector<float> vfloat;
typedef std::vector<double> vdouble;

double 
poissongamma(vdouble&	p,    // Weights "p_j" 
             vvdouble&	a,    // Counts  "A_ji" for up to 10 sources
             vdouble&	d,    // Counts  "D_i" for data.
             bool returnlog=false,  // return log(P) if true
             bool scale=true);     // Scale p_j if true  

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
    iflag = *grad; // some stupidity to avoid compiler warnings
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
PoissonGammaFit::PoissonGammaFit() {}

PoissonGammaFit::PoissonGammaFit(vdouble&	D,  // Counts  "D_i" for data.
                                 bool scale,
                                 int verbosity)
  : _D(D), _scale(scale), _verbosity(verbosity)
{
  if ( _verbosity > 0 )
    cout << "PoissonGammaFit - Started" << endl;

  _status = kSUCCESS;
 
 _M = _D.size();  // Number of bins    (M)

  if ( _M < 1 )
    {
      std::cout 
	<< "**Error - PoissonGammaFit - bin count is zero"
	<< std::endl;
      _status = kZEROBINS;
      return;      
    }
 
  // Get data count

  _nd = 0.0; 					
  for ( int i = 0; i < _M; i++ ) _nd += _D[i];
}

void 
PoissonGammaFit::add(vector<double>& source)
{
  _A.push_back(source);
  _a.push_back(source);
}

bool
PoissonGammaFit::execute(vector<double>& guess)
{
  _N = _A.size();  // Number of sources (N)  
  if ( _N < 1 )
    {
      std::cout 
        << "**Error - PoissonGammaFit - zero sources"
        << std::endl;
      _status = kZEROSRCS;
      return false;
    }

  if ( _N > MAXSRCS )
    {
      std::cout 
        << "**Error - PoissonGammaFit - too many sources: " << _N 
        << std::endl;
      _status = kTOOMANYSRCS;
      return false;
    }

  if ( _A[0].size() != (unsigned int)_M )
    {
      std::cout 
        << "**Error - PoissonGammaFit - mis-match in number of sources\n"
        << "size(D): " << _M << " differs from size(A[0]) = " 
        << _A[0].size()
        << std::endl;
      _status = kMMISMATCH;
      return false;
    }

  // Get total counts per source
  _cov.clear();
  _mode.clear();
  _mean.clear();
  _width.clear();
  _ns.clear();

  for ( int j = 0; j < _N; j++ )
    {
      _cov.push_back(vector<double>(_N));
      _mode.push_back(0);
      _mean.push_back(0);
      _width.push_back(0);
      _ns.push_back(0);
      for ( int i = 0; i < _M; i++ ) _ns[j] += _A[j][i];
    }

  // Ok, use Minuit to find the mode of marginal likelihood
  return _findmode(guess);
}

PoissonGammaFit::~PoissonGammaFit() {}

double 
PoissonGammaFit::logLikelihood(vdouble& p)
{
  for(int i=0; i < _N; i++) if (p[i] < 0) return 0;
  return poissongamma(p, _A, _D, true);
}

double 
PoissonGammaFit::logLikelihood(double* point)
{
  vdouble p(_N);
  copy(point, point+_N, p.begin());
  return poissongamma(p, _A, _D, true);
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


///////////////////////////////////////////////////////////////////////////////
// File:	poissongamma.cc
// Description:	x = poissongamma(p,a,d)
// 
//              compute the marginal likelihood:
//              a product of Poisson distributions
//              and a prior that is a product of gamma distributions.
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
//              Simple 1-D Application:
//              
//              d =  xsec * e_a * a + e_b * b 
//
//              where p_1 = xsec * e_a
//                    p_2 = e_b
//                    a_11= a
//                    a_21= b
//
//              and e_a, e_b are scale factors defined by
//              e = sigma^2 / estimate
//
// WARNING: The number of terms grows exponentially with the number of 
//          sources and the maximum bin count. Therefore, this routine
//          really only makes sense for rather few sources with modest
//          bin counts. 
//
// Created:     20-Dec-2001 Harrison B. Prosper
//              11-Mar-2004 HBP add additional interfaces
//              07-Jun-2005 HBP increase number of sources to 10! See warning
//                          above.
///////////////////////////////////////////////////////////////////////////////

const int MAXBUF = 50000; // Maximum count per bin
const int MAXSRC = 10;    // Maximum number of sources

// Global variables to avoid memory allocation.

static long double cs[MAXSRC][MAXBUF];
static double ns[MAXSRC];
static double si[MAXSRC];
static double x [MAXSRC];
static double y [MAXSRC];


double 
poissongamma(vdouble&	p,  // Weights "p_j" 
             vvdouble&	a,  // Counts  "A_ji" for up to 10 sources
             vdouble&	d,  // Counts  "D_i" for data.
             bool returnlog,    // return log(P) if true
             bool scale)     // Scale p_j if true  
{
  int N = p.size(); // Number of sources (N)
  int M = d.size(); // Number of bins    (M)

  if ( a.size() != (unsigned int)N )
    {
      std::cout << "**Error - poissongamma - mis-match in number of sources\n"
                << "size(p): " << N << " differs from size(A) = " << a.size()
                << std::endl;
      exit(0);
    }

  if ( a[0].size() != (unsigned int)M )
    {
      std::cout << "**Error - poissongamma - mis-match in number of sources\n"
                << "size(d): " << M << " differs from size(A[0]) = " 
                << a[0].size()
                << std::endl;
      exit(0);
    }

  if ( M < 1 ) return -1.0;
  
  if ( ( N < 2 ) || ( N > MAXSRC ) ) return -2.0;
  
  // Get total counts per source

  for ( int j = 0; j < N; j++ )
    {
      ns[j] = 0;
      for ( int i = 0; i < M; i++ ) ns[j] += a[j][i];
    }

  // Get data count

  double nd = 0.0; 					
  for ( int i = 0; i < M; i++ ) nd += d[i];

  // Scale counts

  for ( int j = 0; j < N; j++ )
    {
      if ( scale )
        x[j] = p[j] / (ns[j]+M);
      else
        x[j] = p[j];
      y[j] = x[j] / (1+x[j]);
    }

  // loop over the M terms of the product,
  // corresponding to the M bins

  double prob;
  if ( returnlog )
    prob = 0.0;
  else
    prob = 1.0;
  for ( int i = 0; i < M; i++)
    {
      double di = d[i];  // data count for bin i
      int D = (int)di;

      // compute terms of sum from zero to D
      
      for ( int j = 0; j < N; j++ )
        {
          si[j]    = a[j][i];  // "A_ji"
          cs[j][0] = pow(1+x[j],-(si[j]+1));
        }
      
      if ( D > 0 )
        {
          for ( int k = 1; k < D+1; k++ )
            for ( int j = 0; j < N; j++ )
              cs[j][k] = cs[j][k-1] * y[j] * (si[j]+k)/k;
        }

      // compute sum

      double sum = 0.0;
      switch (N)
        {
        case 2:
          for (int j = 0; j < D+1; j++)
            sum += 
              cs[0][j] *
              cs[1][D-j];
          break;

        case 3:
          for (int j = 0; j < D+1; j++)
            for (int k = 0; k < D+1-j; k++)
              sum += 
                cs[0][j] *
                cs[1][k] *
                cs[2][D-j-k];
          break;

        case 4:
          for (int j = 0; j < D+1; j++)
            for (int k = 0; k < D+1-j; k++)
              for (int l = 0; l < D+1-j-k; l++)
                sum += 
                  cs[0][j] *
                  cs[1][k] *
                  cs[2][l] *
                  cs[3][D-j-k-l];
          break;

        case 5:
          for (int j = 0; j < D+1; j++)
            for (int k = 0; k < D+1-j; k++)
              for (int l = 0; l < D+1-j-k; l++)
                for (int m = 0; m < D+1-j-k-l; m++)
                  sum += 
                    cs[0][j] *
                    cs[1][k] *
                    cs[2][l] *
                    cs[3][m] *
                    cs[4][D-j-k-l-m];
          break;

        case 6:
          for (int j = 0; j < D+1; j++)
            for (int k = 0; k < D+1-j; k++)
              for (int l = 0; l < D+1-j-k; l++)
                for (int m = 0; m < D+1-j-k-l; m++)
                  for (int m1 = 0; m1 < D+1-j-k-l-m; m1++)
                    sum += 
                      cs[0][j] *
                      cs[1][k] *
                      cs[2][l] *
                      cs[3][m] *
                      cs[4][m1] *
                      cs[5][D-j-k-l-m-m1];
          break;

        case 7:
          for (int j = 0; j < D+1; j++)
            for (int k = 0; k < D+1-j; k++)
              for (int l = 0; l < D+1-j-k; l++)
                for (int m = 0; m < D+1-j-k-l; m++)
                  for (int m1 = 0; m1 < D+1-j-k-l-m; m1++)
                    for (int m2 = 0; m2 < D+1-j-k-l-m-m1; m2++)
                      sum += 
                        cs[0][j] *
                        cs[1][k] *
                        cs[2][l] *
                        cs[3][m] *
                        cs[4][m1] *
                        cs[5][m2] *
                        cs[6][D-j-k-l-m-m1-m2];
          break;

        case 8:
          for (int j = 0; j < D+1; j++)
            for (int k = 0; k < D+1-j; k++)
              for (int l = 0; l < D+1-j-k; l++)
                for (int m = 0; m < D+1-j-k-l; m++)
                  for (int m1 = 0; m1 < D+1-j-k-l-m; m1++)
                    for (int m2 = 0; m2 < D+1-j-k-l-m-m1; m2++)
                      for (int m3 = 0; m3 < D+1-j-k-l-m-m1-m2; m3++)
                        sum += 
                          cs[0][j] *
                          cs[1][k] *
                          cs[2][l] *
                          cs[3][m] *
                          cs[4][m1] *
                          cs[5][m2] *
                          cs[6][m3] *
                          cs[7][D-j-k-l-m-m1-m2-m3];
          break;

        case 9:
          for (int j = 0; j < D+1; j++)
            for (int k = 0; k < D+1-j; k++)
              for (int l = 0; l < D+1-j-k; l++)
                for (int m = 0; m < D+1-j-k-l; m++)
                  for (int m1 = 0; m1 < D+1-j-k-l-m; m1++)
                    for (int m2 = 0; m2 < D+1-j-k-l-m-m1; m2++)
                      for (int m3 = 0; m3 < D+1-j-k-l-m-m1-m2; m3++)
                        for (int m4 = 0; m4 < D+1-j-k-l-m-m1-m2-m3; m4++)
                          sum += 
                            cs[0][j] *
                            cs[1][k] *
                            cs[2][l] *
                            cs[3][m] *
                            cs[4][m1] *
                            cs[5][m2] *
                            cs[6][m3] *
                            cs[7][m4] *
                            cs[8][D-j-k-l-m-m1-m2-m3-m4];
          break;

        case 10:
          for (int j = 0; j < D+1; j++)
            for (int k = 0; k < D+1-j; k++)
              for (int l = 0; l < D+1-j-k; l++)
                for (int m = 0; m < D+1-j-k-l; m++)
                  for (int m1 = 0; m1 < D+1-j-k-l-m; m1++)
                    for (int m2 = 0; m2 < D+1-j-k-l-m-m1; m2++)
                      for (int m3 = 0; m3 < D+1-j-k-l-m-m1-m2; m3++)
                        for (int m4 = 0; m4 < D+1-j-k-l-m-m1-m2-m3; m4++)
                          for (int m5 = 0; m5 < D+1-j-k-l-m-m1-m2-m3-m4; m5++)
                            sum += 
                              cs[0][j] *
                              cs[1][k] *
                              cs[2][l] *
                              cs[3][m] *
                              cs[4][m1] *
                              cs[5][m2] *
                              cs[6][m3] *
                              cs[7][m4] *
                              cs[8][m5] *
                              cs[9][D-j-k-l-m-m1-m2-m3-m4-m5];
          break;
        };
      if ( returnlog )
        prob += log(sum);
      else
        prob *= sum;
    }
  return prob;
}
