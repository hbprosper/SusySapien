#!/usr/bin/env python
#------------------------------------------------------------------------------
# Description: Create ntuple analyzer template
# Created: 06-Mar-2010 Harrison B. Prosper
# Updated: 12-Mar-2010 HBP - fix appending of .root
#          08-Jun-2010 HBP - add creation of selector.h
#          02-Sep-2010 HBP - fix variables.txt record splitting bug
#$Revision: 1.9 $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time import *
from glob import glob
from PhysicsTools.LiteAnalysis.boostlib import nameonly, getauthor
#------------------------------------------------------------------------------
if not os.environ.has_key("CMSSW_BASE"):
	print "\t**you must first set up CMSSW"
	sys.exit(0)

PACKAGE = "%s/src/PhysicsTools/LiteAnalysis" % os.environ["CMSSW_BASE"]
TREESTREAM_HPP = "%s/interface/treestream.hpp" % PACKAGE
TREESTREAM_CPP = "%s/src/treestream.cpp" % PACKAGE
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
// $Revision: 1.9 $
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

#include "PhysicsTools/LiteAnalysis/interface/treestream.hpp"

#else

#include "treestream.hpp"

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

void
decodeCommandLine(int argc, char** argv, 
                  std::string& filelist, std::string& histfilename)
{
  filelist = std::string("filelist.txt");
  if ( argc > 1 ) filelist = std::string(argv[1]);

  if ( argc > 2 ) 
    histfilename = std::string(argv[2]);// 2nd (optional) command line argument
  else
    histfilename = std::string(argv[0]);// default: name of program

  // Make sure extension is ".root"
  histfilename = nameonly(histfilename);
  histfilename += ".root";
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

void
saveHistograms(std::string histfilename, 
               TDirectory* dir=gDirectory,
			   TFile* hfile=0,
			   int depth=0)
{
  // Create output file

  if ( depth == 0 )
    {
      std::cout << "Saving histograms to " << histfilename << std::endl;
      hfile = new TFile(histfilename.c_str(), "RECREATE");
    }

  // Important!
  depth++;
  // Check recursion depth 
  if ( depth > 100 )error("saveHistograms is lost in trees!");
  std::string tab(2*depth, ' ');

  // Loop over all histograms

  TList* list = dir->GetList();
  TListIter* it = (TListIter*)list->MakeIterator();

  while ( TObject* o = it->Next() )
    {
      dir->cd();
      
      if ( o->IsA()->InheritsFrom("TDirectory") )
        {
          TDirectory* d = (TDirectory*)o;
          std::cout << tab << "BEGIN " << d->GetName() << std::endl;
          saveHistograms(histfilename, d, hfile, depth);
          std::cout << tab << "END " << d->GetName() << std::endl;
        }
      // Note: All histograms inherit from TH1
      else if ( o->IsA()->InheritsFrom("TH1") )
        {
          TH1* h = (TH1*)o;
          std::cout << tab  << o->ClassName() 
                    << "\\t" << h->GetName()
                    << "\\t" << h->GetTitle()
                    << std::endl;
          hfile->cd();
          h->Write("", TObject::kOverwrite);
        }
    } // end of loop over keys

  hfile->Close();
  delete hfile;
}
  
//-----------------------------------------------------------------------------
// Define variables to be read
//-----------------------------------------------------------------------------
%(vardecl)s

void selectVariables(itreestream& stream)
{
%(selection)s
}

#endif
'''

TEMPLATE_CC =\
'''//-----------------------------------------------------------------------------
// File:        %(name)s.cc
// Description: Analyzer for ntuples created by Mkntuple
// Created:     %(time)s by mkntanalyzer.py
// Author:      %(author)s
// $Revision: 1.9 $
//-----------------------------------------------------------------------------
#include "%(name)s.h"

using namespace std;
//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  // Get file list and histogram filename from command line

  string filelist;
  string histfilename;
  decodeCommandLine(argc, argv, filelist, histfilename);

  // Get names of ntuple files to be processed and open chain of ntuples
  
  vector<string> filenames = getFilenames(filelist);
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

  //---------------------------------------------------------------------------
  // Loop over events
  //---------------------------------------------------------------------------

  for(int entry=0; entry < nevents; ++entry)
    {
      stream.read(entry);
	  // Find SUSY already!
    }
  stream.close();

  saveHistograms(histfilename);
  
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
// $Revision: 1.9 $
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

void initselector()
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

PYTEMPLATE =\
'''#!/usr/bin/env python
# -----------------------------------------------------------------------------
#  File:        %(name)s
#  Description: Analyzer for ntuples created by Mkntuple
#  Created:     %(time)s by mkntanalyzer.py
#  Author:      %(author)s
#  $Revision: 1.9 $
# -----------------------------------------------------------------------------
import os, sys, re
from ROOT import *
from time import sleep
from string import *
from PhysicsTools.LiteAnalysis.AutoLoader import *
# -----------------------------------------------------------------------------
# -- Procedures and functions
# -----------------------------------------------------------------------------
def decodeCommandLine():
	argv = sys.argv
	argc = len(argv)
	filelist = "filelist.txt"
	if argc > 1: filelist = argv[1]
	
	if argc > 2: 
		histfilename = argv[2] # 2nd (optional) command line argument
	else:
		histfilename = argv[0] # default: name of program

	# Make sure extension is ".root"
	histfilename = os.path.basename(histfilename)
	histfilename = split(histfilename, ".")[0] + ".root"
	return (filelist, histfilename)
	
def error(message):
	print "** error ** " + message
	sys.exit(0)

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
def main():

	filelist, histfilename = decodeCommandLine()
	
	#  Get names of ntuple files to be processed and open chain of ntuples
  
	filenames = getFilenames(filelist)
	stream = itreestream(filenames, "Events")
	if not stream.good(): error("unable to open ntuple file(s)")
  
	#  Get number of events to be read
  
	nevents = stream.size();
	print "Number of events:",nevents
	
	# -------------------------------------------------------------------------
	#  Define variables to be read
    # -------------------------------------------------------------------------
%(selection)s

    # -------------------------------------------------------------------------
	#  Book histograms etc.
	# -------------------------------------------------------------------------
	kit.setStyle()

	# -------------------------------------------------------------------------
	#  Loop over events
	# -------------------------------------------------------------------------

	for entry in xrange(nevents):
		stream.read(entry)

		#  Find SUSY...
	  
	stream.close()

	kit.saveHistograms(histfilename)
# -----------------------------------------------------------------------------
main()
'''

MAKEFILE = '''#------------------------------------------------------------------------------
# Description: Makefile to build executable %(filename)s
# Created:     %(time)s by mkanalyzer.py
#
#               available switches:
#
#                 debugflag  (e.g., debugflags=-g)
#                 cppflags
#                 cxxflags
#                 optflag
#                 verbose    (e.g., verbose=1)
#                 withcern   (e.g., withcern=1  expects to find CERN_LIB)
# Author:      %(author)s
#$Revision: 1.9 $
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
#------------------------------------------------------------------------------
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
	# skip first line
	records = records[1:]
	for index in xrange(len(records)):
		record = records[index]
		if record == "": continue
		rtype, branchname, varname, count = split(record, '/')
		count = atoi(count)
		n = 1
		# Take care of duplicate names
		if vars.has_key(varname):
			# duplicate name; for now just postfix with an integer
			n, r, b, c = vars[varname]
			n += 1
			vars[varname][0] = n;
			varname = "%s%d" % (varname, n)
		vars[varname] = [1, rtype, branchname, count]		

	# Construct declarations etc.
	
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
			 'author': AUTHOR
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
	
	s = join("\t", pydeclare, "\n") + "\n" + join("\t", pyselect,"\n")
	outfilename = "%s/%s.py" % (filename, filename)
	names = {'name': outfilename,
			 'time': ctime(time()),
			 'selection': s,
			 'author': AUTHOR
			 }
	record = PYTEMPLATE % names
	open(outfilename,"w").write(record)
	os.system("chmod +x %s" % outfilename)

	print "\tdone!"
#------------------------------------------------------------------------------
main()

