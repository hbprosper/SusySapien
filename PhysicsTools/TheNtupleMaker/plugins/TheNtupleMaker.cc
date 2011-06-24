// -*- C++ -*-
//
// Package:    TheNtupleMaker
// Class:      TheNtupleMaker
// 
/**\class TheNtupleMaker TheNtupleMaker.cc 
          PhysicsTools/TheNtupleMaker/src/TheNtupleMaker.cc

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
//                   Sun Nov 07 HBP - add event setup to fill
//                   Sat Mar 12 2011 HBP - change selectorname to usermacroname
//                   Wed May 04 2011 HBP - change name to TheNtupleMaker
//                   Fri Jun 24 2011 HBP - get provenance info using 
//                                   gSystem->Getenv(...)
//
// $Id: TheNtupleMaker.cc,v 1.5 2011/06/07 07:41:55 prosper Exp $
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
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "PhysicsTools/TheNtupleMaker/interface/treestream.h"
#include "PhysicsTools/TheNtupleMaker/interface/kit.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
#include "PhysicsTools/TheNtupleMaker/interface/CurrentEvent.h"
#include "PhysicsTools/TheNtupleMaker/interface/Configuration.h"
#include "PhysicsTools/TheNtupleMaker/interface/SelectedObjectMap.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TMap.h"
#include "TString.h"
// ---------------------------------------------------------------------------
using namespace std;
// ---------------------------------------------------------------------------
class TheNtupleMaker : public edm::EDAnalyzer 
{
public:
  explicit TheNtupleMaker(const edm::ParameterSet&);
  ~TheNtupleMaker();

private:
  virtual void beginJob();
  //virtual void beginRun(const edm::Run&, const edm::EventSetup&);
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

  // map from variables to buffer location and count
  std::map<std::string, countvalue>   vars;
  bool keep; // true if event is to be kept

  int DEBUG;

  std::string logfilename_;
  std::ofstream* log_;
  std::string analyzername_;
  std::string usermacroname_;
  std::string varscmd_;
  std::string boolcmd_;
  std::string treecmd_;
  std::string usermacrocmd_;

  int count_;
  int imalivecount_;
  int logger_;
  bool haltlogger_;

  TTree* ptree_;
  int inputCount_;
};


TheNtupleMaker::TheNtupleMaker(const edm::ParameterSet& iConfig)
  : output(otreestream(iConfig.getUntrackedParameter<string>("ntupleName"), 
                       "Events", 
                       "created by TheNtupleMaker $Revision: 1.5 $")),
    logfilename_("TheNtupleMaker.log"),
    log_(new std::ofstream(logfilename_.c_str())),
    usermacroname_(""),
    varscmd_(""),
    boolcmd_(""),
    usermacrocmd_(""),
    count_(0),
    imalivecount_(1000),
    logger_(0),
    haltlogger_(false),
    inputCount_(0)
{
  cout << "\nBEGIN TheNtupleMaker Configuration" << endl;

  // --------------------------------------------------------------------------
  // Add a provenance tree to ntuple
  // --------------------------------------------------------------------------
  TFile* file = output.file();
  ptree_ = new TTree("Provenance","created by TheNtupleMaker $Revision: 1.5 $");
  const char* chr=0;
  string cmsver("");
  chr = gSystem->Getenv("CMSSW_VERSION");
  if ( chr ) cmsver = string(cmsver);
  ptree_->Branch("cmssw_version", (void*)(cmsver.c_str()), "cmssw_version/C");

  time_t tt = time(0);
  string ct(ctime(&tt));
  ptree_->Branch("date", (void*)(ct.c_str()), "date/C");

  string hostname("");
  chr = gSystem->Getenv("HOSTNAME");
  if ( chr ) hostname = string(chr);
  ptree_->Branch("hostname", (void*)(hostname.c_str()), "hostname/C");

  //string username = kit::strip(kit::shell("echo $USER"));
  string username("");
  chr = gSystem->Getenv("USER");
  if ( chr ) username = string(chr);
  ptree_->Branch("username", (void*)(username.c_str()), "username/C");

  ptree_->Branch("inputcount", &inputCount_, "inputcount/I");

  // Ok, fill this branch
  file->cd();
  ptree_->Fill();

  // --------------------------------------------------------------------------
  // Cache global configuration 
  Configuration::instance().set(iConfig);
    
  // Get optional configuration parameters

  try
    {
      imalivecount_ = iConfig.getUntrackedParameter<int>("imaliveCount");
    }
  catch (...)
    {}

  // Save Tree header and branch buffers after every autosaveCount Mbytes
  // have been written to file. Default is 100 Mbytes.
  int autosaveCount = 100;
  try
    {
      autosaveCount = iConfig.getUntrackedParameter<int>("autosaveCount");
    }
  catch (...)
    {}
  output.autosave(autosaveCount);

  try
    {
      logger_ = iConfig.getUntrackedParameter<int>("loggerCount");
      haltlogger_ = logger_ >= 0;
    }
  catch (...)
    {}

  // Get optional analyzer name
  try
    {
      analyzername_ = iConfig.getUntrackedParameter<string>("analyzerName");
    }
  catch (...)
    {
      analyzername_ = "";
    }

  // Get name of optional usermacro
  try
    {
      usermacroname_ = kit::nameonly(iConfig.
                                    getUntrackedParameter<string>
                                    ("macroName"));
    }
  catch (...)
    {
      usermacroname_ = "";
    }

  // --------------------------------------------------------------------------
  if ( usermacroname_ != "" )
    {
      // Try to load associated shared library 

      // First find shared lib
      string filestem = string("*") + usermacroname_ + string("*.so");
      string cmd = string("find . -name \"") + filestem + "\"";
      string shlib = kit::shell(cmd);
      if ( shlib == "" )
        {
          string errmess = string("\tfind ") + filestem + string(" failed");
          // Have a tantrum!
          throw cms::Exception("FileNotFound", errmess);
        }

      // Found shared library, so try to load it

      cout << "\t==> Load usermacro library " << shlib << endl;
      
      if ( gSystem->Load(shlib.c_str()) != 0 )
         throw cms::Exception("LoadFailed",
                              "\tfor shared library\n\t\t" + shlib);

      // Create commands to execute usermacro using CINT

      varscmd_ = string("map<string,countvalue>* vars="
                      "(map<string,countvalue>*)0x%x");
      boolcmd_ = string("bool* keep = (bool*)0x%x"); 
      treecmd_ = string("TTree* tree = (TTree*)0x%x"); 
      usermacrocmd_ = string("*keep = ") 
        + usermacroname_ + string("(*vars, *tree);");
    }

  if ( getenv("DBTheNtupleMaker") > 0 )
    DEBUG = atoi(getenv("DBTheNtupleMaker"));
  else
    DEBUG = 0;

  // Write to log file

  *log_ << "Created: " << ct << endl;
  log_->close();

  // --------------------------------------------------------------------------
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
  // Helper methods may optionally contain strings with the format
  //   parameter parameter-name = parameter-value
  //
  vector<string> vrecords = iConfig.
    getUntrackedParameter<vector<string> >("buffers");

  boost::regex getmethod("[a-zA-Z][^ ]*[(].*[)][^ ]*|[a-zA-Z]+$");
  boost::smatch matchmethod;

  boost::regex isparam("^ *p[a-z] *");
  boost::smatch matchparam;

  for(unsigned ii=0; ii < vrecords.size(); ii++)
    {
      if ( DEBUG > 1 ) 
        cout << "record(" << vrecords[ii]  << ")" << endl; 
      
      // Get description for current buffer of variables
      vector<string> bufferrecords = iConfig.
        getUntrackedParameter<vector<string> >(vrecords[ii]);

      // Decode first record which should
      // contain the buffer name, getByLabel and max count, with
      // the exception of buffer edmEvent, which requires only the
      // first field
      string record = bufferrecords[0];

      // If current buffer is a Helper, create a parameter set specific to
      // the Helper from this string.
      //string localparam("");

      // Structure containing information about the methods to be called.
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

          // Check for a Helper parameter

//           if ( boost::regex_search(record, matchparam, isparam) )
//             {
//               std::string param = kit::replace(record,
//                                                matchparam[0], "");

//               string key, value;
//               kit::bisplit(param, key, value, "=");
//               paramset.addUntrackedParameter(key, value);
//               continue;
//             }

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
        throw cms::Exception("PluginLoadFailure")
          << "\taaargh!...let all the evil "
          << "that lurks in the mud hatch out\n"
          << "\tI'm unable to create buffer " + buffer; 

      // ... and initialize it
      buffers.back()->init(output, label, prefix, var, maxcount, 
                           vout, DEBUG);

      // cache addresses of buffers
      buffermap[prefix] = buffers.back();

      if ( DEBUG > 0 )
        cout << "  buffer: " << buffer << " created " << endl << endl;
    }
  vout.close();

  // Cache variable addresses for each buffer

  int index=0;
  cout << endl << endl << " BEGIN Branches " << endl;
  for(unsigned int i=0; i < buffers.size(); ++i)
    {
      vector<string>& vnames = buffers[i]->varnames();
      cout << i+1 << "\t" << vnames.size() << endl;
      for(unsigned int ii=0; ii < vnames.size(); ++ii)
        {
          string name = vnames[ii];
          vars[name] = buffers[i]->variable(name);
          index++;
          cout << "  " 
               << index << "\t" << name << endl;
        }
    }
  cout << " END Branches" << endl << endl;

  // Create ntuple analyzer template if requested

  if ( analyzername_ != "" )
    kit::shell("mkanalyzer.py " + analyzername_ + " variables.txt");


  // Check for crash switch
  
  bool crash = true;
  try
    {
      crash = 
        (bool)Configuration::instance().
        get()->getUntrackedParameter<int>("crashOnInvalidHandle");
    }
  catch (...)
    {}

  if ( crash )
    cout << "\t==> TheNtupleMaker will CRASH if a handle is invalid <==";
  else
    cout << "\t==> TheNtupleMaker will WARN if a handle is invalid <==";
  cout << endl << endl;

  cout << "END TheNtupleMaker Configuration" << endl;
}


TheNtupleMaker::~TheNtupleMaker()
{
  if ( DEBUG > 0 ) cout << "CLEANUP(TheNtupleMaker)" << endl;
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
TheNtupleMaker::analyze(const edm::Event& iEvent, 
                        const edm::EventSetup& iSetup)
{
  // Cache current event and event setup
  CurrentEvent::instance().set(iEvent, iSetup);

  // Call methods for each buffer
  for(unsigned i=0; i < buffers.size(); i++) buffers[i]->fill(iEvent, iSetup);

  inputCount_++;
  count_++;
  if ( count_ % imalivecount_ == 0 )
    cout << "\t=> event count: " << count_ << endl;

  //string message("");  
   //if ( !buffers[i]->fill(iEvent, iSetup) ) message += buffers[i]->message();

  // Check for error report from buffers

//   if ( message != "" )
//     {
//       time_t tt = time(0);
//       string ct(ctime(&tt));
      
//       log_->open(logfilename_.c_str(), ios::app);
//       *log_ << endl
//             << "REPORT RUN: " 
//             << iEvent.id().run() << "\tEvent: " << iEvent.id().event() 
//             << "\t" << ct << endl 
//             << message << endl
//             << "END" << endl;
//       log_->close();
//     }

  // Apply optional cuts

  if ( ! selectEvent(iEvent) ) return;

  // Copy data to output buffers

  output.store();

  //Event kept. Shrink buffers as needed. Shrinking is needed if only
  //certain objects of a given buffer have been selected.

  //shrinkBuffers();


  // Ok, fill this branch
  TFile* file = output.file();
  file->cd();
  ptree_->Fill();

  output.save();
}

bool
TheNtupleMaker::selectEvent(const edm::Event& event)
{
  keep = true;
  if ( usermacrocmd_ == "" ) return keep;

  // Clear selection buffer
  SelectedObjectMap::instance().clear();

  // Execute usermacro (a compiled Root macro)

  TTree* tree = output.tree();

  gROOT->ProcessLine(Form(varscmd_.c_str(), &vars));
  gROOT->ProcessLine(Form(treecmd_.c_str(), tree));
  gROOT->ProcessLine(Form(boolcmd_.c_str(), &keep)); 
  gROOT->ProcessLineFast(usermacrocmd_.c_str());

  if ( DEBUG )
    {
      if ( keep )
        cout << "\t\t** KEEP EVENT(" 
             << "Run: " << event.id().run() 
             << "\tEvent: " << event.id().event() << ")"
             << endl;
      else
        cout << "\t\t** SKIP EVENT("
             << "Run: " << event.id().run() 
             << "\tEvent: " << event.id().event() << ")"
             << endl;
    }

  return keep;
}

void
TheNtupleMaker::shrinkBuffers()
{
  if ( usermacrocmd_ == "" ) return;

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
TheNtupleMaker::beginJob()
{
}

/*
void 
TheNtupleMaker::beginRun(const edm::Run& run, const edm::EventSetup& eventsetup)
{
}
*/

// --- method called once each job just after ending the event loop  ----------
void 
TheNtupleMaker::endJob() 
{
  output.close();
}

//define this as a plug-in
DEFINE_FWK_MODULE(TheNtupleMaker);
