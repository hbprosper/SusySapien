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
    double z1 = -(A1 * x + B1 * y + C1 * x * y + D1 * x * x + E1 * y * y + F1 * x * x * x + G1);
    //double z2 = -(A2 * x + B2 * y );
    double z2 = -(A2 * x + B2 * y + C2 * x * y + D2 * x * x + E2 * y * y + F2 * x * x * x );
    //cout << "";
    //cout << "pars: " << A1 << " " << B1 << " " << C1 << " " << D1 << " " << E1 << " " << F1 << " " << G1 << " " << A2 << " " << B2 << " " << C2 << " " << D2 << " " << E2 << " " << F2 << " " << K << endl;
    double bla = exp(-z1)+K*exp(-z2);
    //cout << "z1, z2: " << z1 << " " << z2 << " bla: " << bla << endl;
    return bla;
    //return z;
  }
  double operator()(double xmin, double xmax, double ymin, double ymax, double* p)
  {
    A1 = p[0];
    B1 = p[1];
    C1 = p[2];
    D1 = p[3];
    E1 = p[4];
    F1 = p[5];
    G1 = p[6];

    A2 = p[7];
    B2 = p[8];
    C2 = p[9];
    D2 = p[10];
    E2 = p[11];
    F2 = p[12];
    //G2 = p[13];

    K = p[13];

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
  double A1;
  double B1;
  double C1;
  double D1;
  double E1;
  double F1;
  double G1;

  double A2;
  double B2;
  double C2;
  double D2;
  double E2;
  double F2;
  //double G2;

  double K;
};
  
//BackgroundFunction model;


