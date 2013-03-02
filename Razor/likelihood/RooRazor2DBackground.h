//---------------------------------------------------------------------------
#ifndef RooRazor2DBackground_h
#define RooRazor2DBackground_h
//---------------------------------------------------------------------------
// Compute Razor background function within a rectangle in the Razor plane
// Created: 27 Feb 2013 HBP & SS
//---------------------------------------------------------------------------
#include "RooAbsReal.h"
#include "RooRealProxy.h"
#include "TMath.h"
#include "Math/SpecFuncMathCore.h"
#include "Math/WrappedFunction.h"
#include "Math/IntegratorMultiDim.h"

//#include "TRIGGER FUNCTION HEADER GOES HERE"

//---------------------------------------------------------------------------
class RooRazor2DBackground : public RooAbsReal
{
public:
  RooRazor2DBackground() {}
  RooRazor2DBackground(const char *name, const char *title,
                       RooAbsReal &_x,  RooAbsReal &_y, 
                       RooAbsReal &_x0, RooAbsReal &_y0,
                       RooAbsReal &_b,  RooAbsReal &_n,
                       int _code=0);

  RooRazor2DBackground(const RooRazor2DBackground& other,
                       const char* name = 0);

  virtual TObject* clone(const char* newname) const 
  { return new RooRazor2DBackground(*this, newname); }

  inline virtual ~RooRazor2DBackground() {}

protected:

  // Observables
  RooRealProxy X;
  RooRealProxy Y;
  
  // Parameters
  RooRealProxy X0;    // X offset
  RooRealProxy Y0;    // Y offset
  RooRealProxy B;     // shape parameter
  RooRealProxy N;     // shape parameter

  int code;           // code (0 - numerical, 1 - analytical)

  double evaluate() const;

private:

  // -------------------------------------------------------------------
  // code for analytical integration (without trigger function)

  inline double Gamma(const Double_t a, const Double_t x) const
  {
    return TMath::Gamma(a) * ROOT::Math::inc_gamma_c(a,x);
  }
  
  inline double Gfun(const Double_t x, const Double_t y) const
  {
    return Gamma(N,B*N*pow(x-X0,1/N)*pow(y-Y0,1/N));
  }

  // -------------------------------------------------------------------
  // background function

  inline double bfunc(const double* x)
  {
    double z = B * pow( fabs((x[0]-X0)*(x[1]-Y0)), 1./N);
    double Nz = N*z;
    
    if ( Nz > 700 )
      {
        std::cout << "** RooRazor2DBackground ** N*z = " << Nz 
                  << " is too large" 
                  << std::endl
                  << "\tMR0=" << X0 
                  << " R0=" << Y0
                  << " B0=" << B
                  << " N0=" << N 
                  << std::endl
                  << "\tMR=" << x[0]
                  << "Rsq=" << x[1] 
                  << std::endl;
        return 1.7e-308;
      }
    if ( z < 1 )
      {
        std::cout << "** RooRazor2DBackground ** z = " << z << " < 1"
                  << std::endl
                  << "\tMR0=" << X0 
                  << " R0=" << Y0
                  << " B0=" << B
                  << " N0=" << N << std::endl
                  << "\tMR=" << x[0]
                  << "Rsq=" << x[1]
                  << std::endl;
        return 1.7e-308;
      }

    return (z-1)*exp(-Nz);
  }
 
  // numerical integration of background function over rectangular bin

  inline double numerical2D(double xmin, double xmax, double ymin, double ymax)
  {
    // Declare function to be integrated
    ROOT::Math::WrappedMemMultiFunction<RooRazor2DBackground,
      double (RooRazor2DBackground::*)(const double*)> 
      fn(*this, &RooRazor2DBackground::bfunc, 2); // 2-dimensions

    // ROOT's N-D Integrator. For now, use defaults
    ROOT::Math::IntegratorMultiDim ifn(fn);
    
    double lower[2] = {xmin, ymin};    // lower bin boundaries
    double upper[2] = {xmax, ymax};    // upper bin boundaries
    
    return ifn.Integral(lower, upper);
  }


  ClassDef(RooRazor2DBackground,1)    
};

//---------------------------------------------------------------------------
#endif
