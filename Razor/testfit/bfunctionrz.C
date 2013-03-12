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

    double z = B0 * pow( fabs((x-X0)*(y-Y0)), 1.0/N0);
    //cout << "z: " << z << endl;
    //if (z < 0) cout << "SCREAAAAAMMMMMM!" << endl;
    //if (z != z)

    double N0z = N0*z;

    double func = 1000*Ntot*(z-1)*exp(-N0z);
    //cout << "pars: " << X0 << " " << Y0 << " " << B0 << " " << N0 << " " << x << " " << y << " " 
    //	 << N0z << " " << endl;
    //cout << "func: " << func << endl;

    return func;

    //double z = -(A * x + B * y + C * x * y + D * x * x + E * y * y + F);
    //return exp(-z);
    //return z;
  }
  double operator()(double xmin, double xmax, double ymin, double ymax, double* p)
  {


    X0 = p[0];
    Y0 = p[1];
    B0 = p[2];
    N0 = p[3];
    Ntot = p[4];

    //cout << A << " " << B << " " << C << " " << D << " " << E << endl; 


    // Declare function to be integrated
    ROOT::Math::WrappedMemMultiFunction<BackgroundFunction,
      double (BackgroundFunction::*)(const double*)> 
      fn(*this, &BackgroundFunction::function, 2); // 2-dimensions

    // ROOT's N-D Integrator. For now, use defaults
    ROOT::Math::IntegratorMultiDim ifn(fn);
    
    double lower[2] = {xmin, ymin};    // lower bin boundaries
    double upper[2] = {xmax, ymax};    // upper bin boundaries
    
    double intg = ifn.Integral(lower, upper);
    //cout << "integral : " << intg << endl;
    return intg;

  }
private:

  double X0;
  double Y0;
  double B0;
  double N0;
  double Ntot;

};
  
//BackgroundFunction model;


