#!/usr/bin/env python
#-----------------------------------------------------------------------------
# File:        trainjetnet.py
# Description: test C++ wrapper of jetnet 3.4 fortran code
# Created:     04-Feb-2006 Harrison B. Prosper
#              13-Feb-2006 HBP
#$Revision:$
#-----------------------------------------------------------------------------
import sys, os
from sys import exit
from math import *
from string import *
from time import sleep
from getopt import getopt, GetoptError
from ROOT import *
#-----------------------------------------------------------------------------
# Constants
#------------------------------------------------------------------------------
SIGFILE= 'sig.dat'
BKGFILE= 'bkg.dat'
NEPOCH = 2000   # Number of epochs
NSTEP  = 10     # Plot every NSTEP epoch
NTRAIN = 10000  # Number of training events/file
NHIDDEN= 10     # Number of hidden nodes

USAGE = '''
Usage:
  ./trainjetnet.py <options> <net-name>

  options:
	   -B   background file name [%(bkgfile)s]
	   -S   signal file name [%(sigfile)s]
	   -H   number of hidden nodes [%(hidden)s]
	   -E   number of training epochs [%(nepoch)s]
	   -P   number of training patterns/file [%(ntrain)s]

  Needs:
	   <net-name>.vars

	   containing input variables
''' % {'sigfile' : SIGFILE,
	   'bkgfile' : BKGFILE,
	   'hidden'  : NHIDDEN,
	   'nepoch'  : NEPOCH,
	   'ntrain'  : NTRAIN}

shortOptions = 'B:S:H:E:P:'

KPLOT  = 5
NBIN   = 50
#-----------------------------------------------------------------------------
def usage():
	print USAGE
	exit(0)

def error(message):
	print "*** Error *** %s" % message
	exit(0)
#-----------------------------------------------------------------------------
if len(sys.argv) < 2: usage()
from PhysicsTools.Mkntuple.AutoLoader import *
#-----------------------------------------------------------------------------
def readData(filename):
	print "reading data from %s..." % filename
	records = map(split, open(filename).readlines())
	header  = records[0]
	records = records[1:]
	varmap  = {}
	for index, name in enumerate(header):
		varmap[name] = index
	return (varmap, records)
#-----------------------------------------------------------------------------
def loadNetwork(title,
				varmap,
				sig,
				bkg,
				vars,
				nrows,
				sample, 
				nn):
	print title

	nrows = min(nrows, min(len(sig), len(bkg)))

	# Set either Test or Train
	nn.setSample(sample)

	input = vdouble(len(vars))

	for record in sig[:nrows]:
		for i, var in enumerate(vars):
			input[i] = atof(record[varmap[var]])
		nn.setPattern(input, 1)

	for record in bkg[:nrows]:
		for i, var in enumerate(vars):
			input[i] = atof(record[varmap[var]])
		nn.setPattern(input, 0)
#-----------------------------------------------------------------------------
class Plot:
	
	def __init__(self, netname, nepoch, nbin):

		kbin   = nepoch / (KPLOT*NSTEP)
		kit.setStyle()
		
		self.coutput = TCanvas("fig_%s_train" % netname,
							   "Training Jetnet", 700, 0, 500, 500)

		# Network output distributions
		
		self.hs = kit.histogram("hs","net output","", nbin, 0, 1, kRed, 1, 2)
		self.hs.SetFillColor(kRed)
		self.hs.SetFillStyle(3004)
		
		self.hb = kit.histogram("hb","net output","", nbin, 0, 1, kBlue,1, 2)
		self.hb.SetFillColor(kBlue)
		self.hb.SetFillStyle(3005)

		# Power vs epoch number

		self.cpower = TCanvas("fig_%s_power" % netname,
							  "Power Time Series", 0, 0, 600, 600)

		self.cpower.Divide(1,2)
		
		self.htrain = kit.histogram("htrain", "epoch", "power", 
									kbin, 0, nepoch, kGreen, 1, 2)
		self.htrain.SetMaximum(1)
		
		self.htest  = kit.histogram("htest", "epoch", "power", 
									kbin, 0, nepoch, kBlack, 1, 2)
		self.htest.SetMaximum(1)
		
		self.hdiff  = kit.histogram("hdiff", "epoch",
									"power(train) - power(test)", 
									kbin, 0, nepoch, kMagenta, 1, 2)
		self.hdiff.SetMinimum(-0.1)
		self.hdiff.SetMaximum(0.1)

	def __del__(self):
		pass
	
	def hist(self, nn):
		hs = self.hs
		hb = self.hb
	
		b  = nn.histogram(0)
		s  = nn.histogram(1)

		sd = vdouble(len(s))
		bd = vdouble(len(b))
		for i in xrange(len(s)):
			sd[i] = s[i]
			bd[i] = b[i]
		kit.setContents(hs, sd)
		kit.setContents(hb, bd)

		self.coutput.cd()
		if hs.GetMaximum() > hb.GetMaximum():
			hs.Draw()
			hb.Draw("SAME")
		else:
			hb.Draw()
			hs.Draw("SAME")
		self.coutput.Update()

	def series(self, epoch, powertrain, powertest):
		self.htrain.Fill(epoch, powertrain)
		self.htest.Fill(epoch, powertest)
		self.hdiff.Fill(epoch, powertrain-powertest)
	
		self.cpower.cd(1)
		self.htrain.Draw("L")
		self.htest.Draw("L SAME")

		self.cpower.cd(2)
		self.hdiff.Draw("L")
		self.cpower.Update()

	def save(self, gtype=".gif"):
		self.coutput.SaveAs(gtype)
		self.cpower.SaveAs(gtype)
