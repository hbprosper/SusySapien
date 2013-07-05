#!/usr/bin/env python
#------------------------------------------------------------------------------
# Description: Create ntuple analyzer using information supplied in a
#              variables.txt file. (See mkvariables.py).
#
# Created: 06-Mar-2010 Harrison B. Prosper
# Updated: 12-Mar-2010 HBP - fix appending of .root
#          08-Jun-2010 HBP - add creation of selector.h
#          02-Sep-2010 HBP - fix variables.txt record splitting bug
#          01-Oct-2010 HBP - add structs
#          02-Oct-2010 HBP - add cloning
#          10-Jan-2011 HBP - merge histFile and skimFile into outputFile
#          11-Mar-2011 HBP - fix naming bug
#          26-Apr-2011 HBP - alert user only if duplicate name is not a leaf
#                            counter
#          04-Jul-2013 HBP - make a better analyzer work area
#
#$Id: mkanalyzer.py,v 1.21 2013/07/05 07:15:14 prosper Exp $
#------------------------------------------------------------------------------
import os, sys, re, posixpath
from string import atof, atoi, replace, lower,\
	 upper, joinfields, split, strip, find
from time import sleep, ctime
from glob import glob
#------------------------------------------------------------------------------
# Functions
#------------------------------------------------------------------------------
getvno = re.compile(r'[0-9]+$')

def usage():
	print '''
	Usage:
	   mkanalyzer.py <analyzer-name> [variables.txt]
	'''
	sys.exit(0)
	
def nameonly(s):
	return posixpath.splitext(posixpath.split(s)[1])[0]

def join(left, a, right):
	s = ""
	for x in a:
		s = s + "%s%s%s" % (left, x, right)
	return s

def getauthor():
	regex  = re.compile(r'(?<=[0-9]:)[A-Z]+[a-zA-Z. ]+')
	record = strip(os.popen("getent passwd $USER").read())
	author = "Shakepeare's ghost"
	if record != "":
		t = regex.findall(record)
		if len(t) > 0: author = t[0]
	return author
#------------------------------------------------------------------------------
AUTHOR = getauthor()

if os.environ.has_key("CMSSW_BASE"):
	CMSSW_BASE = os.environ["CMSSW_BASE"]
	PACKAGE = "%s/src/PhysicsTools/TheNtupleMaker" % CMSSW_BASE
	TREESTREAM_HPP = "%s/interface/treestream.h" % PACKAGE
	TREESTREAM_CPP = "%s/src/treestream.cc" % PACKAGE
	PDG_HPP = "%s/interface/pdg.h" % PACKAGE
	PDG_CPP = "%s/src/pdg.cc" % PACKAGE
else:
	TREESTREAM_HPP = "treestream.h"
	TREESTREAM_CPP = "treestream.cc"
	PDG_HPP = "pdg.h"
	PDG_CPP = "pdg.cc"

# Make sure that we can find treestream etc.

if not os.path.exists(TREESTREAM_HPP):
	print "\n** error ** - required file:\n\t%s\n\t\tNOT found!" % \
		  TREESTREAM_HPP
	sys.exit(0)

if not os.path.exists(TREESTREAM_CPP):
	print "\n** error ** - required file:\n\t%s\n\t\tNOT found!" % \
		  TREESTREAM_CPP
	sys.exit(0)
#------------------------------------------------------------------------------
MACRO_DECL_H =\
'''
//-----------------------------------------------------------------------------
// -- Declare variables
//-----------------------------------------------------------------------------
namespace evt {
%(vardecl)s
//-----------------------------------------------------------------------------
// --- Structs can be filled by calling fillObjects()
//-----------------------------------------------------------------------------
%(structdecl)s
%(structimpl)s
}; // end namespace evt
'''

MACRO_IMPL_H =\
'''
%(impl)s
'''

