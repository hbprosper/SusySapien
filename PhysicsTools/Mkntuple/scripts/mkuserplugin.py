#!/usr/bin/env python
#------------------------------------------------------------------------------
# Create the skeleton of a user plugin
# Created: 27-Aug-2010 Harrison B. Prosper
#$Revision:$
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time import *
from PhysicsTools.LiteAnalysis.boostlib import nameonly
#------------------------------------------------------------------------------
if not os.environ.has_key("CMSSW_RELEASE_BASE"):
	print "\t**you must first set up CMSSW"
	sys.exit(0)
BASE = "%s/src" % os.environ['CMSSW_RELEASE_BASE']

if not os.environ.has_key("CMSSW_BASE"):
	print "\t**you must first set up CMSSW"
	sys.exit(0)
LOCALBASE = "%s/src" % os.environ['CMSSW_BASE']
PWD = os.environ['PWD']
#------------------------------------------------------------------------------
# Determine project directory
project = split(replace(PWD, LOCALBASE + "/", ""), '/')
if len(project) < 2:
	print "\t**must run mkuserplugins in a subpackage directory"
	print "\t**Example: Mkntuple is a subpackage of PhysicsTools"
	sys.exit(0)

PACKAGE, SUBPACKAGE = project[:2]
PROJECTBASE = "%s/%s/%s"   % (LOCALBASE, PACKAGE, SUBPACKAGE)
PLUGINDIR = "%s/plugins"   % PROJECTBASE  
SRCDIR    = "%s/src"       % PROJECTBASE
INCDIR    = "%s/interface" % PROJECTBASE
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
cmd = 'find %s/PhysicsTools/Mkntuple -name "classmap.py"' % LOCALBASE
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
def usage():
	print '''
Usage:
	mkuserplugin.py <CMSSW class-name> s|c [post-fix, default=Helper]

	s = singleton  (zero or one  object  per event)
	c = collection (zero or more objects per event)
	'''
	sys.exit(0)
#------------------------------------------------------------------------------
def wrpluginheader(names):
	template_header = '''#ifndef %(headername)s_H
#define %(headername)s_H
//-----------------------------------------------------------------------------
// Package:     %(package)s
// Sub-Package: %(subpackage)s
// Description: Mkntuple helper class for %(classname)s
// Created:     %(time)s
// Author:      
//$Revision:$
//-----------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include "PhysicsTools/Mkntuple/interface/CurrentEvent.h"
#include "PhysicsTools/Mkntuple/interface/user.h"
%(header)s
//-----------------------------------------------------------------------------
'''
	template_nonamespace = '''
/// A helper for %(classname)s.
class %(name)s : public HelperFor<%(classname)s>
{
public:
  %(name)s();
  ///
  %(name)s(const edm::Event& e);

  virtual ~(name)s();

  // Uncomment if this class has a customized cache method
  // void cache(const %(classname)s& o);
  
  // -- Access Methods
  // -- These are the accessors available to the user.
  // -- Note: access methods must be declared const
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
    %(name)s();   
    ///
    %(name)s(const edm::Event& o);

    virtual ~%(name)s();

    // Uncomment if this class has a customized cache method
    // void cache(const %(classname)s& o);
  
    // -- Access Methods
    // -- These are the accessors available to the user.
    // -- Note: access methods must be declared const
    // -- e.g., int pt() const;
  
  private:
    // -- internals
  };
}
#endif
'''	
	filename = "%(incdir)s/%(name)s.h" % names
	if names['namespace'] == '':
		record = template_header + template_nonamespace
	else:
		record = template_header + template_withnamespace
	record = record % names
		
	out  = open(filename, "w")
	out.write(record)
	out.close()
