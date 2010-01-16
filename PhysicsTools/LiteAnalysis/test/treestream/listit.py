#!/usr/bin/env python
#-----------------------------------------------------------------------------
# Filename: listit.py
# Description: list CMSSW branches
# Created:  31-Oct-2009 HBP
#-----------------------------------------------------------------------------
import os, sys
from sys import exit
from ROOT import *
from time import *
from PhysicsTools.LiteAnalysis import *
#-----------------------------------------------------------------------------
FILENAME= "zmumu_pat.root"
#-----------------------------------------------------------------------------
def main():
	stream = itreestream(FILENAME, "Events")
	if not stream.good(): fatal("unable to open file: %s" % FILENAME)
	stream.ls()
main()
