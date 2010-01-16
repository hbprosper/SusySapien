#ifndef EVENTSTREAM_H
#define EVENTSTREAM_H
// -*- C++ -*-
//
// Package:    PhysicsTools
// Class:      EventStream
// 
/**\class EventStream EventStream.cc 
   PhysicsTools/LiteAnalysis/src/EventStream.cc

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
// $Id: EventStream.h,v 1.2 2008/10/31 02:02:53 harry Exp $
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
#include "TBranch.h"
#include "TLeaf.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
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
class EventStream
{
public:

  ///
  EventStream();

  /// Create an EventStream object, which by default reads EDM-compliant files.
  EventStream(std::string filename, 
	      std::string treename="Events");

  /// Create an EventStream object, which by default reads EDM-compliant files.
  EventStream(std::string filename, 
	      std::vector<std::string>& treenames);
  
  /// Create an EventStream object, which by default reads EDM-compliant files.
  EventStream(std::vector<std::string>& filenames, 
	      std::string treename="Events");

  /// Create an EventStream object, which by default reads EDM-compliant files.
  EventStream(std::vector<std::string>& filenames, 
	      std::vector<std::string>& treenames);

  virtual ~EventStream();
  
  /// Last operation was good.
  bool good();
 
  /// Return number of entries.
  int size();

  /// Return current entry.
  int entry();

  /// Return tree number.
  int number();

  ///
  void close();

  /// Return current file name.
  std::string filename();

  /// Return branch names.
  vstring  branchnames();

  /// Return class names.
  vstring  classnames();

  /// Return class identifiers.
  vstring  identifiers(std::string classname);

  /// Read specified entry.
  bool read(int entry=-1);

  /// Proxy for read
  bool operator[](int entry);

  /// Print some info about stream
  void ls(std::ostream& os=std::cout);

  //  bool select(std::vector<pat::Jet>& o, std::string attribute="")
  //  {return false;}

  //-------------------------------------------------------------------------
  // This is the workhorse. This template function maps the object which is 
  // to receive event data, typically a vector<Y>, to the appropriate branch.
  // The instantiations of this function are in classes.h in the src
  // directory. "attributes" is some sub-string of the branch name 
  // sufficient to identify the branch uniquely.
  //-------------------------------------------------------------------------
  /// Select objects of specified class and attributes.
  template<class T>
  bool select(T& obj, std::string attribute="")
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

      // Alas, we need special handling for a few types

      /*
      if (otype == "GenParticleList")
	{
	  // We can ignore type checking here because the 
	  // "if" statement has done it for us

	  _genpart = (GenParticleList*)(&obj);
	  if (!_mcproduct) _mcproduct = new edm::HepMCProduct();
	  if (!_mcproduct) return false;
	  otype = "edmHepMCProduct"; // Switch to type we really want
	}
      */
      if (otype == "edmEventAuxiliary") otype = "EventAuxiliary";
      
      if ( DEBUG )
	std::cout << std::endl 
		  << " TYPE(" << otype << ")" 
		  << " ATTRIBUTE(" << attribute << ")"
		  << std::endl;
      
      // Search list of branches for best match
      // ------------------------------------------
      // 1. Construct a simple regular expression:
      //    e.g. "recoMuon.*global.*" or ".*mc_doc_id.*"

      vstring words;
      split(attribute, words);
      std::string EDMrstr  = otype + ".*";
      std::string NonEDMrstr = ".*";
      for(unsigned i=0; i < words.size(); i++)
	{ 
	  EDMrstr  += words[i] + ".*";
	  NonEDMrstr += words[i] + ".*";
	}

      if ( DEBUG )
	{
	  std::cout << "EDMrstr(" << EDMrstr << ")" << std::endl;
	  std::cout << "NonEDMrstr(" << NonEDMrstr << ")" << std::endl;
	}

      // 2. Compile two alternative forms of regular expression
      boost::regex EDMre(EDMrstr, boost::regex::icase);
      boost::smatch EDMwhat;

      boost::regex NonEDMre(NonEDMrstr, boost::regex::icase);
      boost::smatch NonEDMwhat;
      
      // 3. Iterate over branch names

      vstring branchname;
      for(unsigned i=0; i < _branchnames.size(); i++)
	{
	  // 4. If matched cache name of branch

	  if ( _EDMcompliant )
	    {
	      // EDM compliant therefore match starts with type
	      if ( boost::regex_search(_branchnames[i], EDMwhat, EDMre) )
		{
		  if ( DEBUG ) std::cout << "\t--> Matched " 
					 << _branchnames[i] 
					 << "\n\t\t---> with REGEX(" 
					 << EDMrstr 
					 << ")" << std::endl;
		  branchname.push_back(_branchnames[i]);
		}
	    }
	  else
	    {
	      // NonEDM compliant therefore match does not start with type
	      if ( boost::regex_search(_branchnames[i],NonEDMwhat, NonEDMre) )
		{
		  if ( DEBUG ) std::cout << "\t--> Matched " 
					 << _branchnames[i] 
					 << "\n\t\t---> with REGEX(" 
					 << NonEDMrstr 
					 << ")" << std::endl;
		  branchname.push_back(_branchnames[i]);
		}
	    }
	}

      // Decide what to do with branch

      std::string rstr="";
      if ( _EDMcompliant )
	rstr = EDMrstr;
      else
	rstr = NonEDMrstr;

      if ( branchname.size() == 0 )
	{
	  // 1. if no match, for now, fall on sword
	  std::cout << "select - REGEX(" << rstr << ") Failed" << std::endl;
	  _status = 2;
	  return false;
	}
      else if ( branchname.size() > 1 )
	{
	  // 2. branch selection is not unique, so complain
	  std::cout << "select - selection REGEX(" 
		    << rstr << ") does NOT return unique match:" << std::endl;
	  for(unsigned i=0; i < branchname.size(); i++)
	    std::cout << "\t" << branchname[i] << std::endl;
	  _status = 3;
	  return false;
	}

      // Found a unique match, so add to list of selected branches, 
      // mapping object's address to branch.

      if ( _branch.find(branchname[0]) == _branch.end() )
	{
	  std::string name = branchname[0];
	  if ( DEBUG ) std::cout << "\t" << name << std::endl;
	  TBranch* branch = ((TTree*)_chain)->GetBranch(name.c_str());
	  if ( branch == 0 ) 
	    {
	      std::cout << "** Error** select -- null address for branch\n\t" 
			<< name << std::endl;
	      exit(1);
	    }

	  // map object's address to branch
	  Branch b;
	  b.branch = branch;
	  b.otype  = otype;
	  b.address= (void*)&obj;

	  // Need to do some special handling...annoying!
	  if ( otype == "EventAuxiliary")
	    {
	      TLeaf* leaf = branch->GetLeaf("EventAuxiliary");
	      leaf->SetAddress( b.address );	      
	    }
	  /*
	  else if ( otype == "edmHepMCProduct" )
	    {
	      b.address = (void*)(_mcproduct);
	      branch->SetAddress( b.address );
	    }
	  */
	  else
	    branch->SetAddress( b.address );

	  // Add branch to branch map

	  _branch[name] = b;
	  
	  if ( DEBUG ) std::cout << "\tSUCCESS(" << name << ") =" 
				 << b.address << std::endl;
	}
      return _status == 0;
    }    

private:
  void      _open();
  void      _update();
  std::string _stripcolons(std::string);

  int       _status;
  int       _current;
  int       _entries;
  TChain*   _chain;
  int       _entry;

  //edm::HepMCProduct* _mcproduct;
  //GenParticleList*   _genpart;

  vstring   _filenames;
  vstring   _treenames;
  vstring   _branchnames;
  vstring   _classnames;
  std::vector<TChain*> _vchain;
  BranchMap _branch;
  bool      _EDMcompliant;
};

//std::ostream& operator<<(std::ostream& os, EventStream& stream);

#endif



