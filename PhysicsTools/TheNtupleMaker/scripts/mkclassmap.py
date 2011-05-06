#!/usr/bin/env python
#---------------------------------------------------------------------------
# File:        mkclassmap.py
# Description: Create a map of classnames to headers
# Created:     26-Aug-2010 Harrison B. Prosper
#---------------------------------------------------------------------------
import os, sys, re
from ROOT import *
from string import *
from time import *
from glob import glob
from getopt     import getopt, GetoptError
from PhysicsTools.TheNtupleMaker.Lib import \
	 parseHeader,\
	 splitHeader,\
	 stripBlanklines,\
	 namespaceName,\
	 getClassname,\
	 cmsswProject
#---------------------------------------------------------------------------
# Constants
#---------------------------------------------------------------------------
PACKAGE, SUBPACKAGE, LOCALBASE, BASE, VERSION = cmsswProject()
if PACKAGE == None:
	print "Please run me in your sub-package directory"
	sys.exit(0)

PROJECTBASE = "%s/%s/%s"   % (LOCALBASE, PACKAGE, SUBPACKAGE)
PYTHONDIR   = "%s/python"   % PROJECTBASE

shortOptions = "Hu:"

USAGE='''
Usage:
  mkclassmap.py [options] [sub-package1 [sub-package2...]]

  options
  -u<sub-package-path>   Update classmap by scanning given sub-package-path

  example:
      mkclassmap.py -u$CMSSW_BASE/src/MyArea/MyAnalysis

  default sub-packages:
                DataFormats/* 
				SimDataFormats/*
				FWCore/Framework
				FWCore/FWLite
				FWCore/MessageLogger
				FWCore/ParameterSet
				FWCore/Utilities
				FWCore/Common
				PhysicsTools/TheNtupleMaker
'''
def usage():
	print USAGE
	sys.exit(0)
#------------------------------------------------------------------------------
# Decode command line
argv = sys.argv[1:]
try:
	opts, pkgs = getopt(argv, shortOptions)
except GetoptError, m:
	print
	print m
	usage()

Update = False
PKGBASE = BASE # default: scan $CMSSW_RELEASE_BASE
CLASSMAPFILE = '%s/classmap.py' % PYTHONDIR
subpkgs = []

for option, value in opts:
	if option == "-H":
		usage()
		
	elif option == "-u":
		Update = True
		subpkgs = split(value)
		if os.path.exists(CLASSMAPFILE):
			execfile(CLASSMAPFILE)
			PKGBASE = ""
		else:
			Update = False
			PKGBASE = LOCALBASE
			subpkgs = ["%s/%s", (PACKAGE, SUBPACKAGE)]
			
if len(subpkgs) > 0:
	SUBPACKAGELIST = subpkgs
else:
	SUBPACKAGELIST = ["DataFormats/*",
					  "SimDataFormats/*",
					  "FWCore/Framework",
					  "FWCore/Common",
					  "FWCore/Utilities",
					  "FWCore/FWLite",
					  "FWCore/MessageLogger",
					  "FWCore/ParameterSet",
					  "PhysicsTools/TheNtupleMaker"
					  ]
#----------------------------------------------------------------------------
# subsystems to ignore

skipsubsystem = re.compile('Alignment|'\
						   'Geometry|'\
						   'Histograms|'\
						   'Provenance|'\
						   'Road|'\
						   'StdDict|'\
						   'Streamer|'\
						   'TestObj|'\
						   'VZero|'\
						   'Wrapped')

skipheader = re.compile('(classes|Fwd|print).h$')
stripnamespace = re.compile('^[a-zA-Z]+::')
#----------------------------------------------------------------------------
def addToMap(fullkey, key, header, cmap):
	if cmap.has_key(fullkey):
		fullkey2 = "%s*" % fullkey
		if not cmap.has_key(fullkey2):
			cmap[fullkey2] = [cmap[fullkey]]
		cmap[fullkey2].append(header)
	else:
		cmap[fullkey] = header

	if cmap.has_key(key):
		key2 = "%s*" % key
		if not cmap.has_key(key2):
			cmap[key2] = [cmap[key]]
		cmap[key2].append(header)
	else:
		cmap[key] = header
