#ifndef POISSONGAMMA_HPP
#define POISSONGAMMA_HPP
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
#include <vector>

typedef std::vector<double>   vdouble;
typedef std::vector<vdouble>  vvdouble;
typedef std::vector<vvdouble> vvvdouble;

/// Poisson/Gamma model
class PoissonGamma
{
public: 

  ///
  PoissonGamma(vvdouble& A,
	       int total=10000);

  ///
  ~PoissonGamma();

  ///
  double     density(vdouble& n,
		     vdouble& D);

private:

  vvdouble  _A;
  int       _N;
  int       _M;
  int       _total;
  vvvdouble _a;
};

#endif
