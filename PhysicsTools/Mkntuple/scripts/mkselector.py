#!/usr/bin/env python
#------------------------------------------------------------------------------
# Description: Create ntuple selector.h file
# Created: 06-Mar-2010 Harrison B. Prosper
# Updated: 05-Oct-2010 HBP - clean up
#$Revision: 1.15 $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time import *
from glob import glob
#------------------------------------------------------------------------------
# Functions
#------------------------------------------------------------------------------
getvno = re.compile(r'[0-9]+$')

def usage():
	print '''
	Usage:
	   mkselector.py <output-filename> [var-filename=variables.txt]
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
	PACKAGE = "%s/src/PhysicsTools/Mkntuple" % CMSSW_BASE
	TREESTREAM_HPP = "%s/interface/treestream.h" % PACKAGE
	TREESTREAM_CPP = "%s/src/treestream.cc" % PACKAGE
	PDG_HPP = "%s/interface/pdg.h" % PACKAGE
	PDG_CPP = "%s/src/pdg.cc" % PACKAGE
	SELECTED_H = "%s/interface/SelectedObjectMap.h" % PACKAGE
else:
	TREESTREAM_HPP = "treestream.h"
	TREESTREAM_CPP = "treestream.cc"
	PDG_HPP = "pdg.h"
	PDG_CPP = "pdg.cc"
	SELECTED_H = "SelectedObjectMap.h"
	
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
SLTEMPLATE=\
'''#ifndef SELECTOR_H
#define SELECTOR_H
//-----------------------------------------------------------------------------
// File:        selector.h
// Description: selector template
// Created:     %(time)s by mkselector.py
// Author:      %(author)s
// $Revision: 1.15 $
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
void keepObject(std::string name, int index)
{
  SelectedObjectMap::instance().set(name, index);
}

//-----------------------------------------------------------------------------
// -- Declare variables to be initialized every event
//-----------------------------------------------------------------------------
%(vardecl)s

//-----------------------------------------------------------------------------
static bool firstCall=true;
void initSelector()
{  
  if ( firstCall )
    {
	  firstCall = false;
	  TTree* tree = (TTree*)gROOT->FindObject("Events");
	  if ( tree == 0 )
	    {
		  std::cout << "\t** initSelector error ** cannot find tree Events"
		            << std::endl;
		  exit(0);
		}
		
      static itreestream stream(tree);     		

%(selection)s
    }
  stream.read(-1);
}
//-----------------------------------------------------------------------------
// --- These structs can be filled by calling fillObjects()
// --- after the call to initSelector()
//-----------------------------------------------------------------------------
%(structdecl)s
%(structimpl)s

#endif
'''
#------------------------------------------------------------------------------
def main():
	print "\n\tmkselector.py"

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
		print "\t** error ** can't find variable file: %s" % varfilename
		sys.exit(0)

	# Read variable names

	records = map(strip, open(varfilename, "r").readlines())

	# Create maps from object name to object type etc.
	# But only group together variables that are meant to be
	# together!

	vars = {}
	vectormap = {}
	
	# skip first line
	records = records[1:]
	for index in xrange(len(records)):
		record = records[index]
		if record == "": continue
		
		rtype, branchname, varname, count = split(record, '/')

		# Get object and "method" names
		t = split(varname,'_')
		objname = t[0]                    # object name
		if len(t) > 1:
			fldname = joinfields(t[1:],'_')   # field name
		else:
			fldname = ''
			
		# Check for leaf counter flag (a "*")
		t = split(count)
		count = atoi(t[0])
		iscounter = t[-1] == "*"
		
		n = 1
		# Take care of duplicate names
		if vars.has_key(varname):
			# duplicate name; add a number to object name
			n, a, b, c, d = vars[varname]
			n += 1
			vars[varname][0] = n;

			objname = "%s%d" % (objname, n)
			if fldname != '':
				varname = "%s_%s" % (objname, fldname)
			else:
				varname = objname

		# update map for all variables
		vars[varname] = [1, rtype, branchname, count, iscounter]

		# vector types must have the same object name and a max count > 1
		if count > 1:
			if fldname != "":
				if not vectormap.has_key(objname): vectormap[objname] = []	
				vectormap[objname].append((rtype, fldname, varname, count))
	

	# Declare all variables
	
	keys = vars.keys()
	keys.sort()
	declare = []
	select  = []

	for varname in keys:
		n, rtype, branchname, count, iscounter = vars[varname]

		# If this is a counter variable with a name identical to that of a
		# vector variable, ignore it
		if iscounter and vectormap.has_key(varname): continue

		if rtype == "bool": rtype = "int"

		if count == 1:
			declare.append("%s\t%s;" % (rtype, varname))
		else:
			# this is a vector
			declare.append("std::vector<%s>\t%s(%d,0);" % \
						   (rtype, varname, count))
		select.append('stream.select("%s", %s);' % (branchname, varname))

	# Create structs for vector variables
	
	keys = vectormap.keys()
	keys.sort()	
	structdecl = []
	structimpl = []

	structimpl.append('void fillObjects()')
	structimpl.append('{')

	for index, objname in enumerate(keys):
		values = vectormap[objname]
		varname= values[0][-2];
		
		structimpl.append('')
		structimpl.append('  %s.resize(%s.size());' % (objname, varname))
		structimpl.append('  for(unsigned int i=0; i < %s.size(); ++i)' % \
						  varname)
		structimpl.append('    {')

		structdecl.append('struct %s_t' % objname)
		structdecl.append('{')
		for rtype, fldname, varname, count in values:
			# treat bools as ints
			if rtype == "bool":
				cast = '(bool)'
			else:
				cast = ''

			structdecl.append('  %s\t%s;' % (rtype, fldname))

			structimpl.append('      %s[i].%s\t= %s%s[i];' % (objname,
															  fldname,
															  cast,
															  varname))
		structdecl.append('};')
		structdecl.append('')
		structdecl.append('std::vector<%s_t> %s(%d);' % (objname,
														 objname,
														 count))
		structdecl.append('')
		structdecl.append('std::ostream& '\
						  'operator<<(std::ostream& os, const %s_t& o)' % \
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
		structdecl.append('')
		
		structimpl.append('    }')	
	structimpl.append('}')  # end of fillObjects()
	
	# Write out files

	# Create C++ code
	records = open(SELECTED_H).readlines()[2:-1]
	
	outfilename = "selector.h"
	names = {'name': filename,
			 'NAME': upper(filename),
			 'time': ctime(time()),
			 'class' : join("", records, ""),
			 'vardecl': join("", declare, "\n"),
			 'selection': join("     ", select, "\n"),
			 'author': AUTHOR,
			 'structdecl': join("", structdecl, "\n"),
			 'structimpl': join("", structimpl, "\n")
			 }
	record = SLTEMPLATE % names
	open(outfilename, "w").write(record)

	print "\tdone!"
#------------------------------------------------------------------------------
main()

