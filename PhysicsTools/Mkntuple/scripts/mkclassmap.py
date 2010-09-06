#!/usr/bin/env python
#---------------------------------------------------------------------------
# File:        mkclassmap.py
# Description: Create a map of classnames to headers
# Created:     26-Aug-2010 Harrison B. Prosper
#$Revision: 1.3 $
#---------------------------------------------------------------------------
import os, sys, re
from ROOT import *
from string import *
from time import *
from glob import glob
from getopt     import getopt, GetoptError
from PhysicsTools.LiteAnalysis.boostlib import \
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
				FWCore/Utilities
				FWCore/Common
				PhysicsTools/LiteAnalysis
				PhysicsTools/Mkntuple
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
					  "PhysicsTools/Mkntuple",
					  "PhysicsTools/LiteAnalysis"
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
#============================================================================
# Main Program
#============================================================================
def main():
	
	print "mkclassmap.py $Revision: 1.3 $\n"

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

				# Check for uninstantiated templates

				if find(fullname, '<') > -1:
					tplate = True
					key = split(fullname, '<')[0]
				else:
					tplate = False
					key = fullname
					
				if Update:
					ClassToHeaderMap[key] = header
					ClassToHeaderMap[fullname] = header
				else:
					cmap[key] = header
					cmap[fullname] = header

				count += 1
				print "%5d\t%s" % (count, fullname)
								
				names.pop()
				
			elif group in ["class", "structclass"]:
				classname, basenames, template = getClassname(record)
				names.append(classname)

	# Write out class to header map

	recs = []
	if Update:
		print "updating classmap.py..."
		keys = ClassToHeaderMap.keys()
		keys.sort()
		for fullname in keys:
			recs.append("'%s': '%s'" % (fullname, ClassToHeaderMap[fullname]))
	else:
		print "creating classmap.py..."
		keys = cmap.keys()
		keys.sort()
		recs = []
		for fullname in keys:
			recs.append("'%s': '%s'" % (fullname, cmap[fullname]))		

	record = joinfields(recs,',\n')		
	outfile = CLASSMAPFILE
	out = open(outfile,'w')
	out.write('# Created: %s\n' % ctime(time()))
	out.write('# Version: %s\n' % VERSION)
	out.write('#$Revision: 1.3 $\n')
	out.write("ClassToHeaderMap = {\\\n")
	out.write(record+'\n')
	out.write("}\n")
#---------------------------------------------------------------------------
main()










