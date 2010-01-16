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

     WARNING: Version 3_X_X of CMSSW breaks this code.

*/
//
// Original Author:  Harrison B. Prosper
//         Created:  Wed Jun 20 2007
//         Updated:  Wed Oct 08 2008 - Handle PAT formats
// $Id: EventStream.cc,v 1.2 2008/10/31 02:02:53 harry Exp $
//
//
//-----------------------------------------------------------------------------
#include "PhysicsTools/LiteAnalysis/interface/EventStream.h"
//-----------------------------------------------------------------------------
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TBranch.h>
#include <TDirectory.h>
#include <TKey.h>
#include <TIterator.h>
#include <iostream>
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
namespace {
  void fatal(std::string message)
  {
    std::cout << "** Error ** " << message << std::endl;
    exit(1);
  }

  void warning(std::string message)
  {
    std::cout << "** Warning ** " << message << std::endl;
  }
}

EventStream::EventStream() 
  : _status(0),
    _current(-1),
    _entries(0),
    _chain(0),
    _entry(-1)
    //    _mcproduct(0),
    //_genpart(0)
{}

EventStream::EventStream(string filename, string treename) 
  : _status(0),
    _current(-1),
    _entries(0),
    _chain(0),
    _entry(-1)
  //_mcproduct(0),
  //_genpart(0)
{
  _filenames.push_back(filename);
  _treenames.push_back(treename);
  _open();

}

EventStream::EventStream(string filename, vector<string>& treenames) 
  : _status(0),
    _current(-1),
    _entries(0),
    _chain(0),
    _entry(-1),
    //_mcproduct(0),
    //_genpart(0),
    _treenames(treenames)
{
  _filenames.push_back(filename);
  _open();

}

EventStream::EventStream(vector<string>& filenames, string treename) 
  : _status(0),
    _current(-1),
    _entries(0),
    _chain(0),
    _entry(-1),
    //_mcproduct(0),
    //_genpart(0),
    _filenames(filenames)
{
  _treenames.push_back(treename);
  _open();
}

EventStream::EventStream(vector<string>& filenames, vector<string>& treenames) 
  : _status(0),
    _current(-1),
    _entries(0),
    _chain(0),
    _entry(-1),
    //_mcproduct(0),
    //_genpart(0),
    _filenames(filenames),
    _treenames(treenames)
{
  _open();
}

EventStream::~EventStream()
{
  close();
  //if (_mcproduct) delete _mcproduct;
  //_mcproduct = 0;
}

bool
EventStream::good()   { return _status == 0; }

int 
EventStream::size()   { return _entries; }

int
EventStream::entry()  { return _entry; }

int 
EventStream::number() { return _current; }

void
EventStream::close()
{
   if ( _chain == 0 ) return;
   delete  _chain;
  _chain = 0;
}

string
EventStream::filename() { return _current > -1 ? _filenames[_current] : ""; }

std::vector<std::string>
EventStream::branchnames() { return _branchnames; } 

std::vector<std::string> 
EventStream::classnames() { return _classnames; } 

// Return class identifiers
std::vector<std::string>  
EventStream::identifiers(std::string classname) 
{ 
  classname = _stripcolons(classname);
  boost::regex re(classname, boost::regex::icase);
  boost::smatch what;

  vector<string> att;

  boost::regex  r("(?<=" + classname + "._).*(?=__)", boost::regex::icase);
  boost::smatch w;

  for(unsigned i=0; i < _branchnames.size(); i++)
    if ( boost::regex_search(_branchnames[i], what, re) )
      if ( boost::regex_search(_branchnames[i], w, r) ) 
	att.push_back(w[0]);
      else
	att.push_back(_branchnames[i]);
  return att; 
} 

// ------------------------------------------------------------------------
// Read event at specified entry. Take care to handle switch between
// different Root-files if we have a chain of them.
// ------------------------------------------------------------------------
bool
EventStream::read(int entry)
{
  if ( entry < 0 )
    {
      _entry++;
      entry = _entry;
    }
  _entry = entry;

  int localentry = _chain->LoadTree(entry);
  //if ( DEBUG ) cout << "read - " << localentry << "\t" << entry << endl;
  if ( localentry < 0 ) return false;
  
  // Update pointers to tree, branches

  if ( _chain->GetTreeNumber() != _current) _update();

  // Load data to memory locations associated with selected branches

  for(BranchMapIter it =_branch.begin(); it != _branch.end(); it++) 
    it->second.branch->GetEntry( localentry );

  // Update GenParticleList if needed
  //if (_genpart) _genpart->update(_mcproduct);

  return true;
}

