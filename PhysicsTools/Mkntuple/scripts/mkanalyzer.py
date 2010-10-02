#!/usr/bin/env python
#------------------------------------------------------------------------------
# Description: Create ntuple analyzer template
# Created: 06-Mar-2010 Harrison B. Prosper
# Updated: 12-Mar-2010 HBP - fix appending of .root
#          08-Jun-2010 HBP - add creation of selector.h
#          02-Sep-2010 HBP - fix variables.txt record splitting bug
#          01-Oct-2010 HBP - add structs
#          02-Oct-2010 HBP - add cloning
#$Revision: 1.12 $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time import *
from glob import glob
from PhysicsTools.Mkntuple.Lib import nameonly, getauthor
#------------------------------------------------------------------------------
if not os.environ.has_key("CMSSW_BASE"):
	print "\t**you must first set up CMSSW"
	sys.exit(0)

PACKAGE = "%s/src/PhysicsTools/Mkntuple" % os.environ["CMSSW_BASE"]
TREESTREAM_HPP = "%s/interface/treestream.h" % PACKAGE
TREESTREAM_CPP = "%s/src/treestream.cc" % PACKAGE
PDG_HPP = "%s/interface/pdg.h" % PACKAGE
PDG_CPP = "%s/src/pdg.cc" % PACKAGE
MKNTUPLE= "%s/src/PhysicsTools/Mkntuple" % os.environ["CMSSW_BASE"]
SELECTEDOBJECTMAP_H = "%s/interface/SelectedObjectMap.h" % MKNTUPLE
AUTHOR = getauthor()


# Make sure that we can find treestream etc.

if not os.path.exists(TREESTREAM_HPP):
	print "\n** error ** - file:\n\t%s\n\t\tNOT found!" % TREESTREAM_HPP
	sys.exit(0)

if not os.path.exists(TREESTREAM_CPP):
	print "\n** error ** - file:\n\t%s\n\t\tNOT found!" % TREESTREAM_CPP
	sys.exit(0)

if not os.path.exists(SELECTEDOBJECTMAP_H):
	print "\n** error ** - file:\n\t%s\n\t\tNOT found!" % SELECTEDOBJECTMAP_H
	sys.exit(0)

# Functions

getvno = re.compile(r'[0-9]+$')

def usage():
	print '''
	Usage:
	   mkanalyzer.py <output-filename> [var-filename=variables.txt]
	'''
	sys.exit(0)

def join(left, a, right):
	s = ""
	for x in a:
		s = s + "%s%s%s" % (left, x, right)
	return s

TEMPLATE_H =\
'''#ifndef %(NAME)s_H
#define %(NAME)s_H
//-----------------------------------------------------------------------------
// File:        %(name)s.h
// Description: Analyzer header for ntuples created by Mkntuple
// Created:     %(time)s by mkntanalyzer.py
// Author:      %(author)s
// $Revision: 1.12 $
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

#ifdef PROJECT_NAME

// --- CMSSW

#include "PhysicsTools/Mkntuple/interface/treestream.h"
#include "PhysicsTools/Mkntuple/interface/pdg.h"

#else

#include "treestream.h"
#include "pdg.h"

#endif

// -- Root

#include "TApplication.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TKey.h"
#include "TH1F.h"
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
struct skimFile
{
  skimFile(itreestream& stream, std::string filename)
   : filename_(filename),
	 file_(new TFile(filename.c_str(), "recreate")),
	 tree_(stream.tree()->CloneTree(0))
  {
	std::cout << "events will be skimmed to file "
			  << filename_ << std::endl;
  }

  void keep()
  {
	file_ = tree_->GetCurrentFile();
	file_->cd();
	tree_->Fill();
  }

  void close()
  {
	std::cout << "==> events skimmed to file " << filename_ << std::endl;
	file_ = tree_->GetCurrentFile();
	file_->Write("", TObject::kOverwrite);
	file_->Close();
  }

  std::string filename_;  
  TFile* file_;
  TTree* tree_;
};

struct commandLine
{
  std::string progname;
  std::string filelist;
  std::string histfilename;
  std::string skimfilename;
};

struct histogramFile
{
  histogramFile(std::string histfilename)
   : filename_(histfilename),
	 file_(new TFile(filename_.c_str(), "recreate")) 
  {}

  void close()
  {
	std::cout << "==> histograms saved to file " << filename_ << std::endl;
	file_->cd();
	file_->Write("", TObject::kOverwrite);
	file_->ls();
	file_->Close();
  }

  std::string filename_;
  TFile* file_;
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
	cl.histfilename = std::string(argv[2]);
  else
	cl.histfilename = cl.progname + std::string("_histograms");

  // 3rd (optional) command line argument
  if ( argc > 3 ) 
	cl.skimfilename = std::string(argv[3]);
  else
	cl.skimfilename = cl.progname + std::string("_skim");

  // Make sure extension is ".root"
  cl.histfilename = nameonly(cl.histfilename);
  cl.histfilename += ".root";

  cl.skimfilename = nameonly(cl.skimfilename);
  cl.skimfilename += ".root";
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
//-----------------------------------------------------------------------------
// -- Declare variables to be read
//-----------------------------------------------------------------------------
%(vardecl)s

//-----------------------------------------------------------------------------
// -- Select variables to be read
//-----------------------------------------------------------------------------
void selectVariables(itreestream& stream)
{
%(selection)s
}

//-----------------------------------------------------------------------------
// --- These structs can be filled by calling update() after stream.read(...)
//-----------------------------------------------------------------------------
%(structdecl)s
%(structimpl)s

#endif
'''

