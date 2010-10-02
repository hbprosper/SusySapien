//-----------------------------------------------------------------------------
//  File:    RGS.cc
//  Purpose: Implement a version of RGS suitable for use in Python.
//  Created: 18-Aug-2000 Harrison B. Prosper, Chandigarh, India
//  Updated: 05-Apr-2002 HBP tidy up
//           17-May-2006 HBP use weightindex instead of a vector of weights
//-----------------------------------------------------------------------------
//$Revision: 1.2 $
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#ifdef PROJECT_NAME
#include "PhysicsTools/Mkntuple/interface/RGS.h"
#else
#include "RGS.h"
#endif

using namespace std;

#ifdef __WITH_CINT__
ClassImp(RGS)
#endif


  string rgsversion() {return string("RGS - $Revision: 1.2 $");}

namespace {
  void error(string message)
  {
    cerr << "RGS ** error *** " << message << endl;
  }
  
  bool inString(string strg, string str)
  {
    int j = strg.find(str,0);
    return (j>-1) && (j<(int)strg.size());
  }
  
  // Extract name of a file without extension
  string nameonly(string filename)
  {
    int i = filename.rfind("/");
    int j = filename.rfind(".");
    if ( j < 0 ) j = filename.size();
    return filename.substr(i+1,j-i-1);
  }
}
//---------------------------------------------------------------------------
// Description: Read from a text file. For use in Root.
// Created: 03-May-2005 Harrison B. Prosper
//---------------------------------------------------------------------------
bool slurpTable(string filename,
                vector<string>& header, 
                vector<vector<double> >& data,
                int start,
                int count,
                bool extend)
{
  ifstream stream(filename.c_str());
  if ( ! stream.good() )
    { 
      error("slurp_table - unable to open "+filename);
      return false;
    }
  
  // Read header
  
  string line;
  getline(stream, line, '\n');
  istringstream inp(line);
  
  header.clear();
  while ( inp >> line ) header.push_back(line);
  
  // Skip the first "start" lines
  
  int n=0;
  for(int i=0; i < start; i++)
    {
      n++;
      if ( !getline(stream, line, '\n') ) break;
    }
  
  // Read "count" rows if count > 0, otherwise read all lines

  if ( extend )
    {
      int nrow=0;
      while ( getline(stream, line, '\n') )
        {
          istringstream inp(line);
          for(int i=0; i < (int)header.size(); i++)
            {
              double x;
              inp >> x;
              data[nrow].push_back(x);
            }
          nrow++;
          if ( count <= 0 ) continue;
          if ( nrow >= count ) break;
        }
    }
  else
    {
      data.clear();
      data.reserve(100000);
      vector<double> d(header.size());
      int nrow=0;
      while ( getline(stream, line, '\n') )
        {	  
          istringstream inp(line);      
          for(int i=0; i < (int)header.size(); i++) inp >> d[i];
          data.push_back(d);
          nrow++;
          if ( count <= 0 ) continue;
          if ( nrow >= count ) break;
        }
    }
  stream.close();
  return true;
}
            
//-----------------------------
// CONSTRUCTORS
//-----------------------------

RGS::RGS()
  : _status(0)
{}

RGS::RGS(vstring& filenames, int start, int numrows)
  : _status(0)
{
  // Definitions:
  //  Cut file
  //    n-tuple containing the variables that define the cuts.
  //
  //  Search file
  //    The file on which the cuts are to be applied
  //
  _init(filenames, start, numrows);
}

RGS::RGS(string filename, int start, int numrows)
  : _status(0)
{
  vstring filenames(1, filename);
  _init(filenames, start, numrows);
}

//-----------------------------
// DESTRUCTOR
//-----------------------------
RGS::~RGS(){}


//-----------------------------
// METHODS
//-----------------------------

bool
RGS::good() { return _status == 0; }


