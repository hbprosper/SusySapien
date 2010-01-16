#!/usr/bin/env python
#-----------------------------------------------------------------------------
# Filename: prbranches.py
# Description: Print CMSSW branches
# Created:  26-Oct-2007 HBP
#-----------------------------------------------------------------------------
import os, sys
from ROOT import *
from PhysicsTools.LiteAnalysis import *
#-----------------------------------------------------------------------------
FILENAME= "zee.root"
#-----------------------------------------------------------------------------
def main():

    print "\n\tprbranches"
    print "\n\topen file: %s\n" % FILENAME
    
    stream = EventStream(FILENAME)
    if not stream.good(): fatal("unable to open file: %s" % FILENAME)
    print

    out = open("CMSBranches.txt","w")
    names = stream.branchnames()
    out.write("CMS Branches: %d\n" % names.size())
    for i, name in enumerate(names):
        out.write("%4d\t%s\n" % (i, name))
    out.close()
#---------------------------------------------------------------------------
main()




