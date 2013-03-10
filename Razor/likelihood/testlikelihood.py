#!/usr/bin/env python
#------------------------------------------------------------------
# Test likelihood function 05-Mar-2013 HBP & SS
#------------------------------------------------------------------
import os,sys
from ROOT import *
from math import *
from random import uniform
#------------------------------------------------------------------
nearlyZero     = 1.0e-300
nearlyInfinite = 1.0e+300

def poisson(N, a):
	if a <= 0:
		return 0.0
	elif N == 0:
		return exp(-a)
	else:
		p = 1.0
		z = exp(-a/N)
		for i in xrange(1,N+1):
			p *= z * a/i
		return p
	
def gamma(a, x):
	return TMath.Gamma(a) * ROOT.Math.inc_gamma_c(a,x)
  
def gfun(MR, Rsq, MR0, R0, B0, N0):
	y = abs((MR-MR0)*(Rsq-R0))
	z = B0 * N0 * y**(1.0/N0)
	return gamma(N0, z)

def razor2DBackground(MR, Rsq, MR0, R0, B0, N0):
	xmin, xmax = MR
	ymin, ymax = Rsq

	print "b2 xmn xmx ymn ymx: ", xmin, xmax, ymin, ymax
	print "X0,Y0,B,N: ", MR0, R0, B0, N0
	
	print xmin, xmax, ymin, ymax

	try:
		y0 = N0 / (B0*N0)**N0
	except OverflowError:
		print "y0 OVERFLOW"
		return nearlyInfinite
	
	y1 = gfun(xmin, ymin, MR0, R0, B0, N0)
	y2 = gfun(xmin, ymax, MR0, R0, B0, N0)
	y3 = gfun(xmax, ymin, MR0, R0, B0, N0)
	y4 = gfun(xmax, ymax, MR0, R0, B0, N0)
	return y0 * (y1 - y2 - y3 + y4)

def razorLikelihood(N, MR, Rsq, s, MR0, R0, B0, N0, btot, L):
	nbins = len(N)
	bfunc = [0]*nbins
	for i in xrange(nbins):
		bfunc[i] = razor2DBackground(MR[i], Rsq[i], MR0, R0, B0, N0)

		# check value of background function
		#if TMath.IsNan(bfunc[i]):
		#if isnan(bfunc[i]):
		#	print "bfunc[%d] is NAN" % i, MR0, R0, B0, N0
		#	return nearlyZero
		#elif isinf(bfunc[i]):
		#	print "bfunc[%d] is INFINITE" % i, MR0, R0, B0, N0
		#	return nearlyInfinite
		#elif bfunc[i] < 0:
		#print "bfunc[%d] is NEGATIVE" % i, MR0, R0, B0, N0
		#return nearlyZero

	# all background values are real numbers
	# get normalization
	bnorm = sum(bfunc)
	if bnorm == 0:
		print "bnorm is ZERO", MR0, R0, B0, N0
		return nearlyZero
	
	# now compute likelihood
	p = 1.0
	for i in xrange(nbins):
		mean = s[i] * L + btot * bfunc[i]/bnorm
		p *= poisson(N[i], mean)

	# check value of likelihood function
	#if isnan(p):
	#	print "likelihood is NAN", MR0, R0, B0, N0
	#	return nearlyZero
	#elif isinf(p):
	#	print "likelihood is INFINITE", MR0, R0, B0, N0
	#	return nearlyInfinite
	#elif p == 0:
	#	print "likelihood is ZERO", MR0, R0, B0, N0
	#	return nearlyZero
	#elif p < 0:
	#	print "likelihood is NEGATIVE", MR0, R0, B0, N0
	#	return nearlyZero
	#else:
	return p
#------------------------------------------------------------------
def main():	
	gSystem.Load("libRooRazor2DBackground.so")
	#gSystem.Load("libBAT")
	rootFilename = "Boris.root"
	workspaceName= "binnedrazor"

	# ----------------------------------------------
	# Get objects needed for rooInterface
	# ----------------------------------------------
	rfile = TFile(rootFilename)
	wspace = rfile.Get(workspaceName)
	
	model = wspace.pdf('model')
	
	nuis  = wspace.set('nuis_s')
	nbins = nuis.getSize()
	print "nbins:", nbins

	# parameters
	s   = [0.0]*nbins # signals

	# set all sigmas to constant zero
	for i in range(nbins):	     
		bin = "_%3.3d" % i		     
		wspace.var('sigma%s' % bin).setConstant()
	wspace.var('L').setConstant()
	wspace.var('btot').setConstant()
	
	# get parameter values
	MR0 = -1358.8
	R0  = -0.128 
	B0  = 0.89   
	N0  = 2.4
	
	## MR0 = wspace.var('MR0').getVal()
	## R0  = wspace.var('R0').getVal()
	## B0  = wspace.var('B0').getVal()
	## N0  = wspace.var('N0').getVal()
	
	btot= wspace.var('btot').getVal()
	L   = wspace.var('L').getVal()
	print MR0, R0, B0, N0, btot, L
	
	# data
	N   = [0]*nbins
	MR  = [0]*nbins
	Rsq = [0]*nbins
	for i in range(nbins):
		bin = "_%3.3d" % i		     
		N[i] = int(wspace.var('N%s' % bin).getVal())
		MR[i] = (wspace.var('MR%s' % bin).getMin(), wspace.var('MR%s' % bin).getMax())
		Rsq[i] = (wspace.var('Rsq%s' % bin).getMin(), wspace.var('Rsq%s' % bin).getMax())
		print "%5d\t%5d %s %s" % (i, N[i], MR[i], Rsq[i])

	print

	# Create a BG function object
	wspace.factory('mr[-1, 1]')
	wspace.factory('rsq[-1, 1]')
        wspace.factory('Razor2DBackground::bf(mr, rsq, MR0, R0, B0, N0, 1)')
	bfunc = wspace.function('bf')
	
	ntrials = 2
	for trial in xrange(ntrials):
		mr0 = uniform(0.9*MR0, 1.1*MR0)
		r0  = uniform(0.9*R0,  1.1*R0)
		b0  = uniform(0.9*B0,  1.1*B0)
		n0  = uniform(0.9*N0,  1.1*N0)

		wspace.var('MR0').setVal(mr0)		
		wspace.var('R0').setVal(r0)
		wspace.var('B0').setVal(b0)
		wspace.var('N0').setVal(n0)

		print "%5d\t%10.3f %10.3f %10.3f %10.3f" % (trial, mr0, r0, b0, n0)
		bf = 0.0
		for i in xrange(nbins):
			wspace.var('mr').setRange(MR[i][0], MR[i][1])
			wspace.var('rsq').setRange(Rsq[i][0], Rsq[i][1])
			wspace.var('mr').setVal(uniform(MR[i][0], MR[i][1]))
			b1 = bfunc.getVal()
			b2 = razor2DBackground(MR[i], Rsq[i], mr0, r0, b0, n0)
			print "\t%10.3e %10.3e" % (b1, b2)
		#continue

		
		p1 = model.getVal()
		p2 = razorLikelihood(N, MR, Rsq, s, mr0, r0, b0, n0, btot, L)
		print 'Likelihoods: '
		print "%10.3f %10.3f %10.3f %10.3f %10.3e %10.3e" % (mr0, r0, b0, n0, p1, p2)
#------------------------------------------------------------------
try:
	main()
except KeyboardInterrupt:
	print
	print "bye cruel world!"
	print
	


