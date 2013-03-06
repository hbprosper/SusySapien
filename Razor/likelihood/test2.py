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

	nbins = len(nuis)
	print "nbins:", nbins

	# set all sigmas to constant zero
	for i in range(nbins):	     
		bin = "_%3.3d" % i		     
		wspace.var('sigma%s' % bin).setConstant()

	# set all variables constant except 'variable'
	variable = 'N0'
	for v in ['B0', 'N0', 'R0', 'btot', 'MR0']:
		if v == variable: continue
		wspace.var(v).setConstant()
	
	x = wspace.var(variable)
	xframe = x.frame()
	model.plotOn(xframe)
	xframe.Draw()

	# block (quit using Root Gui)
	gApplication.Run()
	
	#model.fitTo(data)
	sys.exit(0)
	
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

	nIters  = 100000
	nChains = 2
	rooInterface.MCMCSetNIterationsRun(nIters)
	rooInterface.MCMCSetNChains(nChains)
	# Don't fill histograms of sampled values. This should save some time
	rooInterface.MCMCSetFlagFillHistograms(False)

	msglevel = RooMsgService.instance().globalKillBelow()
	RooMsgService.instance().setGlobalKillBelow(RooFit.FATAL)
	rooInterface.MarginalizeAll()
	output.Close();
#------------------------------------------------------------------
try:
	main()
except KeyboardInterrupt:
	print
	print "bye cruel world!"
	print
	


