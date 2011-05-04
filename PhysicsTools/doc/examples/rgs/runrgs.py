#!/usr/bin/env python
# -----------------------------------------------------------------------------
#  File:        runrgs.py
#  Description: Run Random Grid Search to cuts that best separate signal from
#               background.
# -----------------------------------------------------------------------------
#  Created:     28-Apr-2009 Harrison B. Prosper
# -----------------------------------------------------------------------------
import os, sys, re
from ROOT import *
from string import *
# -----------------------------------------------------------------------------
def error(message):
	print "** %s" % message
	exit(0)
# -----------------------------------------------------------------------------
# Check that all files that are needed are present
VARFILE = "rgs.vars"
if not os.path.exists(VARFILE):
	error("unable to open variables file %s" % VARFILE)

CUTSFILE = "cuts.dat"
if not os.path.exists(CUTSFILE):
	error("unable to open cuts file %s" % CUTSFILE)

SIGFILE = "sig.dat"
if not os.path.exists(SIGFILE):
	error("unable to open signal file %s" % SIGFILE)

BKGFILE = "bkg.dat"
if not os.path.exists(BKGFILE):
	error("unable to open background file %s" % BKGFILE)
# -----------------------------------------------------------------------------
#  Get cut variables and cut directions
# -------------------------------------------
def getvars(filename):
	records = map(split, filter(lambda x: x != "",
								map(strip, open(filename).readlines())))
	vars = vstring()
	cutdir = vstring()
	for v, c in records:
		vars.push_back(v)
		cutdir.push_back(c)
		print "\t\t%-10s: %10s" % (vars.back(), cutdir.back())
	return (vars, cutdir)
# -------------------------------------------
def formatHist(h, color):
	NDIVX =-505
	NDIVY =-505
	MARKERSIZE=1.0

	h.SetMarkerSize(MARKERSIZE)
	h.SetMarkerStyle(20)
	h.SetMarkerColor(color)
    
	h.GetXaxis().CenterTitle()
	h.GetXaxis().SetTitle("efficiency (background)")
	h.GetXaxis().SetTitleOffset(1.2)
	h.GetXaxis().SetNdivisions(NDIVX)
    
	h.GetYaxis().CenterTitle()
	h.GetYaxis().SetTitle("efficiency (signal)")
	h.GetYaxis().SetTitleOffset(1.8)
	h.GetYaxis().SetNdivisions(NDIVY)
# -------------------------------------------			
def getbestcuts(rgs, maxcuts, cutvar, cutdir, smin=100.0):
	
	# Compute significance
	btotal = rgs.total(0)   #  summed background weights
	stotal = rgs.total(1)   #  summed signal weights

	print
	print "\tbackground total: %10.1f" % btotal
	print "\tsignal total:     %10.1f" % stotal

	best   = -1.0
	kbest  = -1
	bmin   = 1.e20
	eb = [0.0]*maxcuts
	es = [0.0]*maxcuts
	sigsig = [0.0]*maxcuts
	for i in xrange(maxcuts):
		b = rgs.count(0, i)  #  background count after the ith cut-point
		s = rgs.count(1, i)  #  signal count after the ith cut-point
		eb[i] = b/btotal
		es[i] = s/stotal
		if s > smin:
			if b < bmin:
				bmin = b
				kbest= i	
	#	if b > 0:
	#		sigsig[i] = s/sqrt(b) # sqrt(2*(-s + (s+b)*log(1+s/b)))
	#	else:
	#		sigsig[i] =-1.0
	#		
	#	if sigsig[i] > best:
	#		kbest = i
	#		best = sigsig[i]

	#  Get best cuts

	if kbest > -1:
		b = rgs.count(0, kbest)   #  background count after the ith cut-point
		s = rgs.count(1, kbest)   #  signal count after the ith cut-point
		effb = eb[kbest]          #  background efficiency
		effs = es[kbest]          #  signal efficiency
	    #isignif= sigsig[kbest]
		if b > 0:
			signif = s / sqrt(b)
		else:
			signif = -1
		out = open("bestcuts.txt","w")
		record = "\trecord:     %10d\n"\
				 "\tsignal:     %10.1f\n"\
				 "\tbackground: %10.1f\n"\
				 "\tsignif:     %10.1f\n"\
				 "\teff(s):     %10.3f\n"\
				 "\teff(b):     %10.3f" % (kbest+2, s, b, signif, effs, effb)
		print record
		out.write(record+"\n")

		cut = rgs.cuts(kbest)
	
		for i in xrange(cut.size()):
			record = "\t%-10s\t%s\t%10.1f" % (cutvar[i],
											  cutdir[i],
											  cut[i])
			print record
			out.write(record+"\n")
		out.close()
	return (es, eb, sigsig, kbest)
