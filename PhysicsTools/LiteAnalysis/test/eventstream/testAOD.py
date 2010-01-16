#!/usr/bin/env python
#-----------------------------------------------------------------------------
# Filename: testAOD.py
# Description:
# Created:  15-Sep-2008 HBP
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
	print "\n\ttestAOD\n"

	filename = "zee.root"

	print "\topening file %s\n" % filename
	stream = EventStream(filename)
	if not stream.good(): fatal("unable to open file")

	nevent = stream.size()
	print "\tnumber of events: %d\n" % nevent

	electrons = vector("reco::GsfElectron")()
	stream.select(electrons)

	jets = vector("reco::CaloJet")()
	stream.select(jets, "iterativeCone")

	genparticles = vector("reco::GenParticle")()
	stream.select(genparticles)

	print "%4s\t%10s%10s%10s" % ("Row", "Npart", "Nele", "Njet")
	
	for (row, stat) in enumerate(stream):

		print "%4d\t%10d%10d%10d" % (row,
									 genparticles.size(),
									 electrons.size(),
									 jets.size())
		if row > 99: break
#---------------------------------------------------------------------------
main()




