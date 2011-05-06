#!/usr/bin/env python
#------------------------------------------------------------------------------
# Create the skeleton of a user plugin
# Created: 27-Aug-2010 Harrison B. Prosper
#$Revision: 1.2 $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time import *
from glob import glob
from getopt     import getopt, GetoptError
from PhysicsTools.TheNtupleMaker.Lib import \
	 nameonly, \
	 cmsswProject, \
	 getauthor
#------------------------------------------------------------------------------
PACKAGE, SUBPACKAGE, LOCALBASE, BASE, VERSION = cmsswProject()
if PACKAGE == None:
	print "Please run me in a sub-package directory:"
	print "  $CMSSW_BASE/src/<package>/<sub-package>"
	sys.exit(0)
if SUBPACKAGE == None:
	print "Please run me in a sub-package directory"
	print "  $CMSSW_BASE/src/<package>/<sub-package>"
	sys.exit(0)
#------------------------------------------------------------------------------
print "Package:     %s" % PACKAGE
print "Sub-package: %s" % SUBPACKAGE

PROJECTBASE = "%s/%s/%s"   % (LOCALBASE, PACKAGE, SUBPACKAGE)
PLUGINDIR = "%s/plugins"   % PROJECTBASE  
SRCDIR    = "%s/src"       % PROJECTBASE
INCDIR    = "%s/interface" % PROJECTBASE
AUTHOR    = getauthor()
#------------------------------------------------------------------------------
# Make sure these directories exist
if not os.path.exists(PLUGINDIR):
	print "\t** directory:\n\t** %s not found!" % PLUGINDIR
	sys.exit(0)
if not os.path.exists(SRCDIR):
	print "\t** directory:\n\t** %s not found!" % SRCDIR
	sys.exit(0)
if not os.path.exists(INCDIR):
	print "\t** directory:\n\t** %s not found!" % INCDIR
	sys.exit(0)
#------------------------------------------------------------------------------
# Load classmap.py
#------------------------------------------------------------------------------
# First try local release
cmd = 'find %s/%s/%s -name "classmap.py"' % (LOCALBASE, PACKAGE, SUBPACKAGE)
t = map(strip, os.popen(cmd).readlines())
if len(t) == 0:
	# try TheNtupleMaker
	cmd = 'find %s/PhysicsTools/TheNtupleMaker -name "classmap.py"' % LOCALBASE
	t = map(strip, os.popen(cmd).readlines())
	if len(t) == 0:
		print "\n\t** unable to locate classmap.py"\
		  "\t** try running mkclassmap.py to create it"
		sys.exit(0)

mapfile = t[0]
try:
	execfile(mapfile)
except:
	print "\n\t** unable to load classmap.py"
	sys.exit(0)
#------------------------------------------------------------------------------
namespace = re.compile(r'^[a-zA-Z]+::')
doublecolon = re.compile(r'::')
#------------------------------------------------------------------------------
shortOptions = "u"
def usage():
	print '''
Usage:
	mkuserplugin.py [options] <CMSSW class> s|c [postfix, default=Helper]

	s = singleton  (at most one  object  per event)
	c = collection (zero or more objects per event)

	options
	-u    undo effect of most recent call to mkuserplugin.py (experimental!)
	      This option does not need the other arguments
	'''
	sys.exit(0)
#------------------------------------------------------------------------------
def wrpluginheader(names):
	template_header = '''#ifndef %(headername)s_H
#define %(headername)s_H
//-----------------------------------------------------------------------------
// Package:     %(package)s
// Sub-Package: %(subpackage)s
// Description: TheNtupleMaker helper class for %(classname)s
// Created:     %(time)s
// Author:      %(author)s      
//$Revision: 1.2 $
//-----------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include "PhysicsTools/TheNtupleMaker/interface/HelperFor.h"
%(header)s
//-----------------------------------------------------------------------------
// Note: The following variables are automatically defined and available to
//       all methods:
//         1. config          pointer to global ParameterSet object
//         2. event           pointer to the current event
//         3. object          pointer to the current helper object
//         4. oindex          index of current helper object
//         5. index           index of item(s) returned by helper 
//         6. count           count per helper object (default = 1)
//       Items 1-6 are initialized by TheNtupleMaker. The count can be changed from
//       its default value of 1 by the helper. However, items 1-5 should not
//       be changed.
//-----------------------------------------------------------------------------
'''
	template_nonamespace = '''
/// A helper for %(classname)s.
class %(name)s : public HelperFor<%(classname)s>
{
public:
  ///
  %(name)s();

  virtual ~%(name)s();

  // Uncomment if helper class does some event-level analysis
  // virtual void analyzeEvent();

  // Uncomment if helper class does some object-level analysis
  // virtual void analyzeObject();
  
  // -- Access Methods

  // -- e.g., int pt() const;
  
private:
  // -- internals
};
#endif
'''
	template_withnamespace = '''
namespace %(namespace)s
{
  /// A helper class for %(classname)s.
  class %(name)s : public HelperFor<%(classname)s>
  {
  public:
	///
	%(name)s();

	virtual ~%(name)s();//////

	// Uncomment if this class does some event-level analysis
	// virtual void analyzeEvent();
	 
	// Uncomment if this class does some object-level analysis
	// virtual void analyzeObject();
  
	// -- Access Methods

	// -- e.g., int pt() const;
	 
  private:
    // -- internals
  };
}
#endif
'''
	filename = "%(incdir)s/%(filename)s.h" % names
	if names['namespace'] == '':
		record = template_header + template_nonamespace
	else:
		record = template_header + template_withnamespace
	record = record % names

	if os.path.exists(filename):
		redofile = "%(incdir)s/.redo.%(filename)s.h" % names
		os.system("cp %s %s" % (filename, redofile))
	else:
		undofile = "%(incdir)s/.undo.%(filename)s.h" % names
		open(undofile,'w').write('\n')
		
	out  = open(filename, "w")
	out.write(record)
	out.close()
