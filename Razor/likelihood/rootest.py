#!/usr/bin/env python
#------------------------------------------------------------------
# Test RooRazor2DBackground - Feb. 2013 HBP & SS
#------------------------------------------------------------------
import os,sys,time
from ROOT import *
from random import uniform
#------------------------------------------------------------------
def main():

	gSystem.Load("libRooRazor2DBackground.so")
	
	# Make the workspace:
	wspace = RooWorkspace('wspace')

	wspace.factory('MR0[200]')
	wspace.factory('R0[0.01]')
	wspace.factory('B0[3.141]')
	wspace.factory('N0[2.0]')

	wspace.factory('MR[755, 756]')
	wspace.factory('R[0.05,  0.06]')
	
	wspace.factory('Razor2DBackground::bapprox(MR, R, MR0, R0, B0, N0)')
	wspace.factory('Razor2DBackground::bexact(MR, R, MR0, R0, B0, N0, 1)')

	wspace.Print()
	
	# Change parameters randomly and compare approximate and exact
	# integrals

	MR0 = wspace.var('MR0')
	R0  = wspace.var('R0')

	bapprox  = wspace.function("bapprox")
	bexact   = wspace.function("bexact")
	
	print "="*58
	for i in xrange(10):
		mr0 = uniform(200, 600)
		MR0.setVal(mr0)
		
		r0 = uniform(0.01, 0.05)
		R0.setVal(r0)
				   
		print "%5d\tMR0 = %f\tR0 = %f" % (i+1, mr0, r0) 
		print "\t\tapprox: %10.4e"\
			  "\texact:  %10.4e" % (bapprox.getVal(), bexact.getVal())

#------------------------------------------------------------------
try:
	main()
except KeyboardInterrupt:
	print
	print "bye cruel world!"
	print
	


