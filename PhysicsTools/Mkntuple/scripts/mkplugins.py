#!/usr/bin/env python
#------------------------------------------------------------------------------
# create plugins.cc
# Created: 05-Jan-2010 Harrison B. Prosper
# Updated: 15-Feb-2010 HBP - run mkclasslist.py if needed
#          01-May-2011 HBP - split plugin definitions into separate files
#$Revision: 1.15 $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time import *
from PhysicsTools.Mkntuple.Lib import nameonly
#------------------------------------------------------------------------------
if not os.environ.has_key("CMSSW_BASE"):
	print "\t**you must first set up CMSSW"
	sys.exit(0)
LOCALBASE = "%s/src" % os.environ['CMSSW_BASE']

# Load classmap.py

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
isVector  = re.compile(r'(?<=^vector\<).+(?=\>)')
hasnspace = re.compile(r'::')
#------------------------------------------------------------------------------
def exclass(x):
	m = isVector.findall(x)
	if len(m) == 0:
		x = strip(x)
		return x
	else:
		x = strip(m[0])
		return x
#------------------------------------------------------------------------------
# If classes.txt does not exit, run mkclasslist.py to create it.
#------------------------------------------------------------------------------
# Make sure we are in the plugins directory
localdir = split(os.environ["PWD"],"/").pop()
if localdir != "plugins":
	print "\t** mkplugins.py should be run from the plugins directory!"
	sys.exit(0)
	
if not os.path.exists("classes.txt"):
	print "** required file classes.txt not found.\n"\
		  "** I'll try to create it...but for that you should"
	rootfile = raw_input("** enter the names of a reco file and a pat file: ")
	for filename in split(rootfile):
		if not os.path.exists(filename):
			print "\nfile %s not found...try again!" % filename
			sys.exit(0)
	
	os.system("mkclasslist.py %s" % rootfile)
	
if not os.path.exists("classes.txt"):
	print """
	** mkclasselist.py failed to create classes.txt...
	** try running mkclasslist.py by hand
	"""
	sys.exit(0)
#------------------------------------------------------------------------------
records = map(strip, open("classes.txt").readlines())
cnames = map(lambda x: (x[0], exclass(strip(x[1:]))), records)
cnames.sort()

names = {'time': ctime(time())}

buildfile = open("BuildFile").read()

count = 0
for index, (ctype, name) in enumerate(cnames):
	# Find associated header
	if not ClassToHeaderMap.has_key(name):
		#print "\t**can't find header for\n\t%s" % name
		continue
	
	header = ClassToHeaderMap[name]
	if type(header) == type([]): header = header[0]
	bname = hasnspace.sub("", name)	

	# singleton or not, which is it?
	if ctype in ['s','S']:
		ctype = 'true'
	else:
		ctype = 'false'
		
	names['buffername'] = bname
	names['classname']  = name
	names['singleton']  = ctype
	shortname = replace(name, "::", "")
	names['shortname']  = shortname

	# special processing for Event class
	if shortname == "edmEvent":
		names['bufferheader'] = "BufferEvent"
	else:
		names['bufferheader'] = "Buffer"
		
	pluginname = "plugin_%(shortname)s.cc" % names
	
	count += 1
	print "%5d\t%s" % (count, pluginname)
	
	out  = open(pluginname, "w")
	record = \
'''// -------------------------------------------------------------------------
// File::   plugin_%(shortname)s.cc
// Created: %(time)s by mkplugins.py
// $Revision: 1.15 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/%(bufferheader)s.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
'''
	out.write(record % names)
	
	header = '#include "%s"' % header	
	names['header'] = header
	record = '''%(header)s

typedef Buffer<%(classname)s, %(singleton)s> %(buffername)s_t;
DEFINE_EDM_PLUGIN(BufferFactory, %(buffername)s_t,
                  "%(buffername)s");\n''' % names
	out.write(record)
	out.close()

	# Add an entry to BuildFile if one does not yet exist
	if find(buildfile, pluginname) < 0:
		rec = \
'''
<library file=plugin_%(shortname)s.cc
         name=%(shortname)s>
  <flags EDM_PLUGIN=1>
</library>''' % names
		buildfile += rec
	
open("BuildFile", "w").write(buildfile)
os.system("rm -rf BuildFile.xml; scram b -c")