# -------------------------------------------
def efflimits(x):
	x1 = sum(x)/len(x)
	x2 = sum(map(lambda z: z*z, x))/len(x)
	x2 = sqrt(x2-x1*x1)
	xmax = x1 + x2
	k = int(xmax/0.05)
	xmax = k*0.05
	return (0.0, xmax)
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
def main():
	print "\n\t\t=== Random Grid Search ===\n"
	
	# -------------------------------------------------------------------------
	#  Load RGS class and style file
	# -------------------------------------------------------------------------
	gROOT.ProcessLine(".L rgsearch.cc+")
	gROOT.ProcessLine(".x style.C")

	# -------------------------------------------------------------------------
	#  Get cut variables
	# -------------------------------------------------------------------------
	print "\t==> get variables"
	cutvar, cutdir = getvars("rgs.vars")
	
	# -------------------------------------------------------------------------
	#  Create RGS object
	# 
	#  Needed:
	#   A file of cut-points - usually a signal file, which ideally is
	#  not the same as the signal file on which the RGS algorithm is run.
	# -------------------------------------------------------------------------
	start = 0    
	maxcuts = 50000 #  maximum number of cut-points to consider

	print "\t==> create RGS object"
	rgs = RGS(CUTSFILE, start, maxcuts)
	
	# -------------------------------------------------------------------------
	#  Add signal and background data to RGS object
	#  Weight each event using the value in field "weight", if it exists. 
	# -------------------------------------------------------------------------
	numrows = 0 #  Load all the data from the files
	
	print "\t==> load background data"
	numrows = 1310662
	rgs.add(BKGFILE, start, numrows, "weight")
	print
	
	print "\t==> load signal data"
	numrows = 83716
	rgs.add(SIGFILE, start, numrows, "weight")
	print
	
	# -------------------------------------------------------------------------
	#  Run!
	# -------------------------------------------------------------------------
	rgs.run(cutvar, cutdir)
	rgs.save("results35v2.root", 350)
	
	es, eb, signif, kbest = getbestcuts(rgs, maxcuts, cutvar, cutdir)
	sys.exit(0)
	# -------------------------------------------------------------------------
	#  Plot results
	# -------------------------------------------------------------------------
	nbins= 100
	bmin, bmax = 0.0, 0.00001 # efflimits(eb)
	smin, smax = 0.0, 0.04 # efflimits(es)
	nh = 5
	hrgs = [None]*(nh+1)
	color = [kBlue, kGreen, kYellow, kRed, kBlack]
	threshold = [0.2, 0.4, 0.6, 0.8, 1.2]
	for i in xrange(nh):
		hname = "hrgs%d" % i
		hrgs[i] = TH2F(hname, "", nbins, bmin, bmax, nbins, smin, smax)
		formatHist(hrgs[i], color[i])

	hrgs[nh] = TH2F("hbest", "", nbins, bmin, bmax, nbins, smin, smax)
	formatHist(hrgs[nh], kMagenta)
	hrgs[nh].SetMarkerStyle(21)
	hrgs[nh].SetMarkerSize(2)
	
	maxsignif = signif[kbest]
	
	#  Plot
	
	for i in xrange(maxcuts):
		if (i % 1000 == 0): print i
		
		if signif[i] < 0: continue
		
		sigf = signif[i] / maxsignif
		
		#  Plot es vs eb
		for k in xrange(nh):
			if sigf < threshold[k]:
				hrgs[k].Fill(eb[i], es[i])
				break

	hrgs[nh].Fill(eb[kbest], es[kbest])
	crgs = TCanvas("fig_rgs", "RGS Distribution", 750, 0, 500, 500)
	crgs.cd()
	hrgs[0].Draw()
	for j in xrange(1, nh+1): hrgs[j].Draw("same")
	crgs.Update()
	crgs.SaveAs(".gif")
	gSystem.Sleep(2000)
# -----------------------------------------------------------------------------
main()



