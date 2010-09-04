#!/usr/bin/env python
#------------------------------------------------------------------------------
# Create the skeleton of a user sub-pacakge
# Created: 03-Sep-2010 Harrison B. Prosper
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
PWD = os.path.realpath(os.environ['PWD'])

# Get author's name
getauthor = re.compile(r'(?<=[0-9]:)[A-Z]+.+?(?=:/)')
record = strip(os.popen("getent passwd $USER").read())
if record != "":
	AUTHOR = getauthor.findall(record)[0]
else:
	AUTHOR = "Shakepeare's ghost"
#------------------------------------------------------------------------------
# Determine package directory
project = split(replace(PWD, LOCALBASE + "/", ""), '/')
if len(project) < 1:
	print "\t**Please run mksubpkg.py in your package directory"
	sys.exit(0)

PACKAGE = project[0]
print "Package:     %s" % PACKAGE
#------------------------------------------------------------------------------
def usage():
	print '''
Usage:
	mksubpkg.py <sub-package-name>
	'''
	sys.exit(0)
#------------------------------------------------------------------------------
def main():
	argv = sys.argv[1:]
	argc = len(argv)
	if argc < 1:
		usage()

	SUBPACKAGE = argv[0]
	print "Sub-Package: %s" % SUBPACKAGE
	print "Author:      %s" % AUTHOR
	
	names = {}
	names['pkg']    = PACKAGE
	names['subpkg'] = SUBPACKAGE
	names['author'] = AUTHOR
	names['mkntuple'] = 'PhysicsTools/Mkntuple'
	names['lib'] = 'PhysicsToolsMkntuple'
	cmd = '''
	mkdir -p %(subpkg)s
	cd %(subpkg)s
	sed -e "s/%(lib)s/%(pkg)s%(subpkg)s/g" $CMSSW_BASE/src/%(mkntuple)s/BuildFile  > BuildFile 
	mkdir -p interface
	mkdir -p python
	cp $CMSSW_BASE/src/PhysicsTools/Mkntuple/python/classmap.py python
	mkdir -p plugins
	grep "<use name" $CMSSW_BASE/src/%(mkntuple)s/plugins/BuildFile > plugins/BuildFile
	echo "<use name=%(pkg)s/%(subpkg)s>" >> plugins/BuildFile
	mkdir -p src
	''' % names
	os.system(cmd)
#------------------------------------------------------------------------------
main()