#------------------------------------------------------------------------------
def wrplugincode(names):
	template = '''//-----------------------------------------------------------------------------
// Package:     %(package)s
// Sub-Package: %(subpackage)s
// Description: Mkntuple helper class for %(classname)s
// Created:     %(time)s
// Author:      
//$Revision:$
//-----------------------------------------------------------------------------
#include "%(package)s/%(subpackage)s/interface/%(filename)s.h"
//-----------------------------------------------------------------------------
using namespace std;
%(usingnamespace)s
//-----------------------------------------------------------------------------
%(name)s::%(name)s() {}
  
%(name)s::%(name)s(const edm::Event& e)
  : HelperFor<%(classname)s>(e) {}
    
%(name)s::~%(name)s() {}

// -- Write customized cache method if default in HelperFor is inadequate
//void %(name)s::cache(const %(classname)s& o)
//{
//  object = &o; // cache a pointer to object to be helped
//  count  = 1;  // change as appropriate
//
//  -- some code --
//
//  return  -- some-value --
//}

// -- Access Methods
//double %(name)s::someMethod(...)  const
//{
//  -- some-code --
//  return  -- some-value --
//}
''' % names
	filename = "%(srcdir)s/%(filename)s.cc" % names
	out  = open(filename, "w")
	out.write(template)
	out.close()
#------------------------------------------------------------------------------
def wrplugin(names):	
	template = '''// ----------------------------------------------------------------------------
// Created: %(time)s by mkuserplugin.py
//$Revision:$
// ----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "%(package)s/%(subpackage)s/interface/%(filename)s.h"
typedef UserBuffer<%(classname)s, %(fullname)s, %(ctype)s>
%(buffername)s_t;
DEFINE_EDM_PLUGIN(BufferFactory, %(buffername)s_t,
                  "%(buffername)s");\n''' % names
	
	filename = "%(plugindir)s/userplugin_%(filename)s.cc" % names
	out  = open(filename, "w")
	out.write(template)
	out.close()
#------------------------------------------------------------------------------
def main():
	argv = sys.argv[1:]
	argc = len(argv)
	if argc < 2:
		usage()

	classname = argv[0]
	ctype = argv[1]
	if argc > 2:
		postfix = argv[1]
	else:
		postfix = "Helper"

	# Find header associated with class
	if ClassToHeaderMap.has_key(classname):
		header = ClassToHeaderMap[classname]
	else:
		header = raw_input('Enter header for class "%s" starting at src: ' %
						   classname)
		if header == "":
			sys.exit(0)

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
	filename = doublecolon.sub("", name)
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

	#------------------------------------------------------------------------
	wrpluginheader(names)
	wrplugincode(names)
	wrplugin(names)

	#------------------------------------------------------------------------
	# Update BuildFile in plugins directory
	#------------------------------------------------------------------------
	updated = False
	buildfile = "%s/BuildFile" % PLUGINDIR
	if not os.path.exists(buildfile):
		updated = True
		out = open(buildfile, 'w')
		record = '''<use name=PhysicsTools/LiteAnalysis>
<use name=PhysicsTools/Mkntuple>
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
	if find(record, filename) < 0:
		updated = True
		record += "<library file=userplugin_%(filename)s.cc "\
				  "name=userplugin_%(filename)s>\n"\
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
	if not os.path.exists(classesfile):
		updated = True
		out = open(classesfile, 'w')
		record ='''$Revision:$
%(header)s
		'''
		out.write(record)
		out.close()
	record = open(classesfile).read()
	header = '%(package)s/%(subpackage)s/interface/%(filename)s.h' % names
	if find(record, header) < 0:
		updated = True
		record +='#include "%s"\n' % header

	if updated:
		open(classesfile,'w').write(record)
		print "\tupdated:       src/classes.h"

	#------------------------------------------------------------------------
	# Update classes_def.xml
	#------------------------------------------------------------------------
	updated = False
	classesfile = "%s/classes_def.xml" % SRCDIR
	if not os.path.exists(classesfile):
		updated = True
		out = open(classesfile, 'w')
		record ='''<lcgdict>
</lcgdict>
		'''
		out.write(record)
		out.close()
	record = open(classesfile).read()
	if find(record, fullname) < 0:
		updated = True
		rec = ' <class name="%(fullname)s"/>\n</lcgdict>' % names
		record = replace(record, '</lcgdict>', rec)

	if updated:
		open(classesfile,'w').write(record)
		print "\tupdated:       src/classes_def.xml"
#------------------------------------------------------------------------------
main()
