//-----------------------------------------------------------------------------
// File:	PoissonGamma.cpp
// Description: Compute (by MC integration) the marginal likelihood:
//              a product of Poisson distributions
//              and a prior that is a product of gamma distributions.
//
//              Model:
//
//              d_i = Sum_j p_j * a_ji,  i=1..M,   j=1..N
// 
//              where p_j = n_j / Sum_k a_jk, k=1..M.
// 
//              and n_j is the mean count of source j.
//
//              D_i  is count corresponding to true mean d_i
//              A_ji is effective count corresponding to true mean a_ji
//
// Usage:
//        PoissonGamma p(a, total)
//        x = p.density(n, D)
//
// Created:     21-Jun-2005 Harrison B. Prosper
// Updated:     14-May-2008 Use RandGamma from CLHEP
//-----------------------------------------------------------------------------
#include <cmath>
#include "PhysicsTools/LiteAnalysis/interface/PoissonGamma.hpp"
#include "CLHEP/Random/RandGamma.h"
#include "CLHEP/Random/MTwistEngine.h"
//-----------------------------------------------------------------------------
using namespace CLHEP;

PoissonGamma::PoissonGamma(vvdouble& A, 
			   int total) 
  : _A(A),                // Counts  "A_ji" 
    _N(_A.size()),        // Number of sources (N)
    _M(_A[0].size()),     // Number of bins    (M)
    _total(total),        // Number of MC sampling points
    _a(vvvdouble())
{ 
  MTwistEngine* engine = new MTwistEngine();
  for (int k = 0; k < _total; k++)
    {
      _a.push_back( vvdouble() );
      for (int j = 0; j < _N; j++)
	{
	  _a[k].push_back(vdouble(_M));
	  for (int i = 0; i < _M; i++)
	    {
	      double q = static_cast<double>(_A[j][i] + 1);
	      _a[k][j][i] = RandGamma::shoot(engine, q, 1);
	    }
	}
    }
}

PoissonGamma::~PoissonGamma() {}

double 
PoissonGamma::density(vdouble&	n,  // Weights "n_j" 
		      vdouble&	D)  // Counts  "D_i" for data.
{
  int N = n.size(); // Number of sources (N)  
  if ( _N != N )
    {
      std::cout << "**Error - PoissonGamma - mis-match in number of sources\n"
		<< "size(n): " << N << " differs from size(A) = " << _N
		<< std::endl;
      exit(0);
    }

  int M = D.size(); // Number of bins    (M)
  if ( _M != M )
    {
      std::cout << "**Error - PoissonGamma - mis-match in number of sources\n"
		<< "size(D): " << M << " differs from size(A[0]) = " 
		<< _M
		<< std::endl;
      exit(0);
    }
  
  // Loop over the swarm of points that represent the prior and
  // compute, 

  double prob = 0;
  for (int k = 0; k < _total; k++)
    {
      // Loop over M bins
      double p = 1.0;
      for ( int i = 0; i < _M; i++ )
	{
	  // Compute Poisson parameter for ith bin
	  // d_i = Sum_j n_j a_ji / Sum_r a_jr
	  double d = 0;
	  for ( int j = 0; j < _N; j++ )
	    { 
	      double asumj = 0;
	      for (int r = 0; r < _M; r++) asumj += _a[k][j][r];
	      d += n[j] * _a[k][j][i] / asumj;
	    }

	  // Compute Poisson probability for current bin
	  double q = exp(-d);
	  int Di = static_cast<int>(D[i]);
	  if ( Di > 0 ) for ( int r = 1; r <= Di; r++ ) q *= d / r;
	  p *= q;
	}
      prob += p;
    }
  prob /= _total;
  return prob;
}

