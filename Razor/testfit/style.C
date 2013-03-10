// ----------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

#include "TROOT.h"
#include "TMath.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TFitResult.h"
#include "TF1.h"
#include "TMinuit.h"
#include "TPolyLine.h"
// ----------------------------------------------------------------------------
using namespace std;
// ----------------------------------------------------------------------------
namespace {
  const int LINEWIDTH= 2;
  const int TEXTFONT = 42;
  const int NDIV     = 510;
  const int WIDTH    = 500;
  const int HEIGHT   = 500;
  const double TEXTSIZE = 0.04;
  const double MARKERSIZE = 0.8;
}
// ----------------------------------------------------------------------------

void working()
{
  cout << ".";
  cout.flush();
}

string 
strip(string line)
{
  int l = line.size();
  if ( l == 0 ) return string("");
  int n = 0;
  while (((line[n] == 0)    ||
          (line[n] == ' ' ) ||
          (line[n] == '\n') ||
          (line[n] == '\t')) && n < l) n++;
  
  int m = l-1;
  while (((line[m] == 0)    ||
          (line[m] == ' ')  ||
          (line[m] == '\n') ||
          (line[m] == '\t')) && m > 0) m--;
  return line.substr(n,m-n+1);
}

string 
shell(string cmd)
{
  FILE* f = popen(cmd.c_str(),"r");
  int buffsize=8192;
  char s[8192];
  int n = fread(s,1,buffsize,f);
  pclose(f);
  string result = strip(string(s).substr(0,n));
  return result;
}

void 
split(string str, vector<string>& vstr)
{
  vstr.clear();
  istringstream stream(str);
  while ( stream )
    {
      string stringy;
      stream >> stringy;
      if ( stream ) vstr.push_back(stringy);
    }
}

string
replace(string& str, string oldstr, string newstr)
{
  return string(TString(str).ReplaceAll(oldstr, newstr).Data());
}

void error(string message)
{
  cout << "**ERR** " << message << endl;
  exit(0);
}

//----------------------------------------------------------------------------
// Style Utilities
//----------------------------------------------------------------------------

void fixhist(TH1F* h, const char* xtitle, const char* ytitle)
{
  if ( xtitle ) h->GetXaxis()->SetTitle(xtitle);
  h->GetXaxis()->SetTitleOffset(1.3);
  h->SetNdivisions(505, "X");

  if ( ytitle ) h->GetYaxis()->SetTitle(ytitle);
  h->GetYaxis()->SetTitleOffset(1.6);
  h->SetNdivisions(505, "Y");
}

void 
setStyle()
{
  TStyle* style = new TStyle("CMSstyle","CMS Style");
  gStyle->SetPalette(1);

  // For the canvas:
  style->SetCanvasBorderMode(0);
  style->SetCanvasColor(kWhite);
  style->SetCanvasDefH(500); //Height of canvas
  style->SetCanvasDefW(500); //Width of canvas
  style->SetCanvasDefX(0);   //Position on screen
  style->SetCanvasDefY(0);

  // For the Pad:
  style->SetPadBorderMode(0);
  style->SetPadColor(kWhite);
  style->SetPadGridX(kFALSE);
  style->SetPadGridY(kFALSE);
  style->SetGridColor(kGreen);
  style->SetGridStyle(3);
  style->SetGridWidth(1);
    
  // For the frame:
  style->SetFrameBorderMode(0);
  style->SetFrameBorderSize(1);
  style->SetFrameFillColor(0);
  style->SetFrameFillStyle(0);
  style->SetFrameLineColor(1);
  style->SetFrameLineStyle(1);
  style->SetFrameLineWidth(1);
    
  // For the histo:
  style->SetHistLineColor(1);
  style->SetHistLineStyle(0);
  style->SetHistLineWidth(LINEWIDTH);

  style->SetEndErrorSize(2);
  style->SetErrorX(0.);
    
  style->SetMarkerSize(0.5);
  style->SetMarkerStyle(20);

  //For the fit/function:
  style->SetOptFit(1);
  style->SetFitFormat("5.4g");
  style->SetFuncColor(2);
  style->SetFuncStyle(1);
  style->SetFuncWidth(1);

  //For the date:
  style->SetOptDate(0);

  // For the statistics box:
  style->SetOptFile(0);
  style->SetOptStat("");
  // To display the mean and RMS:
  //style->SetOptStat("mr"); 
  style->SetStatColor(kWhite);
  style->SetStatFont(TEXTFONT);
  style->SetStatFontSize(0.03);
  style->SetStatTextColor(1);
  style->SetStatFormat("6.4g");
  style->SetStatBorderSize(1);
  style->SetStatH(0.2);
  style->SetStatW(0.3);
    
  // Margins:
  style->SetPadTopMargin(0.05);
  style->SetPadBottomMargin(0.16);
  style->SetPadLeftMargin(0.20); // 0.20
  style->SetPadRightMargin(0.10);// 0.10

  // For the Global title:
  style->SetOptTitle(0); 
  style->SetTitleFont(TEXTFONT);
  style->SetTitleColor(1);
  style->SetTitleTextColor(1);
  style->SetTitleFillColor(10);
  style->SetTitleFontSize(0.05);

  // For the axis titles:
  style->SetTitleColor(1, "XYZ");
  style->SetTitleFont(TEXTFONT, "XYZ");
  style->SetTitleSize(0.05, "XYZ");
  style->SetTitleXOffset(1.25);    //(0.9);
  style->SetTitleYOffset(1.25);    //(1.25);

  // For the axis labels:
  style->SetLabelColor(1, "XYZ");
  style->SetLabelFont(TEXTFONT, "XYZ");
  style->SetLabelOffset(0.007, "XYZ");
  style->SetLabelSize(0.05, "XYZ");

  // For the axis:
  style->SetAxisColor(1, "XYZ");
  style->SetStripDecimals(kTRUE);
  style->SetTickLength(0.03, "XYZ");
  style->SetNdivisions(505, "XYZ");
  // To get tick marks on the opposite side of the frame
  style->SetPadTickX(1);  
  style->SetPadTickY(1);

  // Change for log plots:
  style->SetOptLogx(0);
  style->SetOptLogy(0);
  style->SetOptLogz(0);

  // Postscript options:
  style->SetPaperSize(20.,20.);
  style->cd();
}

