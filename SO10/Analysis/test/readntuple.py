#!/usr/bin/env python
# -----------------------------------------------------------------------------
#  File: readntuple.py
#        Example of how to read ntuples created by Mkntuple
#  Created: 14-Feb-2010 Harrison B. Prosper
#$Revision:$
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
      
	mucharge = vint(MAXMUON, 0)
	mupt     = vfloat(MAXMUON, 0)
	mueta    = vfloat(MAXMUON, 0)
	muphi    = vfloat(MAXMUON, 0)
	isStandAlone = vint(MAXMUON, 0)

	#  Note use of wildcards
  
	stream.select("*Muon_muons.charge", mucharge)
	stream.select("*Muon_muons.pt",  mupt)
	stream.select("*Muon_muons.eta", mueta)
	stream.select("*Muon_muons.phi", muphi)
	stream.select("*Muon_muons.isStand*", isStandAlone)
	
	#  Book a few histograms

	kit.setStyle()
	
	cch  = kit.canvas("ch",  0, 500, 500)
	hch  = kit.histogram("hch", "charge", "", 40, -2, 2)

	cpt  = kit.canvas("pt",  4, 500, 500)
	hpt  = kit.histogram("hpt", "p_{T} [GeV/c]", "", 50, 0, 100)

	ceta = kit.canvas("eta", 8, 500, 500)
	heta = kit.histogram("heta", "#eta", "", 50, -2.5, 2.5)

	cphi = kit.canvas("phi", 12, 500, 500)
	hphi = kit.histogram("hphi", "#phi", "", 50, -5, 5)

	cn   = kit.canvas("n",   16, 500, 500)
	hn   = kit.histogram("hn", "muon multiplicity", "", 10, 0, 10)

	#  Loop over events

	nentries = stream.entries()

	print "Number of entries: %d" % nentries

	count = 0
	for entry in  xrange(nentries):

		stream.read(entry)

		nmu = mupt.size()
		if nmu < 1: continue
		count += 1

		#  Fill histograms for standalone muons

		n = 0
		for i in xrange(nmu):
			if isStandAlone[i] < 1: continue
			n += 1
			hch.Fill(mucharge[i])
			hpt.Fill(mupt[i])
			heta.Fill(mueta[i])
			hphi.Fill(muphi[i])
		hn.Fill(n)

		if count % 10 == 0:
			kit.plot(cch, hch)
			kit.plot(cpt, hpt)
			kit.plot(ceta,heta)
			kit.plot(cphi,hphi)
			kit.plot(cn,  hn)

	# close n-tuple file
	
	stream.close()

	kit.plot(cch,  hch,  "s")
	kit.plot(cpt,  hpt,  "s")
	kit.plot(ceta, heta, "s")
	kit.plot(cphi, hphi, "s")
	kit.plot(cn,   hn,   "s")

	sleep(10)
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

