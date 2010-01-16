//-----------------------------------------------------------------------------
#include "TF2.h"

bool first=true;
double 
f2(double* x, double* y)
{
  if ( first )
    {
      first = false;
      cout << "first call" << endl;
    }
  return exp(-0.5*(pow(*x - 0.5,2)-pow(*y - 0.5,2)));
}

void 
dumb()
{
  TCanvas* c = new TCanvas("c", "", 500, 500);

  //TF2* f = new TF2("f2", f2, 0, 1, 0, 1, 2);
  TF2* f = new TF2("f2", f2,0,1,0,1,2);
  f->Draw("surf");
  c->Update();
}
