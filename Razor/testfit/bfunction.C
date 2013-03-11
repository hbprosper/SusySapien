// ----------------------------------------------------------------------------
// Fit to data in Razor plane
// Created: 10 February 2013 HBP & SS
// ----------------------------------------------------------------------------
#include <iostream>
#include "Math/WrappedFunction.h"
#include "Math/IntegratorMultiDim.h"
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// Model for spectrum
// ----------------------------------------------------------------------------

class BackgroundFunction
{
public:
  BackgroundFunction() {}
  ~BackgroundFunction() {}

  double function(const double* X)
  {
    double x = X[0];
    double y = X[1];
    double z = -(A * x + B * y + C * x * y + D * x * x + E * y * y + G * x * x * x + F);
    //cout << "pars: " << A << " " << B << " " << C << " " << D << " " << E << " " << F << endl;
    return exp(-z);
    //return z;
  }
  double operator()(double xmin, double xmax, double ymin, double ymax, double* p)
  {
    A = p[0];
    B = p[1];
    C = p[2];
    D = p[3];
    E = p[4];
    F = p[5];
    G = p[6];

    //cout << A << " " << B << " " << C << " " << D << " " << E << endl; 


    // Declare function to be integrated
    ROOT::Math::WrappedMemMultiFunction<BackgroundFunction,
      double (BackgroundFunction::*)(const double*)> 
      fn(*this, &BackgroundFunction::function, 2); // 2-dimensions

    // ROOT's N-D Integrator. For now, use defaults
    ROOT::Math::IntegratorMultiDim ifn(fn);
    
    double lower[2] = {xmin, ymin};    // lower bin boundaries
    double upper[2] = {xmax, ymax};    // upper bin boundaries
    
    return ifn.Integral(lower, upper);
  }
private:
  double A;
  double B;
  double C;
  double D;
  double E;
  double F;
  double G;
};
  
//BackgroundFunction model;


