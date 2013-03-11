// ----------------------------------------------------------------------------
// Fit to data in Razor plane
// Created: 10 February 2013 HBP & SS
// ----------------------------------------------------------------------------
#include "Math/WrappedFunction.h"
#include "Math/IntegratorMultiDim.h"
// ----------------------------------------------------------------------------
const int MAXITER=5000;
const double TOLERANCE=5.e-4;

int NPAR=7;
int NBINS=0;
double N[5000];
double EN[5000];
double X[5000];
double Y[5000];
double EY[5000];
double XMIN[5000];
double XMAX[5000];
double YMIN[5000];
double YMAX[5000];
double P[7];
double eP[7];


// ----------------------------------------------------------------------------
// Model for spectrum
// ----------------------------------------------------------------------------

//gROOT->ProcessLine(".L bfunction.C+");

BackgroundFunction model;

double integral2term(double xmin, double xmax, double ymin, double ymax, double A, double B)
{

  double px = A*(xmax*xmax - xmin*xmin)*(ymax - ymin);
  double py = B*(ymax*ymax - ymin*ymin)*(xmax - xmin);

  return 0.5*(px + py);
}

bool usechi = true;



double chisq(double* p)
{
  double f = 0;
  for(int j=0; j < NBINS; j++)
    {
      double T = model(XMIN[j], XMAX[j], YMIN[j], YMAX[j], p);
      double Y = (N[j] - T)/EN[j];
      f += Y*Y;
    }
  return f;
}