TEMPLATE_H =\
'''#ifndef %(NAME)s_H
#define %(NAME)s_H
//-----------------------------------------------------------------------------
// File:        %(name)s.h
// Description: Analyzer header for ntuples created by TheNtupleMaker
// Created:     %(time)s by mkanalyzer.py
// Author:      %(author)s
//-----------------------------------------------------------------------------
// -- System

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "treestream.h"
#include "pdg.h"

// -- Root

#include "TROOT.h"
#include "TApplication.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TKey.h"
#include "TH1F.h"
#include "TH2F.h"
//-----------------------------------------------------------------------------
// -- Declare variables
//-----------------------------------------------------------------------------
namespace evt {
%(vardecl)s
//-----------------------------------------------------------------------------
// --- Structs can be filled by calling fillObjects()
// --- after the call to stream.read(...)
//-----------------------------------------------------------------------------
%(structdecl)s
%(structimpl)s
//-----------------------------------------------------------------------------
// --- Call saveSelectedObjects() just before call to addEvent if
// --- you wish to save only the selected objects
//-----------------------------------------------------------------------------
%(selectimpl)s
//-----------------------------------------------------------------------------
// -- Select variables to be read
//-----------------------------------------------------------------------------
void selectVariables(itreestream& stream)
{
%(selection)s
}
}; // end namespace evt


//-----------------------------------------------------------------------------
// -- Utilities
//-----------------------------------------------------------------------------
void
error(std::string message)
{
  std::cout << "** error ** " << message << std::endl;
  exit(0);
}

std::string 
strip(std::string line)
{
  int l = line.size();
  if ( l == 0 ) return std::string("");
  int n = 0;
  while (((line[n] == 0)    ||
	  (line[n] == ' ' ) ||
	  (line[n] == '\\n') ||
	  (line[n] == '\\t')) && n < l) n++;

  int m = l-1;
  while (((line[m] == 0)    ||
	  (line[m] == ' ')  ||
	  (line[m] == '\\n') ||
	  (line[m] == '\\t')) && m > 0) m--;
  return line.substr(n,m-n+1);
}

std::string
nameonly(std::string filename)
{
  int i = filename.rfind("/");
  int j = filename.rfind(".");
  if ( j < 0 ) j = filename.size();
  return filename.substr(i+1,j-i-1);
}
//-----------------------------------------------------------------------------
struct outputFile
{
  outputFile(std::string filename)
   : filename_(filename),
	 file_(new TFile(filename_.c_str(), "recreate")),
	 tree_(0),
	 b_weight_(0),
	 entry_(0),
	 SAVECOUNT_(50000)
  {
	file_->cd();
	hist_ = new TH1F("counts", "", 1,0,1);
	hist_->SetBit(TH1::kCanRebin);
	hist_->SetStats(0);
  }

  outputFile(std::string filename, itreestream& stream, int savecount=50000) 
   : filename_(filename),
	 file_(new TFile(filename.c_str(), "recreate")),
	 tree_(stream.tree()->CloneTree(0)),
	 b_weight_(tree_->Branch("eventWeight", &weight_, "eventWeight/D")),
	 entry_(0),
	 SAVECOUNT_(savecount)
  {
	std::cout << "events will be skimmed to file "
			  << filename_ << std::endl;
	file_->cd();
	hist_ = new TH1F("counts", "", 1,0,1);
	hist_->SetBit(TH1::kCanRebin);
	hist_->SetStats(0);
  }

  void addEvent(double weight=1)
  {
    if ( tree_ == 0 ) return;
	
    weight_ = weight;
	file_   = tree_->GetCurrentFile();
	file_->cd();
	tree_->Fill();

	entry_++;
	if ( entry_ %(percent)s SAVECOUNT_ == 0 )
	  tree_->AutoSave("SaveSelf");
  }

  void count(std::string cond, double w=1)
  {
    hist_->Fill(cond.c_str(), w);
  }
  
  void close()
  {
  	std::cout << "==> histograms saved to file " << filename_ << std::endl;
    if ( tree_ != 0 )
	  {
	    std::cout << "==> events skimmed to file " << filename_ << std::endl;
	    file_ = tree_->GetCurrentFile();
	  }
	file_->cd();
	//file_->Write("", TObject::kWriteDelete);
	file_->Write();
	file_->ls();
	file_->Close();
  }

  std::string filename_;  
  TFile* file_;
  TTree* tree_;
  TH1F*  hist_;
  TBranch* b_weight_;
  double     weight_;
  int    entry_;
  int    SAVECOUNT_;
};

struct commandLine
{
  std::string progname;
  std::string filelist;
  std::string outputfilename;
};


void
decodeCommandLine(int argc, char** argv, commandLine& cl)
{
  cl.progname = std::string(argv[0]);

  // 1st (optional) argument
  if ( argc > 1 )
	cl.filelist = std::string(argv[1]);
  else
	cl.filelist = std::string("filelist.txt");

  // 2nd (optional) command line argument
  if ( argc > 2 ) 
	cl.outputfilename = std::string(argv[2]);
  else
	cl.outputfilename = cl.progname + std::string("_histograms");

  // Make sure extension is ".root"
  std::string name = cl.outputfilename;
  if ( name.substr(name.size()-5, 5) != std::string(".root") )
    cl.outputfilename += std::string(".root");
}

// Read ntuple filenames from file list

std::vector<std::string>
getFilenames(std::string filelist)
{
  std::ifstream stream(filelist.c_str());
  if ( !stream.good() ) error("unable to open file: " + filelist);

  // Get list of ntuple files to be processed

  std::vector<std::string> v;
  std::string filename;
  while ( stream >> filename )
	if ( strip(filename) != "" ) v.push_back(filename);
  return v;
}

#endif
'''

