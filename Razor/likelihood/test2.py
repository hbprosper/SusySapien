#!/usr/bin/env python
#------------------------------------------------------------------
# Test BAT/RooFit interface 03-Mar-2013 HBP & SS
#------------------------------------------------------------------
import os,sys
from ROOT import *
#------------------------------------------------------------------
def main():

	gSystem.Load("libRooRazor2DBackground.so")
	gSystem.Load("libBAT")
	rootFilename = "Boris.root"
	workspaceName= "binnedrazor"

	# ----------------------------------------------
	# Get objects needed for rooInterface
	# ----------------------------------------------
	rfile = TFile(rootFilename)
	wspace = rfile.Get(workspaceName)
	
	data  = wspace.data('data')
	prior = wspace.pdf('prior')
	model = wspace.pdf('model')
	nuis  = wspace.set('nuis_s')
	pois  = wspace.set('nuis_b')

	for name in ['data', 'model', 'prior', 'nuis', 'pois']:
		print "\n\t<== %s ==>" % name
		exec('%s.Print()' % name)

	# ----------------------------------------------
	# set up BAT/RooFit interface
	# ----------------------------------------------
	BCAux.SetStyle()
        # open log file with default level of logging
	BCLog.OpenLog("bat.log")
	BCLog.SetLogLevel(BCLog.detail)

	rooInterface = BCRooInterface()
	rooInterface.Initialize(data, model, prior, nuis, pois)

	output = BCModelOutput(rooInterface, "batman.root");
	output.WriteMarkovChain()

	nMCMC = 100000
	rooInterface.MCMCSetNIterationsRun(nMCMC)
	sys.exit(0)
	
	rootInterface.MarginalizeAll()
	output.Close();
#------------------------------------------------------------------
try:
	main()
except KeyboardInterrupt:
	print
	print "bye cruel world!"
	print
	


