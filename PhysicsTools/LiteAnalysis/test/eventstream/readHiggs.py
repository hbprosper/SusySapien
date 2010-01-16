#!/usr/bin/env python
#-----------------------------------------------------------------------------
# Filename: readHiggs.py
# Description:
# Created:  15-Sep-2009 HBP
#-----------------------------------------------------------------------------
import os
import sys
from sys import exit
from ROOT import *
from math import *
from string import *
from PhysicsTools.LiteAnalysis import *
#-----------------------------------------------------------------------------
# Constants
#-----------------------------------------------------------------------------
def main():
	print "\n\treadHiggs\n"

	filename = "higgs.root"

	print "\topening file %s\n" % filename
	stream = EventStream(filename)
	if not stream.good(): fatal("unable to open file")

	nevent = stream.size()
	print "\tnumber of events: %d\n" % nevent

	electrons = vector("pat::Electron")()
	stream.select(electrons)

	muons = vector("pat::Muon")()
	stream.select(muons)

	jets = vector("pat::Jet")()
	stream.select(jets)

	mets = vector("pat::MET")()
	stream.select(mets)
	
	print "%4s\t%4s\t%4s\t%4s" % ("Row", "Njet", "Nele", "Nmu")
	
	for (row, stat) in enumerate(stream):

		print "%4d\t%4d\t%4d\t%4d" % (row,
									  jets.size(),
									  electrons.size(),
									  muons.size())
		if row >= 99: break
#---------------------------------------------------------------------------
main()




