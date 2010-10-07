#!/usr/bin/env python
#------------------------------------------------------------------------------
# Description: Create ntuple selector.h file
# Created: 06-Mar-2010 Harrison B. Prosper
# Updated: 05-Oct-2010 HBP - clean up
#$Revision: 1.1 $
#------------------------------------------------------------------------------
import os, sys, re, posixpath
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
	   mkselector.py <output-filename> [variables.txt]
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
	VARMAP  = "%s/interface/VariableMap.h" % PACKAGE
	SELECTED= "%s/interface/SelectedObjectMap.h" % PACKAGE
else:
	VARMAP  = "VariableMap.h" % PACKAGE
	SELECTED= "SelectedObjectMap.h" % PACKAGE
	
# Make sure that we can find treestream etc.

if not os.path.exists(VARMAP):
	print "\n** error ** - required file:\n\t%s\n\t\tNOT found!" % VARMAP
	sys.exit(0)
#------------------------------------------------------------------------------
# For later
TMP=\
	  '''
// Classes
%(class)s

// Functions
inline
void keepObject(std::string name, int index)
{
  SelectedObjectMap::instance().set(name, index);
}
	  '''
HEADER=\
'''#ifndef %(NAME)s_H
#define %(NAME)s_H
//-----------------------------------------------------------------------------
// File:        %(name)s.h
// Description: event selector
// Created:     %(time)s by mkselector.py
// Author:      %(author)s
// $Revision: 1.1 $
//-----------------------------------------------------------------------------
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <stdlib.h>
#include "TROOT.h"
#include "TTree.h"
#include "TMap.h"
//-----------------------------------------------------------------------------
// -- Declare variables to be initialized every event
//-----------------------------------------------------------------------------
%(decl)s
//-----------------------------------------------------------------------------
void initializeEvent(TMap& vars)
{
  std::vector<double>* v=0;
  
%(impl)s
}
#endif
'''

MACRO=\
		  '''//-----------------------------------------------------------------------------
// File:        %(name)s.C
// Description: event selector
// Created:     %(time)s by mkselector.py
// Author:      %(author)s
// $Revision: 1.1 $
//-----------------------------------------------------------------------------
#include "%(name)s.h"
//-----------------------------------------------------------------------------
bool %(name)s(TMap& vars)
{
  initializeEvent(vars);

  // perform selection
  // if ( miserable-event ) return false;
  return true;
}
'''
COMPILE=\
		  '''#!/usr/bin/env python
import os, sys
from ROOT import *
#------------------------------------------------------------------------------
if os.environ.has_key("CMSSW_BASE"):
	rbase= os.environ["CMSSW_RELEASE_BASE"]
	base = os.environ["CMSSW_BASE"]
	arch = os.environ["SCRAM_ARCH"]
	
	incp = "-I" + base + "/src"
	gSystem.AddIncludePath(incp)

	incp = "-I" + rbase + "/src"
	gSystem.AddIncludePath(incp)

	libp = "-L" + base + "/lib/" + arch + " -lPhysicsToolsMkntuple" 
	gSystem.AddLinkedLibs(libp)

	libp = "-L" + rbase + "/lib/" + arch 
	gSystem.AddLinkedLibs(libp)
	
	gROOT.ProcessLine(".L %(name)s.C+");
'''
COMPILE=\
		  '''#!/usr/bin/env python
import os, sys
from ROOT import *
#------------------------------------------------------------------------------
if not os.environ.has_key("CMSSW_BASE"): sys.exit(0)
print "rootcint"
os.system("rootcint -f dict.cc -c  linkdef.h; "\
          "mv dict.h tmp2.h)

open("tmp1.h").write("#include <map>\n"\
                     "#include <vector>\n"\
					 "#include <string>\n")
					 
os.system("cat tmp1.txt tmp2.h > dict.h")
print "compile"
os.system("g++ -c dict.cc `root-config --cflags` -pipe -O2 -fPIC -Wall")
os.system("g++ -c %(name)s.cc `root-config --cflags` -pipe -O2 -fPIC -Wall")
pirnt "link"
os.system("g++ -shared dict.o %(name)s.o "\
          "`root-config --libs` -o lib%(name)s.so")
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

		# vector types must have the same object name and a max count > 1
		if count > 1:
			if fldname != "":
				if not vectormap.has_key(objname): vectormap[objname] = []	
				vectormap[objname].append((rtype, fldname, varname, count))
				
		# update map for all variables
		vars[varname] = [1, rtype, branchname, count, iscounter]	

	# Declare all variables
	
	keys = vars.keys()
	keys.sort()
	decl = []
	impl = []

	for varname in keys:
		n, rtype, branchname, count, iscounter = vars[varname]

		# If this is a counter variable with a name identical to that of a
		# vector variable, ignore it
		if iscounter and vectormap.has_key(varname): continue

		if rtype == "bool": rtype = "int"

		if count == 1:
			decl.append("%s\t%s;" % (rtype, varname))
			impl.append('  v = ((Varvector*)(vars.GetValue("%s")))->value;' \
						% branchname)
			impl.append('  assert(v);')
			impl.append('  %s = (*v)[0];' % varname)

		else:
			# this is a vector
			decl.append("std::vector<%s>\t%s(%d,0);" % \
						(rtype, varname, count)) 		
			
			impl.append('  v = ((Varvector*)(vars.GetValue("%s")))->value;' \
						% branchname)
			impl.append('  assert(v);')
			impl.append('  %s.resize(v->size());' % varname)
			impl.append('  copy(v->begin(), v->end(), %s.begin());' % varname)
			impl.append('')
			
	# Create C++ codes

	names = {'name'  : filename,
			 'NAME'  : upper(filename),
			 'time'  : ctime(time()),
			 'class' : join("", records, ""),
			 'decl'  : join("", decl, "\n"),
			 'impl'  : join("", impl, "\n"),
			 'author': AUTHOR,
			 's': '%s',
			 }

	record = HEADER % names
	outfilename = "%(name)s.h" % names
	open(outfilename, "w").write(record)

	record = MACRO % names
	outfilename = "%(name)s.C" % names
	open(outfilename, "w").write(record)

## 	record = COMPILE % names
## 	outfilename = "compile_%(name)s.py" % names
## 	open(outfilename, "w").write(record)
## 	os.system("chmod +x compile_%(name)s.py" % names)

	print "\tdone!"
#------------------------------------------------------------------------------
main()