TEMPLATE_CC =\
'''//-----------------------------------------------------------------------------
// File:        %(name)s.cc
// Description: Analyzer for ntuples created by TheNtupleMaker
// Created:     %(time)s by mkanalyzer.py
// Author:      %(author)s
//-----------------------------------------------------------------------------
#include "%(name)s.h"
using namespace std;
using namespace evt;
//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  // Get file list and histogram filename from command line

  commandLine cmdline;
  decodeCommandLine(argc, argv, cmdline);

  // Get names of ntuple files to be processed and open chain of ntuples

  vector<string> filenames = getFilenames(cmdline.filelist);
  itreestream stream(filenames, "%(treename)s");
  if ( !stream.good() ) error("unable to open ntuple file(s)");

  // Get number of events to be read

  int nevents = stream.size();
  cout << "Number of events: " << nevents << endl;

  // Select variables to be read

  selectVariables(stream);


  // The root application is needed to make canvases visible during
  // program execution. If this is not needed, just comment out the following
  // line

  TApplication app("analyzer", &argc, argv);

  /*
	 Notes:
	
	 1. Use
	   ofile = outputFile(cmdline.outputfile, stream)
	
	 to skim events to output file in addition to writing out histograms.
	
	 2. Use
	   ofile.addEvent(event-weight)
	
	 to specify that the current event is to be added to the output file.
	 If omitted, the event-weight is defaulted to 1.
	
	 3. Use
	    ofile.count(cut-name, event-weight)
	
	 to keep track, in the count histogram, of the number of events
	 passing a given cut. If omitted, the event-weight is taken to be 1.
	 If you want the counts in the count histogram to appear in a given
	 order, specify the order, before entering the event loop, as in
	 the example below
	 
	    ofile.count("NoCuts", 0)
		ofile.count("GoodEvent", 0)
		ofile.count("Vertex", 0)
		ofile.count("MET", 0)
  */
  
  outputFile ofile(cmdline.outputfilename);

  //---------------------------------------------------------------------------
  // Declare histograms
  //---------------------------------------------------------------------------



  //---------------------------------------------------------------------------
  // Loop over events
  //---------------------------------------------------------------------------

  for(int entry=0; entry < nevents; ++entry)
	{
	  // Read event into memory
	  stream.read(entry);

	  // Uncomment the following line if you wish to copy variables into
	  // structs. See the header file %(name)s.h to find out what structs
	  // are available. Each struct contains the field "selected", which
	  // can be set as needed. Call saveSelectedObjects() before a call to
	  // addEvent if you wish to save only the selected objects.
	  
	  // fillObjects();
	  
	  // ---------------------
	  // -- event selection --
	  // ---------------------


	  // ---------------------
	  // -- fill histograms --
	  // ---------------------	  

	}

  stream.close();
  ofile.close();
  return 0;
}
'''