#-----------------------------------------------------------------------------
def main():

	# Check for command line options
	argv = sys.argv[1:]
	try:
		options, names = getopt(argv, shortOptions)
	except GetoptError, m:
		print m
		usage()
		
	if len(names) == 0: usage()
	netname = names[0]
	varfile = "%s.vars" % netname
	if not os.path.exists(varfile): error("Can't find %s" % varfile)

	# Set defaults
	
	sigfile = SIGFILE
	bkgfile = BKGFILE
	nhidden = NHIDDEN
	nepoch  = NEPOCH
	ntrain  = NTRAIN
	
	for option, value in options:
		if   option == '-B':
			bkgfile = value
		elif option == '-S':
			sigfile = value			
		elif option == '-H':
			nhidden = atoi(value)
		elif option == '-E':
			nepoch = atoi(value)
		elif option == '-P':
			ntrain = atoi(value)

	if not os.path.exists(bkgfile): error("Can't find %s" % bkgfile)
	if not os.path.exists(sigfile): error("Can't find %s" % sigfile)
	#-----------------------------------------------------------------------
	# Set up plots

	plot = Plot(netname, nepoch, NBIN)

	#-----------------------------------------------------------------------
	# Get variable names and data
	#-----------------------------------------------------------------------
	vlist = filter(lambda x: x != "",
				   map(strip, open(varfile).readlines()))
	vars = vstring()
	print "Variables"
	for x in vlist:
		print "\t%s" % x
		vars.push_back(x)

	varmap, sig = readData(sigfile)
	varmap, bkg = readData(bkgfile)

	ntrain = min( min(len(sig), len(bkg)) / 2 , ntrain)
	ntest  = ntrain
	print "Number of training events/file %s\n" % ntrain 
	#-----------------------------------------------------------------------
	# Define network
	#-----------------------------------------------------------------------
	nn = jetnet.NN(vars, nhidden)

	loadNetwork("load training data",
				varmap,
				sig,
				bkg,
				vars,
				ntrain,
				jetnet.NN.kTRAINING, nn)

	loadNetwork("load testing data",
				varmap,
				sig[ntrain:],
				bkg[ntrain:],
				vars,
				ntest,
				jetnet.NN.kTESTING, nn)

	#-----------------------------------------------------------------------
	# Train!
	#-----------------------------------------------------------------------
	nn.begin()
	nn.printParameters()

	# RMS:   Error function: Sum (t_i - net(x_i))**2 / N
	# AUC:   Area Under Curve (that is, eff(s) vs eff(b) curve)
	# POWER: Chip Stewart number

	kplot = 0
	for epoch in xrange(nepoch):

		nn.train(epoch)

		if epoch % NSTEP == 0:

			rms0 = nn.test(jetnet.NN.kTRAINING, 0.5, NBIN)
			pwr0 = nn.power()

			rms1 = nn.test(jetnet.NN.kTESTING, 0.5, NBIN)
			pwr1 = nn.power()

			kplot += 1
			if kplot == KPLOT:
				kplot = 0

				print "%10s %10s %10s %10s %10s" % ("epoch", 
													"", "RMS",
													"", "power")
				print "%10d %10.4f %10.4f %10.4f %10.4f" % (epoch,
															rms0, rms1,
															pwr0, pwr1)
				plot.hist(nn)        
				plot.series(epoch, pwr0, pwr1)

	nn.end()
	plot.save()
	
	#-----------------------------------------------------------------------
	# Save weights and network function
	#-----------------------------------------------------------------------
	nn.save(netname)
	os.system("rm -rf *.net")
#-----------------------------------------------------------------------------
main()

