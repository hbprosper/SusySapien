// ----------------------------------------------------------------------------
// Fit to data in Razor plane
// Created: 10 February 2013 HBP & SS
// ----------------------------------------------------------------------------
#define FIT 0 // 0 for razor; 1 for razorrz


#include "Math/WrappedFunction.h"
#include "Math/IntegratorMultiDim.h"

#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TMinuit.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TMath.h"

#if FIT == 0
#include "bfunction.C"
#elif FIT == 1
#include "bfunctionrz.C"
#endif

// ----------------------------------------------------------------------------
const int MAXITER=5000;
const double TOLERANCE=5.e-4;

#if FIT == 0
int NPAR=7;
double P[7];
double eP[7];
#elif FIT == 1
int NPAR=5;
double P[5];
double eP[5];
#endif

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

void draw1d(TH1D* h1, TH1D* h2);


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
      double y = (N[j] - T)/EN[j];
      f += y*y;
    }
  return f;
}

double nll(double* p)
{
  double f = 0;
  for(int j=0; j < NBINS; j++)
    {
      double T = model(XMIN[j], XMAX[j], YMIN[j], YMAX[j], p);
      //cout << " T " << T << endl;
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

void fitrazor(string filename = "BorisQCD.root", int nb = 20)
{
  string fit = "";
  if(FIT == 0)
    fit = "razor";
  else if(FIT == 1)
    fit = "razorrz";

  cout << endl << "\t<===== fitrazor ======>" << endl;
  cout << " \t Doing fit for background function " << fit << endl;
  //gROOT->ProcessLine(".L style.C+");
  //setStyle();
  gStyle->SetOptFit(0);

  // ------------------------------------------------------------
  // Initialize global variables
  // ------------------------------------------------------------
 
  TFile* rfile = new TFile(filename.c_str());
  TH2D* h_MR_Rsq = (TH2D*)rfile->Get("h_MR_Rsq");

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
  int nbins = nb;

  TH2D* h_MR_Rsq_dt = new TH2D("h_MR_Rsq_dt", "h_MR_Rsq_dt", nbins, 0, 1, nbins, 0, 1);
  TH2D* h_MR_Rsq_md = new TH2D("h_MR_Rsq_md", "h_MR_Rsq_md", nbins, 0, 1, nbins, 0, 1);
  TH2D* h_MR_Rsq_rs = new TH2D("h_MR_Rsq_rs", "h_MR_Rsq_rs", nbins, 0, 1, nbins, 0, 1);

  TH1D* h_MR_dt = new TH1D("h_MR_dt", "h_MR_dt", nbins, 0, 1);
  TH1D* h_MR_md = new TH1D("h_MR_md", "h_MR_md", nbins, 0, 1);
  TH1D* h_MR_rs = new TH1D("h_MR_rs", "h_MR_rs", nbins, 0, 1);

  TH1D* h_Rsq_dt = new TH1D("h_Rsq_dt", "h_Rsq_dt", nbins, 0, 1);
  TH1D* h_Rsq_md = new TH1D("h_Rsq_md", "h_Rsq_md", nbins, 0, 1);
  TH1D* h_Rsq_rs = new TH1D("h_Rsq_rs", "h_Rsq_rs", nbins, 0, 1);

  TH1D* h_rs = new TH1D("h_rs", "h_rs", 20, -2, 3);

  double xcont_dt[nbins];
  double ycont_dt[nbins];
  double xcont_md[nbins];
  double ycont_md[nbins];
  for (int i=0; i<nbins; i++) {
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
      in++;
    }
  }

  NBINS = in;

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
  // Will depend on the function to be used
  // ------------------------------------------------------------

  cout << "fit = " << fit << endl;
  if(fit == "razor"){
    if(NPAR != 7){
      cout << "NPAR must be set equal to 7" << endl;
      exit(0);
    }
    
    double A[] = {0,0,0,0,0,0,0};
    string Aname[] = {"A","B","C","D","E","F","G"};
    double Astep[] = {0.1,0.1,0.1,0.1,0.1,0.1,0.1};
    double Amin[] = {-2000,-1000,-1000,-2000,-1000,0,-1000};
    double Amax[] = {2000,1000,1000,2000,1000,100,1000};

    for(int i=0; i<NPAR; ++i)
      minuit.mnparm(i, Aname[i], A[i], Astep[i], Amin[i], Amax[i], ierflag);

  } else if(fit == "razorrz") {
    cout << "doing razorrz fit" << endl;
    if(NPAR != 5){
      cout << "NPAR must be set equal to 5" << endl;
      exit(0);
    }
    double A[] = {(-1358.8-MRmn)/MRrange, (-0.128-Rsqmn)/Rsqrange, 0.89, 2.4, 10};
    string Aname[] = {"X0","Y0","B0","N0","Ntot"};
    double Astep[] = {0.1,0.1,0.1,0.1,0.1};
    double Amin[] = {(-5000-MRmn)/MRrange, -500, 0., 0.5, 0};
    double Amax[] = {50, (10-Rsqmn)/Rsqrange, 15, 5, 1000};

    for(int i=0; i<NPAR; ++i)
      minuit.mnparm(i, Aname[i], A[i], Astep[i], Amin[i], Amax[i], ierflag);

  }
  

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


  //int in = 0;
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
      //cout << XMIN[in] << " " << XMAX[in] << " " << YMIN[in] << " " << YMAX[in] << endl;
      h_MR_Rsq_dt->SetBinContent(i,j,N[in]);
      double md = model(XMIN[in], XMAX[in], YMIN[in], YMAX[in], P);
      h_MR_Rsq_md->SetBinContent(i,j,md);
      double rs = (N[in] - md)/EN[in];
      h_MR_Rsq_rs->SetBinContent(i,j,rs);
      h_rs->Fill(rs);
      xcont_dt[j-1] = xcont_dt[j-1] + N[in];
      ycont_dt[i-1] = ycont_dt[i-1] + N[in];
      xcont_md[j-1] = xcont_md[j-1] + md;
      ycont_md[i-1] = ycont_md[i-1] + md;
      in++;
    }
  }


  for (int i=0; i<20; i++) {
    h_MR_dt->SetBinContent(i+1, xcont_dt[i]);
    h_Rsq_dt->SetBinContent(i+1, ycont_dt[i]);
    h_MR_md->SetBinContent(i+1, xcont_md[i]);
    h_Rsq_md->SetBinContent(i+1, ycont_md[i]);
  }


  TCanvas *c = new TCanvas("c", "c", 1200, 800);
  c->Divide(3,2);
  c->cd(1);
  h_MR_Rsq_dt->Draw("col4z");
  c->cd(2);
  h_MR_Rsq_md->Draw("col4z");
  c->cd(3);
  h_MR_Rsq_rs->Draw("col4z");
  TPad* p1 = (TPad*)c->cd(4);
  p1->SetLogy(1);
  draw1d(h_MR_dt, h_MR_md);
  TPad* p2 = (TPad*)c->cd(5);
  p2->SetLogy(1);
  draw1d(h_Rsq_dt, h_Rsq_md);
  c->cd(6);
  c->SetLogy(0);
  h_rs->Draw();

  string outfilename = fit + ".pdf";
  c->Print(outfilename.c_str());

}