#------------------------------------------------------------------------------
def wrplugincode(names):
	template = '''//-----------------------------------------------------------------------------
// Package:     %(package)s
// Sub-Package: %(subpackage)s
// Description: TheNtupleMaker helper class for %(classname)s
// Created:     %(time)s
// Author:      %(author)s      
//$Revision: 1.2 $
//-----------------------------------------------------------------------------
#include "%(package)s/%(subpackage)s/interface/%(filename)s.h"
//-----------------------------------------------------------------------------
using namespace std;
%(usingnamespace)s
//-----------------------------------------------------------------------------
// This constructor is called once per job
%(name)s::%(name)s()
  : HelperFor<%(classname)s>() {}
    
%(name)s::~%(name)s() {}

// -- Called once per event
//void %(name)s::analyzeEvent()
//{
//
//}

// -- Called once per object
//void %(name)s::analyzeObject()
//{
//
//}

// -- Access Methods
//double %(name)s::someMethod(...)  const
//{
//  return  //-- some-value --
//}
''' % names

	
	filename = "%(srcdir)s/%(filename)s.cc" % names
	if os.path.exists(filename):
		redofile = "%(srcdir)s/.redo.%(filename)s.cc" % names
		os.system("cp %s %s" % (filename, redofile))
	else:
		undofile = "%(srcdir)s/.undo.%(filename)s.cc" % names
		open(undofile,'w').write('\n')
		
	out  = open(filename, "w")
	out.write(template)
	out.close()
#------------------------------------------------------------------------------
def wrplugin(names):	
	template = '''// ----------------------------------------------------------------------------
// Created: %(time)s by mkuserplugin.py
// Author:      %(author)s      
//$Revision: 1.2 $
// ----------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/UserBuffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
#include "%(package)s/%(subpackage)s/interface/%(filename)s.h"
typedef UserBuffer<%(classname)s, %(fullname)s, %(ctype)s>
%(buffername)s_t;
DEFINE_EDM_PLUGIN(BufferFactory, %(buffername)s_t,
                  "%(buffername)s");\n''' % names

	undofile = "%(plugindir)s/.undo.userplugin_%(filename)s.cc" % names
	open(undofile,'w').write('\n')
	
	filename = "%(plugindir)s/userplugin_%(filename)s.cc" % names
	out  = open(filename, "w")
	out.write(template)
	out.close()
