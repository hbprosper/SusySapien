//---------------------------------------------------------------------------
// File: RooRazor2DBackground.cc
// Description:
// Compute Razor background function within a rectangle in the Razor plane
// Created: 27-Feb-2013 based on RooRazor2DTail_SYS.cc HBP & SS
// Updated: 03-Mar-2013 HBP - use valueOK method to check values of z and N*z
//$Revision: 1.2 $
//---------------------------------------------------------------------------
#include <iostream>
#include "RooFit.h"
#include "RooRazor2DBackground.h"
//---------------------------------------------------------------------------
ClassImp(RooRazor2DBackground)
//---------------------------------------------------------------------------

RooRazor2DBackground::RooRazor2DBackground(const char *name, const char *title,
                                           RooAbsReal &_x,  RooAbsReal &_y,
                                           RooAbsReal &_x0, RooAbsReal &_y0, 
                                           RooAbsReal &_b,  RooAbsReal &_n,
                                           int _code)
  : RooAbsReal(name, title), 
  X("X",  "X observable", this, _x),
  Y("Y",  "Y observable", this, _y),
  X0("X0","X Offset", this, _x0),
  Y0("Y0","Y Offset", this, _y0),
  B("B",  "shape parameter", this, _b),
  N("N",  "shape parameter", this, _n),
  code(_code),
  underflowCount(0),
  negativeCount(0)
{  
}

//---------------------------------------------------------------------------
RooRazor2DBackground::RooRazor2DBackground(const RooRazor2DBackground& other, 
                                           const char* name) 
  : RooAbsReal(other, name), 
    X("X",  this, other.X), 
    Y("Y",  this, other.Y), 
    X0("X0",this, other.X0),
    Y0("Y0",this, other.Y0),
    B("B",  this, other.B),
    N("N",  this, other.N),
    code(other.code),
    underflowCount(other.underflowCount),
    negativeCount(other.negativeCount)
{
}
//---------------------------------------------------------------------------
double RooRazor2DBackground::evaluate() const
{
  double integral = 0;
  double xmin = X.min();
  double xmax = X.max();
  double ymin = Y.min();
  double ymax = Y.max();

  //std::cout << "b1 xmn xmx ymn ymx: " << xmin << " " << xmax << " " << ymin << " " << ymax << std::endl;
  //  std::cout << "X,Y,X0,Y0,B,N: " << X << " " << Y << " " << X0 << " " << Y0 << " " 
  //    << B << " " << N << " " << std::endl; 
  //std::cout << "X0,Y0,B,N: " << X0 << " " << Y0 << " " << B << " " << N << " " << std::endl; 

  // check values of z and N*Z at center of bin
  double z, Nz;
  if ( ! ((RooRazor2DBackground*)this)->valuesOK(0.5*(xmin+xmax), 0.5*(ymin+ymax), z, Nz) ) return 1.7e-308;

  switch (code)
    {
    default:
    case 0:
      {
        // (RooRazor2DBackground*)this is a hack to discard "const"

        integral = ((RooRazor2DBackground*)this)->numerical2D(xmin, 
                                                              xmax,
                                                              ymin,
                                                              ymax);
      }
      break;

    case 1:
      {
        integral 
          = (N/pow(B*N, N))
          * (Gfun(xmin, ymin) -
             Gfun(xmin, ymax) - 
             Gfun(xmax, ymin) + 
             Gfun(xmax, ymax));
      }
      break;
    }
  return integral;
}