PYTEMPLATELIB =\
'''# -----------------------------------------------------------------------------
#  File:        %(name)slib.py
#  Description: Analyzer for ntuples created by TheNtupleMaker
#  Created:     %(time)s by mkntanalyzer.py
#  Author:      %(author)s
# -----------------------------------------------------------------------------
from ROOT import *
from time import sleep
from string import *
from array import array
from math import *
from sys import exit
import os, sys, re
# -----------------------------------------------------------------------------
STANDALONE = True  # set to False to work within CMSSW
if STANDALONE:
    gROOT.ProcessLine(".L treestream.cc+")
    gROOT.ProcessLine(".L pdg.cc+")
else:
    from PhysicsTools.TheNtupleMaker.AutoLoader import *
# -----------------------------------------------------------------------------
# -- Classes, procedures and functions
# -----------------------------------------------------------------------------
class outputFile:

    def __init__(self, filename, stream=None, savecount=50000):
		if stream != None:
		    print "events will be skimmed to file", filename
		    self.tree = stream.tree().CloneTree(0)
			self.weight = Double()
			self.b_weight = self.tree.Branch("eventWeight", self.weight,
											 "eventWeight/D")
			self.SAVECOUNT = savecount
		else:
			self.tree = None
			self.b_weight = None

		self.entry = 0
			
		self.filename = filename
		self.file = TFile(filename, "recreate")

		self.hist = TH1F("counts", "", 1, 0, 1)
		self.hist.SetBit(TH1.kCanRebin)
		self.hist.SetStats(0)

		self.b_weight = 0

	def addEvent(self, weight=1.0):
		if self.tree == None: return
		
		self.file = self.tree.GetCurrentFile()
		self.file.cd()
		self.tree.Fill()

		self.entry += 1		
		if self.entry %(percent)s self.SAVECOUNT == 0:
			self.tree.AutoSave("SaveSelf")
				
	def count(self, cond, w=1):
		self.hist.Fill(cond, w)
		
	def close(self):
		print "==> histograms saved to file", self.filename
		if self.tree != None:			
			print "==> events skimmed to file", self.filename
			self.file = self.tree.GetCurrentFile()
			
		self.file.cd()
		#self.file.Write("", TObject.kOverwrite)
		self.file.Write()
		self.file.ls()
		self.file.Close()
# -----------------------------------------------------------------------------
class commandLine:
	def __init__(self):
		pass

def decodeCommandLine():
	argv = sys.argv
	argc = len(argv)

	cl = commandLine()
	cl.progname = split(os.path.basename(argv[0]),'.')[0]

	if argc > 1:
		cl.filelist = argv[1]
	else:
		cl.filelist = "filelist.txt"

	if argc > 2: 
		cl.outputfilename = argv[2] # 2nd (optional) command line argument
	else:
		cl.outputfilename = cl.progname + "_histograms"

	# Make sure extension is ".root"
	if cl.outputfilename[:-5] != ".root": cl.outputfilename += ".root"
	print "==> output to:", cl.outputfilename

	return cl
# -----------------------------------------------------------------------------
def error(message):
	print "** error ** " + message
	sys.exit(0)
# -----------------------------------------------------------------------------
#  Read ntuple filenames from file list

def getFilenames(filelist):
	if not os.path.exists(filelist):
		error("unable to open file: " + filelist)

	# Get ntuple file names
	filenames = filter(lambda x: x != "",
					   map(strip, open(filelist).readlines()))
	v = vector("string")()
	for filename in filenames: v.push_back(filename)
	return v
# -----------------------------------------------------------------------------
TEXTFONT = 42
TEXTSIZE = 0.031
#------------------------------------------------------------------------------
def setStyle():
	gROOT.SetStyle("Pub")
	style = gROOT.GetStyle("Pub")
	style.SetPalette(1)
	
	# For the canvas
	style.SetCanvasBorderMode(0)
	style.SetCanvasColor(kWhite)
	style.SetCanvasDefH(500)
	style.SetCanvasDefW(500)
	style.SetCanvasDefX(0)
	style.SetCanvasDefY(0)

	# For the pad
	style.SetPadBorderMode(0)
	style.SetPadColor(kWhite)
	style.SetPadGridX(kFALSE)
	style.SetPadGridY(kTRUE)
	style.SetGridColor(kGreen)
	style.SetGridStyle(3)
	style.SetGridWidth(1)

	# For the frame
	style.SetFrameBorderMode(0)
	style.SetFrameBorderSize(1)
	style.SetFrameFillColor(0)
	style.SetFrameFillStyle(0)
	style.SetFrameLineColor(1)
	style.SetFrameLineStyle(1)
	style.SetFrameLineWidth(1)

	# For the histogram
	style.SetHistLineColor(1)
	style.SetHistLineStyle(0)
	style.SetHistLineWidth(1)
	style.SetEndErrorSize(2)
	style.SetErrorX(0.)
	style.SetMarkerSize(0.1)
	style.SetMarkerStyle(20)

	# For the fit/function
	style.SetOptFit(1)
	style.SetFitFormat("5.4g")
	style.SetFuncColor(2)
	style.SetFuncStyle(1)
	style.SetFuncWidth(1)

	#For the date
	style.SetOptDate(0)

	# For the statistics box
	style.SetOptFile(0)
	style.SetOptStat("")
	# To display the mean and RMS
	#style.SetOptStat("mr") 
	style.SetStatColor(kWhite)
	style.SetStatFont(TEXTFONT)
	style.SetStatFontSize(TEXTSIZE)
	style.SetStatTextColor(1)
	style.SetStatFormat("6.4g")
	style.SetStatBorderSize(1)
	style.SetStatH(0.2)
	style.SetStatW(0.3)

	# Margins
	style.SetPadTopMargin(0.05)
	style.SetPadBottomMargin(0.16)
	style.SetPadLeftMargin(0.16)
	style.SetPadRightMargin(0.16)

	# For the global title
	style.SetOptTitle(0)
	style.SetTitleFont(TEXTFONT)
	style.SetTitleColor(1)
	style.SetTitleTextColor(1)
	style.SetTitleFillColor(10)
	style.SetTitleFontSize(TEXTSIZE*1.1)

	# For the axis titles
	style.SetTitleColor(1, "XYZ")
	style.SetTitleFont(TEXTFONT, "XYZ")
	style.SetTitleSize(TEXTSIZE*1.2, "XYZ") # 0,05
	style.SetTitleXOffset(1.25) # 0.9
	style.SetTitleYOffset(1.25) # 1.25

	# For the axis labels
	style.SetLabelColor(1, "XYZ")
	style.SetLabelFont(TEXTFONT, "XYZ")
	style.SetLabelOffset(0.006, "XYZ")
	style.SetLabelSize(TEXTSIZE*1.2, "XYZ")

	# For the axis
	style.SetAxisColor(1, "XYZ")
	style.SetStripDecimals(kTRUE)
	style.SetTickLength(0.03, "XYZ")
	style.SetNdivisions(505, "XYZ")

	# To get tick marks on the opposite side of the frame
	style.SetPadTickX(1)  
	style.SetPadTickY(1)

	# Change for log plots
	style.SetOptLogx(0)
	style.SetOptLogy(0)
	style.SetOptLogz(0)

	# Postscript options
	style.SetPaperSize(20.,20.)

	style.cd()
# -----------------------------------------------------------------------------
#  Define variables to be read
# -----------------------------------------------------------------------------
cmdline = decodeCommandLine()

#  Get names of ntuple files to be processed and open chain of ntuples

filenames = getFilenames(cmdline.filelist)
stream = itreestream(filenames, "%(treename)s")
if not stream.good(): error("unable to open ntuple file(s)")

%(selection)s
'''