class Scribe
{
public:
  Scribe()
    : xpos(0),
      ypos(0),
      textsize(0),
      scale(0),
      linewidth(0),
      line(0),
      t(0)
  {}

  Scribe(float xxpos,
	 float yypos,
	 float size=0.04,
	 int font=42) 
    : xpos(xxpos),
      ypos(yypos),
      textsize(size),
      scale(1.5),
      linewidth(scale*textsize),
      line(0),
      t(new TLatex())
  {    
    t->SetNDC();
    t->SetTextSize(textsize);
    t->SetTextFont(font);
    t->SetTextAlign(12);
  }
  
  ~Scribe() { if ( t != 0 ) delete t; }

  void write(std::string text, float xoffset=0)
  {
    float y = ypos;
    if ( y < 0 ) return;
    t->DrawLatex(xpos+xoffset, y, text.c_str());
    ypos -= linewidth;
    line++;
  }
  
  void vspace(float f=0.5)
  {
    float y = ypos;
    if ( y < 0 ) return;
    t->DrawLatex(xpos, y, " ");
    ypos -= linewidth * f;
    line++;
  }
 
private:
  float xpos;
  float ypos;
  float ymin;
  float ymax;
  bool  logy;
  float textsize;
  float scale;
  float linewidth;
  int   line;
  TLatex* t;
};

Scribe* addTitle(int which=0)
{
  double xtext = 0.20;
  double ytext = 0.975;
  Scribe* s = new Scribe(xtext, ytext, 0.035);
  if ( which == 7 )
    s->write("#font[22]{CMS               "
	     "    5.1 fb^{-1} (#sqrt{s}=7 TeV)}");
  else if ( which == 8 )
    s->write("#font[22]{CMS               "
	     "    5.3 fb^{-1} (#sqrt{s}=8 TeV)}");
  else
   s->write("#font[22]{CMS             "
	     "    5.1 fb^{-1} (#sqrt{s}=7 TeV)"
	     " + 5.3 fb^{-1} (#sqrt{s}=8 TeV)}");
  return s;
}


string expo(double x, string fmt="%4.2f")
{
  char rec[80];
  sprintf(rec, "%10.3e", x);

  string s = string(rec);
  s = replace(s, "e", " ");
  vector<string> a;
  split(s, a);

  double n = atof(a[0].c_str());
  int m = atoi(a[1].c_str());
  
  char num[20];
  sprintf(num, fmt.c_str(), n);
  sprintf(rec, "%s #times 10^{%d}", num, m);
  s = string(rec);
  return s;
}


class Percentiles
{
public:
  Percentiles(int nbins) 
    : _nbins(nbins),
    _points(std::vector<std::vector<double> >(nbins, std::vector<double>()))
  {}

  ~Percentiles() {}

  void add(int index, double point)
  {
    if ( index < 0 ) return;
    if ( index >= _nbins ) return;
    _points[index].push_back(point);
  }

  std::vector<double> operator()(int index, std::vector<double>& percent)
  {
    sort(_points[index].begin(), _points[index].end());
    int np = _points[index].size();
    vector<double> ps;
    for(unsigned int i=0; i < percent.size(); i++)
      {
	double x = percent[i] * np;
	int k = x;
	double f = x - k;
	double c1= _points[index][k];
	double c = c1;
	if ( k < np - 1 )
	  {
	    double c2 = _points[index][k+1];
	    c  = c1*(1-f) + c2*f;
	  }
	ps.push_back(c);
      }
    return ps;
  }

private:
  int _nbins;
  std::vector<std::vector<double> > _points;
};
// --------------------------------------	
// compute polyline
// --------------------------------------
TPolyLine* mkpline(vector<double>& xx, 
		   vector<double>& y1, 
		   vector<double>& y2, 
		   int color=kYellow, 
		   int fstyle=3000, 
		   int lwidth=2)
{
  int nbins = xx.size();

  // lower curve
  vector<double> x(xx.size());
  vector<double> y(y1.size());
  copy(xx.begin(), xx.end(), x.begin());
  copy(y1.begin(), y1.end(), y.begin());

  // upper curve
  for(int i=0; i < nbins; i++)
    {
      int j = nbins - i - 1;
      x.push_back(xx[j]);
      y.push_back(y2[j]);
    }
  // close polyline
  x.push_back(xx[0]);
  y.push_back(y1[0]);

  TPolyLine* pl = new TPolyLine(2*nbins, &x[0], &y[0]);
  pl->SetLineColor(color);
  pl->SetLineWidth(lwidth);
  pl->SetFillColor(color);
  pl->SetFillStyle(fstyle);
  return pl;
}