void 
RGS::add(string filename, 
         int    start, 
         int    numrows,
         string weightname)
{ 
  _searchname.push_back(nameonly(filename));  
  _searchdata.push_back(vvdouble());
  _weightindex.push_back(-1); // Index to weight field
  _status = 0;

  vector<string> var;
  if ( ! slurpTable(filename, var, _searchdata.back(), start, numrows) )
    {
      error(string("unable to read file ") + filename);
      _status = -1;
      return;
    }

  for(int i = 0; i < (int)var.size(); i++)
    {
      if ( weightname == var[i] )
        {
          _weightindex.back() = i;
          cout << "RGS will weight events with the variable "
               << weightname
               << " in column " << i << endl;
          break;
        }
    }
}

void 
RGS::add(vector<string>&   filename, 
         int start, int numrows,
         string   weightname)
{ 
  _searchname.push_back(nameonly(filename[0]));  
  _searchdata.push_back(vvdouble());
  _weightindex.push_back(-1); // Index to weight field
  _status = 0;

  int index=0;
  bool extend=false;
  for(int ifile=0; ifile < (int)filename.size(); ifile++)
    {
      vector<string> var;
      if ( ! slurpTable(filename[ifile], var, _searchdata.back(), 
                         start, numrows, extend) )
        {
          error(string("unable to read file ") + filename[ifile]);
          _status = -1;
          return;
        }
      extend = true;
      
      for(int i = 0; i < (int)var.size(); i++)
        {
          if ( weightname == var[i] ) _weightindex.back() = index;
          index++;
        }
    }
}

// The workhorse

void RGS::run(vstring&  cutvar,        // Variables defining cuts 
              vstring&  cutdir,        // Cut direction
              int nprint)
{
  _index.clear();
  _status = 0;

  // Make sure len of cutvar and cutdir are the same
  if ( cutvar.size() < cutdir.size() )
    {
      error("RGS::run ** length(cutvar) < Length(cutdir)");
      exit(1);
    }
  
  // Adjust sizes of buffers
  
  _totals.resize(_searchdata.size(),0.0);
  _counts.resize(_searchdata.size(),vdouble());
  for (int i = 0; i < (int)_counts.size(); i++)
    _counts[i].resize(_cutdata.size(), 0.0);
  
  // Decode cut
  /////////////
  int code = -1;
  for (int i = 0; i < (int)cutdir.size(); i++)
    {
      if      ( inString(cutdir[i],">") )
        code = GT;
      else if ( inString(cutdir[i],"<") )
        code = LT;
      if   (  ( inString(cutdir[i],"|") ) )
        code = code + 2;
      _cutcode.push_back(code);
      
      // Load indices into data objects
      if ( _varmap.find(cutvar[i]) != _varmap.end() )
        _index.push_back(_varmap[cutvar[i]]); // Pointer map into data
      else
        {
          error(string("cut variable ") + cutvar[i] + string(" NOT found"));
          exit(0);
        }
    } 
  
  // Loop over files to be processed
  //////////////////////////////////
  
  
  for (int file = 0; file < (int)_searchdata.size(); file++)
    {
      int  weightindex = _weightindex[file];
      vvdouble& sdata  = _searchdata[file];

      cout << "\tProcessing dataset: " << _searchname[file] 
           << "\twith " << sdata.size() << " rows" << endl;
      
      if ( nprint > 0 )
        {
          cout << "\tApply cuts: " << endl;
          for (int cut = 0; cut < (int)cutvar.size(); cut++)
            cout << "\t\t" << _var[_index[cut]] << "\t" << cutdir[cut] << endl;
        }
      
      // Check whether to use event weighting
      ///////////////////////////////////////
      
      bool useEventWeight = weightindex > -1;
      
      // Loop over cut sets (points)
      //////////////////////////////
      
      for (int cutset = 0; cutset < (int)_cutdata.size(); cutset++)
        {
          if ( (nprint > 0) && (cutset % nprint) == 0 )
            cout << "             " << cutset << endl;
          
          // Loop over rows of file to be processed
          /////////////////////////////////////////
          
          _counts[file][cutset] = 0; // Count per file per cut-point
          
          for (int row = 0; row < (int)sdata.size(); row++)
            {
              // Apply each cut in current cut set
              
              bool passed=true;
              for (int cut = 0; cut < (int)cutdir.size(); cut++)
                {
                  int   jcut = _index[cut];
                  float x    = sdata[row][jcut];
                  float xcut = _cutdata[cutset][jcut];
                  
                  switch (_cutcode[cut])
                    {
                    case GT:
                      passed = x > xcut;
                      break;
                  
                    case LT:
                      passed = x < xcut;
                      break;
		      
                    case ABSGT:
                      passed = abs(x) > abs(xcut);
                      break;
                  
                    case ABSLT:
                      passed = abs(x) < abs(xcut);
                      break;
                  
                    default:
                      passed = true;
                      break;
                    }

                  // If any cut fails, there is no point continuing.
                  // So break out of loop
                  if ( !passed ) break;
                }
          
              // Keep a running sum of events that pass all cuts
              // of current cut-set

              float weight = 1.0;
              if ( useEventWeight ) weight = weight * sdata[row][weightindex];

              if ( cutset == 0 ) _totals[file] += weight;
          
              if ( passed ) _counts[file][cutset] += weight;
            }
        }
    }
}

