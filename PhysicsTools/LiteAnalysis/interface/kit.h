#ifndef KIT_H
#define KIT_H
// -*- C++ -*-
//
// Package:    PhysicsTools
// Class:      kit
// 
/**\class kit kit.cc 
   PhysicsTools/LiteAnalysis/src/kit.cc

 Description: A class of utilities. These functions are placed in a class 
              so that Reflex can handle overloading automatically. This is
	      just a collection of boilderplate code, written over the years, 
              to lessen clutter.
 
 Implementation:
     As simple as possible
*/
//
// Original Author:  Harrison B. Prosper
//         Created:  Fri Apr 04 2008
// $Id: kit.h,v 1.4 2008/10/31 02:02:53 harry Exp $
//
//$Revision:$
//-----------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <stdio.h>
//-----------------------------------------------------------------------------
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "TLorentzVector.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TMath.h"
//-----------------------------------------------------------------------------


typedef std::vector<double>  vdouble;
typedef std::vector<std::vector<double> > vvdouble;

struct MatchedPair
{
  int first;
  int second;
  double distance;
  bool operator<(const MatchedPair& o) const 
  { return this->distance < o.distance; }
};

// Simple struct to collect together standard attributes

// Note: Unlike a class, all attributes of a struct are public

struct PtThing
{
  PtThing() {}

  ~PtThing() {}

  // Copy constructor
  PtThing(const PtThing& rhs)
  {
    *this = rhs; // this will call assignment operator
  }

  // Assignment 
  PtThing& operator=(const PtThing& rhs)
  {
    index  = rhs.index;
    id     = rhs.id;
    type   = rhs.type;
    pt  = rhs.pt;
    eta = rhs.eta;
    phi = rhs.phi;
    et  = rhs.et;
    charge = rhs.charge;
    var = rhs.var;
    return *this;
  }

  float deltaR(PtThing& thing)
  {
    float deta = eta - thing.eta;
    float dphi = phi - thing.phi;
    
    // Make sure acute
    if ( fabs(dphi) > TMath::Pi() ) dphi = TMath::TwoPi() - fabs(dphi);
    return sqrt(deta*deta+dphi*dphi);
  }

  // Compare direction of this PtThing with another using deltaR

  bool matches(PtThing& thing, float drcut=0.4)
  {
    return deltaR(thing) < drcut;
  }

  int index;
  int id;
  std::string type;
  float pt;
  float eta;
  float phi;
  float et;
  float charge;

  // Map for additional variables
  std::map<std::string, float> var;

  // To sort in descending pt
  bool operator<(const PtThing& o) const { return o.pt < this->pt; }
};






//--------------------------------------------------------------------------
// A simple class to model a count. Each time the (function) object is 
// called, the count is incremented for the given string. The object is
// self-initializing; that is, their is no need for an explicit zeroing of
// the count.
//
// Usage:
// 
// Count count;
//
// increment count
// count("Selected Muon");
//
// To display the counts at the end, do
//
// count.ls();
//
//--------------------------------------------------------------------------
class Count
{
public:
  Count() : _var(std::map<std::string, int>()) {}
  ~Count(){}

  int operator()(std::string var) 
  {
    if ( _var.find(var) == _var.end() ) _var[var] = 0;
    _var[var]++;
    return _var[var];
  }

  // List counts. Default = list to screen

  void ls(std::ostream& os = std::cout)
  {
    int index = 0;
    for(std::map<std::string, int>::iterator 
          it = _var.begin(); it != _var.end(); it++)
      {
        index++;
        std::string var = it->first + 
          "   .......................................................";
        int count  = it->second;
        var = var.substr(0,40);
        char record[80];
        sprintf(record,"%5d %-40s %10d", index, var.c_str(), count);
        os << record << std::endl;
      }
  }

private:
  std::map<std::string, int> _var;
};

namespace {
  std::map<std::string, int> MAPSTRINT;
}

struct kit
{
  enum PDGID
  {
    BOTTOM  = 5,
    TOP     = 6,
    
    ELECTRON=11,
    NUE     =12,
    MUON    =13,
    NUMU    =14,
    TAU     =15,
    NUTAU   =16,

    PHOTON  =22,
    ZBOSON  =23,
    WBOSON  =24,
    
    GLUINO   =1000021,
    CHI10    =1000022,
    CHI20    =1000023,
    CHI30    =1000025,
    CHI40    =1000035,
    CHI1P    =1000024,
    CHI2P    =1000037,
    GRAVITINO=1000039
  };

