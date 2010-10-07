// -*- C++ -*-
//
// Package:    Mkntuple
// Class:      Mkntuple
// 
/**\class Mkntuple Mkntuple.cc 
          PhysicsTools/Mkntuple/src/Mkntuple.cc

 Description: Creates a simple n-tuple from RECO or PAT files.

 Implementation:

     A sense of beauty and common sense

     (A) Introduction

     This analyzer creates simple Root n-tuples using information
     specified in a configuration file. In the configuration file, one
     specifies a buffer for each object type to be read from the input RECO
     or PAT file, the methods to be called and optionally the names that are 
     to be given to the corresponding variables in the output n-tuple. The
     default naming scheme is
     
                stripped-classname + "_" + getByLabel + "." + method

     (B) Mechanism

     For each class, e.g., pat::Muon, we allocate a buffer that is
     initialized with the methods to be called, every event. The return
     value of each method called (once, per event, for every instance 
     of an object of a given type) is stored in a variable whose address 
     is known to Root. The buffer object has two methods: init(...) and 
     fill(...). The init(...) method tells the buffer which methods of
     the associated class to call. The fill(...) method calls these
     methods and caches their return values. The commit() method of the
     n-tuple object writes the cached values to the n-tuple.

     The buffers are allocated dynamically via the CMS plugin mechanism. (The
     plugin factory and plugins are defined in pluginfactory.cc and plugins.cc,
     respectively). The list of buffer objects to be allocated is specified
     in the configuration file, under the list "buffers".
*/
//
// Original Author:  Sezen SEKMEN & Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
//         Updated:  Sun Jan 17 HBP - add log file
//                   Sun Jun 06 HBP - add variables.txt file
//
// $Id: Mkntuple.cc,v 1.19 2010/10/05 11:22:47 prosper Exp $
// ---------------------------------------------------------------------------
#include <boost/regex.hpp>
#include <memory>
#include <iostream>
#include <fstream>
#include <cassert>
#include <time.h>
#include <stdlib.h>

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/FileBlock.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
 
#include "PhysicsTools/Mkntuple/interface/treestream.h"
#include "PhysicsTools/Mkntuple/interface/kit.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "PhysicsTools/Mkntuple/interface/CurrentEvent.h"
#include "PhysicsTools/Mkntuple/interface/Configuration.h"
#include "PhysicsTools/Mkntuple/interface/SelectedObjectMap.h"

#include "TROOT.h"
#include "TSystem.h"
#include "TMap.h"
#include "TString.h"
// ---------------------------------------------------------------------------
using namespace std;

// class Varvector : public TObject 
// { 
// public:
//   Varvector(std::vector<double>* v) 
//     : value(v) {} 
//   ~Varvector() {}
//   std::vector<double>* value;
//   ClassDef(Varvector,0)
// };
// ---------------------------------------------------------------------------
class Mkntuple : public edm::EDAnalyzer 
{
public:
  explicit Mkntuple(const edm::ParameterSet&);
  ~Mkntuple();

private:
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  bool selectEvent(const edm::Event& iEvent);

  void shrinkBuffers();

  // Object that models the output n-tuple.
  otreestream output;

  // Object that models the allocated buffers, one per object to be read.
  std::vector<BufferThing*> buffers;

  // addresses of buffers
  std::map<std::string, BufferThing*> buffermap;
  std::map<std::string, std::vector<double>*> vars;
  bool keep; // true if event is to be kept

  int DEBUG;
  int event_;
  std::string logfilename_;
  std::ofstream* log_;
  std::string analyzername_;
  std::string selectorname_;
  std::string selectorcmd_;
};


