#!/usr/bin/env python
# -----------------------------------------------------------------------------
#  File: readrecontuple.py
#        Example of how to read ntuples created by Mkntuple
#  Created: 14-Feb-2010 Harrison B. Prosper
#$Revision: 1.2 $
# -----------------------------------------------------------------------------
import os, sys, re
from ROOT import *
from time import sleep
from PhysicsTools.LiteAnalysis.AutoLoader import *
# -----------------------------------------------------------------------------
MAXTRACK=500
MAXMUON=50
MAXJET=50
def error(message):
	print "** error ** %s" % message
	sys.exit(0)
# -----------------------------------------------------------------------------
def readntuple(filename="ntuple.root"):

	#  Open file
  
	stream = itreestream(filename, "Events")
	if not stream.good(): error("unable to read " + filename)
      
	#  Define variables to be read in

	gevent = gEvent()

	#  Note use of wildcards

  	stream.select("*genP*.pdgId",  gevent.pdgId)
	stream.select("*genP*.status", gevent.status)
	stream.select("*genP*.pt",     gevent.pt)
	stream.select("*genP*.eta",    gevent.eta)
	stream.select("*genP*.phi",    gevent.phi)
	stream.select("*genP*.mass",   gevent.mass)
	stream.select("*genP*.charge", gevent.charge)

  	stream.select("*genP*PdgId",   gevent.motherpdgId)
	stream.select("*genP*Status",  gevent.motherstatus)
	stream.select("*genP*Pt",      gevent.motherpt)
	stream.select("*genP*Eta",     gevent.mothereta)
	stream.select("*genP*Phi",     gevent.motherphi)
	stream.select("*genP*Mass",    gevent.mothermass)

	muIsolsumPt = vfloat(MAXMUON, 0);
	stream.select("*muons*isol*sumPt", muIsolsumPt);
	
	#  Loop over events

	nentries = stream.entries()

	print "Number of entries: %d" % nentries

	nentries = 10

	for entry in  xrange(nentries):

		stream.read(entry)

		gevent.update()

		print "Event: %d" % entry
		print gevent.table()
		print

	# close n-tuple file
	
	stream.close()
# -----------------------------------------------------------------------------
def main():

	# Get n-tuple filename
	
	argv = sys.argv[1:]
	if len(argv) < 1:
		filename = "ntuple.root"
	else:
		filename = argv[0]

	if not os.path.exists(filename):
		error("can't find file %s" % filename)

	readntuple(filename)
# -----------------------------------------------------------------------------
main()