void draw1d(TH1D* h1, TH1D* h2)
{

  double maxi = TMath::Max(h1->GetMaximum(), h2->GetMaximum());

  maxi = maxi*1.1;

  h1->SetMaximum(maxi);

  h1->SetLineColor(kRed);
  h2->SetLineColor(kBlue);

  h1->Draw();
  h2->Draw("same");

  double KS = h1->KolmogorovTest(h2);
  char tex[80];
  sprintf(tex, "KS: %f", KS);

  double chi2 = 0.;
  int ndof = 0;
  for (int i=1; i<h1->GetNbinsX()+1; i++){
    double n1 = h1->GetBinContent(i);
    double n2 = h2->GetBinContent(i);
    if(n1 >= 3){
      double x = ((n1-n2)*(n1-n2))/n2;
      chi2 = chi2 + x;
      ndof++;
    }
  }

  char tex2[80];
  sprintf(tex2, "Chi2/ndof: %.4f", chi2/ndof);

  TLegend *l = new TLegend(0.6, 0.70, 0.85, 0.85);
  l->SetFillStyle(0000);
  l->SetBorderSize(0);
  l->AddEntry(h1, "data", "l");
  l->AddEntry(h2, "model", "l");
  l->Draw("same");

  TLatex *t1 = new TLatex(0.6, 0.6, tex);
  t1->SetNDC();
  t1->Draw("same");

  TLatex *t2 = new TLatex(0.55, 0.55, tex2);
  t2->SetNDC();
  t2->Draw("same");

}
