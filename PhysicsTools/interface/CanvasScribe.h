#ifndef CANVASSCRIBE_H
#define CANVASSCRIBE_H
//-----------------------------------------------------------------------------
// File: CanvasScribe.h
// Description: A simple class to (LaTeX) lines to a canvas
// Created: 23-Feb-2005 Harrison B. Prosper
//$Revision: 1.2 $
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
      @param xpos - horizontal starting position of text
      @param ypos - vertical starting position of text
      @param ymin - minimum vertical position of text buffer
      @param ymax - maximum vertical position of text buffer
      @param logy - set true for vertical log scale
      @param size - text size
      @param font - text font
   */
  CanvasScribe(float xpos,
               float ypos,
               float ymin, 
               float ymax,
               bool  logy,
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
  float ypos;
  float ymin;
  float ymax;
  bool  logy;
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