TEMPLATE_CC =\
'''//-----------------------------------------------------------------------------
// File:        %(name)s.cc
// Description: Analyzer for ntuples created by Mkntuple
// Created:     %(time)s by mkntanalyzer.py
// Author:      %(author)s
// $Revision: 1.12 $
//-----------------------------------------------------------------------------
#include "%(name)s.h"
#ifdef PROJECT_NAME
#include "PhysicsTools/Mkntuple/interface/pdg.h"
#else
#include "pdg.h"
#endif
using namespace std;
//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  // Get file list and histogram filename from command line

  commandLine cmdline;
  decodeCommandLine(argc, argv, cmdline);

  // Get names of ntuple files to be processed and open chain of ntuples

  vector<string> filenames = getFilenames(cmdline.filelist);
  itreestream stream(filenames, "Events");
  if ( !stream.good() ) error("unable to open ntuple file(s)");

  // Get number of events to be read

  int nevents = stream.size();
  cout << "Number of events: " << nevents << endl;

  // Select variables to be read

  selectVariables(stream);

  //---------------------------------------------------------------------------
  // Book histograms etc.
  //---------------------------------------------------------------------------
  // The root application is needed to make canvases visible during
  // program execution. If this is not needed, just comment out the following
  // line

  TApplication app("analyzer", &argc, argv);

  histogramFile hfile(cmdline.histfilename);

  // Histograms


  //---------------------------------------------------------------------------
  // Loop over events
  //---------------------------------------------------------------------------

  // Call this to open a skim file
  //skimFile skim(stream, cmdline.skimfilename);

  for(int entry=0; entry < nevents; ++entry)
	{
	  stream.read(entry);

	  // Call this to update structs
	  // update();

	  // ---------------------
	  // -- Event Selection --
	  // ---------------------

	  // if ( !SUSY ) continue;

	  // Call this to send current event to skim file
	  //skim.keep();
	}

  // Close all files, including skim file if one was created above
  stream.close();
  hfile.close();
  //skim.close();

  return 0;
}
'''

SLTEMPLATE=\
'''#ifndef SELECTOR_H
#define SELECTOR_H
//-----------------------------------------------------------------------------
// File:        selector.h
// Description: selector template
// Created:     %(time)s by mkntanalyzer.py
// Author:      %(author)s
// $Revision: 1.12 $
//-----------------------------------------------------------------------------
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include "TROOT.h"
#include "TTree.h"
#include "TLeaf.h"
//-----------------------------------------------------------------------------

// Classes

%(class)s

// Functions

inline
void keep(std::string name, int index)
{
  SelectedObjectMap::instance().set(name, index);
}

int leafcount(TLeaf* leaf)
{
  int count = 0;
  TLeaf* leafcounter = leaf->GetLeafCounter(count);
  if ( leafcounter != 0 )
	// Variable length array
	count = leafcounter->GetMaximum();
  else
	// Either fixed length array or a simple variable
	count = leaf->GetLen();
  return count;
}

// Variables

%(vardecl)s

void initSelector()
{
  TTree* tree = (TTree*)gROOT->FindObject("Events");
  if ( ! tree )
  {
	std::cout << "** error ** selector, tree pointer is zero" << std::endl;
	exit(0);
  }

  // Fill variables

%(varimpl)s
}

#endif

'''

