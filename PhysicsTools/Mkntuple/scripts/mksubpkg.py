#!/usr/bin/env python
#------------------------------------------------------------------------------
# Create the skeleton of a user sub-pacakge
# Created: 03-Sep-2010 Harrison B. Prosper
#$Revision: 1.2 $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time import *
from PhysicsTools.LiteAnalysis.boostlib import nameonly, getauthor
#------------------------------------------------------------------------------
PACKAGE, SUBPACKAGE, LOCALBASE, BASE, VERSION = cmsswProject()
if PACKAGE == None:
	print "Please run mkdocs.py in your package directory"
	sys.exit(0)
	
print "Package:     %s" % PACKAGE
AUTHOR = getauthor()
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
	mkdir -p test
	''' % names
	os.system(cmd)
#------------------------------------------------------------------------------
main()
