#!/usr/bin/env python
#------------------------------------------------------------------------------
# Description: Create ntuple analyzer template
# Created: 06-Mar-2010 Harrison B. Prosper
#$Revision:$
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time import *
from glob import glob
from PhysicsTools.LiteAnalysis.Lib import nameonly
#------------------------------------------------------------------------------
if not os.environ.has_key("CMSSW_BASE"):
	print "\t**you must first set up CMSSW"
	sys.exit(0)

getvno = re.compile(r'[0-9]+$')

def usage():
	print '''
	Usage:
	   mkanalyzer.py <output-filename> [var-filename=Mkntuple.log]
	'''
	sys.exit(0)

def join(left, a, right):
	s = ""
	for x in a:
		s = s + "%s%s%s" % (left, x, right)
	return s

TEMPLATE =\
'''//-----------------------------------------------------------------------------
// File:        %(name)s
// Description: Analyzer for ntuples created by Mkntuple
// Created:     %(time)s by mkntanalyzer.py
// $Revision:$
//-----------------------------------------------------------------------------

// -- System

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

// -- CMSSW

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/LiteAnalysis/interface/treestream.hpp"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"

// -- Root

#include "TApplication.h"
#include "TCanvas.h"
#include "TH1F.h"
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------

// -- Procedures and functions

void
error(string message)
{
  cout << "** error ** " << message << endl;
  exit(0);
}

// Read ntuple filenames from file list

vector<string>
getFilenames(int argc, char** argv)
{
  string filelist("filelist.txt");
  if ( argc > 1 ) filelist = string(argv[1]);

  // Open file list
  
  ifstream stream(filelist.c_str());
  if ( !stream.good() ) error("unable to open file: " + filelist);

  // Get list of ntuple files to be processed
  
  vector<string> v;
  string filename;
  while ( stream >> filename )
    if ( kit::strip(filename) != "" ) v.push_back(filename);
  return v;
}

//=============================================================================

int main(int argc, char** argv)
{
  cout << "enabling autoloader...";
  AutoLibraryLoader::enable();
  cout << "done!" << endl;
  
  // The root application is needed to make canvases visible during
  // program execution. If this is not needed, just comment out the following
  // line

  TApplication app("analyzer", &argc, argv);
  
  // Get names of ntuple files to be processed and open chain of ntuples
  
  vector<string> filenames = getFilenames(argc, argv);
  itreestream stream(filenames, "Events");
  if ( !stream.good() ) error("unable to open ntuple file(s)");
  
  // Get number of events to be read
  
  int nevents = stream.size();
  cout << "Number of events: " << nevents << endl;
  
  //---------------------------------------------------------------------------
  // Define variables to be read
  //---------------------------------------------------------------------------
 %(selection)s

  //---------------------------------------------------------------------------
  // Book histograms etc.
  //---------------------------------------------------------------------------
  kit::setStyle();

  //---------------------------------------------------------------------------
  // Loop over events
  //---------------------------------------------------------------------------

  for(int entry=0; entry < nevents; entry++)
    {
      stream.read(entry);

	  // Find SUSY...
	  
    }
  stream.close();
  return 0;
}
'''
PYTEMPLATE =\
'''#!/usr/bin/env python
# -----------------------------------------------------------------------------
#  File:        %(name)s
#  Description: Analyzer for ntuples created by Mkntuple
#  Created:     %(time)s by mkntanalyzer.py
#  $Revision:$
# -----------------------------------------------------------------------------
import os, sys, re
from ROOT import *
from time import sleep
from string import *
from PhysicsTools.LiteAnalysis.AutoLoader import *
# -----------------------------------------------------------------------------
# -- Procedures and functions
# -----------------------------------------------------------------------------
def error(message):
	print "** error ** " + message
	sys.exit(0)

#  Read ntuple filenames from file list

def getFilenames():
	argv = sys.argv
	argc = len(argv)
	filelist = "filelist.txt"
	if argc > 1: filelist = argv[1]

	if not os.path.exists(filelist):
		error("unable to open file: " + filelist)
		
	# Get ntuple file names
	filenames = filter(lambda x: x != "",
					   map(strip, open(filelist).readlines()))
	v = vector("string")()
	for filename in filenames: v.push_back(filename)
	return v
# =============================================================================
def main():

	#  Get names of ntuple files to be processed and open chain of ntuples
  
	filenames = getFilenames()
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
# =============================================================================
main()

'''
#------------------------------------------------------------------------------
def main():
	print "\n\tmkntanalyzer.py"

	# Decode command line
	
	argv = sys.argv[1:]
	argc = len(argv)
	if argc < 1: usage()

	filename = nameonly(argv[0])
	if argc > 1:
		varfilename = argv[1]
	else:
		varfilename = "Mkntuple.log"
	if not os.path.exists(varfilename):
		print "mkntanalyzer.py - can't find variable file: %s" % varfilename
		sys.exit(0)

	# Be sure to rename existing file
	
	files = glob("%s.cc*" % filename)
	if len(files) > 0:
		fname = files[-1]
		vno = getvno.findall(fname)
		if len(vno) == 0:
			vno = 0
		else:
			vno = atoi(vno[0])
		vno += 1
		cmd = '''
		cp %s.cc %s.cc.%d
		''' % (filename, filename, vno)
		os.system(cmd)

		files = glob("%s.py*" % filename)
		if len(files) > 0:
			cmd = '''
			cp %s.py %s.py.%d
			''' % (filename, filename, vno)
			os.system(cmd)		

	# Read variable names
	
	records = map(strip, open(varfilename, "r").readlines())
	vars = {}
	active = False
	for index in xrange(len(records)):
		record = records[index]
		if record == "": continue
		if record == "END VARIABLES": break
		if record == "BEGIN VARIABLES":
			active = True
			continue
		if not active: continue
		
		rtype, branchname, varname, count = split(record)
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

	# Write out files
	
	s = join("  ", declare, "\n") + "\n" + join("  ", select, "\n")
	outfilename = "%s.cc" % filename
	names = {'name': outfilename,
			 'time': ctime(time()),
			 'selection': s[1:]}
	record = TEMPLATE % names
	open(outfilename,"w").write(record)

	s = join("\t", pydeclare, "\n") + "\n" + join("\t", pyselect,"\n")
	outfilename = "%s.py" % filename
	names = {'name': outfilename,
			 'time': ctime(time()),
			 'selection': s}
	record = PYTEMPLATE % names
	open(outfilename,"w").write(record)
	os.system("chmod +x %s" % outfilename)
	print "\tdone!"
#------------------------------------------------------------------------------
main()