#------------------------------------------------------------------------------
def undo():
	names = {}
	names['subpackage'] = SUBPACKAGE
	names['plugindir']  = PLUGINDIR
	names['srcdir']     = SRCDIR
	names['incdir']     = INCDIR

	# undo userplugin
	t = glob("%(plugindir)s/.undo.userplugin_*.cc" % names)
	if len(t) > 0:
		undofile = replace(t[0], '.undo.', '')
		print "delete %s" % undofile
		os.system('rm -rf %s; rm -rf %s' % (undofile, t[0]))

	# redo plugins/BuildFile
	t = glob("%(plugindir)s/.redo.BuildFile" % names)
	if len(t) > 0:
		redofile = replace(t[0], '.redo.', '')
		print "restore %s" % redofile
		os.system('mv %s %s; rm -rf' % (t[0], redofile))

	# redo src/classes.h
	t = glob("%(srcdir)s/.redo.classes.h" % names)
	if len(t) > 0:
		redofile = replace(t[0], '.redo.', '')
		print "restore %s" % redofile
		os.system('mv %s %s' % (t[0], redofile))

	# redo src/classes_def.xml
	t = glob("%(srcdir)s/.redo.classes_def.xml" % names)
	if len(t) > 0:
		redofile = replace(t[0], '.redo.', '')
		print "restore %s" % redofile
		os.system('mv %s %s' % (t[0], redofile))
				
	# undo user plugin source
	t = glob("%(srcdir)s/.undo.*.cc" % names)
	if len(t) > 0:
		undofile = replace(t[0], '.undo.', '')
		print "delete %s" % undofile
		os.system('rm -rf %s; rm -rf %s' % (undofile, t[0]))

	# redo user plugin source
	t = glob("%(srcdir)s/.redo.*.cc" % names)
	if len(t) > 0:
		redofile = replace(t[0], '.redo.', '')
		print "restore %s" % redofile
		os.system('mv %s %s' % (t[0], redofile))

	# undo user plugin header
	t = glob("%(incdir)s/.undo.*.h" % names)
	if len(t) > 0:
		undofile = replace(t[0], '.undo.', '')
		print "delete %s" % undofile
		os.system('rm -rf %s; rm -rf %s' % (undofile, t[0]))

	# redo user plugin header
	t = glob("%(incdir)s/.redo.*.h" % names)
	if len(t) > 0:
		redofile = replace(t[0], '.redo.', '')
		print "restore %s" % redofile
		os.system('mv %s %s' % (t[0], redofile))

	cmd = '''
	cd plugins
	rm -rf BuildFile.xml
	scram b -c
	cd ..
	'''
	os.system(cmd)
	sys.exit(0)
