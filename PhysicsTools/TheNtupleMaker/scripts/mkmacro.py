#!/usr/bin/env python
#------------------------------------------------------------------------------
# File: mkusermacro.py
# Description: Create ntuple selector.h file
# Created: 06-Mar-2010 Harrison B. Prosper
# Updated: 05-Oct-2010 HBP - clean up
#          12-Mar-2011 HBP - give user option to add variables
#          07-May-2012 HBP - fix object selection bug
#          13-May-2012 HBP - add more comments, include object counters
#$Id: mkmacro.py,v 1.5 2012/05/08 01:58:06 prosper Exp $
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
	   mkmacro.py <usermacro-name> [variables.txt]
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

HEADER=\
'''#ifndef %(NAME)s_H
#define %(NAME)s_H
//-----------------------------------------------------------------------------
// File:        %(name)s.h
// Description: user macro called by TheNtupleMaker
// Created:     %(time)s by mkmacro.py
// Author:      %(author)s
//
// WARNING: It is better not to edit this header. You may inadvertently lose
//          your changes!
//
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
//-----------------------------------------------------------------------------

// TNM: for internal use only
struct countvalue
{
  int*    count;
  double* value;
};
typedef std::map<std::string, countvalue> VarMap;
typedef std::map<std::string, std::vector<int> > IndexMap;

/**----------------------------------------------------------------------------
  All user-defined variables and functions should be declared in this struct
  in the program %(name)s.cc, not in this header.
  
  This coding technique avoids the need to modify the header.
  An object of this type, called "local", is allocated in beginJob().
 ----------------------------------------------------------------------------*/

class %(name)sInternal;

// ----------------------------------------------------------------------------
class %(name)s
{
public:
  %(name)s(TTree* tree_, VarMap* varmap_, IndexMap* indexmap_)
    : tree(tree_), varmap(varmap_), indexmap(indexmap_)
%(init)s
	{}

  ~%(name)s() {}

  void beginJob();
  void initialize() { initialize_(); }
  void endJob();
  
  bool analyze();

  /**--------------------------------------------------------------------------
  call these functions to select the specified objects

  example:
  
    select("jet");    to be called from beginJob()
 
  and
  
    select("jet", i); to be called from analyze() for every object
  
    to be kept
  ---------------------------------------------------------------------------*/
  void select(std::string name)
  {
    (*indexmap)[name] = std::vector<int>();
  }
  
  void select(std::string name, int index)
  {
    if ( indexmap->find(name) == indexmap->end() ) return;
    (*indexmap)[name].push_back(index);
  }

private:
  TTree*    tree;
  VarMap*   varmap;
  IndexMap* indexmap;
  %(name)sInternal* local;

  // ------------------------------------------------------------------------
  // --- Initialize variables
  // ------------------------------------------------------------------------
%(decl)s

  void initialize_()
  {
    // clear object selection map every event
	
    for(IndexMap::iterator item=indexmap->begin(); 
        item != indexmap->end();
        ++item)
      item->second.clear();	
	
%(impl)s
  }
  
public:
  ClassDef(%(name)s,1)
};
ClassImp(%(name)s)

#endif
'''