PYTEMPLATE =\
'''#!/usr/bin/env python
# -----------------------------------------------------------------------------
#  File:        %(name)s.py
#  Description: Analyzer for ntuples created by TheNtupleMaker
#  Created:     %(time)s by mkntanalyzer.py
#  Author:      %(author)s
# -----------------------------------------------------------------------------
from ROOT import *
from string import *
from %(name)slib import *
import os, sys, re
# -----------------------------------------------------------------------------
# -- Procedures and functions
# -----------------------------------------------------------------------------


# -----------------------------------------------------------------------------
def main():

	# Get number of events
	nevents = stream.size()
	print "Number of events:", nevents


	# Notes:
	#
	# 1. Use
	#   ofile = outputFile(cmdline.outputfile, stream)
	#
	# to skim events to output file in addition to writing out histograms.
	#
	# 2. Use
	#   ofile.addEvent(event-weight)
	#
	# to specify that the current event is to be added to the output file. If
	# omitted, the event-weight is taken to be 1.
	#
	# 3. Use
	#    ofile.count(cut-name, event-weight)
	#
	# to keep track, in the count histogram, of the number of events passing
	# a given cut. If omitted, the event-weight is taken to be 1. If you want
	# the counts in the count histogram to appear in a given order, specify
	# the order, before entering the event loop, as in the example below
	# 
	#   ofile.count("NoCuts", 0)
	#	ofile.count("GoodEvent", 0)
	#	ofile.count("Vertex", 0)
	#	ofile.count("MET", 0)
	
	ofile = outputFile(cmdline.outputfilename)

	# -------------------------------------------------------------------------
	# Define histograms
	# -------------------------------------------------------------------------
	setStyle()



	# -------------------------------------------------------------------------
	# Loop over events
	# -------------------------------------------------------------------------
	for entry in xrange(nevents):
		stream.read(entry)

		# -- Event selection

	stream.close()
	ofile.close()
# -----------------------------------------------------------------------------
main()
'''

MAKEFILE = '''#------------------------------------------------------------------------------
# Description: Makefile to build analyzers
# Created:     %(time)s by mkanalyzer.py
# Author:      %(author)s
#------------------------------------------------------------------------------
ifndef ROOTSYS
$(error *** Please set up Root)
endif

# Sub-directories
srcdir	:= src
tmpdir	:= tmp
libdir	:= $(PWD)/lib
incdir	:= include

$(shell mkdir -p tmp)
$(shell mkdir -p lib)


# Set this equal to the @ symbol to suppress display of instructions
# while make executes
ifdef verbose
AT 	:=
else
AT	:= @
endif

# Get list of sources to be compiled into applications
appsrcs	:= $(wildcard *.cc)

# Construct list of applications
applications := $(appsrcs:.cc=)

# Construct names of object models from list of sources
appobjs	:= $(addprefix $(tmpdir)/,$(appsrcs:.cc=.o))

# Get list of sources to be made into shared libraries
cppsrcs	:= $(wildcard $(srcdir)/*.cpp)
cppobjs	:= $(subst $(srcdir)/,$(tmpdir)/,$(cppsrcs:.cpp=.o))

ccsrcs	:= $(wildcard $(srcdir)/*.cc) 
ccobjs	:= $(subst $(srcdir)/,$(tmpdir)/,$(ccsrcs:.cc=.o))
objects	:= $(cppobjs) $(ccobjs)

sharedlib := $(libdir)/lib%(name)s.so

# Display list of applications to be built
#say	:= $(shell echo -e "Apps: $(applications)" >& 2)
#say	:= $(shell echo -e "AppObjs: $(appobjs)" >& 2)
#say	:= $(shell echo -e "Objects: $(objects)" >& 2)
#$(error bye!) 

#-----------------------------------------------------------------------

# 	Define which compilers and linkers to use

# 	C++ Compiler
CXX	:= g++


# 	Define paths to be searched for C++ header files (#include ....)

CPPFLAGS:= -I. -I$(incdir) -I$(srcdir) $(shell root-config --cflags) 

# 	Define compiler flags to be used
#	-c		perform compilation step only 
#	-g		include debug information in the executable file
#	-O2		optimize
#	-ansi	require strict adherance to C++ standard
#	-Wall	warn if source uses any non-standard C++
#	-pipe	communicate via different stages of compilation
#			using pipes rather than temporary files

CXXFLAGS:= -c -g -O2 -ansi -Wall -pipe -fPIC

#	C++ Linker

LD	:= g++

OS	:= $(shell uname -s)
ifeq ($(OS),Darwin)
	LDSHARED	:= $(LD) -dynamiclib
else
	LDSHARED	:= $(LD) -shared
endif

#	Linker flags

LDFLAGS := -g

# 	Libraries

LIBS	:=  \
$(shell root-config --libs) \
-L$(libdir) -lanalyzer -lMinuit  -lMathMore -lMathCore


#	Rules
#	The structure of a rule is
#	target : source
#		command
#	The command makes a target from the source. 
#	$@ refers to the target
#	$< refers to the source

all:	$(sharedlib) $(applications) 

bin:	$(applications)

lib:	$(sharedlib)

# Syntax:
# list of targets : target pattern : source pattern


# Make applications depend on shared libraries to force the latter
# to be built first

$(applications)	: %(percent)s	: $(tmpdir)/%(percent)s.o  $(sharedlib)
	@echo "---> Linking $@"
	$(AT)$(LD) $(LDFLAGS) $< $(LIBS) -o $@

$(sharedlib)	: $(objects)
	@echo "---> Linking `basename $@`"
	$(AT)$(LDSHARED) $(LDFLAGS) -fPIC $(objects) -o $@

$(cppobjs)	: $(tmpdir)/%(percent)s.o	: $(srcdir)/%(percent)s.cpp
	@echo "---> Compiling `basename $<`" 
	$(AT)$(CXX) $(CXXFLAGS) $(CPPFLAGS)  $< -o $@ >& $*.FAILED
	$(AT)rm -rf $*.FAILED

$(ccobjs)	: $(tmpdir)/%(percent)s.o	: $(srcdir)/%(percent)s.cc
	@echo "---> Compiling `basename $<`" 
	$(AT)$(CXX) $(CXXFLAGS) $(CPPFLAGS)  $< -o $@ >& $*.FAILED
	$(AT)rm -rf $*.FAILED

$(appobjs)	: $(tmpdir)/%(percent)s.o	: %(percent)s.cc
	@echo "---> Compiling `basename $<`" 
	$(AT)$(CXX) $(CXXFLAGS) $(CPPFLAGS)  $< -o $@ >& $*.FAILED
	$(AT)rm -rf $*.FAILED
# 	Define clean up rules
clean   :
	rm -rf $(tmpdir)/*.o

veryclean   :
	rm -rf $(tmpdir)/*.o $(applications) $(libdir)/*.so
'''



