#ifndef HISTOGRAMCACHE_HPP
#define HISTOGRAMCACHE_HPP
//--------------------------------------------------------------------
// File:    HistogramCache.hpp
// Purpose: Cache histograms from a file and provide access to them by
//          name. The name must be provided in the format:
//                [dir1/[dir2/..]]histogramName
//
// Created: 11-Oct-2004 Harrison B. Prosper
// $Revision: 1.1 $
//--------------------------------------------------------------------
#include <map>
#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
//--------------------------------------------------------------------
#include "TObject.h"
#include "TList.h"
#include "TKey.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TH2.h"
//--------------------------------------------------------------------
// Cache TH1F and TH2F histograms from a TFile. 

class HistogramCache
{
public:
  //
  HistogramCache() {}
  
  //
  HistogramCache(std::string filename, bool verbose=false);
  
  //
  ~HistogramCache();
  
  // Return true if specified histogram exists in cache; false otherwise.
  bool                  exists(std::string histname);
  
  // Given a histogram name, return its contents. 
  std::vector<double>   contents(std::string histname, 
                                 int RebinSize=1, 
                                 double Normalization=-1.0,
                                 std::string option="");
  // given a 1d histogram, return its content
  std::vector<double>   contents_TH1(TH1D *h, 
                                     int RebinSize, 
                                     double Normalization);
  
  // return the type of the histogram with this name.
  std::string           type(std::string histname);
    
  // Return a pointer to the histogram. 
  TH1*                  histogram(std::string histname);
    
  // List contents of cache.
  void                  ls(std::ostream& os=std::cout);

  // List contents of cache.
  std::string           str();
    
private:
  
  std::string                 _filename;
  TFile*                      _file;
  bool                        _verbose;
  
  std::ostringstream*         _os;
  
  std::stack<std::string>     _path;
  std::map<std::string, TH1*> _hist;
  
  std::string _name(TH1* h);
  void        _check(int depth);
  void        _build(TDirectory* d, int depth=-1);
};
#endif
