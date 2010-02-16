#!/usr/bin/env python
#------------------------------------------------------------------------------
# File: rlist.py
# Description: list contents of a simple root file
# Created: 06-Dec-2005 Harrison B. Prosper
#------------------------------------------------------------------------------
import os, sys
from ROOT import *
from PhysicsTools.LiteAnalysis.AutoLoader import *
#------------------------------------------------------------------------------
if len(sys.argv) < 2:
    print '''
    Usage:
          rlist.py <root-file-name> [tree-name]
    '''
    sys.exit(0)

filename = sys.argv[1]
if not os.path.exists(filename):
    print "can't find %s" % filename
    sys.exit(0)

print
if len(sys.argv) > 2:
	treename = sys.argv[2]
	tuple = itreestream(filename, treename)
	tuple.ls()
	tuple.close()
else:
	tuple = itreestream(filename)
	tuple.ls()
	tuple.close()
