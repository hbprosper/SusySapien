//-----------------------------------------------------------------------------
// File:        plot.C
// Description: Plot projections of a 3-D density modeled with a KDE
// Created:     01-Nov-2009 Harrison B. Prosper, Panjab University, Chandigarh
//-----------------------------------------------------------------------------
#include "TF2.h"

double 
fcn(double* pt, double* eta)
{
  return exp(-0.5*(pow(*pt - 0.5,2)+pow(*eta - 0.5,2)));
  //cout << "F: " << F << endl;

//   vector<double> inp(3);
//   inp[0] = *pt;
//   inp[1] = *eta;
//   inp[2] = 0.5;
//   return (*F)(inp);
}

void 
plot()
{
  gROOT->Reset();

  cout  << "\n\n\t\t*** plot KDE ***\n" << endl;
   
  setStyle();

  //------------------------------------
  // Instantiate the KDE object from
  // the class generated by build.C
  //------------------------------------
  jetdata  kde;
  jetdata* F = &kde;

  TCanvas* c  = new TCanvas("fig_pt_eta_KDE",  "pt_eta", 0,  200, 500, 500);
  TF2* f2 = new TF2("f2", fcn);
  c->cd();
  f2->Draw("cont");
  c->Update();

  //------------------------------------
  // Get data
  //------------------------------------
  vector<double> pt;
  vector<double> eta;
  vector<double> phi;
  loaddata("jetdata.dat", 20000, pt, eta, phi);

  int ndim = 3;
  int Npoint = pt.size()/2;
  
  //------------------------------------
  // Book histograms
  //------------------------------------
  int NBIN=50;

  TCanvas* cPt  = new TCanvas("fig_pt_KDE",  "Pt",    0,  10, 500, 500);
  TH1F* hpt  = bkhist("pt", "p_{T}", "", NBIN, 0, 1, kBlack, 1, kBlue);

  TCanvas* cEta = new TCanvas("fig_eta_KDE", "Eta", 400,  10, 500, 500);
  TH1F* heta = bkhist("eta", "#eta", "", NBIN, 0, 1, kBlack, 1, kBlue);

  TCanvas* cPhi = new TCanvas("fig_phi_KDE", "Phi", 800,  10, 500, 500);
  TH1F* hphi = bkhist("phi", "#phi", "", NBIN, 0, 1, kBlack, 1, kBlue);

  //------------------------------------
  // Fill histograms
  //------------------------------------
  for(int i=Npoint; i < (int)pt.size(); i++)
    {
      hpt->Fill(pt[i]);
      heta->Fill(eta[i]);
      hphi->Fill(phi[i]);

      if ( i % 100 == 0 )
        {
          cPt->cd();
          hpt->Draw("EP");
          cPt->Update();

          cEta->cd();
          heta->Draw("EP");
          cEta->Update();

          cPhi->cd();
          hphi->Draw("EP");
          cPhi->Update();
        }
    }

  //------------------------------------
  // Compute KDE at centers of bins, 
  // then overlay on histograms
  //------------------------------------
  double xpt[1000],  dpt[1000];
  double xeta[1000], deta[1000];
  double xphi[1000], dphi[1000];

  for(int j=0; j < NBIN; j++)
    {
      int bin=j+1;
      double width;
      double norm;
      double x;

      width   = hpt->GetBinWidth(bin);
      norm    = hpt->Integral() * width;
      xpt[j]  = hpt->GetBinLowEdge(bin) + 0.5*width;
      dpt[j]  = norm * kde(xpt[j], "pt");


      width   = heta->GetBinWidth(bin);
      norm    = heta->Integral() * width;
      xeta[j] = heta->GetBinLowEdge(bin) + 0.5*width;
      deta[j] = norm * kde(xeta[j], "eta");

      width   = hphi->GetBinWidth(bin);
      norm    = hphi->Integral() * width;
      xphi[j] = hphi->GetBinLowEdge(bin) + 0.5*width;
      dphi[j] = norm * kde(xphi[j], "phi");
    }

  TGraph* gpt  = bkgraph(NBIN, xpt,  dpt,  "p_{T}", "", 0, 1, kRed);
  cPt->cd();
  hpt->Draw("EP");
  gpt->Draw("L same");
  cPt->Update();
  cPt->SaveAs(".gif");

  TGraph* geta = bkgraph(NBIN, xeta, deta, "#eta",  "", 0, 1, kRed);
  cEta->cd();
  heta->Draw("EP");
  geta->Draw("L same");
  cEta->Update();
  cEta->SaveAs(".gif");

  TGraph* gphi = bkgraph(NBIN, xphi, dphi, "#phi",  "", 0, 1, kRed);
  cPhi->cd();
  hphi->Draw("EP");
  gphi->Draw("L same");
  cPhi->Update();
  cPhi->SaveAs(".gif");
}