  ///
  static
  float deltaPhi(float phi1, float phi2);

  ///
  static
  float deltaR(float eta1, float phi1, float eta2, float phi2);

  ///
  static
  std::vector<MatchedPair>  deltaR(std::vector<TLorentzVector>& v1, 
                                   std::vector<TLorentzVector>& v2);
  ///
  static
  std::vector<MatchedPair>  deltaR(std::vector<math::XYZVector>& v1, 
                                   std::vector<math::XYZVector>& v2);
  ///
  static
  std::vector<MatchedPair>  deltaR(std::vector<PtThing>& v1, 
                                   std::vector<PtThing>& v2);

  template <class T>
  static
  void sort(std::vector<T>& v)
  {
    std::sort(v.begin(), v.end());
  }

  ///
  template <class T>
  static
  bool  hasKey(std::map<std::string, T>& d, std::string var)
  {
    return d.find(var) != d.end();
  }

  ///
  static
  std::string      particleName(reco::GenParticle& p);

  ///
  static
  std::string      particleName(reco::GenParticle* p);

  ///
  static
  std::string      particleName(int PDGID);


  ///
  static
  TLorentzVector   lorentzVector(reco::GenParticle& p);

  ///
  static
  TLorentzVector   lorentzVector(reco::GenParticle* p);

  ///
  static
  TLorentzVector   lorentzVector(math::XYZVector& p);
  
  ///
  static
  TVector3         vector3(reco::GenParticle& p);
  
  ///
  static
  TVector3         vector3(math::XYZVector& p);

  ///
  static
  std::vector<reco::GenParticle*>  daughters(reco::GenParticle& p, 
                                             bool skipsame=true);

  ///
  static
  std::vector<reco::GenParticle*>  daughters(reco::GenParticle* p,
                                             bool skipsame=true);

  ///
  static
  int  find(std::vector<reco::GenParticle*>& p, int id, int start=0);

  ///
  static
  int  find(std::vector<reco::GenParticle>& p, int id, int start=0);
  
  ///
  static
  std::string               table(std::vector<reco::GenParticle>& vp);

  ///
  static
  std::string               tree(reco::GenParticle& p,
                                 int printLevel=0,
                                 int maxdepth=10,
                                 int depth=0);
  ///
  static
  void                      tree(std::ostream& os,
                                 reco::GenParticle& p,
                                 int printLevel=0,
                                 int maxdepth=10,
                                 int depth=0);

  ///
  static
  void                      tree(std::ostream& os,
                                 int    index,
                                 int    nhep,
                                 int*   pdgid,
                                 int*   status,
                                 double p[][5],
                                 int    d[][2],
                                 int printlevel=0, 
                                 int maxdepth=10, 
                                 int depth=0);
  ///
  static
  std::string               tree(int    index,
                                 int    nhep,
                                 int*   pdgid,
                                 int*   status,
                                 double p[][5],
                                 int    d[][2],
                                 int printlevel=0, 
                                 int maxdepth=10, 
                                 int depth=0);

  ///
  static
  std::string         nameonly(std::string name);

  ///
  static
  std::string         strip(std::string line);

  ///
  static
  std::string         truncate(std::string  s, 
                               std::string  substr,
                               int direction=1);

  ///
  void                bisplit(std::string  s, 
                              std::string& left, 
                              std::string& right, 
                              std::string  delim,
                              int direction=1);
  ///
  static
  void                split(std::string str, 
                            std::vector<std::string>& vstr);

  ///
  static
  std::string         shell(std::string cmd);

  ///
  static
  void                classMethods(std::string classname,
                                   std::vector<std::string>& methods,
                                   std::map<std::string, int>& m=MAPSTRINT,
                                   int depth=0);

  // -------------------
  // HISTOGRAM UTILITIES
  // -------------------

  ///
  static
  void                setStyle();

  ///
  static
  std::vector<double> contents(TH1F* hist);

  ///
  static
  std::vector<double> cdf(TH1F* hist);

  ///
  static
  void                setContents(TH1F* hist, std::vector<double>& c);

  ///
  static
  void                setErrors(TH1F* hist, std::vector<double>& err);

  ///
  static
  TH1F*               divideHistograms(TH1F* N, TH1F* D, std::string ytitle);

  
  ///
  static
  TCanvas*            canvas(std::string name, 
                             int n=0, 
                             int width=500,
                             int height=500);
  
  ///
  static
  TLegend*            legend(std::string title, int nlines, 
                             float xmin=0.62, 
                             float xmax=0.87, 
                             float ymax=0.93);
  