double nll(double* p)
{
  double f = 0;
  for(int j=0; j < NBINS; j++)
    {
      double T = model(XMIN[j], XMAX[j], YMIN[j], YMAX[j], p);
      cout << " T " << T << endl;
      f += T - N[j]*log(T);
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
  fval  = usechi ? chisq(xval) : nll(xval);
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
 
  TFile* rfile = new TFile("../likelihood/Borishist.root");
  TH2D* h_MR_Rsq = rfile->Get("h_MR_Rsq");

  int nxbn = h_MR_Rsq->GetXaxis()->GetNbins();
  int nybn = h_MR_Rsq->GetYaxis()->GetNbins();

  double MRmn = h_MR_Rsq->GetXaxis()->GetXmin();
  double MRmx = h_MR_Rsq->GetXaxis()->GetXmax();
  double MRrange = MRmx - MRmn;

  double Rsqmn = h_MR_Rsq->GetYaxis()->GetXmin();
  double Rsqmx = h_MR_Rsq->GetYaxis()->GetXmax();
  double Rsqrange = Rsqmx - Rsqmn;


  cout << MRmn << " " << MRmx << " " << Rsqmn << " " << Rsqmx << endl;


  // Make histograms:

  TH2D* h_MR_Rsq_dt = new TH2D("h_MR_Rsq_dt", "h_MR_Rsq_dt", 10, 0, 1, 10, 0, 1);
  TH2D* h_MR_Rsq_md = new TH2D("h_MR_Rsq_md", "h_MR_Rsq_md", 10, 0, 1, 10, 0, 1);
  TH2D* h_MR_Rsq_rs = new TH2D("h_MR_Rsq_rs", "h_MR_Rsq_rs", 10, 0, 1, 10, 0, 1);

  TH1D* h_MR_dt = new TH1D("h_MR_dt", "h_MR_dt", 10, 0, 1);
  TH1D* h_MR_md = new TH1D("h_MR_md", "h_MR_md", 10, 0, 1);
  TH1D* h_MR_rs = new TH1D("h_MR_rs", "h_MR_rs", 10, 0, 1);

  TH1D* h_Rsq_dt = new TH1D("h_Rsq_dt", "h_Rsq_dt", 10, 0, 1);
  TH1D* h_Rsq_md = new TH1D("h_Rsq_md", "h_Rsq_md", 10, 0, 1);
  TH1D* h_Rsq_rs = new TH1D("h_Rsq_rs", "h_Rsq_rs", 10, 0, 1);


  //exit(0);

  double p[7];

  p[0] = -4.001244e+00;
  p[1] = -8.638729e+00;
  p[2] = 3.172665e+00;
  p[3] = -5.202797e+00;
  p[4] = 3.209276e+00;
  p[5] = 9.960449e+00;
  p[6] = 4.995733e+00;


  double xcont_dt[10];
  double ycont_dt[10];
  double xcont_md[10];
  double ycont_md[10];
  for (int i=0; i<10; i++) {
    xcont_dt[i] = 0;
    ycont_dt[i] = 0;
    xcont_md[i] = 0;
    ycont_md[i] = 0;
  }

  int in = 0;
  for (int i=1; i<=h_MR_Rsq->GetXaxis()->GetNbins(); i++) {
    for (int j=1; j<=h_MR_Rsq->GetXaxis()->GetNbins(); j++) {
      //cout << i << " " << j << " " << h_MR_Rsq->GetBinContent(i,j) << endl;
      N[in] = h_MR_Rsq->GetBinContent(i,j);
      if (N[in] == 0) continue;
      //N[in] = log(N[in]);
      //EN[in] = 1./sqrt(N[in]);
      EN[in] = sqrt(N[in]);
      XMIN[in] = h_MR_Rsq->GetXaxis()->GetBinLowEdge(i);
      XMIN[in] = (XMIN[in] - MRmn) / MRrange;
      XMAX[in] = h_MR_Rsq->GetXaxis()->GetBinUpEdge(i);
      XMAX[in] = (XMAX[in] - MRmn) / MRrange;
      YMIN[in] = h_MR_Rsq->GetYaxis()->GetBinLowEdge(j);
      YMIN[in] = (YMIN[in] - Rsqmn) / Rsqrange;
      YMAX[in] = h_MR_Rsq->GetYaxis()->GetBinUpEdge(j);
      YMAX[in] = (YMAX[in] - Rsqmn) / Rsqrange;
      cout << XMIN[in] << " " << XMAX[in] << " " << YMIN[in] << " " << YMAX[in] << endl;
      h_MR_Rsq_dt->SetBinContent(i,j,N[in]);
      double md = model(XMIN[in], XMAX[in], YMIN[in], YMAX[in], p);
      h_MR_Rsq_md->SetBinContent(i,j,md);
      double rs = (N[in] - md)/EN[in];
      h_MR_Rsq_rs->SetBinContent(i,j,rs);
      xcont_dt[j-1] = xcont_dt[j-1] + N[in];
      ycont_dt[i-1] = ycont_dt[i-1] + N[in];
      xcont_md[j-1] = xcont_md[j-1] + md;
      ycont_md[i-1] = ycont_md[i-1] + md;
      in++;
    }
  }


  for (int i=0; i<10; i++) {
    h_MR_dt->SetBinContent(i+1, xcont_dt[i]);
    h_Rsq_dt->SetBinContent(i+1, ycont_dt[i]);
    h_MR_md->SetBinContent(i+1, xcont_md[i]);
    h_Rsq_md->SetBinContent(i+1, ycont_md[i]);
  }


  NBINS = in;

  TCanvas *c = new TCanvas("c", "c", 1200, 800);
  c->Divide(3,2);
  c->cd(1);
  h_MR_Rsq_dt->Draw("col4z");
  c->cd(2);
  h_MR_Rsq_md->Draw("col4z");
  c->cd(3);
  h_MR_Rsq_rs->Draw("col4z");
  c->cd(4);
  draw1d(h_MR_dt, h_MR_md);
  c->cd(5);
  draw1d(h_Rsq_dt, h_Rsq_md);

  /*
  in = 0;
  double p[6];
  p[0] = TMath::Pi();
  p[1] = TMath::E();
  p[2] = 0;
  p[3] = 0;
  p[4] = 0;
  p[5] = 0;
  cout << model(XMIN[0], XMAX[0], YMIN[0], YMAX[0], p) << endl;

  cout << integral2term(XMIN[0], XMAX[0], YMIN[0], YMAX[0], p[0], p[1]) << endl;

  exit(0);

  */

  // ------------------------------------------------------------
  // Perform fit
  // ------------------------------------------------------------
 
  TMinuit minuit(NPAR);
  //minuit.SetPrintLevel(-1);

  // NB: UP = 0.5 if using a log-likelihood
  //        = 1.0 if using a chisq (see binlimits.h)

  double UP = usechi ? 1.0 : 0.5;

  minuit.SetFCN(errFunc);
  minuit.SetErrorDef(UP); 
  int ierflag = 0;

  // ------------------------------------------------------------
  // Set parameters for fit
  // ------------------------------------------------------------
  //double A     = -1312;
  double A     = 0;
  double Astep = 0.1;
  double Amin  = -2000;
  double Amax  = 2000;
  minuit.mnparm(0, "A", A, Astep, Amin, Amax, ierflag);   

  //double B     = -533;
  double B     = 0;
  double Bstep = 0.1;
  double Bmin  = -1000;
  double Bmax  = 1000;
  minuit.mnparm(1, "B", B, Bstep, Bmin, Bmax, ierflag);   

  //double C     = 93.8;
  double C     = 0;
  double Cstep = 0.1;
  double Cmin  = -1000;
  double Cmax  = 1000;
  minuit.mnparm(2, "C", C, Cstep, Cmin, Cmax, ierflag);   

  //double D     = 1169;
  double D     = 0;
  double Dstep = 0.1;
  double Dmin  = -2000;
  double Dmax  = 2000;
  minuit.mnparm(3, "D", D, Dstep, Dmin, Dmax, ierflag);   

  //double E     = 483;
  double E     = 0;
  double Estep = 0.1;
  double Emin  = -1000;
  double Emax  = 1000;
  minuit.mnparm(4, "E", E, Estep, Emin, Emax, ierflag);   

  //double F     = 0;
  double F     = 0;
  double Fstep = 0.1;
  double Fmin  = 0;
  double Fmax  = 10;
  minuit.mnparm(5, "F", F, Fstep, Fmin, Fmax, ierflag);   

  //double G     = 0;
  double G     = 0;
  double Gstep = 0.1;
  double Gmin  = -1000;
  double Gmax  = 1000;
  minuit.mnparm(6, "G", G, Gstep, Gmin, Gmax, ierflag);   
 
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
  char record[100];
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


  // Get chisq:
  cout << "Chisq/d.o.f.: " << chisq(P)/(NBINS - NPAR) << endl; 


}

 
// Draw functions:

void draw1d(TH1D* h1, TH1D* h2)
{

  max = max(h1->GetMaximum(), h2->GetMaximum());

  max = max*1.1;

  h1->SetMaximum(max);

  h1->SetLineColor(kRed);
  h2->SetLineColor(kBlue);

  h1->Draw();
  h2->Draw("same");

}
