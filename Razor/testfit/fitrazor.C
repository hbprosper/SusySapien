// ----------------------------------------------------------------------------
// Fit to data in Razor plane
// Created: 10 February 2013 HBP & SS
// ----------------------------------------------------------------------------
const int MAXITER=5000;
const double TOLERANCE=5.e-4;

int NPAR=5;
int NBINS=0;
double N[5000];
double EN[5000];
double X[5000];
double Y[5000];
double EY[5000];
// ----------------------------------------------------------------------------
// Model for spectrum
// ----------------------------------------------------------------------------
double model(double* x, double* p)
{
  // compute background
  return b;
}

double chisq(double* p)
{
  double f = 0;
  for(int j=0; j < NBINS; j++)
    {
      double T = model(&X[j], p);
      double Y = (N[j] - T)/EN[j];
      f += Y*Y;
    }
  return f;
}

// function to be minimized
void errFunc(int&    npar, 
             double* grad, 
             double& fval,
             double* xval,
             int     iflag)
{
  fval  = chisq(xval);
  iflag = *grad; // some stupidity to avoid compiler warnings
  iflag = npar;  // more stupidity to avoid compiler warnings
}

void fitrazor()
{
  cout << endl << "\t<===== fitrazor ======>" << endl;
  gROOT->ProcessLine(".L style.C");
  setStyle();
  gStyle->SetOptFit(0);

  // ------------------------------------------------------------
  // Initialize global variables
  // ------------------------------------------------------------
 
  // ------------------------------------------------------------
  // Perform fit
  // ------------------------------------------------------------
 
  TMinuit minuit(NPAR);
  //minuit.SetPrintLevel(-1);

  // NB: UP = 0.5 if using a log-likelihood
  //        = 1.0 if using a chisq (see binlimits.h)

  double UP = 1.0;
  minuit.SetFCN(errFunc);
  minuit.SetErrorDef(UP); 

  // ------------------------------------------------------------
  // Set parameters for fit
  // ------------------------------------------------------------
  double A     = 0;
  double Astep = 0.1;
  double Amin  = 0;
  double Amax  = 1000;
  minuit.mnparm(1, "A", A, Astep, Amin, Amax, ierflag);   

 
  //minuit.FixParameter(4);

  // Fit with MIGRAD
  double arglist[2];
  arglist[0] = MAXITER;
  arglist[1] = TOLERANCE;
  minuit.mnexcm("MIGRAD", arglist, 2, ierflag); 
  cout << endl << "fit status: " << ierflag << endl;
  if ( ierflag != 0 )
    {
      cout << "\t===> FIT FAILED" << endl;
      exit(0);
    }
  double fmin, fedm, errdef;
  int nvpar, nparx, istatus;
  minuit.mnstat(fmin, fedm, errdef, nvpar, nparx, istatus);
  cout << "covariance matrix status: " << istatus << endl;
  // ------------------------------------------------------------
  cout << endl << "Fit results:" << endl;
  sprintf(record, "fitresults.txt");
  ofstream fout(record);
  for(int k=0; k < NPAR; k++)
    { 
      minuit.GetParameter(k, P[k], eP[k]);
      sprintf(record, "%4d\t%15.6e %10.3e", k, P[k], eP[k]);
      cout << record << endl;
      fout << record << endl;
    }
  
  // Get error matrix
  double errmat[100];
  minuit.mnemat(errmat, NPAR);
  cout << endl;
  fout << endl;
  for(int k=0; k < NPAR; k++)
    { 
      for(int j=0; j < NPAR; j++)
        {
          sprintf(record, "%12.5e ", errmat[k*NPAR+j]);
          cout << record;
          fout << record;
        }
      cout << endl;
      fout << endl;
    }
  fout.close();
}

 
