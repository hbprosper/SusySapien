//-----------------------------------------------------------------------------
// File: CanvasScribe.cpp
// Description: A simple class to (LaTeX) lines to a TCanvas
// Created: 23-Feb-2005 Harrison B. Prosper
//-----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/CanvasScribe.h"

#ifdef __WITH_CINT__
  ClassImp(CanvasScribe)
#endif

CanvasScribe::CanvasScribe()
  : xpos(0),
    ymin(0),
    ymax(0),
    textsize(0),
    scale(0),
    linewidth(0),
    line(0),
    t(0)
{}

CanvasScribe::CanvasScribe(float xxpos, 
			   float yymin, 
			   float yymax, 
			   float size, 
			   int font) 
  : xpos(xxpos),
    ymin(yymin),
    ymax(yymax),
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
  float ypos = ymax - line * linewidth;
  if ( ypos < ymin ) return;
  t->DrawLatex(xpos+xoffset, ypos, text.c_str());
  line++;
}

