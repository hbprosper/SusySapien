#ifndef POISSONGAMMAFIT_H
#define POISSONGAMMAFIT_H
///////////////////////////////////////////////////////////////////////////////
// File:    PoissonGammaFit.h
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

/** Fit Poisson/gamma model to data.
    The data consists of a sequence of \f$i = 1 \ldots M\f$ counts 
    \f$D_i\f$ (i.e., a histogram),
    presumed to arise from a mixture of sources, say \f$t\bar{t}\f$,
    \f$W + jets\f$, and \f$QCD\f$ events. The goal is to find the yield of
    each source, given \f$j = 1 \ldots N\f$ sequences of counts 
    \f$A_{ji}\f$ that describe the ``shapes" of the \f$N\f$ sources.
    In other words, given a data histogram \f$D\f$ and \f$N\f$ histograms 
    \f$A_j = A_{j1}, \ldots, A_{jM}\f$, one for each source \f$j\f$, 
    we want to find the yield of each. This class solves the
    problem using a Bayesian approach \ref bayes1 "[1]".
<p>
    <b>Model</b>
    <p>
    - <i>Likelihood</i> - The likelihood function is assumed to be given by
    \f[ 
    p(D|p, a) = \prod_{i=1}^M\prod_{j=1}^N \mbox{Poisson}(D_i|d_i),
    \f]
    where
    <br>
    \f$ d_i = \sum_{j=1}^N \, p_j \,  a_{ji},  \, \, \, i=1 \ldots M.\f$
    <br>
    The unknown parameters \f$p_j\f$ and \f$a_{ji}\f$ are 
    the source ``strengths" and
    expected source counts, respectively.
    <p>
    - <i>Prior</i> - The prior is factorized as follows 
    \f$
    \pi(p, a) = \pi(a|p) \, \pi(p)
    \f$ and 
    \f$\pi(a|p)\f$ is assumed to be a product of gamma densities:
    \f[ p(a|p) = \prod_{i=1}^M\prod_{j=1}^N 
    \mbox{gamma}(a_{ji}|A_{ji}+1, 1),
    \f]
    where
    \f[
    \mbox{gamma}(x|\alpha,\beta) = 
    \beta^{\alpha} x^{\alpha-1} e^{-\beta x} / \Gamma(\alpha).
    \f]
    <p>
    - <i>Posterior</i> - The posterior density of the source ``strengths" is
    given by 
    \f[
    p(p|D) = p(D|p) \, \pi(p) \, / \, \int p(D|p) \, \pi(p) \, dp,
    \f]
    where the marginal likelihood \f$p(D|p)\f$, that is, \f$p(D|p, a)\f$
    integrated with respect to the nuisance parameters \f$a_{ji}\f$,
    is given in Ref.\ref bayes1 "[1]".
    
    The form of the prior follows from the assumption that each count 
    \f$A_{ji}\f$ is associated with a Poisson distribution and that the
    the magnitude of the count \f$A_{ji}\f$ is such that the 
    likelihood dominates
    the prior. When that assumption is not justified, a reasonable 
    procedure is to use
    Jeffreys prior\ref bayes2 "[2]",
which leads to the replacement of
    \f$A_{ji}\f$ with \f$A_{ji}-1/2\f$. For most practical applications, 
    however, this will not make much of a difference.
    <p>
    <b>Example</b>

<p>
<b>References</b>
<br>
\anchor bayes1 [1] P.C. Bhat, H.B. Prosper, and 
S.S. Snyder, <i>Bayesian analysis of multisource data</i>, 
Phys. Lett. B<b>407</b>:73-78, 1997, 
<a href="http://lss.fnal.gov/archive/test-preprint/fermilab-pub-96-397.shtml">
FERMILAB-PUB-96-397</a>.
<br>
\anchor bayes2 [2] L. Demortier, S. Jain, and H.B. Prosper,
    <i>Reference priors for high energy physics</i>, 
    Phys. Rev. D<b>82</b>:034002, 2010, 
<a href="http://arxiv.org/pdf/1002.1111">e-Print: arXiv:1002.1111</a>.
 */
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
  PoissonGammaFit();

  ///
  PoissonGammaFit(vdouble&	D,      // Data vector
                  bool scale=true,  // Scale so that "p_j" are counts
                  int  verbosity=0);
  ///
  virtual ~PoissonGammaFit();

  /// True if all is well.
  bool       good();

  /// Add a source
  void       add(vdouble& source);

  /// Execute the fit.
  bool       execute(std::vector<double>& guess);

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

  ///
  double     logLikelihood(double* x);

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