Mkntuple::Mkntuple(const edm::ParameterSet& iConfig)
  : output(otreestream(iConfig.getUntrackedParameter<string>("ntupleName"), 
                       "Events", 
                       "made by Mkntuple $Revision: 1.19 $")),
    event_(0),
    logfilename_("Mkntuple.log"),
    log_(new std::ofstream(logfilename_.c_str())),
    selectorname_(""),
    selectorcmd_("")
{
  cout << "\nBEGIN Mkntuple" << endl;

  // Cache config
  Configuration::instance().set(iConfig);

  // Get optional analyzer name
  try
    {
      analyzername_ = iConfig.getUntrackedParameter<string>("analyzerName");
    }
  catch (...)
    {
      analyzername_ = "";
    }

  // Get name of optional selector
  try
    {
      selectorname_ = kit::nameonly(iConfig.
                                    getUntrackedParameter<string>
                                    ("selectorName"));
    }
  catch (...)
    {
      selectorname_ = "";
    }

  if ( selectorname_ != "" )
    {
      // Try to load associated shared library

      // First find shared lib
      string filestem = selectorname_ + string("*.so");
      string cmd = string("find . -name \"") + filestem + "\"";
      string shlib = kit::shell(cmd);
      if ( shlib == "" )
        {
          string errmess = string("\tfind ") + filestem + string(" failed");
          // Have a tantrum!
          throw cms::Exception("FileNotFound", errmess);
        }

      cout << "\t==> Set address of variable vars" << endl;
      gROOT->ProcessLine(Form("map<string, vector<double>*>* vars"
                              "=(map<string, vector<double>*>*)0x%x", &vars));

      cout << "\t==> Set address of variable keep" << endl;
      gROOT->ProcessLine(Form("bool* keep = (bool*)0x%x", &keep)); 
      
      // Found shared library, so try to load it
      if ( gSystem->Load(shlib.c_str()) != 0 )
        throw cms::Exception("LoadFailed",
                             "\tunable to load selector shared library\n\t\t" +
                             shlib);

      // Create command to execute selector
      
      selectorcmd_ = string("*keep = ") 
        + selectorname_ + string("(*vars);");
      cout << "\t==> Loaded selector library: " << shlib 
           << endl << endl;
    }

  if ( getenv("DBMkntuple") > 0 )
    DEBUG = atoi(getenv("DBMkntuple"));
  else
    DEBUG = 0;

  // Write to log file

  time_t tt = time(0);
  string ct(ctime(&tt));
  *log_ << "Created: " << ct << endl;
  log_->close();


  // Write branches and variables to file

  ofstream vout("variables.txt");
  vout << "tree: Events " << ct << endl;
  
  // Allocate buffers.
  //
  // Each buffer descriptor (a series of strings) comprises
  // One line containing
  //    buffer     - name of buffer to allocate
  //    label      - used by getByLabel
  //    maxcount   - maximum number of objects to write out. If omitted,
  //                 assumed to be unity
  //    [prefix]   - prefix for names of n-tuple output variables
  //                 otherwise, take prefix = <buffer>_<label>
  // followed by the list of methods
  // 
  vector<string> vrecords = iConfig.
    getUntrackedParameter<vector<string> >("buffers");

  boost::regex getmethod("[a-zA-Z][^ ]*[(].*[)][^ ]*");
  boost::smatch matchmethod;

  for(unsigned ii=0; ii < vrecords.size(); ii++)
    {
      if ( DEBUG > 1 ) 
        cout << "record(" << vrecords[ii]  << ")" << endl; 
      
      // Get description for current buffer of variables
      vector<string> bufferrecords = iConfig.
        getUntrackedParameter<vector<string> >(vrecords[ii]);

      // Decode first record, which should
      // contain the buffer name, getByLabel and max count, with
      // the exception of buffer edmEvent, which requires only the
      // first field
      string record = bufferrecords[0];

      if ( DEBUG > 1 ) 
        cout << " record(" << record  << ")" << endl; 

      vector<VariableDescriptor> var;
      vector<string> field;              
      kit::split(record, field);

      string buffer = field[0];                 // Buffer name
      if ( DEBUG > 0 )
        cout << "  buffer: " << buffer << endl;

      string label("");
      string prefix = buffer;
      int maxcount=1;

      // edmEventHelper does not use getByLabel since it is just a 
      // helper for edm::Event, so don't crash if there is no
      // getByLabel

      if ( buffer.substr(0,8) != "edmEvent" )
        {
          if ( field.size() < 3 )
            // Have a tantrum!
            throw edm::Exception(edm::errors::Configuration,
                                 "cfg error: "
                                 "you need at least 3 fields in first line of"
                                 " each buffer block\n"
                                 "\tyou blocks you stones you worse than "
                                 "senseless things...");
        }

      // getByLabel
      if ( field.size() > 1 ) label = field[1];

      // max object count to store
      if ( field.size() > 2 ) maxcount  = atoi(field[2].c_str());  

      // n-tuple variable prefix
      if ( field.size() > 3 ) 
        prefix = field[3]; 
      else if (label != "")
        // replace double colon with an "_"
        prefix += string("_") + kit::replace(label, "::", "_");

      //DB
      if ( DEBUG > 1 )
        cout 
          << "   buffer("
          << RED  
          << buffer 
          << BLACK 
          << ")"
          << " label("    
          << BLUE 
          << label 
          << DEFAULT_COLOR 
          << ")"
          << " maxcount(" << maxcount << ")"
          << " prefix("   << prefix << ")"
          << endl;
      
      // Loop over requested methods for current buffer.
      // The first of the pair is the method name.
      // The second of the pair is the name of the associated output variable.
      // If the latter is omitted, it is assumed to be the same as the
      // method name.

      for(unsigned i=1; i < bufferrecords.size(); i++)
        {
          string record = bufferrecords[i];

          // Get method
      
          if ( ! boost::regex_search(record, matchmethod, getmethod) ) 
            // Yet another tantrum!
            throw edm::Exception(edm::errors::Configuration,
                                 "regex error: "
                                 "I can't get method name from \n" +
                                 record);
          string method = kit::strip(matchmethod[0]);
      
          // Get optional method alias name
          
          string varname = method;

          // The left sub-string is the return type of the method
          // The right sub-string is the optional alias
          string left, right;
          kit::bisplit(record, left, right, method);

          string rtype = kit::strip(left);
          right = kit::strip(right);
          if ( right != "" )  varname = right;

          var.push_back(VariableDescriptor(rtype, method, varname));

          if ( DEBUG > 0 )
            cout << "\trtype:  <" << RED   << rtype << BLACK << ">"
                 << "\tmethod: <" << BLUE  << method << BLACK << ">"
                 << "\tvarname:<" << GREEN << varname << BLACK << ">" 
                 << endl;
        }
      
      // Create a buffer of appropriate type...
      // 
      // Note: ->create(...) returns an auto_ptr to BufferThing. 
      // Ordinarily, an auto_ptr owns the object pointed to. But, a 
      // push_back makes a copy of the thing pushed back. The act of 
      // copying the auto_ptr, auto_ptr<BufferThing>, transfers 
      // ownership of the object from the auto_ptr to the vector. 
      // Consequently, in principle, it is up to the user of the 
      // vector to deallocate the memory occupied by the buffer 
      // objects when they are no longer needed. However, the plugin 
      // mechanism assumes (not unreasonably) that it is in charge of 
      // plugins and so is responsible for cleaning up allocated space.
      
      if ( DEBUG > 0 )
        cout 
          << "  create buffer: " << buffer << endl;
          
      buffers.push_back( BufferFactory::get()->create(buffer) );
      if (buffers.back() == 0)
        throw edm::Exception(edm::errors::Configuration,
                             "plugin error: "
                             "unable to create buffer " + buffer + 
                             "\n\t...let all the evil "
                             "that lurks in the mud hatch out\n");

      // ... and initialize it
      buffers.back()->init(output, label, prefix, var, maxcount, 
                           vout, DEBUG);

      if ( DEBUG > 0 )
        cout << "  buffer: " << buffer << " created " << endl << endl;

      // cache addresses of buffers
      buffermap[prefix] = buffers.back();

      // cache variable addresses
      boost::regex getname("[^[]+");
      boost::smatch m;
      vector<string>& vnames = buffers.back()->varnames();
      for(unsigned int iname=0; iname < vnames.size(); ++iname)
        {
          string fullname = vnames[iname];
          boost::regex_search(fullname, m, getname);
          string name = m[0];
          vars[name] = buffers.back()->variable(name);
        }
    }
  vout.close();

  // Create ntuple analyzer template if requested
  if ( analyzername_ != "" )
    kit::shell("mkanalyzer.py " + analyzername_ + " variables.txt");

  cout << "END Mkntuple" << endl;
}