bool
EventStream::operator[](int entry) 
{ 
  if ( ! read(entry) )
    {
      PyErr_SetString(PyExc_IndexError, "index out of range");
      boost::python::throw_error_already_set(); // bail out
      return false;
    }
  return true;
}

void
EventStream::ls(std::ostream& os) 
//std::ostream& operator<<(std::ostream& os, EventStream& stream)
{
  vector<string> clnames = this->classnames();
  for(unsigned i=0; i < clnames.size(); i++)
    {
      os << clnames[i] << endl;
      vector<string> ids = this->identifiers(clnames[i]);
      for(unsigned j=0; j < ids.size(); j++)
	os << "\t" << ids[j] << endl;
      os << endl;
    }
}

//-----------------------------------------------------------
// Private methods
//-----------------------------------------------------------
std::string
EventStream::_stripcolons(std::string text)
{
  // Strip away "::"
  boost::regex e("::");
  return boost::regex_replace(text, e, "");
}

void
EventStream::_open()
{
  _status = 0;

  if ( DEBUG ) std::cout << std::endl 
			 << "Reading filename " << _filenames[0] << std::endl;

  // -----------------------------------------------------------------
  // Open first file, get tree(s) and then get branches.
  // -----------------------------------------------------------------
  TFile* file = new TFile(_filenames[0].c_str());
  if ( ! file || (file != 0 && ! file->IsOpen()) )
    {
      _status=1;
      return;
    }

  // -----------------------------------------------------------------
  // Find out if this is an EDM-compliant file by checking for a 
  // top-level tree called MetaData
  // -----------------------------------------------------------------
  _EDMcompliant = false;
  TDirectory* dir = (TDirectory*)file;
  TKey* key=0;
  TIter nextkey(dir->GetListOfKeys());
  while ( key = (TKey*)nextkey() )
    {
      dir->cd();
      TObject* o = key->ReadObj();
      if ( o->IsA()->InheritsFrom("TTree") )
	{
	  TTree* t = (TTree*)o;
	  _EDMcompliant = string(t->GetName()) == "MetaData";
	  if (_EDMcompliant) break;
	}
    }

  // ----------------------------------------------------------------- 
  // Get Tree(s)
  // -----------------------------------------------------------------

  bool IsNtuple = true; // true if this really is an n-tuple

  for (unsigned tt=0; tt < _treenames.size(); tt++)
    {
      TTree* tree = (TTree*)file->Get(_treenames[tt].c_str());
      if ( ! tree ) 
	fatal("EventStream - unable to get tree " + _treenames[tt]);

      if ( DEBUG ) cout << "Scan tree: " << _treenames[tt] << endl;

      // -----------------------------------------------------------------
      // Get all branch names ending in ".obj"
      // -----------------------------------------------------------------
      TObjArray* array = tree->GetListOfBranches();
      if ( ! array )
	fatal("Unable to GetListOfBranches");

      // For EDM compliant Root tuples

      boost::regex  rpat("(?<=configurableAnalysis\\_).*(?=\\_)");
      boost::smatch wpat;

      // For CMSSW objects

      boost::regex  r("[^_]+");
      boost::smatch w;
      boost::regex  r2("s$");

      // Add branches ending in ".obj"

      map<string, int> buffer;

      // Add a few special branches

      int nitems = array->GetEntries();
      for (int i = 0; i < nitems; i++)
	{
	  TBranch* b = (TBranch*)((*array)[i]);
	  TBranch* branch = b;
	  string name = string(branch->GetName());

	  // We need a much more secure way to check for an
	  // n-tuple. For now we'll use this ugly hack.
	  if ( name.substr(0,4) == "reco" ) IsNtuple = false;
	  
	  if ( name != "EventAuxiliary" ) name += string("obj");

	  // Try a couple of variations on the branchname

	  branch = tree->GetBranch( name.c_str() );
	  if ( branch == 0 )
	    {
	      // Try without "obj" postfix
	      name = string(b->GetName());
	      branch = tree->GetBranch( name.c_str() );
	      if ( branch == 0 )
		{
		  cout << "*** pointer for branch " 
		       << name << " is zero" << endl;
		  continue;
		}
	    }
	  if ( DEBUG ) cout << i << "\t" << name << endl;

	  _branchnames.push_back(name);

	  // Check if this is an EDM compliant object
	  if ( boost::regex_search(name, wpat, rpat) )
	    {
	      string s = wpat[0];
	      if ( buffer.find(s) == buffer.end() )
		{
		  buffer[s]=0;
		  _classnames.push_back(s);
		  if ( DEBUG ) cout << "\t\t\tClass<" << s << ">" << endl;
		}
	    }
	  else if ( boost::regex_search(name, w, r) )
	    {
	      string s = w[0];
	      s = boost::regex_replace(s, r2, "");
	      if ( buffer.find(s) == buffer.end() )
		{
		  buffer[s]=0;
		  _classnames.push_back(s);
		  if ( DEBUG ) cout << "\t\t\tVar<" << s << ">" << endl;
		}
	    }
	}
    }
  file->Close();

  // -----------------------------------------------------------------
  // Create a chain of Root-files 
  // -----------------------------------------------------------------
  _vchain.push_back( new TChain(_treenames[0].c_str()) );
  if ( _vchain.back() == 0 ) 
    fatal("Unable to create chain " + _treenames[0]);

  if ( DEBUG ) cout << "Tree: " << _treenames[0] << endl;

  if ( _EDMcompliant )
    {
      if ( IsNtuple )
	{
	  _vchain.back()->SetMakeClass(1);
	  cout << "\tThis seems to be an EDM compliant n-tuple\n" << endl;
	}
      else
	{
	  _vchain.back()->SetMakeClass(0);
	  cout << "\tThis seems to be an EDM compliant file\n" << endl;
	}
    }
  else
    {
      _vchain.back()->SetMakeClass(1);
      cout << "\tThis lseems to be a non-EDM compliant n-tuple file\n" << endl;
    }
  
  for(unsigned i=0; i < _filenames.size(); i++)
    {
      if ( _filenames[i] == "" ) 
	{
	  cout << "\tWarning: filename " << i << " is blank!" << endl;
	  continue;
	}
      _vchain.back()->Add(_filenames[i].c_str());
      if ( DEBUG ) cout << "\tAdd file: " << _filenames[i] << endl;
    }

  // This pointer will be used to access all trees

  _chain = _vchain[0];
  _entries = _chain->GetEntries();
  if ( DEBUG ) cout << "\tEventStream - entries: " << _entries << endl << endl;

  // -----------------------------------------------------------------
  // Make other chains friends of first
  // -----------------------------------------------------------------
  for (unsigned tt=1; tt < _treenames.size(); tt++)
    {
      _vchain.push_back( new TChain(_treenames[tt].c_str()) );
      if ( _vchain.back() == 0 ) 
	fatal("Unable to create chain "+_treenames[tt]);

      // Make current chain a friend of first, so that we have effectively
      // a single chain comprising one of more trees

      if ( DEBUG ) cout << "\tAdd friend chain: " << _treenames[tt] << endl;
      _chain->AddFriend(_treenames[tt].c_str()); 
    }
}

// ------------------------------------------------------------------------
// Update the branches. We do this when we switch from one 
// file to another in a chain of Root-files.
// ------------------------------------------------------------------------
void 
EventStream::_update()
{
  _status = 0;
  _current = _chain->GetTreeNumber();
  for(BranchMapIter it =_branch.begin(); it != _branch.end(); it++)
    {
      string name = it->first;
      if ( DEBUG ) cout << "\tremap branch: " << name << endl;
      it->second.branch  = (TBranch*)_chain->GetBranch(name.c_str());
      if ( it->second.branch == 0 ) 
	fatal("update - pointer is zero for branch " + name);

      // re-map branch to associated data buffer

      if ( it->second.otype == "EventAuxiliary" )
	{
	  TLeaf* leaf = it->second.branch->GetLeaf("EventAuxiliary");
	  leaf->SetAddress( it->second.address );
	}
      else
	{
	  it->second.branch->SetAddress( it->second.address );
	}
    }
}


