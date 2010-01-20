#!/usr/bin/env python
#------------------------------------------------------------------------------
# create plugins.cc
# Created: 05-Jan-2010 Harrison B. Prosper
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time import *
from boostlib import nameonly
#------------------------------------------------------------------------------
if not os.environ.has_key("CMSSW_BASE"):
	print "\t**you must first set up CMSSW"
	sys.exit(0)
	
getclass  = re.compile(r'(?<=[0-9] ).+$',re.M)
isVector  = re.compile(r'(?<=^vector\<).+(?=\>)')
hasnspace = re.compile(r'::')
MDIR      = "%s/src/PhysicsTools/LiteAnalysis/dataformats/methods" % \
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
# Make sure we are in the plugins directory
localdir = split(os.environ["PWD"],"/").pop()
if localdir != "bin":
	print "\t** mkplugins.py should be run from the bin directory!"
	sys.exit(0)
	
if not os.path.exists("adapters.txt") or \
   not os.path.exists("classes.txt") or \
   not os.path.exists("headers.txt"):
	print "** required files adapters.txt, etc. not found.\n"\
		  "** I'll try to create them...but for that you should"
	rootfile = raw_input("** enter a root file name: ")
	if not os.path.exists(rootfile):
		print "\nfile %s not found...try again!" % rootfile
		sys.exit(0)

	# run mkclasslist.py
	
	os.system("mkclasslist.py %s" % rootfile)
	
if not os.path.exists("adapters.txt") or \
   not os.path.exists("classes.txt") or \
   not os.path.exists("headers.txt"):
	print """
	** mkclasselist.py failed to create required files...
	** try running mkclasslist.py separately
	"""
	sys.exit(0)
#------------------------------------------------------------------------------
adapters  = map(lambda x: nameonly(x), open("adapters.txt").readlines())
admap = {}
for i, x in enumerate(adapters): admap[x]=i
cnames   = map(exclass,
			   getclass.findall(open("classes.txt").read()))
cnames.sort()

headers  = open("headers.txt").read()

names = {'time': ctime(time())}

out  = open("plugins.cc","w")
record = '''// ----------------------------------------------------------------------------
// Created: %(time)s by mkplugins.py
// $Revision: 1.1.2.1 $
//
// Define all Buffer plugins. Need to use typedefs otherwise the
// DEFINE macro gets confused by classes with multiple template
// arguments.
// ----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
'''
out.write(record % names)

count = 0
for name in cnames:
	
	bname = hasnspace.sub("", name)	

	# Check for a methods file
	methodsfile = "%s/a%s.txt" % (MDIR, bname)
	if not os.path.exists(methodsfile): continue

	count += 1
	print "%5d\t%s" % (count, name)

	names['buffername'] = bname
	names['class'] = name
	names['typedef'] = "%s_t" % bname
	c = split(name, "::")
	namespace = c[0]
	shortname = c[-1]

	# Look for associated header file
	
	getheader = re.compile(r'DataFormats/.+/%s\.h$' % shortname,re.M)
	h = getheader.findall(headers)
	if len(h) == 0:
		print "\t*** warning - can't find header for class %s\n" % name
		continue

	elif len(h) == 1:
		header = h[0]
		
	else:
		print "\t*** warning - multiple header matches for class %s" % \
			  shortname
		#print h

		# Match pat to Pat and reco to Reco

		header = ""
		if namespace == "pat":
			for x in h:
				if find(x, '/Pat') > -1:
					header = x
					break
		elif namespace == "reco":
			for x in h:
				if find(x, '/Pat') > -1: continue
				if find(x, 'Reco') > -1:
					header = x
					break
				header = x
		if header != "":
			print "\t   *** chosen: %s" % header

	if header == "":
		print "\t*** warning - "\
			  "can't decide which header for class %s" % name
		continue
	
	header = '#include "%s"' % header	
	aname = "a%s" % bname
	if admap.has_key(aname):
		names['adapterclass'] = aname
		header += '\n#include '\
				  '"PhysicsTools/LiteAnalysis/dataformats/adapters/%s.h"' % \
				  aname
	else:
		names['adapterclass'] = name

	names['header'] = header
	record = '''
%(header)s
typedef Buffer<%(class)s, %(adapterclass)s> %(typedef)s;
DEFINE_EDM_PLUGIN(BufferFactory, %(typedef)s, "%(buffername)s");\n''' % \
			names
	out.write(record)
out.close()

