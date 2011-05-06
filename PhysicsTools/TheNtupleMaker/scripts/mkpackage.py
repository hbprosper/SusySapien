#!/usr/bin/env python
#------------------------------------------------------------------------------
# Create the skeleton of a user package
# Created: 03-Sep-2010 Harrison B. Prosper
#$Revision: 1.1.1.1 $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time import *
from PhysicsTools.TheNtupleMaker.Lib import \
	 nameonly, getauthor, cmsswProject
#------------------------------------------------------------------------------
#               CMS jargon
# PACAKGE    -> SUBSYSTEM
# SUBPACKAGE -> PACKAGE
#
PACKAGE, SUBPACKAGE, LOCALBASE, BASE, VERSION = cmsswProject()
if PACKAGE == None:
	print " The CMS directory structure is"
	print ""
	print "      $CMSSW_BASE/src/<subsystem>/<package>"
	print ""
	print "First create a <subsystem> directory in $CMSSW_BASE/src:"
	print "      mkdir <subsystem>"
	print 
	print "Then:"
	print "      cd <subsystem>"
	print ""
	print "      mkpackage.py <package>"
	sys.exit(0)
	
print "Subsystem:     %s" % PACKAGE
AUTHOR = getauthor()
#------------------------------------------------------------------------------
def usage():
	print '''
Usage:
	mkpackage.py <package>
	'''
	sys.exit(0)
#------------------------------------------------------------------------------
def main():
	argv = sys.argv[1:]
	argc = len(argv)
	if argc < 1:
		usage()

	SUBPACKAGE = argv[0]
	print "Package: %s" % SUBPACKAGE
	print "Author:      %s" % AUTHOR
	
	names = {}
	names['pkg']    = PACKAGE
	names['subpkg'] = SUBPACKAGE
	names['prog']   = lower(PACKAGE) + lower(SUBPACKAGE)
	names['author'] = AUTHOR
	names['mkntuple'] = 'PhysicsTools/TheNtupleMaker'
	names['lib'] = 'PhysicsToolsTheNtupleMaker'
	
	cmd = '''
	mkdir -p %(subpkg)s
	cd %(subpkg)s
	sed -e "s/%(lib)s/%(pkg)s%(subpkg)s/g" $CMSSW_BASE/src/%(mkntuple)s/BuildFile  > .b
	sed -e "s|/UtilAlgos>|/UtilAlgos>\\n<use name=%(mkntuple)s>|g" .b > BuildFile
	rm -rf .b
	
	mkdir -p interface
	mkdir -p python
	cp $CMSSW_BASE/src/PhysicsTools/TheNtupleMaker/python/classmap.py python
	mkdir -p plugins
	grep "<use name" $CMSSW_BASE/src/%(mkntuple)s/plugins/BuildFile > plugins/BuildFile
	echo "<use name=%(pkg)s/%(subpkg)s>" >> plugins/BuildFile
	mkdir -p src
	mkdir -p test
	sed -e "s/PhysicsTools.TheNtupleMaker/%(pkg)s.%(subpkg)s/g" $CMSSW_BASE/src/%(mkntuple)s/test/config.py  > test/config.py
	mkdir -p bin
	sed -e "s/plugins_t/%(prog)s/g" $CMSSW_BASE/src/%(mkntuple)s/bin/BuildFile   | egrep -v "test" > bin/BuildFile
 	echo "int main(int argc, char** argv) {return 0;}" > bin/%(prog)s.cc

	rm -rf BuildFile.xml
	scram b -c

	cd plugins
	rm -rf BuildFile.xml
	scram b -c
	cd ..

	cd bin
	rm -rf BuildFile.xml
	scram b -c
	cd ..
	''' % names
	os.system(cmd)
#------------------------------------------------------------------------------
main()
