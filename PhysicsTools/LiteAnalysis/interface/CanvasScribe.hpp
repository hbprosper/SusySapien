#ifndef CANVASSCRIBE_HPP
#define CANVASSCRIBE_HPP
//-----------------------------------------------------------------------------
// File: CanvasScribe.hpp
// Description: A simple class to (LaTeX) lines to a canvas
// Created: 23-Feb-2005 Harrison B. Prosper
//-----------------------------------------------------------------------------
#include <string>
#include <TLatex.h>

/// Write to a TCanvas.
class CanvasScribe
{
public:
  ///
  CanvasScribe();

  /** TCanvas writer.
      @param xxpos - horizontal starting position of text
      @param yymin - minimum vertical position of text buffer
      @param yymax - maximum vertical position of text buffer
      @param size - text size
      @param font - text font
   */
  CanvasScribe(float xxpos, 
               float yymin, 
               float yymax, 
               float size=0.04, 
               int font=42);

  ~CanvasScribe();

  /** Write a line of text.
      @param text - line of text
      @param xoffset - starting position relative to xxpos
   */
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
