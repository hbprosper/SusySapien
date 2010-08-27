#!/usr/bin/env python
#------------------------------------------------------------------------------
# create plugins.cc
# Created: 05-Jan-2010 Harrison B. Prosper
# Updated: 15-Feb-2010 HBP - run mkclasslist.py if needed
#$Revision: 1.9 $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time import *
from PhysicsTools.LiteAnalysis.boostlib import nameonly
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

out  = open("plugins.cc","w")
record = '''// ----------------------------------------------------------------------------
// Created: %(time)s by mkplugins.py
// $Revision: 1.9 $
// ----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
'''
out.write(record % names)
record ='''
#include "FWCore/Framework/interface/Event.h"
typedef Buffer<edm::Event, true> edmEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmEvent_t, "edmEvent");\n'''
out.write(record)

count = 0
for ctype, name in cnames:
	# Find associated header
	if not ClassToHeaderMap.has_key(name):
		print "\t**can't find header for\n\t%s" % name
		continue
	
	header = ClassToHeaderMap[name]	
	bname = hasnspace.sub("", name)	

	count += 1
	print "%5d\t%s" % (count, name)

	# singleton or not, which is it?
	if ctype in ['s','S']:
		ctype = 'true'
	else:
		ctype = 'false'
		
	names['buffername'] = bname
	names['classname']  = name
	names['singleton']  = ctype
	c = split(name, "::")
	namespace = c[0]
	shortname = c[-1]
	
	header = '#include "%s"' % header	
	names['header'] = header
	record = '''
%(header)s
typedef Buffer<%(classname)s, %(singleton)s> %(buffername)s_t;
DEFINE_EDM_PLUGIN(BufferFactory, %(buffername)s_t,
                  "%(buffername)s");\n''' % names
	out.write(record)
out.close()