#------------------------------------------------------------------------------
def main():
	# Decode command line
	argv = sys.argv[1:]
	try:
		opts, vars = getopt(argv, shortOptions)
	except GetoptError, m:
		print
		print m
		usage()

	for option, value in opts:
		if option == "-H":
			usage()
		elif option == "-u":
			undo()

	# No options, check other arguments
	argc = len(argv)
	if argc < 2:
		usage()

	classname = argv[0]
	ctype = argv[1]
	if len(ctype) != 1:
		usage()

	if not ctype in ['s','c', 'S', 'C']:
		usage()
	
	if argc > 2:
		postfix = argv[2]
	else:
		postfix = "Helper"

	# Find header associated with class
	if ClassToHeaderMap.has_key(classname):
		header = ClassToHeaderMap[classname]
	else:
		header = raw_input('Enter header for class "%s" starting at src: ' %
						   classname)
		if header == "": sys.exit(0)
	if type(header) == type([]): header = header[0]
	
	# Check that header exists
	filename = "%s/%s" % (BASE, header)
	if not os.path.exists(filename):
		print "\t** can't find %s" % filename
		sys.exit(0)
	
	# Objects of this type can be singletons or form collections
	if ctype in ['s', 'S']:
		ctype = "true"
	else:
		ctype = "false"

	# Create names of helper class and associated buffer plugin
	nspace = namespace.findall(classname)
	if len(nspace) == 0:
		nspace = ""
		nspacewithcolon = ""
	else:
		nspacewithcolon = nspace[0]
		nspace = doublecolon.sub("", nspacewithcolon)

	name  = namespace.sub("",classname) + postfix      # remove namespace
	bname = doublecolon.sub("", classname) + postfix   # remove "::"
	filename = doublecolon.sub("", bname)
	if nspacewithcolon != "":
		fullname = nspacewithcolon + name
	else:
		fullname = name
		
	print "class:  %s" % classname
	print "header: %s\n" % header
	print "\thelper class:  %s" % nspacewithcolon+name
	print "\thelper buffer: %s" % bname
	print "\thelper code:   src/%s.cc" % filename
	print "\t               interface/%s.h" % filename
	print "\t               plugins/userplugin_%s.cc" % filename
	print

	names = {}
	names['filename']   = filename
	names['time']       = ctime(time())
	names['ctype']      = ctype
	names['name']       = name
	names['fullname']   = fullname
	names['headername'] = upper(bname)
	names['classname']  = classname
	
	names['namespace']  = nspace
	if nspace != "":
		usingnspace = "using namespace %(namespace)s;" % names
	else:
		usingnspace = ""
	names['usingnamespace']  = usingnspace
	
	names['buffername'] = bname		
	names['header']     = '#include "%s"' % header
	names['package']    = PACKAGE
	names['subpackage'] = SUBPACKAGE
	names['plugindir']  = PLUGINDIR
	names['srcdir']     = SRCDIR
	names['incdir']     = INCDIR
	names['author']     = AUTHOR
	#------------------------------------------------------------------------
	wrpluginheader(names)
	wrplugincode(names)
	wrplugin(names)

	#------------------------------------------------------------------------
	# Update BuildFile in plugins directory
	#------------------------------------------------------------------------
	updated = False
	buildfile = "%s/BuildFile" % PLUGINDIR
	redofile  = "%s/.redo.BuildFile" % PLUGINDIR
	if os.path.exists(buildfile):
		os.system("cp %s %s" % (buildfile, redofile))
	else:
		updated = True
		out = open(buildfile, 'w')
		record = '''<use name=PhysicsTools/TheNtupleMaker>
<use name=FWCore/FWLite>
<use name=FWCore/PluginManager>

<use name=boost_regex>
<use name=boost_python>
<use name=rootrflx>
<use name=rootminuit>
<use name=rootmath>
<use name=f77compiler>
'''
		out.write(record)
		out.close()

		
	pkg = joinfields(split(header, '/')[:2],'/')
	record = open(buildfile).read()
	
	# Add package/sub-package required for helper

	if find(record, pkg) < 0:
		updated = True
		record += "\n<use name=%s>\n" % pkg

	pkg = '%(package)s/%(subpackage)s' % names
	if find(record, pkg) < 0:
		updated = True
		record += "\n<use name=%s>\n" % pkg
		
	if find(record, filename) < 0:
		updated = True
		record += "<library file=userplugin_%(filename)s.cc "\
				  "name=%(filename)s>\n"\
				  "<flags EDM_PLUGIN=1>\n"\
				  "</library>\n" % names
	if updated:
		open(buildfile, 'w').write(record)
		print "\tupdated:       plugins/BuildFile"

	#------------------------------------------------------------------------
	# Update classes.h
	#------------------------------------------------------------------------
	updated = False
	classesfile = "%s/classes.h" % SRCDIR
	redofile = "%s/.redo.classes.h" % SRCDIR
	
	if os.path.exists(classesfile):
		os.system("cp %s %s" % (classesfile, redofile))
	else:
		updated = True
		out = open(classesfile, 'w')
		record ='''//$Revision: 1.2 $
//--------------------------------------------------------------------''' % \
		names
		out.write(record)
		out.close()
		os.system("cp %s %s" % (classesfile, redofile))

	record = open(classesfile).read()

	# update includes
	
	getincludes = re.compile("#include .*(?=\n)", re.M)
	includes = getincludes.findall(record)
	header = '%(package)s/%(subpackage)s/interface/%(filename)s.h' % names
	
	if len(includes) == 0:
		updated = True
		record += '\n#include "%s"' % header
	else:
		recincs  = joinfields(includes, '\n')
		if find(recincs, header) < 0:
			updated = True
			newrec = '%s\n#include "%s"\n' % (includes[-1], header)
			record = replace(record, includes[-1], newrec)

	# update namespace
	
	if find(record, "namespace") < 0:
		record += "\nnamespace\n{\n}\n"
		
	gettemplates = re.compile("(?<={)[^}]+", re.M)
	rectemps     = gettemplates.findall(record)[0]
	template = "HelperFor<%(classname)s>" % names
	if find(rectemps, template) < 0:
		updated = True
		newrec = "  %s t_%s;\n}" % (template,
										names['buffername'])
		record = replace(record, "}", newrec)
	
	if updated:
		open(classesfile,'w').write(record)
		print "\tupdated:       src/classes.h"

	#------------------------------------------------------------------------
	# Update classes_def.xml
	#------------------------------------------------------------------------
	updated = False
	classesfile = "%s/classes_def.xml" % SRCDIR
	redofile = "%s/.redo.classes_def.xml" % SRCDIR
	if os.path.exists(classesfile):
		os.system("cp %s %s" % (classesfile, redofile))
	else:
		updated = True
		out = open(classesfile, 'w')
		record ='''<lcgdict>
</lcgdict>
		'''
		out.write(record)
		out.close()
		os.system("cp %s %s" % (classesfile, redofile))
		
	record = open(classesfile).read()
	if find(record, fullname) < 0:
		updated = True
		rec = ' <class name="HelperFor<%(classname)s>"/>\n'
		rec +=' <class name="%(fullname)s"/>\n</lcgdict>'
		rec = rec % names
		record = replace(record, '</lcgdict>', rec)

	if updated:
		open(classesfile,'w').write(record)
		print "\tupdated:       src/classes_def.xml"


	cmd = '''
	cd plugins
	rm -rf BuildFile.xml
	scram b -c
	cd ..
	'''
	os.system(cmd)
	
#------------------------------------------------------------------------------
main()
