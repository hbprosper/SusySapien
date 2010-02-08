#!/usr/bin/env python
#------------------------------------------------------------------------------
# create plugins.cc
# Created: 05-Jan-2010 Harrison B. Prosper
#$Revision: 1.3 $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time import *
from boostlib import nameonly
from classmap import ClassToHeaderMap
#------------------------------------------------------------------------------
if not os.environ.has_key("CMSSW_BASE"):
	print "\t**you must first set up CMSSW"
	sys.exit(0)
	
getclass  = re.compile(r'(?<=[0-9] ).+$',re.M)
isVector  = re.compile(r'(?<=^vector\<).+(?=\>)')
hasnspace = re.compile(r'::')
TXTDIR    = "%s/src/PhysicsTools/LiteAnalysis/dataformats/txt" % \
			os.environ["CMSSW_BASE"]
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
# If adapters.txt, objects.txt and headers.txt do not exist, run
# mkobjectlist.py first.
#------------------------------------------------------------------------------
# Make sure we are in the bin directory
localdir = split(os.environ["PWD"],"/").pop()
if localdir != "plugins":
	print "\t** mkplugins.py should be run from the plugins directory!"
	sys.exit(0)
	
if not os.path.exists("classes.txt"):
	print "** required file classes.txt not found.\n"\
		  "** I'll try to create them...but for that you should"
	rootfile = raw_input("** enter a root file name: ")
	if not os.path.exists(rootfile):
		print "\nfile %s not found...try again!" % rootfile
		sys.exit(0)

	# run mkclasslist.py
	
	os.system("mkclasslist.py %s" % rootfile)
	
if not os.path.exists("classes.txt"):
	print """
	** mkclasselist.py failed to create required files...
	** try running mkclasslist.py separately
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
// $Revision: 1.3 $
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