double
RGS::total(int index) 
{
  if ( index < 0 || index > (int)_totals.size()-1 )
    {
      _status = rBADINDEX;
      return 0;
    }
  return _totals[index];
}

double
RGS::count(int index, int cutindex) 
{
  if ( index < 0 || index > (int)_totals.size()-1 )
    {
      _status = rBADINDEX;
      return 0;
    }
  if ( cutindex < 0 || cutindex > (int)_cutdata.size()-1 )
    {
      _status = rBADINDEX;
      return 0;
    }
  return _counts[index][cutindex];
}

vdouble
RGS::cuts(int index)
{ 
  if ( index < 0 || index > (int)_cutdata.size()-1 )
    {
      _status = rBADINDEX;
      return vdNULL;
    }
  vdouble cut(_cutcode.size());
  for(unsigned i=0; i < _cutcode.size(); i++)
    cut[i] = _cutdata[index][_index[i]];
  return cut;
}

vstring
RGS::cutvars()   
{ 
  vstring cutvar(_cutcode.size());
  for(unsigned i=0; i < _cutcode.size(); i++)
    cutvar[i] = _var[_index[i]];
  return cutvar;
}

int
RGS::ncuts() { return _cutdata.size(); }

vstring&
RGS::vars()   { return _var; }

int
RGS::ndata(int index)
{ 
  if ( index < 0 || index > (int)_searchdata.size()-1 )
    {
      _status = rBADINDEX;
      return 0;
    }
  return _searchdata[index].size();
}

vdouble&
RGS::data(int index, int event)
{ 
  if ( index < 0 || index > (int)_searchdata.size()-1 )
    {
      _status = rBADINDEX;
      return vdNULL;
    }
  return _searchdata[index][event]; 
}

//--------------------------------
// PRIVATE METHODS
//--------------------------------

void
RGS::_init(vstring& filenames, int start, int numrows)
{
  _cutdata.clear();
  _cutcode.clear();
  _searchname.clear();
  _searchdata.clear();
  _weightindex.clear();
  _varmap.clear();
  _totals.clear();
  _counts.clear();

  _status = rSUCCESS;

  int index=0;
  bool extend=false;
  for(int ifile=0; ifile < (int)filenames.size(); ifile++)
    {
      vector<string> var;
      if ( ! slurpTable(filenames[ifile], 
                         var, _cutdata, start, numrows, extend) )
        {
          cout << "**Error** unable to read file " << filenames[ifile] << endl;
          _status = rFAILURE;
          return;
        }
      extend = true;
      
      for(int i = 0; i < (int)var.size(); i++)
        {
          _varmap[var[i]] = index; // Map variable names to ordinal value
          _var.push_back(var[i]);  // Map ordinal value to variable name
          index++;
        }
    }
}
