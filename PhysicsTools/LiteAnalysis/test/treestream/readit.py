#!/usr/bin/env python
#-----------------------------------------------------------------------------
# Filename: readit.py
# Description: read CMSSW branches
# Created:  31-Oct-2009 HBP
#-----------------------------------------------------------------------------
import os, sys
from sys import exit
from ROOT import *
from time import *
from PhysicsTools.LiteAnalysis import *
#-----------------------------------------------------------------------------
FILENAME= "zee_pat.root"
GENPT="GenParticle*pt"
#-----------------------------------------------------------------------------
def main():

	print "\n\t\t*** readit ***\n"
	
	stream = itreestream(FILENAME, "Events")
	if not stream.good(): fatal("unable to open file: %s" % FILENAME)

	nevent = stream.size()
	print "\n\nNumber of events: %d" % nevent
	
	gpt = vdouble(2000)
	stream.select(GENPT, gpt);
	if not stream.good(): fatal("unable to get branch %s" % GENPT)

	print "enter event loop"
	c  = 0.0
	t0 = time()
	for event in stream:
		c += gpt.size()
	dt = time() - t0
	rate = nevent/dt
	c /= nevent
	
	print "Read rate:              %10.1f events/sec" % rate
	print "Average # GenParticles: %10.1f" % c
	
	
main()
