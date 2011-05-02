//-----------------------------------------------------------------------------
// File: CanvasScribe.cpp
// Description: A simple class to (LaTeX) lines to a TCanvas
// Created: 23-Feb-2005 Harrison B. Prosper
//$Revision:$
//-----------------------------------------------------------------------------
#include <cmath>
#include "PhysicsTools/Mkntuple/interface/CanvasScribe.h"
#ifdef __WITH_CINT__
  ClassImp(CanvasScribe)
#endif

CanvasScribe::CanvasScribe()
  : xpos(0),
    ypos(0),
    ymin(0),
    ymax(0),
    logy(false),
    textsize(0),
    scale(0),
    linewidth(0),
    line(0),
    t(0)
{}

CanvasScribe::CanvasScribe(float xxpos,
                           float yypos,
                           float yymin, 
                           float yymax,
                           bool  logyy,
                           float size, 
                           int font) 
  : xpos(xxpos),
    ypos(logyy ? log(yypos) : yypos),
    ymin(logyy ? log(yymin) : yymin),
    ymax(logyy ? log(yymax) : yymax),
    logy(logyy),
    textsize(size),
    scale(1.8),
    linewidth((ymax-ymin)*scale*textsize),
    line(0),
    t(0)
{      
  t = new TLatex();
  t->SetTextSize(textsize);
  t->SetTextFont(font);
  t->SetTextAlign(12);
}

CanvasScribe::~CanvasScribe() { if ( t != 0 ) delete t; }

void 
CanvasScribe::write(std::string text, float xoffset)
{
  float y = ypos - line * linewidth;
  if ( y < ymin ) return;
  if ( logy ) y = exp(y);
  t->DrawLatex(xpos + xoffset, y, text.c_str());
  line++;
}

