#ifndef CANVASSCRIBE_HPP
#define CANVASSCRIBE_HPP
//-----------------------------------------------------------------------------
// File: CanvasScribe.hpp
// Description: A simple class to (LaTeX) lines to a canvas
// Created: 23-Feb-2005 Harrison B. Prosper
//-----------------------------------------------------------------------------
#include <string>
#include <TLatex.h>

class CanvasScribe
{
public:
  CanvasScribe();

  CanvasScribe(float xxpos, 
	       float yymin, 
	       float yymax, 
	       float size=0.04, 
	       int font=42);

  ~CanvasScribe();

  void write(std::string text, float xoffset=0.0);

 private:
  float xpos;
  float ymin;
  float ymax;
  float textsize;
  float scale;
  float linewidth;
  int   line;
  TLatex* t;
#ifdef __WITH_CINT__
 public:
  ClassDef(CanvasScribe,1)
#endif
};
#endif