Mkntuple::~Mkntuple()
{
  if ( DEBUG > 0 ) cout << "CLEANUP(Mkntuple)" << endl;
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
Mkntuple::analyze(const edm::Event& iEvent, 
                  const edm::EventSetup& iSetup)
{
  // Cache current event
  event_++;
  CurrentEvent::instance().set(iEvent, event_);

  // Loop over allocated buffers and for each call its fill method
  
  string message("");  
  for(unsigned i=0; i < buffers.size(); i++)
    if ( !buffers[i]->fill(iEvent) ) message += buffers[i]->message();

  // Check for error report from buffers

  if ( message != "" )
    {
      time_t tt = time(0);
      string ct(ctime(&tt));
      
      log_->open(logfilename_.c_str(), ios::app);
      *log_ << endl
            << "REPORT " << event_ << "\t" << ct << endl 
            << message << endl
            << "END" << endl;
      log_->close();
    }

  // Apply optional cuts

  if ( ! selectEvent(iEvent) ) return;

  // Copy data to output buffers

  output.store();

  // Event kept. Shrink buffers as needed. Shrinking is needed if only
  // certain objects of a given buffer have been selected.

  //shrinkBuffers();

  output.save();
}

bool
Mkntuple::selectEvent(const edm::Event& event)
{
  keep = true;
  if ( selectorcmd_ == "" ) return keep;

  // Clear selection buffer
  SelectedObjectMap::instance().clear();

  // Execute selector
  
  gROOT->ProcessLineFast(selectorcmd_.c_str());
  if ( keep )
    cout << "\t\t** KEEP EVENT(" << event_ << ")"
         << "\tRun: " << event.id().run() 
         << "\tEvent: " << event.id().event()
         << endl;
  else
    cout << "\t\t** SKIP EVENT(" << event_ << ")"
         << "\tRun: " << event.id().run() 
         << "\tEvent: " << event.id().event()
         << endl;

  return keep;
}

void
Mkntuple::shrinkBuffers()
{
  if ( selectorcmd_ == "" ) return;

  map<string, vector<int> >& smap = SelectedObjectMap::instance().get();
  map<string, vector<int> >::iterator iter = smap.begin();
  for(iter=smap.begin(); iter != smap.end(); ++iter)
    {
      string name(iter->first);
      vector<int>& index = iter->second;
      
      if ( buffermap.find(name) != buffermap.end() )
        {
          //DEBUG
          if (DEBUG > 0) 
            cout << "\t** SHRINK( " << name << " ) " << index.size() << endl;
          buffermap[name]->shrink(index);
        }
      else
        throw edm::Exception(edm::errors::Configuration,
                             "object selection error: "
                             "bad internal buffer name - " + name + 
                             "\n\tyou blocks you stones you worse "
                             "than senseless things\n");
    }
}

// --- method called once each job just before starting event loop  -----------
void 
Mkntuple::beginJob()
{
}

// --- method called once each job just after ending the event loop  ----------
void 
Mkntuple::endJob() 
{
  output.close();
}

//define this as a plug-in
DEFINE_FWK_MODULE(Mkntuple);
