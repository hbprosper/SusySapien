#!/usr/bin/env python
#---------------------------------------------------------------------------
# File:        findhdr.py
# Description: find header for given class using classmap.py
# Created:     04-Sep-2010 Harrison B. Prosper
#$Revision:$
#---------------------------------------------------------------------------
import os, sys, re
from ROOT import *
from string import *
from time import *
from glob import glob
from getopt     import getopt, GetoptError
#---------------------------------------------------------------------------
# Constants
#---------------------------------------------------------------------------
# Check for a CMSSW release
if not os.environ.has_key("CMSSW_RELEASE_BASE"):
	print "\t*** Please setup a CMSSW release\n"
	sys,exit(0)

if not os.environ.has_key("CMSSW_BASE"):
	print "\t*** Please setup a CMSSW release\n"
	sys,exit(0)
	
USAGE='''
Usage:
  findhdr.py <full-classname>

  uses classmap.py (created using mkclassmap.py)
'''
def usage():
	print USAGE
	sys.exit(0)

PWD = os.path.realpath(os.environ['PWD'])
BASE      = "%s/src" % os.environ["CMSSW_RELEASE_BASE"]
LOCALBASE = "%s/src" % os.environ["CMSSW_BASE"]
VERSION   = os.environ["CMSSW_VERSION"]
#------------------------------------------------------------------------------
# Determine project directory
project = split(replace(PWD, LOCALBASE + "/", ""), '/')
if len(project) < 2:
	print "\t**Please run findhdr.py in your subpackage directory"
	sys.exit(0)

PACKAGE, SUBPACKAGE = project[:2]
PROJECTBASE = "%s/%s/%s"   % (LOCALBASE, PACKAGE, SUBPACKAGE)
#------------------------------------------------------------------------------
# Load classmap.py
#------------------------------------------------------------------------------
# First try local release
cmd = 'find %s/%s/%s -name "classmap.py"' % (LOCALBASE, PACKAGE, SUBPACKAGE)
t = map(strip, os.popen(cmd).readlines())
if len(t) == 0:
	# try Mkntuple
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
def main():
	argv = sys.argv[1:]
	if len(argv) < 1:
		usage()
	classname = argv[0]
	if not ClassToHeaderMap.has_key(classname):
		return
	else:
		print ClassToHeaderMap[classname]
#------------------------------------------------------------------------------
main()