PYTEMPLATELIB =\
'''# -----------------------------------------------------------------------------
#  File:        %(name)slib.py
#  Description: Analyzer for ntuples created by Mkntuple
#  Created:     %(time)s by mkntanalyzer.py
#  Author:      %(author)s
#  $Revision: 1.12 $
# -----------------------------------------------------------------------------
from ROOT import *
from time import sleep
from string import *
from PhysicsTools.Mkntuple.AutoLoader import *
import os, sys, re
# -----------------------------------------------------------------------------
# -- Classes, procedures and functions
# -----------------------------------------------------------------------------
class skimFile:
	def __init__(self, stream, filename):
		print "events will be skimmed to file", filename
		self.filename = filename
		self.file = TFile(filename, "recreate")
		self.tree = stream.tree().CloneTree(0)

	def keep(self):
		self.file = self.tree.GetCurrentFile()
		self.file.cd()
		self.tree.Fill()

	def close(self):
		print "==> events skimmed to file", self.filename
		self.file = self.tree.GetCurrentFile()
		self.file.Write("", TObject.kOverwrite)
		self.file.Close()
# -----------------------------------------------------------------------------
class histogramFile:
	def __init__(self, histfilename):
		self.filename = histfilename
		self.file = TFile(self.filename, "recreate")

	def close(self):
		print "==> histograms saved to file", self.filename
		self.file.cd()
		self.file.Write("", TObject.kOverwrite)
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
		cl.histfilename = argv[2] # 2nd (optional) command line argument
	else:
		cl.histfilename = cl.progname + "_histograms"

	if argc > 3:
		cl.skimfilename = argv[3]
	else:
		cl.skimfilename = cl.progname + "_skim"

	# Make sure extension is ".root"
	cl.histfilename = os.path.basename(cl.histfilename)
	cl.histfilename = split(cl.histfilename, ".")[0] + ".root"

	cl.skimfilename = os.path.basename(cl.skimfilename)
	cl.skimfilename = split(cl.skimfilename, ".")[0] + ".root"
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
stream = itreestream(filenames, "Events")
if not stream.good(): error("unable to open ntuple file(s)")

%(selection)s
'''

PYTEMPLATE =\
'''#!/usr/bin/env python
# -----------------------------------------------------------------------------
#  File:        %(name)s.py
#  Description: Analyzer for ntuples created by Mkntuple
#  Created:     %(time)s by mkntanalyzer.py
#  Author:      %(author)s
#  $Revision: 1.12 $
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

	#  Get number of events
	nevents = stream.size()
	print "Number of events:", nevents

	# Call this if you want to skim events
	#skim = skimFile(stream, skimfilename)

	# -------------------------------------------------------------------------
	#  Book histograms etc.
	# -------------------------------------------------------------------------
	setStyle()

	hfile = histogramFile(cmdline.histfilename)

	# -------------------------------------------------------------------------
	#  Loop over events
	# -------------------------------------------------------------------------
	for entry in xrange(nevents):
		stream.read(entry)

		# -- Event selection
		#if not SUSY: continue
		#skim.keep() # keep current event

	stream.close()
	hfile.close()
	#skim.close() #  close skim file
# -----------------------------------------------------------------------------
main()
'''