#============================================================================
# Main Program
#============================================================================
def main():
	
	print "mkclassmap.py $Revision: 1.9 $\n"

	subpackagelist = SUBPACKAGELIST
	filelist = []
	if Update:
		for subpackage in subpackagelist:
			print "scan sub-package: %s" % subpackage
			file = "%s/interface/*.h" % subpackage
			hlist = glob(file)
			hlist.sort()
			filelist += hlist
	else:

		for subpackage in subpackagelist:
			package, subpkg = split(subpackage,'/')
			if subpkg == "*":

				cmd = "%s%s" % (LOCALBASE, subpackage)
				subsystems = filter(lambda x: os.path.isdir(x), glob(cmd))
				if len(subsystems) == 0:
					cmd = "%s%s" % (PKGBASE, subpackage)
					subsystems = filter(lambda x: os.path.isdir(x), glob(cmd))
					
				subsystems = map(lambda x: split(x, '/').pop(), subsystems)
			else:
				subsystems = [split(subpackage, '/').pop()]

			for subsystem in subsystems:
				if skipsubsystem.match(subsystem) != None: continue
				
				dirpath = "%s%s/%s" % (PKGBASE, package, subsystem)
				if not os.path.exists(dirpath):
					dirpath = "%s%s/%s" % (LOCALBASE, package, subsystem)
					if not os.path.exists(dirpath):
						"** directory %s not found" % dirpath
						continue
				print "scan sub-package: %s/%s" % (package, subsystem)
				file = "%s/interface/*.h" % dirpath
				hlist = glob(file)
				hlist.sort()
				filelist += hlist

	# Filter headers
	filelist = filter(lambda x: skipheader.search(x) == None, filelist)
	
	#-------------------------------------------------
	# Loop over header files to be scanned
	#-------------------------------------------------
	cmap = {}
	count = 0
	for index, file in enumerate(filelist):
		if not os.path.exists(file):
			print "** file %s not found" % file
			continue
		
		file = os.path.abspath(file)
		
		# Scan header and parse it for classes
		
		record, items = parseHeader(file)
		if record == '': continue
		records = splitHeader(record)
		if len(records) == 0: continue

		# Now strip away path up to "/src/" in pathname of header
		
		header = file
		k = rfind(header, "/src/") # search from right
		if k > 0: header = header[k+5:]
		
		names = []
		for irecord, (record, group, start, end) in enumerate(records):

			# Get actual record from items map

			key = strip(record)
			if items.has_key(key):
				record = items[key]
				if type(record) == type(()):
					record, extraRecord = record
			record = stripBlanklines(record)
			
			if group == "namespace":
				name = strip(namespaceName(record))
				if name != '': names.append(name)

			elif group == "endnamespace":
				if len(names) > 0: names.pop()
				
			elif group in ["endclass", "endstructclass"]:
				
				fullname = joinfields(names, "::")

				# Check for uninstantiated templates and
				# create keys

				if find(fullname, '<') > -1:
					tplate = True
					fullkey = split(fullname, '<')[0]
				else:
					tplate = False
					fullkey = fullname					
				key = stripnamespace.sub("", fullkey)
				
				if Update:
					addToMap(fullkey, key, header, ClassToHeaderMap)
				else:
					addToMap(fullkey, key, header, cmap)

				count += 1
				print "%5d\t%s" % (count, fullkey)
								
				names.pop()
				
			elif group in ["class", "structclass"]:
				classname, basenames, template = getClassname(record)
				names.append(classname)

	# Write out class to header map

	if Update:
		print "updating classmap.py..."
		hmap = ClassToHeaderMap
	else:
		print "creating classmap.py..."
		hmap = cmap

	recs = []
	keys = hmap.keys()
	keys.sort()
	for key in keys:
		value = hmap[key]
		if type(value) == type(""):
			recs.append("'%s': '%s'" % (key, value))
		else:
			recs.append("'%s': %s"   % (key, value))

	record = joinfields(recs,',\n')		
	outfile = CLASSMAPFILE
	out = open(outfile,'w')
	out.write('# Created: %s\n' % ctime(time()))
	out.write('# Version: %s\n' % VERSION)
	out.write('#$Revision: 1.9 $\n')
	out.write("ClassToHeaderMap = {\\\n")
	out.write(record+'\n')
	out.write("}\n")
#---------------------------------------------------------------------------
main()










