#!/usr/bin/env python
#------------------------------------------------------------------------------
# Description: Create ntuple selector.h file
# Created: 06-Mar-2010 Harrison B. Prosper
# Updated: 05-Oct-2010 HBP - clean up
#$Revision: 1.3 $
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
	   mkselector.py <selector-name> [variables.txt]
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
	SELECTED= "%s/interface/SelectedObjectMap.h" % PACKAGE
else:
	SELECTED= "SelectedObjectMap.h" % PACKAGE
	
## if not os.path.exists(SELECTED):
## 	print "\n** error ** - required file:\n\t%s\n\t\tNOT found!" % SELECTED
## 	sys.exit(0)
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
// $Revision: 1.3 $
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
// -- Declare variables to be initialized every event
//-----------------------------------------------------------------------------
%(decl)s
//-----------------------------------------------------------------------------
struct countvalue
{
  int*    count;
  double* value;
};
typedef std::map<std::string, countvalue> mapvars;

void initializeEvent(mapvars& vars)
{
  countvalue v;
  int count=0;
  
%(impl)s
}

//-----------------------------------------------------------------------------
// --- These structs can be filled by calling fillObjects()
// --- after the call to initializeEvents(...)
//-----------------------------------------------------------------------------
%(structdecl)s
%(structimpl)s

#endif
'''

MACRO=\
		  '''//-----------------------------------------------------------------------------
// File:        %(name)s.cc
// Description: event selector
// Created:     %(time)s by mkselector.py
// Author:      %(author)s
// $Revision: 1.3 $
//-----------------------------------------------------------------------------
#include "%(name)s.h"
//-----------------------------------------------------------------------------
using namespace std;

bool %(name)s(mapvars& vars)
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
		  '''#------------------------------------------------------------------------------
# Created: %(time)s
#------------------------------------------------------------------------------
name	:= %(name)s
AT		:= @
#------------------------------------------------------------------------------
CINT	:= rootcint
CXX		:= g++
LDSHARED:= g++
#------------------------------------------------------------------------------
DEBUG	:= -ggdb
CPPFLAGS:= -I. $(shell root-config --cflags)    
CXXFLAGS:= $(DEBUG) -pipe -O2 -fPIC -Wall
LDFLAGS := -shared 
#------------------------------------------------------------------------------
LIBS	:= $(shell root-config --glibs)
#------------------------------------------------------------------------------
linkdef	:= linkdef.h
header  := header.h
cintsrc	:= dict.cc
cintobj	:= dict.o

cppsrc 	:= $(name).cc
cppobj  := $(name).o

objects	:= $(cintobj) $(cppobj) 
library	:= lib$(name).so
$(shell rm -rf header.h linkdef.h)
#-----------------------------------------------------------------------
lib:	$(library)

$(library)	: $(objects)
	@echo "---> Linking $@"
	$(AT)$(LDSHARED) $(LDFLAGS) $+ $(LIBS) -o $@
	@rm -rf $(cintsrc) $(objects) $(linkdef) $(header)

$(cppobj)	: $(cppsrc)
	@echo "---> Compiling `basename $<`" 
	$(AT)$(CXX)	$(CXXFLAGS) $(CPPFLAGS) -c $<

$(cintobj)	: $(cintsrc)
	@echo "---> Compiling `basename $<`"
	$(AT)$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<

$(cintsrc) : $(header) $(linkdef)
	@echo "---> Generating dictionary `basename $@`"
	$(AT)$(CINT) -f $@ -c $(CPPFLAGS) $+

$(header)	: $(linkdef)
	@echo "---> Creating $(header)"
	@echo -e "#ifndef HEADER_H"  >  $(header)
	@echo -e "#define HEADER_H" >>	$(header)
	@echo -e "#include <map>" >>	$(header)
	@echo -e "#include <string>" >>	$(header)
	@echo -e "#include <vector>" >>	$(header)
	@echo -e "struct countvalue { int* count; double* value; };" >> $(header)
	@echo -e "bool selector(std::map<std::string, countvalue>&);" \
	>> $(header)
	@echo -e \"#endif" >> $(header)

$(linkdef)	:
	@echo "---> Creating $(linkdef)"
	@echo -e "#include <map>"  >	$(linkdef)
	@echo -e "#include <string>" >>	$(linkdef)
	@echo -e "#include <vector>" >>	$(linkdef)
	@echo -e "#include <$(header)>" >> $(linkdef)
	@echo -e "#ifdef __CINT__" >> $(linkdef)
	@echo -e "#pragma link off all globals;" >> $(linkdef)
	@echo -e "#pragma link off all classes;" >> $(linkdef)
	@echo -e "#pragma link off all functions;" >> $(linkdef)
	@echo -e "#pragma link C++ class countvalue+;" >> $(linkdef)
	@echo -e "#pragma link C++ class map<string, countvalue>+;" \
	>> $(linkdef)
	@echo -e "#pragma link C++ function selector;" >> $(linkdef)
	@echo -e "#endif" >> $(linkdef)

clean   :
	@rm -rf dict.* $(header) $(linkdef) $(objects)  $(library)
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

		# If this is a counter variable ignore it
		if iscounter: continue

		if rtype == "bool": rtype = "int"

		if count == 1:
			decl.append("%s\t%s;" % (rtype, varname))
			impl.append('  v = vars["%s"];' % branchname)
			impl.append('  assert(v.value!=0);')
			impl.append('  %s = *(v.value);' % varname)
			impl.append('')

		else:
			# this is a vector
			decl.append("std::vector<%s>\t%s(%d,0);" %(rtype, varname, count))
			impl.append('  v = vars["%s"];' % branchname)
			impl.append('  assert(v.value!=0); assert(v.count!=0);')
			impl.append('  count = *(v.count);')
			impl.append('  %s.resize(count);' % varname)
			impl.append('  copy(v.value, v.value+count, %s.begin());'% varname)
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
	
			
	# Create C++ codes

	names = {'name'  : filename,
			 'NAME'  : upper(filename),
			 'time'  : ctime(time()),
			 'class' : join("", records, ""),
			 'decl'  : join("", decl, "\n"),
			 'impl'  : rstrip(join("", impl, "\n")),
			 'author': AUTHOR,
			 's': '%s',
			 'structdecl': join("", structdecl, "\n"),
			 'structimpl': join("", structimpl, "\n")			 
			 }

	record = HEADER % names
	outfilename = "%(name)s.h" % names
	open(outfilename, "w").write(record)

	record = MACRO % names
	outfilename = "%(name)s.cc" % names
	open(outfilename, "w").write(record)

	record = COMPILE % names
	outfilename = "%(name)s.mk" % names
	open(outfilename, "w").write(record)

	print "\tdone!"
#------------------------------------------------------------------------------
main()