TMP2='''
//-----------------------------------------------------------------------------
// --- These structs can be filled by calling fillObjects()
// --- after the call to initializeEvents(...)
//-----------------------------------------------------------------------------
%(structdecl)s
%(structimpl)s
'''
MACRO=\
		  '''//-----------------------------------------------------------------------------
// File:        %(name)s.cc
// Description: user macro
// Created:     %(time)s by mkmacro.py
// Author:      %(author)s
//-----------------------------------------------------------------------------
#include "%(name)s.h"
using namespace std;
/**----------------------------------------------------------------------------
  All user-defined variables and functions should be declared in this struct.
  This coding technique avoids the need to modify the header %(name)s.h.
  An object of this type, called "local", is allocated in beginJob().

  
  IMPORTANT: If some of these variables are to be added to the ntuple,
             make sure the type of each variable matches the type format
			 specified in the call to tree->Branch(...). See example below.
 ----------------------------------------------------------------------------*/
struct %(name)sInternal
{
  int counter;

  //float HT;
};

/**----------------------------------------------------------------------------
  call these functions to select the specified objects

  example:
  
    select("jet");    to be called from beginJob()
 
  and
  
    select("jet", i); to be called from analyze() for every object
  
    to be kept
 ----------------------------------------------------------------------------*/
void %(name)s::beginJob()
{
  local = new %(name)sInternal();
  local->counter = 0;

  // Add a float variable to ntuple
  // Note match between the type of the variable HT (a float) and its
  // format specifier ("HT/F")
  //tree->Branch("HT", &local->HT, "HT/F");

  // Define objects that are to be selected in analyze()
  //select("jet");
}

void %(name)s::endJob()
{
  if ( local ) delete local;
}
//-----------------------------------------------------------------------------

bool %(name)s::analyze()
{
  local->counter++;

  // compute variables
  // apply cuts etc.

  //local->HT = 0;
  //for(unsigned int i=0; i < jet_pt.size(); ++i)
  //{
  //  if ( !(jet_pt[i] > 100) ) continue;
  //  if ( !(jet_pt[i] < 400) ) continue;
  //
  //  select("jet", i);
  //  local->HT += jet_pt[i];
  //} 
  
  //if ( miserable-event )
  //  return false;
  //else
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

	libp = "-L" + base + "/lib/" + arch + " -lPhysicsToolsTheNtupleMaker" 
	gSystem.AddLinkedLibs(libp)

	libp = "-L" + rbase + "/lib/" + arch 
	gSystem.AddLinkedLibs(libp)
	
	gROOT.ProcessLine(".L %(name)s.C+");
'''
COMPILE=\
		  '''#------------------------------------------------------------------------------
# Created: %(time)s
#------------------------------------------------------------------------------
name	:= %(name)s
AT      := @
#------------------------------------------------------------------------------
CINT	:= rootcint
CXX     := g++
LDSHARED:= g++
#------------------------------------------------------------------------------
DEBUG	:= -ggdb
CPPFLAGS:= -I. $(shell root-config --cflags)
CXXFLAGS:= $(DEBUG) -pipe -O2 -fPIC -Wall
LDFLAGS := -shared 
#------------------------------------------------------------------------------
LIBS	:= $(shell root-config --glibs)
#------------------------------------------------------------------------------
linkdef	:= $(name)Linkdef.h
header  := %(name)s.h
cinthdr := dict.h
cintsrc	:= dict.cc
cintobj	:= dict.o

cppsrc 	:= $(name).cc
cppobj  := $(name).o

objects	:= $(cintobj) $(cppobj) 
library	:= lib$(name).so
#-----------------------------------------------------------------------
lib:	$(library)

$(library)	: $(objects)
	@echo "---> Linking $@"
	$(AT)$(LDSHARED) $(LDFLAGS) $+ $(LIBS) -o $@
	@rm -rf $(cinthdr) $(cintsrc) $(objects) 

$(cppobj)	: $(cppsrc)
	@echo "---> Compiling `basename $<`" 
	$(AT)$(CXX)	$(CXXFLAGS) $(CPPFLAGS) -c $<

$(cintobj)	: $(cintsrc)
	@echo "---> Compiling `basename $<`"
	$(AT)$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<

$(cintsrc) : $(header) $(linkdef)
	@echo "---> Generating dictionary `basename $@`"
	$(AT)$(CINT) -f $@ -c $(CPPFLAGS) $+

$(linkdef)	:
	@echo "---> Creating $(linkdef)"
	@echo -e "#include <map>"  >	$(linkdef)
	@echo -e "#include <string>" >>	$(linkdef)
	@echo -e "#include <vector>" >>	$(linkdef)
	@echo -e "#ifdef __CINT__" >> $(linkdef)
	@echo -e "#pragma link off all globals;" >> $(linkdef)
	@echo -e "#pragma link off all classes;" >> $(linkdef)
	@echo -e "#pragma link off all functions;" >> $(linkdef)
	@echo -e "#pragma link C++ class countvalue+;" >> $(linkdef)
	@echo -e "#pragma link C++ class map<string, countvalue>+;" \
	>> $(linkdef)
	@echo -e "#pragma link C++ class map<string, vector<int> >+;" \
	>> $(linkdef)
	@echo -e "#pragma link C++ class %(name)s+;" \
	>> $(linkdef)	
	@echo -e "#endif" >> $(linkdef)

clean   :
	@rm -rf dict.* $(objects)  $(library) $(linkdef)
'''
#------------------------------------------------------------------------------
def main():
	print "\n\tmkmacro.py"

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


	# skip first line
	records = records[1:]
	stnamemap = {}
	reclist = []
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
			
		reclist.append((rtype, branchname, varname, count))
		
	# Loop over branch names
	
	# If a variable name matches a struct name, this will generate a
	# multiply defined error. One of the names must be altered. Let's
	# take this to be the variable name.

	vars = {}
	usednames = {}
	vectormap = {}
	
	for index, (rtype, branchname, varname, count) in enumerate(reclist):

		# Check that varname is not the same as that of a potential
		# struct
		if stnamemap.has_key(varname):
			print "\t**warning: multiply defined name, %s; changing " % varname
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

		# Check for leaf counter flag (a "*")

		t = split(count)
		count = atoi(t[0]) # Change type to an integer
		iscounter = t[-1] == "*"

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
	decl = []
	impl = []
	init = []
	for index, varname in enumerate(keys):
		n, rtype, branchname, count, iscounter = vars[varname]

		# If this is a counter variable ignore it
		#if iscounter: continue

		if rtype == "bool": rtype = "int"

		if iscounter:
			decl.append("  %s\t%s;" % (rtype, varname))
			impl.append('    countvalue& v%d = (*varmap)["%s"];' %(index,
																   branchname))
			impl.append('    if ( v%d.count )' % index)
			impl.append('      %s = *v%d.count;' % (varname, index))
			impl.append('    else')
			impl.append('      %s = 0;' % varname)
			impl.append('')			

		elif count == 1:
			decl.append("  %s\t%s;" % (rtype, varname))
			impl.append('    countvalue& v%d = (*varmap)["%s"];' % (index,
															   branchname))
			impl.append('    if ( v%d.value )' % index)
			impl.append('      %s = *v%d.value;' % (varname, index))
			impl.append('    else')
			impl.append('      %s = 0;' % varname)
			impl.append('')

		else:
			# this is a vector
			init.append(" %s\t(std::vector<%s>(%d,0))" %\
						(varname, rtype, count))
			decl.append("  std::vector<%s>\t%s;" %(rtype, varname))
			impl.append('    countvalue& v%d = (*varmap)["%s"];' % (index,
															   branchname))
			impl.append('    if ( v%d.value )' % index)
			impl.append('      {')
			impl.append('        %s.resize(*v%d.count);' % (varname, index))
			impl.append('        copy(v%d.value, v%d.value+*v%d.count, '\
						'%s.begin());'% (index, index, index, varname))
			impl.append('      }')
			impl.append('    else')
			impl.append('      %s.clear();' % varname)
			impl.append('')

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

		structdecl.append('struct %s_s' % objname)
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
	structimpl.append('}')  # end of fillObjects()
			
	# Create C++ codes

	names = {'name'  : filename,
			 'NAME'  : upper(filename),
			 'time'  : ctime(time()),
			 'class' : join("", records, ""),
			 'decl'  : join("", decl, "\n"),
			 'impl'  : rstrip(join("", impl, "\n")),
			 'init'  : join("    ,", init, "\n"),
			 'author': AUTHOR,
			 's': '%s',
			 'structdecl': join("", structdecl, "\n"),
			 'structimpl': join("", structimpl, "\n")			 
			 }

	outfilename = "%(name)s.h" % names
	record = HEADER % names
	open(outfilename, "w").write(record)


	outfilename = "%(name)s.cc" % names
	if os.path.exists(outfilename):
		print "\t** %s already exists...new version NOT created!" % \
		outfilename
	else:
		record = MACRO % names	
		open(outfilename, "w").write(record)

	record = COMPILE % names
	outfilename = "%(name)s.mk" % names
	open(outfilename, "w").write(record)

	print "\n\tto compile and link the macro %(name)s\n\tdo" % names
	print "\t\tmake -f %(name)s.mk\n" % names
#------------------------------------------------------------------------------
main()

