#!/usr/bin/env python
#------------------------------------------------------------------------------
# File: setupMkntuple.py
# Description: 1. call mkdocs.py (in test directory)
#              2. call mkclasslist.py to create classes.txt
#              3. call mkplugins.py
# Created: Sat Mar 13 2010 Harrison B. Prosper
#------------------------------------------------------------------------------
import os, sys
from string import *
#------------------------------------------------------------------------------
if not os.environ.has_key("CMSSW_BASE"):
	print "** you need to set up a local CMSSW release"
	sys.exit(0)
	
BASE = os.environ["CMSSW_BASE"]
MKNTUPLE_DIR = "%s/src/PhysicsTools/Mkntuple" % BASE
if not os.path.exists(MKNTUPLE_DIR):
	print "** you can't do much without Mkntuple!"
	sys.exit(0)
	
TEST_DIR = "%s/test" % MKNTUPLE_DIR
PLUGINS_DIR = "%s/plugins" % MKNTUPLE_DIR
#------------------------------------------------------------------------------
def main():
	argv = sys.argv[1:]
	if len(argv) == 0:
		print "\nUsage:\n"\
			  "\tpython scripts/setupMkntuple.py <reco-file> <pat-file>\n"\
			  "\n\tThe files are needed to create a list of classes and "\
			  "getByLabels\n"\
			  "\tusing edmDumpEventContent\n"
		sys.exit(0)
	rootfiles = joinfields(argv, ' ')
	
	# First copy python scripts to bin/$SCRAM_ARCH
	print "\n\tStep 1. - copy python scripts to bin area"
	cmd = "cp $CMSSW_BASE/src/PhysicsTools/Mkntuple/scripts/*.py "\
		  "$CMSSW_BASE/bin/$SCRAM_ARCH"
	os.system(cmd)
	
	if not os.path.exists("%s/map" % TEST_DIR):
		print "\n\tStep 2. - call mkdocs.py to create directories" \
			  "\n\t\t\ttxt"\
			  "\n\t\t\thtml"\
			  "\n\t\t\tmap"\
			  "\n\t\t\tmethods"		
		cmd = '''
		cd $CMSSW_BASE/src/PhysicsTools/Mkntuple/test
		mkdocs.py'''
		print cmd

	if not os.path.exists("%s/classes.txt" % PLUGINS_DIR):
		print "\n\tStep 3. - call mkclasslist.py to create file" \
			  "\n\t\t\tclasses.txt"
		cmd = '''
		cd $CMSSW_BASE/src/PhysicsTools/Mkntuple/plugins
		mkclasslist.py %s''' % rootfiles
		print cmd
		
	if not os.path.exists("%s/plugins.cc" % PLUGINS_DIR):
		print "\n\tStep 4. - call mkplugins.py to create file" \
			  "\n\t\t\tplugins.cc"
		cmd = '''
		cd $CMSSW_BASE/src/PhysicsTools/Mkntuple/plugins
		mkplugins.py'''
		print cmd

	print "\n\n\t** now do scramv1 b clean, followed by scramv1 b\n"
#------------------------------------------------------------------------------
main()
