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
// $Id: Mkntuple.cc,v 1.3 2010/01/20 04:57:23 prosper Exp $
//
//
// ---------------------------------------------------------------------------
#include <boost/regex.hpp>
#include <memory>
#include <iostream>
#include <fstream>
#include <cassert>
#include <time.h>
 
#include "PhysicsTools/LiteAnalysis/interface/treestream.hpp"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "TROOT.h"

using namespace std;

class Mkntuple : public edm::EDAnalyzer 
{
public:
  explicit Mkntuple(const edm::ParameterSet&);
  ~Mkntuple();


private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // Object that models the output n-tuple.
  otreestream output;

  // Object that models the allocated buffers, one per object to be read.
  std::vector<BufferThing*> buffers;

  int DEBUG;
  int event_;
  std::string logfilename_;
  std::ofstream* log_;
};


Mkntuple::Mkntuple(const edm::ParameterSet& iConfig)
  : output(otreestream(iConfig.getUntrackedParameter<string>("ntupleName"), 
                       "Events", 
                       "The time has come the walrus said")),
    event_(0),
    logfilename_("Mkntuple.log"),
    log_(new std::ofstream(logfilename_.c_str()))
{
  cout << GREEN << "BEGIN Mkntuple" << BLACK << endl;

  try
    {
      DEBUG = iConfig.getUntrackedParameter<int>("debug");
    }
  catch (...)
    {
      DEBUG = 0;
    }

  // Write to log file

  time_t tt = time(0);
  string ct(ctime(&tt));
  *log_ << "Mkntuple log started: " << ct << endl;

  // Allocate buffers.
  //
  // Each buffer descriptor (a series of strings) comprises
  // One line containing
  //    buffer     - name of buffer to allocate
  //    label      - used by getByLabel
  //    maxcount   - maximum number of objects to write out. If omitted,
  //    [prefix]   - prefix for names of n-tuple output variables
  //                 otherwise, take prefix = <buffer>_<label>
  // followed by the list of methods
  // 
  vector<string> vrecords = iConfig.
    getUntrackedParameter<vector<string> >("buffers");

  for(unsigned ii=0; ii < vrecords.size(); ii++)
    {
      if ( DEBUG > 1 ) 
        cout << "record(" << vrecords[ii]  << ")" << endl; 
      
      // Get description for current buffer of variables
      vector<string> bufferrecords = iConfig.
        getUntrackedParameter<vector<string> >(vrecords[ii]);

      // Decode first record, which should
      // contain the buffer name, getByLabel and max count
      string record = bufferrecords[0];

      if ( DEBUG > 1 ) 
        cout << " record(" << record  << ")" << endl; 

      vector<VariableDescriptor> var;
      vector<string> field;              
      kit::split(record, field);
      if ( field.size() < 3 )
        // Have a tantrum!
        throw edm::Exception(edm::errors::Configuration,
                             "cfg error: "
                             "you need at least 3 fields in first line of"
                             " each buffer block\n"
                             + RED +
                             "\tyou blocks you stones you worse than "
                             "senseless things..." + BLACK);
      
      string buffer = field[0];                 // Buffer name
      string label  = field[1];                 // getByLabel
      int maxcount  = atoi(field[2].c_str());   // max object count to store
      string prefix = buffer + "_" + label;
      if (field.size() == 4) prefix = field[3]; // n-tuple variable prefix
      
      *log_ << "  create buffer: " << buffer << endl;      
      
      //DB
      if ( DEBUG > 0 )
        cout 
          << GREEN
          << "   buffer("   << buffer << ")"
          << BLACK
          << " label("    << label << ")"
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
      
          boost::regex getmethod("[a-zA-Z][^ ]*[(].*[)]");
          boost::smatch matchmethod;
          if ( ! boost::regex_search(record, matchmethod, getmethod) ) 
            // Yet another tantrum!
            throw edm::Exception(edm::errors::Configuration,
                                 "regex error: " +
                                 RED +
                                 "I can't get method name from \n" +
                                 BLACK +
                                 record);
          string method = kit::strip(matchmethod[0]);
      
          if ( DEBUG > 1 ) 
            cout << "    method: " << method << endl;
          
          // Get optional method alias name
          
          string varname = kit::truncate(method,"(");
          boost::regex getalias("(?<=[)]) .+");
          boost::smatch matchalias;
          if ( boost::regex_search(record, matchalias, getalias) ) 
            varname = kit::strip(matchalias[0]);
          
          //       // Get return type

          //       string rtype("");
          //       boost::regex getrtype(".+(?= " + method + ")");
          //       boost::smatch matchrtype;
          //       if ( boost::regex_search(record, matchrtype, getrtype) ) 
          //         rtype = kit::strip(matchrtype[0]);
          
          var.push_back(VariableDescriptor(method, varname));

          if ( DEBUG > 1 )
            cout << "\t  varname(" << varname << ")"
                 << endl;
          
          // Write some info to log file

          *log_ << "    method: " << method << endl;
        
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
          << "  created buffer: " << buffer << endl << endl;
          
      buffers.push_back( BufferFactory::get()->create(buffer) );
      if (buffers.back() == 0)
        throw edm::Exception(edm::errors::Configuration,
                             "plugin error: "
                             "unable to create buffer " + buffer + 
                             "\n\tso" +
                             RED + 
                             "...let all the evil "
                             "that lurks in the mud hatch out\n"
                             + BLACK);
      // ... and initialize it
      buffers.back()->init(output, label, prefix, var, maxcount, DEBUG);
    }
  log_->close();

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
  event_++;

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

  output.commit();
}


// --- method called once each job just before starting event loop  -----------
void 
Mkntuple::beginJob(const edm::EventSetup&)
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