  ///
  static
  TH1F*               histogram(std::string hname,
                                std::string xtitle,
                                std::string ytitle,
                                int    nbin,
                                float  xmin,
                                float  xmax,
                                int    color=kBlack,
                                int    linestyle=1,
                                int    linewidth=1);
  
  ///
  static
  TH2F*               histogram(std::string hname,
                                std::string xtitle,
                                int    nbinx,
                                float  xmin,
                                float  xmax,
                                std::string ytitle,
                                int    nbiny,
                                float  ymin,
                                float  ymax,
                                int    color=kBlack,
                                int    linestyle=1,
                                int    linewidth=1);

  ///
  static
  TGraph*             graph(std::vector<double>& x, 
                            std::vector<double>& y,
                            std::string xtitle,
                            std::string ytitle,
                            float  xmin,
                            float  xmax,
                            float  ymin,
                            float  ymax,
                            int    color=kBlack,
                            int    linestyle=1,
                            int    linewidth=2);

  ///
  static
  void                plot(TCanvas* canvas, 
                           TH1F* h1, std::string option="");

  ///
  static
  void                plot(TCanvas* canvas, 
                           std::vector<TH1F*>& h, std::string option="");

  // Statistics

  /////////////////////////////////////////////////////////////////////////////
  // File:    poissongamma.hpp
  // Description:	x = poissongamma(p,a,d[,returnlog,[scale]])
  // 
  //              compute the marginal likelihood:
  //              a product of Poisson distributions
  //              and a prior that is a product of gamma distributions.
  //
  //              Model:
  //
  //              d_i = Sum p_j * a_ji,  i=1..M,   j=1..N
  // 
  //              D_i  is count corresponding to true mean d_i
  //              A_ji is count corresponding to true mean a_ji
  //
  //              D is a vector of M observed counts (that is, over M bins)
  //              p is a vector of N parameters (that is, over N sources)
  //              A is a vector of vectors of size N x M counts.
  //
  // WARNING: The number of terms grows exponentially with the number of 
  //          sources and the maximum bin count. Therefore, this routine
  //          really only makes sense for rather few sources with modest
  //          bin counts. When the number of sources is > 4 and/or the 
  //          maximum observed bin count is large use poissongammamc,
  //          which does the calculation by Monte Carlo integration.
  //
  // Created: 20-Dec-2001 Harrison B. Prosper
  //          11-Mar-2004 HBP change typedefs
  //          21-Jun-2005 HBP add poissongammasmc
  /////////////////////////////////////////////////////////////////////////////

  
  ///
  static
  double logpoisson(vdouble&	p,  // Weights "p_j" 
		    vvdouble&	A,  // Counts  "A_ji" for up to 10 sources
		    vdouble&	D,  // Counts  "D_i" for data.
		    bool scale=true); // Scale p_j if true  

  ///
  static
  double poissongamma(vdouble&	p,        // Weights "p_j" 
		      vvdouble&	A,        // Counts "A_ji" for up to 10 sources
		      vdouble&	D,        // Counts  "D_i" for data.
		      bool returnlog=false, // return log(P) if true
		      bool scale=true);     // Scale p_j if true  
  
  /// For simple 1-D application.
  static
  double poissongamma1D(double xsec,
			double Lum,
			double sigmaLum,
			double B,
			double sigmaB,
			double Datum);
  

  ///
  static
  double poissongamma2(double	  p1,
		       double     p2,
		       vdouble&   A1,
		       vdouble&   A2,
		       vdouble&	D,
		       bool returnlog=false,
		       bool scale=true);

  ///
  static
  double poissongamma5(double	p1,
		       double     p2,
		       double     p3,
		       double     p4,
		       double     p5,
		       vdouble&   A1,
		       vdouble&   A2,
		       vdouble&   A3,
		       vdouble&   A4,
		       vdouble&   A5,
		       vdouble&	D,
		       bool returnlog=false,
		       bool scale=true);

  ///
  static
  void initpoissongammamc(vvdouble& A,       // Counts  "A_ji" 
			  int total=10000);  // Number of MC sampling points

  ///
  static
  double poissongammamc(vdouble&	p,
			vvdouble&	A,
			vdouble&	D,
			bool returnlog=false,
			bool scale=true);

};


/*
std::ostream& operator<<(std::ostream& os, 
			 std::vector<reco::GenParticle*>& vp);

std::ostream& operator<<(std::ostream& os, 
			 reco::GenParticle* p);
*/

#endif