MAKEFILE = '''#------------------------------------------------------------------------------
# Description: Makefile to build executable %(filename)s
# Created:     %(time)s by mkanalyzer.py
#
#               available switches:
#
#                 debugflag  (e.g., debugflag=-ggdb [default])
#                 cppflags
#                 cxxflags
#                 optflag
#                 verbose    (e.g., verbose=1)
#                 withcern   (e.g., withcern=1  expects to find CERN_LIB)
# Author:      %(author)s
#$Revision: 1.12 $
#------------------------------------------------------------------------------
ifndef ROOTSYS
$(error *** Please set up Root)
endif
withroot:=1
#------------------------------------------------------------------------------
program := %(filename)s
cppsrcs	:= $(wildcard *.cpp)
ccsrcs  := $(wildcard *.cc)
cppobjs	:= $(patsubst %(percent)s.cpp,tmp/%(percent)s.o,$(cppsrcs))
ccobjs	:= $(patsubst %(percent)s.cc,tmp/%(percent)s.o,$(ccsrcs))
objects	:= $(ccobjs) $(cppobjs)
say     := $(shell echo "Program: $(program)" >& 2)
#------------------------------------------------------------------------------
ifdef GCC_DIR
GCC_BIN_PREFIX	:= $(GCC_DIR)/bin/
else
GCC_BIN_PREFIX	:=
endif
C++	    := $(GCC_BIN_PREFIX)g++
LDSHARED:= $(GCC_BIN_PREFIX)g++
C++VER	:= $(shell $(C++) --version)
COMP	:= $(word 1, $(C++VER))
CTYPE	:= $(word 2, $(C++VER))
CVER	:= $(word 3, $(C++VER))
say 	:= $(shell echo "$(COMP) $(CTYPE) $(CVER)" >& 2)
#------------------------------------------------------------------------------
ifdef verbose
	AT =
else
	AT = @
endif
#------------------------------------------------------------------------------
# Products to compile/link against
#------------------------------------------------------------------------------
ifdef withcern
	ifndef CERN_LIB
		ifdef CERN_DIR
			CERN_LIB:= $(CERN_DIR)/lib
		else
			say:=$(error CERN_LIB must point to CERN lib directory)
		endif
	endif
	cernlib	:= -L$(CERN_LIB) -lpacklib -lmathlib -lkernlib
endif

ifdef withroot
	rootcpp	:= $(shell root-config --cflags)
	rootlib	:= $(shell root-config --glibs) -lTreePlayer
endif
#------------------------------------------------------------------------------
# Switches/includes
# debug flag is on by default
#------------------------------------------------------------------------------
debugflag:=-ggdb

ifndef optflag
	optflag:=-O2
endif

CPPFLAGS:= -I. $(rootcpp) $(cppflags)
CXXFLAGS:= -c -pipe $(optflag) -fPIC -Wall $(cxxflags) $(debugflag)
LDFLAGS	:= $(ldflags) $(debugflag)
LIBS	:= $(libs) $(rootlib) $(cernlib)
#------------------------------------------------------------------------------
# Rules
#------------------------------------------------------------------------------
bin:	$(program)

$(program)	: $(objects)
	@echo "---> Linking $@"
	$(AT)$(LDSHARED) $(LDFLAGS) $(objects) $(LIBS) -o $@
	@echo ""

$(cppobjs)	: tmp/%(percent)s.o : %(percent)s.cpp
	@echo "---> Compiling $<" 
	$(AT)$(CXX)	$(CXXFLAGS) $(CPPFLAGS) $< -o $@

$(ccobjs)	: tmp/%(percent)s.o : %(percent)s.cc
	@echo "---> Compiling $<" 

	$(AT)$(CXX)	$(CXXFLAGS) $(CPPFLAGS) $< -o $@

# Define clean up rule
clean   	:
	rm -rf $(objects) $(program)
'''


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
		print "mkntanalyzer.py - can't find variable file: %s" % varfilename
		sys.exit(0)

	# Read variable names

	records = map(strip, open(varfilename, "r").readlines())

	vars = {}
	objmap = {}
	# skip first line
	records = records[1:]
	for index in xrange(len(records)):
		record = records[index]
		if record == "": continue
		rtype, branchname, varname, count = split(record, '/')

		# Get object and "method" names
		t = split(varname,'_')
		objname = t[0]
		metname = joinfields(t[1:],'_')

		count = atoi(count)
		n = 1
		# Take care of duplicate names
		if vars.has_key(varname):
			# duplicate name; add a number to object name
			n, r, b, c = vars[varname]
			n += 1
			vars[varname][0] = n;

			objname = "%s%d" % (objname, n)
			if metname != '':
				varname = "%s_%s" % (objname, metname)
			else:
				varname = objname

		# update maps
		vars[varname] = [1, rtype, branchname, count]
		if not objmap.has_key(objname): objmap[objname] = []
		objmap[objname].append((rtype, metname, count))

	# Construct declarations etc.

	# Create structs
	keys = objmap.keys()
	keys.sort()	
	structdecl = []

	structimpl = []

	structimpl.append('void update()')
	structimpl.append('{')

	for objname in keys:
		values = objmap[objname]
		count = values[0][-1];
		if count == 1: continue # ignore single variables

		structimpl.append('')
		structimpl.append('  %s.resize(n%s);' % (objname, objname))
		structimpl.append('  for(int i=0; i < n%s; ++i)' % objname)
		structimpl.append('    {')

		structdecl.append('struct s_%s' % objname)
		structdecl.append('{')
		for rtype, metname, c in values:
			# treat bools as ints
			if rtype == "bool":
				cast = '(bool)'
			else:
				cast = ''
			structdecl.append('  %s\t%s;' % (rtype, metname))

			structimpl.append('      %s[i].%s = %s%s_%s[i];' % (objname,
																metname,
																cast,
																objname,
																metname))
		structdecl.append('};')
		structdecl.append('')
		structdecl.append('std::vector<s_%s> %s(%d);' % (objname,
														 objname,
														 count))
		structdecl.append('')
		structdecl.append('std::ostream& '\
						  'operator<<(std::ostream& os, const s_%s& o)' % \
						  objname)
		structdecl.append('{')
		structdecl.append('  char r[1024];')
		structdecl.append('  os << "%s" << std::endl;' % objname)
		for rtype, metname, c in values:
			structdecl.append('  sprintf(r, "  %s: %s\\n", "%s", (double)o.%s); '\
							  'os << r;' % ("%-32s", "%f", metname, metname))
		structdecl.append('  return os;')
		structdecl.append('}')
		structdecl.append('')

		structimpl.append('    }')	

	structimpl.append('}')

	keys = vars.keys()
	keys.sort()
	pydeclare = []
	pyselect  = []
	declare = []
	select  = []
	varimpl= []
	varimpl.append('TLeaf* leaf=0;')
	varimpl.append('int N=0;\n')
	for varname in keys:
		n, rtype, branchname, count = vars[varname]

		# treat bools as ints

		if rtype == "bool": rtype = "int"

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

		# Code for selector.h

		varimpl.append('leaf = tree->GetLeaf("%s");' % branchname)
		if count == 1:
			if rtype != "double":
				varimpl.append('%s = static_cast<%s>(leaf->GetValue());' % \
							   (varname, rtype))
			else:
				varimpl.append('%s = leaf->GetValue();' % varname)
		else:
			varimpl.append('N    = min(leafcount(leaf), %d);' % count)
			varimpl.append('%s.resize(N);' % varname)
			varimpl.append('for(int i=0; i < N; ++i)')
			if rtype != "double":
				varimpl.append('  %s[i] = static_cast<%s>'\
							   '(leaf->GetValue(i));' % (varname, rtype))
			else:
				varimpl.append('  %s[i] = leaf->GetValue(i);' % varname)

		varimpl.append('')


	# Write out files

	# Put everything into a directory
	cmd = '''
	mkdir -p %(dir)s/tmp
	cp %(hpp)s %(dir)s
	cp %(cpp)s %(dir)s
	''' % {'dir': filename,
		   'hpp': TREESTREAM_HPP,
		   'cpp': TREESTREAM_CPP}
	os.system(cmd)

	cmd = '''
	cp %(hpp)s %(dir)s
	cp %(cpp)s %(dir)s
	''' % {'dir': filename,
		   'hpp': PDG_HPP,
		   'cpp': PDG_CPP}

	os.system(cmd)

	# Create Makefile

	names = {'name': filename,
			 'filename': filename,
			 'time': ctime(time()),
			 'author': AUTHOR,
			 'percent': '%'
			 }

	record = MAKEFILE % names
	open("%s/Makefile" % filename, "w").write(record)	

	# Create C++ code

	outfilename = "%s/%s.h" % (filename, filename)
	names = {'name': filename,
			 'NAME': upper(filename),
			 'time': ctime(time()),
			 'vardecl': join("", declare, "\n"),
			 'selection': join("  ", select, "\n"),
			 'author': AUTHOR,
			 'structdecl': join("", structdecl, "\n"),
			 'structimpl': join("", structimpl, "\n")
			 }

	record = TEMPLATE_H % names
	open(outfilename,"w").write(record)

	outfilename = "%s/%s.cc" % (filename, filename)
	record = TEMPLATE_CC % names
	open(outfilename,"w").write(record)

	# Create selector.h code

	records = open(SELECTEDOBJECTMAP_H).readlines()[2:-1]
	names = {'vardecl': join("", declare, "\n"),
			 'class':   join("", records, ""),
			 'time':    ctime(time()),
			 'varimpl': join("  ", varimpl, "\n"),
			 'author': AUTHOR
			 }
	record = SLTEMPLATE % names
	open("selector.h","w").write(record)

	# Create python code

	s = join("", pydeclare, "\n") + "\n" + join("", pyselect,"\n")
	outfilename = "%s/%slib.py" % (filename, filename)
	names = {'name': filename,
			 'time': ctime(time()),
			 'selection': s,
			 'author': AUTHOR,
			 'htab': "%s%s\\t%s\\t%s"
			 }
	record = PYTEMPLATELIB % names
	open(outfilename,"w").write(record)

	outfilename = "%s/%s.py" % (filename, filename)
	record = PYTEMPLATE % names
	open(outfilename,"w").write(record)
	os.system("chmod +x %s" % outfilename)

	print "\tdone!"
#------------------------------------------------------------------------------
main()

