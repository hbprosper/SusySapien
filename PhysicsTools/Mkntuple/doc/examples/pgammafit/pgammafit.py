#!/usr/bin/env python
#------------------------------------------------------------------------------
#- File: pgammafit.py
#- Description: Example of use of PoissonGammaFit
#               Fit a mixture of QCD (QCD+Z), W+jets, top (tt, tqb, tb, tW)
#               to (MC) data. The data for the sources are specified as
#               2-d histograms as are the data. The example is the expectation,
#               after cuts, for an integrated luminosity of 10/pb. The MC
#               data were provided by Lukas V.
#- Created: 20-Sep-2010 Harrison B. Prosper
#------------------------------------------------------------------------------
from string import *
from ROOT import *
from time import sleep
import os, sys, re
import PhysicsTools.Mkntuple.AutoLoader
#------------------------------------------------------------------------------
sources = '''
QCD
W
top
'''
# Histogram names
sources = split(strip(sources))
hname   = map(lambda x: "h%s" % x, sources)
hnameD  = "hdata"
getcount= re.compile(r'[a-zA-Z]+ +[0-9.]+')
#------------------------------------------------------------------------------
def contents(h):
	c = vdouble()
	for i in range(h.GetNbinsX()):
		binx = i+1
		for j in range(h.GetNbinsY()):
			biny = j+1
			c.push_back( h.GetBinContent(binx, biny) )
	return c
#------------------------------------------------------------------------------
def main():

	# Read expected counts from the counts.txt file and place them in a map
	
	counts = map(lambda x: (x[0], atof(x[1])),
				 map(split, getcount.findall(open("counts.txt").read())))
	count = {}
	for key, value in counts: count[key] = value

	# Set some standard style and create a canvas

	kit.setStyle()
 	canvas = TCanvas("fig_fitresults",
					 "m_T vs b_tag", 
					 50, 50, 500, 500)

	# Read histograms into memory
	
	filename = "histograms.root"
	hcache   = HistogramCache(filename)

	# Plot data histogram
	
	hdata = hcache.histogram(hnameD)
	canvas.cd()
	hdata.Draw("lego2")
	canvas.Update()
	sleep(1)
	
	# Get data distribution, unfolded into a 1-d vector, D, and plot

	D = contents(hdata)
	h1d = []
	h1d.append( kit.histogram("hdata1d", "bin", "", len(D), 0, len(D)))
	kit.setContents(h1d[-1], D)
	canvas.cd()
	h1d[-1].Draw("EP")
	canvas.Update()
	sleep(1)
				
	# Get distribution of sources, also unfolded into 1-d vectors, A[j]

	h = []
	A = vvdouble()
	for i, source in enumerate(sources):
		h.append( hcache.histogram(hname[i]) )
		canvas.cd()
		h[-1].Draw("lego2")
		canvas.Update()
		sleep(1)

		A.push_back( contents(h[-1]) )
		h1d.append( kit.histogram("%s1d" % hname[i],
								  hname[i], "", len(D), 0, len(D)) )
		kit.setContents(h1d[-1], A[i])
		h1d[-1].SetFillColor(i+1)
		canvas.cd()
		h1d[-1].Draw()
		canvas.Update()
		sleep(1)

	# Construct a PoissonGamma object and fit...
	
	total = sum(D)
	print "total data count: %d" % total
	

	print "Fit..."
	pgfit = PoissonGammaFit(A, D)
	guess = vdouble(len(sources), total/3)
	pgfit.execute(guess)
	
	if not pgfit.good():
		print "Fit failed"
		sys.exit(0)

	# Get mode and width of posterior density, which, because PoissonGammaFit
	# uses a flat prior for the yields, are the same as those of the
	# Poisson/gamma marginal density. 
	
	mode  = pgfit.mode()
	error = pgfit.width()
	logevidence = pgfit.logEvidence()

	# Print results
	
	print
	print "%-10s %10s\t%10s    %10s" %  \
		  ('source', "true count", " estimated count", "")
	for index, s in enumerate(sources):
		print "%-10s %10.f\t%10.0f +/-%-10.0f" % \
			  (s, count[s], mode[index], error[index])
#------------------------------------------------------------------------------
main()