README = '''Created: %(time)s

    o To build the default program (%(name)s) do

	  make

    o To build another program in this directory do

	  make program=<program-name>

    example:

	  make program=findSUSYalready


    o To run the program, first create a text file (default name=filelist.txt)
	containing a list of the ntuples to be analyzed, one filename per line.
	Then do

	  ./%(name)s

	If you wish to specify a different file list, say datafile.list, do

	  ./%(name)s datafile.list

	If you wish to change the name of the root output file, say
	datahist.root, do

	   ./%(name)s datafile.list datahist.root


For details, please refer to the documentation at:

	https://twiki.cern.ch/twiki/bin/viewauth/CMS/TheNtupleMaker
	
'''
#------------------------------------------------------------------------------
def cmp(x, y):
	if len(y) < len(x):
		return -1
	elif len(y) == len(x):
		return 0
	else:
		return 1
#------------------------------------------------------------------------------
def main():
	print "\n\tmkanalyzer.py"

	# Decode command line

	argv = sys.argv[1:]
	argc = len(argv)
	if argc < 1: usage()

	filename = nameonly(argv[0])

	if argc > 1:
		varfilename = argv[1]
	else:
		varfilename = "variables.txt"
	if not os.path.exists(varfilename):
		print "mkanalyzer.py - can't find variable file: %s" % varfilename
		sys.exit(0)


	# Check for macro mode
	macroMode = argc > 2
	

	# Read variable names

	records = map(strip, open(varfilename, "r").readlines())

	# Get tree name(s)

	t = split(records[0])
	if lower(t[0]) == "tree:":
		treename = t[1]
	else:
		treename = "Events"
	start = 1
	for record in records[1:]:
		record = strip(record)
		if record == "": break
		t = split(record)
		if lower(t[0]) == "tree:":
			treename += " %s" % t[1]
			start += 1

   	# Done with header, so loop over branch names
	# and get list of potential struct names (first field of
	# varname.
	
	records = records[start:]
	
	stnamemap = {}
	for index in xrange(len(records)):
		record = records[index]
		if record == "": continue

		# split record into its fields
		# varname = variable name as determined by mkvariables.py
		
		rtype, branchname, varname, count = split(record, '/')
		t = split(varname,'_')
		if len(t) > 1: # Need at least two fields
			key = t[0]
			if not stnamemap.has_key(key): stnamemap[key] = 0
			stnamemap[key] += 1
	
	# Loop over branch names
	
	# If a variable name matches a struct name, this will generate a
	# multiply defined error. One of the names must be altered. Let's
	# take this to be the variable name.

	usednames = {}
	vars = {}
	vectormap = {}
	
	for index in xrange(len(records)):
		record = records[index]
		if record == "": continue
		
		rtype, branchname, varname, count = split(record, '/')

		# Fix annoying types
		if rtype == "bool":
			rtype = "int"
		elif rtype == "long64":
			rtype = "long"
		elif rtype == "int32":
			rtype = "int"
		elif rtype == "uchar":
			rtype = "int"
		elif rtype == "uint":
			rtype = "int"			

		# Check for leaf counter flag (a "*")

		t = split(count)
		count = atoi(t[0]) # Change type to an integer
		iscounter = t[-1] == "*"
		
		# Check that varname is not the same as that of a potential
		# struct
		if stnamemap.has_key(varname):
			if not iscounter:
				# This is not a leaf counter, so alert user
				print "\t**warning: multiply defined name, %s; changing " % \
					  varname
				print "\t           varname to %s1" % varname
			varname = "%s1" % varname
			
		# Get object and field names
		objname = ''
		fldname = ''
		t = split(varname,'_')
		if len(t) > 0:

			# we have at least two fields in varname
			
			key = t[0]
			if stnamemap.has_key(key):

				# This branch potentially belongs to a struct.

				objname = key
				# Make sure the count for this branch matches that
				# of existing struct
				if not usednames.has_key(objname):
					usednames[objname] = count;
									
				if usednames[objname] == count:
					fldname = replace(varname, '%s_' % objname, '')
				else:
					objname = ''
					fldname = ''

		# Take care of duplicate names
		n = 1
		if vars.has_key(varname):
			# duplicate name; add a number to object name
			n, a, b, c, d = vars[varname]
			n += 1
			vars[varname][0] = n;

			if fldname != '':
				objname = "%s%d"  % (objname, n)
				varname = "%s_%s" % (objname, fldname)
			else:
				varname = "%s%d" % (varname, n)

		# update map for all variables
		vars[varname] = [1, rtype, branchname, count, iscounter]

		# vector types must have the same object name and a max count > 1
		if count > 1:
			if fldname != "":
				
				# Make sure fldname is valid			
				if fldname[0] in ['0','1','2','3','4','5','6','7','8','9']:
					fldname = 'f%s' % fldname
		
				if not vectormap.has_key(objname): vectormap[objname] = []	
				vectormap[objname].append((rtype, fldname, varname, count))
				#print "%s.%s (%s)" % (objname, fldname, count)
				
	# Declare all variables
	
	keys = vars.keys()
	keys.sort()
	pydeclare = []
	pyselect  = []
	declare = []
	select  = []
	impl = []
	for index, varname in enumerate(keys):
		n, rtype, branchname, count, iscounter = vars[varname]

		# If this is a counter variable with a name identical to that of a
		# vector variable, ignore it
		if iscounter and vectormap.has_key(varname): continue

		if macroMode:

			if iscounter:
				impl.append('  countvalue& v%d = (*varmap)["%s"];'%\
							(index, branchname))
				impl.append('  if ( v%d.count )' % index)
				impl.append('    %s = *v%d.count;' % (varname, index))
				impl.append('  else')
				impl.append('    %s = 0;' % varname)
				impl.append('')			

			elif count == 1:
				impl.append('  countvalue& v%d = (*varmap)["%s"];' %\
							(index, branchname))
							 
				impl.append('  if ( v%d.value )' % index)
				impl.append('    %s = *v%d.value;' % (varname, index))
				impl.append('  else')
				impl.append('    %s = 0;' % varname)
				impl.append('')
			else:
				# this is a vector
				impl.append('  countvalue& v%d = (*varmap)["%s"];' % \
							(index, branchname))
							
				impl.append('  if ( v%d.value )' % index)
				impl.append('    {')
				impl.append('      %s.resize(*v%d.count);' % (varname, index))
				impl.append('      copy(v%d.value, v%d.value+*v%d.count, '\
							'%s.begin());'% (index, index, index, varname))
				impl.append('    }')
				impl.append('  else')
				impl.append('    %s.clear();' % varname)
				impl.append('')


		if count == 1:
			declare.append("%s\t%s;" % (rtype, varname))
			if rtype in ["float", "double"]:
				pydeclare.append("%s\t= Double()" % varname)
			else:
				pydeclare.append("%s\t= Long()" % varname)
		else:
			# this is a vector
			declare.append("std::vector<%s>\t%s(%d,0);" % \
						   (rtype, varname, count))
			pydeclare.append('%s\t= vector("%s")(%d)' % \
							 (varname, rtype, count))
		select.append('stream.select("%s", %s);' % (branchname, varname))
		pyselect.append('stream.select("%s", %s)'  % (branchname, varname))



	# Create structs for vector variables
	
	keys = vectormap.keys()
	keys.sort()	
	structdecl = []
	structimpl = []

	selectdecl = []
	selectimpl = []
	
	selectimpl.append('void saveSelectedObjects()')
	selectimpl.append('{')
	selectimpl.append('  if ( ! fillObjectsCalled ) return;')
	selectimpl.append('  int n = 0;')

	structimpl.append('static bool fillObjectsCalled = false;')
	structimpl.append('void fillObjects()')
	structimpl.append('{')
	structimpl.append('  fillObjectsCalled = true;')
	for index, objname in enumerate(keys):
		values = vectormap[objname]
		varname= values[0][-2];
		
		structimpl.append('')
		structimpl.append('  %s.resize(%s.size());' % (objname, varname))
		structimpl.append('  for(unsigned int i=0; i < %s.size(); ++i)' % \
						  objname)
		structimpl.append('    {')
		structimpl.append('      %s[i].selected\t= false;' % objname)

		selectimpl.append('')
		selectimpl.append('  n = 0;')
		selectimpl.append('  for(unsigned int i=0; i < %s.size(); ++i)' % \
						  objname)
		selectimpl.append('    {')
		selectimpl.append('      if ( ! %s[i].selected ) continue;' % \
						  objname)

		structdecl.append('struct %s_s' % objname)
		structdecl.append('{')

		structdecl.append('  %s\t%s;' % ("bool", "selected"))
		
		for rtype, fldname, varname, count in values:
			# treat bools as ints
			if rtype == "bool":
				cast = '(bool)'
			else:
				cast = ''

						# this is a vector
	
			structdecl.append('  %s\t%s;' % (rtype, fldname))

			structimpl.append('      %s[i].%s\t= %s%s[i];' % (objname,
															  fldname,
															  cast,
															  varname))

			selectimpl.append('      %s[n]\t= %s[i].%s;' % (varname,
															objname,
															fldname))
			
		structdecl.append('};')
		structdecl.append('std::vector<%s_s> %s(%d);' % (objname,
														 objname,
														 count))
		structdecl.append('')
		structdecl.append('std::ostream& '\
						  'operator<<(std::ostream& os, const %s_s& o)' % \
						  objname)
		structdecl.append('{')
		structdecl.append('  char r[1024];')
		structdecl.append('  os << "%s" << std::endl;' % objname)
		
		for rtype, fldname, varname, count in values:
			structdecl.append('  sprintf(r, "  %s: %s\\n", "%s",'\
							  ' (double)o.%s); '\
							  'os << r;' % ("%-32s", "%f", fldname, fldname))
		structdecl.append('  return os;')
		structdecl.append('}')
		structdecl.append('//-----------------------------------------'
						  '------------------------------------')

		structimpl.append('    }')
		selectimpl.append('      n++;')
		selectimpl.append('    }')
		#selectimpl.append('  n%s = n;' % objname) # HBP June 5 2012
		
	structimpl.append('}')  # end of fillObjects()
	selectimpl.append('  fillObjectsCalled = false;')
	selectimpl.append('}')  # end of saveObjects()
	
	# Write out files

	if macroMode:
		
		names = {'NAME': upper(filename),
				 'name': filename,
				 'time': ctime(),
				 'author': AUTHOR,
				 'vardecl': join("", declare, "\n"),
				 'selection': join("  ", select, "\n"),
				 'structdecl': join("", structdecl, "\n"),
				 'structimpl': join("", structimpl, "\n"),
				 'selectimpl': join("", selectimpl, "\n"),
				 'impl': join("", impl, "\n"),
				 'treename': treename,
				 'percent': '%' }

		record = MACRO_DECL_H % names
		outfilename = "%s_decl.h" % filename
		open(outfilename, "w").write(record)

		record = MACRO_IMPL_H % names
		outfilename = "%s_impl.h" % filename
		open(outfilename, "w").write(record)
		sys.exit(0)

	# Put everything into a directory

	cmd = '''
	mkdir -p %(dir)s/tmp
	mkdir -p %(dir)s/lib
	mkdir -p %(dir)s/src
	mkdir -p %(dir)s/include
	cp %(hpp)s %(dir)s/include
	cp %(cpp)s %(dir)s/src
	''' % {'dir': filename,
		   'hpp': TREESTREAM_HPP,
		   'cpp': TREESTREAM_CPP}
	os.system(cmd)

	cmd = '''
	cp %(hpp)s %(dir)s/include
	cp %(cpp)s %(dir)s/src
	''' % {'dir': filename,
		   'hpp': PDG_HPP,
		   'cpp': PDG_CPP}
	
	os.system(cmd)

	# Create Makefile

	names = {'name': filename,
			 'filename': filename,
			 'time': ctime(),
			 'author': AUTHOR,
			 'percent': '%'
			 }

	record = MAKEFILE % names
	open("%s/Makefile" % filename, "w").write(record)	

	# Create C++ code

	outfilename = "%s/%s.h" % (filename, filename)

	names = {'NAME': upper(filename),
			 'name': filename,
			 'time': ctime(),
			 'author': AUTHOR,
			 'vardecl': join("", declare, "\n"),
			 'selection': join("  ", select, "\n"),
			 'structdecl': join("", structdecl, "\n"),
			 'structimpl': join("", structimpl, "\n"),
			 'selectimpl': join("", selectimpl, "\n"),
			 'treename': treename,
			 'percent': '%' }

	record = TEMPLATE_H % names
	open(outfilename,"w").write(record)

	###
	#print "OUT( %s )" % outfilename
	
	# Create cc file if one does not yet exist
	
	outfilename = "%s/%s.cc" % (filename, filename)
	if not os.path.exists(outfilename):
		record = TEMPLATE_CC % names
		open(outfilename,"w").write(record)

	# Create README

	outfilename = "%s/README" % filename
	record = README % names
	open(outfilename,"w").write(record)

	# Create python code

	outfilename = "%s/%slib.py" % (filename, filename)

	s = join("", pydeclare, "\n") + "\n" + join("", pyselect,"\n")

	names = {'name': filename,
			 'treename': treename,
			 'percent': '%',
			 'time': ctime(),
			 'selection': s,
			 'author': AUTHOR,
			 'htab': "%s%s\\t%s\\t%s"
			 }
	record = PYTEMPLATELIB % names
	open(outfilename,"w").write(record)

	# Create Python script if one does not yet exist
	
	outfilename = "%s/%s.py" % (filename, filename)
	if not os.path.exists(outfilename):
		record = PYTEMPLATE % names
		open(outfilename,"w").write(record)
		os.system("chmod +x %s" % outfilename)

	print "\tdone!"
#------------------------------------------------------------------------------
main()

