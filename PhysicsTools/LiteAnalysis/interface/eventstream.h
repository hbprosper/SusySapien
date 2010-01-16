#ifndef EVENTSTREAM_H
#define EVENTSTREAM_H
// -*- C++ -*-
//
// Package:    PhysicsTools
// Class:      eventstream
// 
/**\class eventstream eventstream.cc 
   PhysicsTools/LiteAnalysis/src/eventstream.cc

 Description: An interface to the various CMS root file formats: 

           EDM compliant
              1. RAW, AOD etc.
              2. PAT
              3. ntuples
           NonEDM compliant
              4. ntuples

 Implementation:
     Common sense and a sense of beauty. Ask not what you can do for the
     code, but what the code can do for you.
*/
//
// Original Author:  Harrison B. Prosper
//         Created:  Wed Jun 20 19:53:47 EDT 2007
// $Id: eventstream.h,v 1.2 2008/10/31 02:02:53 harry Exp $
//
//
//-----------------------------------------------------------------------------
#include <Python.h>
#include <boost/python/errors.hpp>
#include <boost/python/type_id.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <sstream>
#include <cassert>
#include <vector>
#include <string>
#include <map>
//-----------------------------------------------------------------------------
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"
#include "TLeaf.h"
//-----------------------------------------------------------------------------
namespace 
{
  bool DEBUG=false;
  void split(std::string str, std::vector<std::string>& vstr)
  {
    vstr.clear();
    std::istringstream stream(str.c_str());
    while ( stream )
      {
	std::string s;
	stream >> s;
	if ( stream ) vstr.push_back(s);
      }
  }
 
  struct Branch
  {
    TBranch*    branch;
    void*       address;
    std::string otype;
  };
}
typedef std::vector<std::string> vstring;
typedef std::map<std::string, Branch> BranchMap;
typedef std::map<std::string, Branch>::iterator BranchMapIter;
//-----------------------------------------------------------------------------

///
class eventstream
{
public:

  ///
  eventstream();

  /// Create an eventstream object.
  eventstream(std::string filename);

  virtual ~eventstream();
  
  /// Last operation was good.
  bool good();
 
  /// Return number of entries.
  int size();

  ///
  void close();

  /// Read specified entry.
  bool read(int entry=-1);

  /// Proxy for read
  bool operator[](int entry);

  //-------------------------------------------------------------------------
  // This is the workhorse. This template function maps the object which is 
  // to receive event data, typically a vector<Y>, to the appropriate branch.
  // The instantiations of this function are in classes.h in the src
  // directory. "attributes" is some sub-string of the branch name 
  // sufficient to identify the branch uniquely.
  //-------------------------------------------------------------------------
  /// Select objects of specified class and attributes.
  template<class T>
  bool get(T& obj, std::string attribute)
    {
      _status = 0;

      
      // Extract type X assuming type is vector< X," or just X
      
      std::string otype = boost::python::type_id< T >().name();

      boost::regex e("(?<=[<]).*(?=,)");
      boost::smatch w;
      if ( boost::regex_search(otype, w, e) ) otype = w[0];
      otype = _stripcolons(otype); // object type
      
      if ( DEBUG )
        std::cout << std::endl 
                  << " OTYPE(" << otype << ")" << std::endl;



    }    

private:
  void      _open();
  void      _update();
  std::string _stripcolons(std::string);

  int       _status;
  int       _entries;
  TFile*    _file;
  fwlite::Event _ev;

  int       _entry;

};


#endif



