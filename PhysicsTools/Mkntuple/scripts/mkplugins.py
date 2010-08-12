#!/usr/bin/env python
#------------------------------------------------------------------------------
# create plugins.cc
# Created: 05-Jan-2010 Harrison B. Prosper
# Updated: 15-Feb-2010 HBP - run make docs if needed
#$Revision: 1.7 $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time import *
from PhysicsTools.LiteAnalysis.Lib import nameonly
#------------------------------------------------------------------------------
if not os.environ.has_key("CMSSW_BASE"):
	print "\t**you must first set up CMSSW"
	sys.exit(0)
BASE = os.environ['CMSSW_BASE']

# Load classmap.py

cmd = 'find %s -name "classmap.py"' % BASE
t = map(strip, os.popen(cmd).readlines())
if len(t) == 0:
	print "\n\t** unable to locate classmap.py"\
		  "\t** try running mkdocs.py in the test directory"
	sys.exit(0)

mapfile = t[0]
try:
	execfile(mapfile)
except:
	print "\n\t** unable to load classmap.py"
	sys.exit(0)
#------------------------------------------------------------------------------
getclass  = re.compile(r'(?<=[0-9] ).+$',re.M)
isVector  = re.compile(r'(?<=^vector\<).+(?=\>)')
hasnspace = re.compile(r'::')
TXTDIR    = "%s/txt" % os.environ["PWD"]
#------------------------------------------------------------------------------
def exclass(x):
	m = isVector.findall(x)
	if len(m) == 0:
		x = strip(x)
		#print "class(%s)" % x
		return x
	else:
		x = strip(m[0])
		#print "class(%s)" % x
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
cnames   = map(exclass,
			   getclass.findall(open("classes.txt").read()))
cnames.sort()

names = {'time': ctime(time())}

out  = open("plugins.cc","w")
record = '''// ----------------------------------------------------------------------------
// Created: %(time)s by mkplugins.py
// $Revision: 1.7 $
// ----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
'''
out.write(record % names)

count = 0
for name in cnames:

	# Find associated header
	if not ClassToHeaderMap.has_key(name):
		print "\t**can't find header for\n\t%s" % name
		continue
	
	header = ClassToHeaderMap[name]
	
	bname = hasnspace.sub("", name)	

	count += 1
	print "%5d\t%s" % (count, name)

	names['buffername'] = bname
	names['classname']  = name

	c = split(name, "::")
	namespace = c[0]
	shortname = c[-1]
	
	header = '#include "%s"' % header	
	names['header'] = header
	record = '''
%(header)s
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<%(classname)s>,
                  "%(buffername)s");\n''' % names
	out.write(record)
out.close()

