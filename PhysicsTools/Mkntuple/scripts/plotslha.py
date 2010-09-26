#!/usr/bin/env python
#------------------------------------------------------------------------------
# File: plotslha.py
# Description: plot SUSY mass spectrum from an SLHA file
# Created: 22 Sep 2010 Harrison B. Prosper & Sezen Sekmen
#$Revision:$
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time import sleep
from getopt import getopt, GetoptError
from ROOT import *
#------------------------------------------------------------------------------
OPTIONS= 'dm:t:w:' # command line options
def usage():
	print '''
Usage
     ./plotslha.py [options] <slha file name>
	 
	 options:

	 -d                     plot decays [plot masses]
	 -m <maximum mass>      maximum mass (for y-axis) [find automatically]
	 -t "title"             title for plot [SUSY Mass Spectrum]
	 -w <canvas-width>      canvas width in pixels [1000]
'''
	sys.exit(0)
#------------------------------------------------------------------------------
LSCALE = 0.0138  # line length relative to x-range

# Font code = 10 * fontnumber + precision with fontnumber 1..14
# fontnumber= 1..14
# precision = 0 fast hardware fonts
# precision = 1 scalable and rotatable hardware fonts
# precision = 2 scalable and rotatable hardware fonts
# precision = 3 scalable and rotatable hardware fonts. Text size in pixels. 
TimesNewRomanBold = 32 
Arial             = 42
FSCALE = 0.04 # font size relative to x-range

# Text alignment = 10 * horizontal + vertical
# For horizontal alignment:
#   1=left adjusted, 2=centered, 3=right adjusted
# For vertical alignment:
#   1=bottom adjusted, 2=centered, 3=top adjusted
ALIGN_LEFT_BOTTOM = 11
#------------------------------------------------------------------------------
PDGID = '''
1
2
3
4
5
6
11
12
13
14
15
16
21
22
23
24
25
35
36
37
39
1000001
2000001
1000002
2000002
1000003
2000003
1000004
2000004
1000005
2000005
1000006
2000006
1000011
2000011
1000012
1000013
2000013
1000014
1000015
2000015
1000016
1000021
1000022
1000023
1000025
1000035
1000024
1000037
'''
PDGID  = map(atoi, split(strip(PDGID)))

IGNORE = [5,
		  24,
		  1000003,
		  2000003,
		  1000004,
		  2000004,
		  1000013,
		  2000013,
		  1000014]
NAMES = {\
    '1'   : ('d',kBlack),
    '2'   : ('u',kBlack),
    '3'   : ('s',kBlack),
    '4'   : ('c',kBlack),
    '5'   : ('b',kBlack),
    '6'   : ('t',kBlack),
	
    '11'   : ('e^{#pm}',kBlack),
    '12'   : ('#nu_{e}',kBlack),
    '13'   : ('#mu^{#pm}',kBlack),
    '14'   : ('#nu_{#mu}',kBlack),
    '15'   : ('#tau^{#pm}',kBlack),
    '16'   : ('#nu_{#tau}',kBlack),
    '21'   : ('g',kBlack),
    '22'   : ('#gamma',kBlack),
    '23'   : ('Z^{0}',kBlack),
    '24'   : ('W^{#pm}',kBlack),
	
	'25'	: ('h',kViolet-6),
	'35'	: ('H',kViolet-6),
	'36'	: ('A',kViolet-6),
	'37'	: ('H^{#pm}',kViolet-6),
	
	'39'    : ('G',kMagenta),
	
	'1000001'	: ('#tilde{d}_{L}',kBlue),
	'2000001'	: ('#tilde{d}_{R}',kBlue),
	'1000002'	: ('#tilde{u}_{L}',kBlue),
	'2000002'	: ('#tilde{u}_{R}',kBlue),
	'1000003'	: ('#tilde{s}_{L}',kBlue),
	'2000003'	: ('#tilde{s}_{R}',kBlue),
	'1000004'	: ('#tilde{c}_{L}',kBlue),
	'2000004'	: ('#tilde{c}_{R}',kBlue),
	'1000005'	: ('#tilde{b}_{1}',kBlue),
	'2000005'	: ('#tilde{b}_{2}',kBlue),
	'1000006'	: ('#tilde{t}_{1}',kBlue),
	'2000006'	: ('#tilde{t}_{2}',kBlue),
	'1000011'	: ('#tilde{e}_{L}',kRed),
	'2000011'	: ('#tilde{e}_{R}',kRed),
	'1000012'	: ('#tilde{#nu}_{e_{L}}',kRed),
	'1000013'	: ('#tilde{#mu}_{L}',kRed),
	'2000013'	: ('#tilde{#mu}_{R}',kRed),
	'1000014'	: ('#tilde{#nu}_{#mu_{L}}',kRed),
	'1000015'	: ('#tilde{#tau}_{1}',kRed),
	'2000015'	: ('#tilde{#tau}_{2}',kRed),
	'1000016'	: ('#tilde{#nu}_{#tau_{L}}',kRed),
	'1000021'	: ('#tilde{g}',kCyan+4),
	'1000022'	: ('#tilde{#chi}_{1}^{0}',kGreen+4),
	'1000023'	: ('#tilde{#chi}_{2}^{0}',kGreen+4),
	'1000025'	: ('#tilde{#chi}_{3}^{0}',kGreen+4),
	'1000035'	: ('#tilde{#chi}_{4}^{0}',kGreen+4),
	'1000024'	: ('#tilde{#chi}_{1}^{#pm}',kGreen+3),
	'1000037'	: ('#tilde{#chi}_{2}^{#pm}',kGreen+3)}
# add integer keys
for key in NAMES.keys():
	NAMES[atoi(key)] = NAMES[key]
#------------------------------------------------------------------------------
def setStyle():
   gStyle.SetOptTitle(0)
   gStyle.SetOptStat(0)
   
   gStyle.SetPadLeftMargin(0.09)
   gStyle.SetPadRightMargin(0.03)
   gStyle.SetTitleOffset(0.001, "Y")
   
   gStyle.SetFrameBorderMode(0)
   gStyle.SetPadBorderMode(0)
   gStyle.SetCanvasBorderMode(0)
   
   gStyle.SetCanvasColor(kWhite)
   gStyle.SetPadColor(kWhite)
   gStyle.SetFrameFillColor(kWhite)

   gStyle.SetTitleFont(Arial, "XY")
   gStyle.SetLabelFont(Arial, "XY")
#------------------------------------------------------------------------------
def makeHistogram(hname, xtitle, ytitle, nbin, xmin, xmax, ymin, ymax):	
   h = TH1F(hname, "", nbin, xmin, xmax)

   h.GetXaxis().CenterTitle()
   h.GetXaxis().SetTitle("")
   h.GetXaxis().SetTitleOffset(1.3)
   h.SetNdivisions(505, "X")
   
   h.GetYaxis().CenterTitle()
   h.GetYaxis().SetTitle(ytitle)
   h.GetYaxis().SetTitleOffset(0.8)
   h.SetNdivisions(510, "Y")

   h.SetMinimum(ymin)
   h.SetMaximum(ymax)

   for x in xtitle: h.Fill(x, 1)
   return h
#------------------------------------------------------------------------------
def getMassBlock(records, maxmass):
	block = {}
	token = "BLOCK MASS"	
	ltoken= len(token)

	found = False
	findmaxmass = maxmass < 0
	for index, record in enumerate(records):
		
		# Loop until we find the block
		if token == record[:ltoken]:
			found = True
			continue
		if not found: continue

		# we are in the block, so look for the terminal character
		# which is a single "#" on a line
		if record == "#": break

		# skip any other lines that start with a "#"
		if record[0] == "#": continue

		field = split(record)
		pdgid = atoi(field[0])
		mass  = atof(field[1])
		
		if not NAMES.has_key(pdgid):
			name = "UNK"
			color= kBlack
		else:
			name, color  = NAMES[pdgid]
		if findmaxmass:
			if mass > maxmass: maxmass = mass

		block[pdgid] = (mass, name, color)
	return (block, maxmass)
#------------------------------------------------------------------------------
def getDecayBlocks(records):
	blocks= {}
	token = "DECAY"	
	ltoken= len(token)

	found = False
	PDG = 0
	for index, record in enumerate(records):
		
		# Loop until we find the block
		if token == record[:ltoken]:
			#print record
			found = True
			field = split(record)
			PDG = atoi(field[1]) # particle id
			if not NAMES.has_key(PDG):
				name = "UNK"
				color= kBlack
			else:
				name, color = NAMES[PDG]
			blocks[PDG] = [(name, color)]
			continue
		if not found: continue

		# we are in the block, so look for the terminal character
		# which is a single "#" on a line. Then continue looking for
		# the next decay block
		if record == "#":
			found = False
			continue

		# skip any other lines that start with a "#"
		if record[0] == "#": continue

		field = split(record)
		BF  = atof(field[0]) # branching fraction
		NDA = atoi(field[1]) # number of daughters
		D   = []
		for i in range(NDA):
			pdgid = atoi(field[2+i])
			key = abs(pdgid)
			if not NAMES.has_key( key ):
				name = "UNK"
				color= kBlack
			else:
				# Fix name according to whether the PDG id is
				# positive or negative
				name, color = NAMES[key]
				if key < 7:
					if pdgid < 0:
						name = "#bar{%s}" % name
				elif find(name, "#pm") > -1:
					if key < 16:
						if pdgid > 0:
							name = replace(name, "#pm", "-")
						else:
							name = replace(name, "#pm", "+")
					else:
						if pdgid > 0:
							name = replace(name, "#pm", "+")
						else:
							name = replace(name, "#pm", "-")
			D.append((pdgid, name, color))
		blocks[PDG].append((D, BF))
	return blocks
#------------------------------------------------------------------------------
def plotMasses(masses, output, maxmass, **kargs):
   """ Plot SUSY mass spectrum
   """
   # Decode keyword arguments
   
   if kargs.has_key("title"):
	   title = kargs['title']
   else:
	   title = "SUSY Mass Spectrum"

   if kargs.has_key("width"):
	   width = kargs['width']
   else:
	   width = 1000
   height = width / 2

   # Define plot area in user coordinates [xmin, xmax] x [ymin, ymax]
   
   xmin = 0.0;
   xmax = 1.0;
   xrange = xmax-xmin;
   ymin = 0.0;
   ymax = maxmass;
   yrange = ymax-ymin
   
   # Scale font size relative to x-range
   
   fsize  = FSCALE * xrange

   # Fix font size
   
   gStyle.SetTitleSize(fsize, "XY")
   gStyle.SetLabelSize(fsize, "XY")

   # Create canvas
   
   c = TCanvas('cm', "SUSY Mass Spectrum", 10, 10, width, height)
   c.SetGridy()

   # Create histogram

   h = makeHistogram("hm",
					 ['Purple Things',
					  'Blue Things',
					  'Red Things',
					  'Green Things'],
					 "mass [GeV/c^{2}]",
					 4, xmin, xmax, ymin, ymax)
 
   # Scale line size relative to x-range
   
   lwidth = LSCALE * xrange

   # Plot title
   
   ptitle = TLatex()
   ptitle.SetTextAlign(ALIGN_LEFT_BOTTOM)
   ptitle.SetTextFont(TimesNewRomanBold)
   ptitle.SetTextSize(fsize)
   ptitle.SetTextColor(kBlack)

   # Lines and labels
   line = TLine()
   line.SetLineWidth(1)

   label = TLatex()
   label.SetTextAlign(ALIGN_LEFT_BOTTOM)
   label.SetTextFont(TimesNewRomanBold)
   label.SetTextSize(fsize)

   # Now plot

   c.cd()
   h.Draw()
   ptitle.DrawLatex(0.05 * xrange, 0.92 * yrange, title)
   
   space  = 0.1*lwidth
   xstep  = lwidth + space + 0.6 * fsize
   
   column = 0
   for pdgId in PDGID:
	   if not masses.has_key(pdgId): continue
	   if pdgId in IGNORE: continue
		
	   mass, name, colour = masses[pdgId]
	   xoffset = lwidth + column * xstep
		
	   line.SetLineColor( colour )
	   line.DrawLine( xoffset, mass, xoffset + lwidth, mass )

	   label.SetTextColor( colour )
	   label.DrawLatex(xoffset + lwidth + space, mass, name )
	   
	   column += 1 # increment column index

   c.Update()
   c.SaveAs( "%s.eps" % output )
   c.SaveAs( "%s.gif" % output )
   return (c, h) # needed to prevent plot from vanishing!
#------------------------------------------------------------------------------
def plotDecays(decays, output, **kargs):
   """ Plot SUSY decays
   """
   # Decode keyword arguments
   
   if kargs.has_key("title"):
	   title = kargs['title']
   else:
	   title = "SUSY Decays"

   if kargs.has_key("width"):
	   width = kargs['width']
   else:
	   width = 1000
   height = width / 2

   # Define plot area in user coordinates [xmin, xmax] x [ymin, ymax]
   
   xmin = 0.0;
   xmax = 1.0;
   xrange = xmax-xmin;
   ymin = 1.e-8;
   ymax = 10.0;
   yrange = ymax-ymin
   
   # Scale font size relative to x-range
   
   fsize  = FSCALE * xrange

   # Fix font size
   
   gStyle.SetTitleSize(fsize, "XY")
   gStyle.SetLabelSize(fsize, "XY")
   
   # Create canvas
   
   c = TCanvas('cd', "SUSY Decays", 100, 100, width, height)
   c.SetGridy()
   c.SetLogy()
   
   # Create histogram

   h = makeHistogram("hd",
					 ['Purple Things',
					  'Blue Things',
					  'Red Things',
					  'Green Things'],
					 "Branching Fraction",
					 4, xmin, xmax, ymin, ymax)

   # Scale line size relative to x-range
   
   lwidth = LSCALE * xrange

   # Plot title
   
   ptitle = TLatex()
   ptitle.SetTextAlign(ALIGN_LEFT_BOTTOM)
   ptitle.SetTextFont(TimesNewRomanBold)
   ptitle.SetTextSize(fsize)
   ptitle.SetTextColor(kBlack)

   # Lines and labels
   line = TLine()
   line.SetLineWidth(1)

   label = TLatex()
   label.SetTextAlign(ALIGN_LEFT_BOTTOM)
   label.SetTextFont(TimesNewRomanBold)
   label.SetTextSize(fsize)

   # Now plot

   c.cd()
   h.Draw()
   ptitle.DrawLatex(0.05 * xrange, 0.92 * yrange, title)
   
   space  = 0.1*lwidth
   xstep  = lwidth + space + 0.6 * fsize

   EOC = False
   column = 0
   for pdgId in PDGID:
	   if not decays.has_key(pdgId): continue
	   if pdgId in IGNORE: continue

	   # get particle name, color, daughters and branching fraction

	   inputs = decays[pdgId]
	   name, color = inputs[0]
	   inputs = inputs[1:]
	   
	   for D, BF in inputs:

		   xoffset = lwidth + column * xstep
		   if (xoffset + 5 * lwidth) > xmax:
			   EOC = True
			   break
		   
		   line.SetLineColor( color )
		   line.DrawLine( xoffset, BF, xoffset + lwidth, BF )

		   xpos = xoffset + lwidth + space
		   label.SetTextColor( color )
		   label.DrawLatex(xpos, BF, "%s " % name)
		   
		   for id, dname, dcolor in D:
			   label.SetTextColor( dcolor )
			   xpos += 0.5*fsize				   
			   label.DrawLatex(xpos, BF, dname)
			   if dname[0] in ['W','Z','H']:
				   xpos += 0.25*fsize

			   column += 1 # increment column index
	   if EOC: break
   c.Update()
   c.SaveAs( "%s.eps" % output )
   c.SaveAs( "%s.gif" % output )
   return (c, h) # needed to prevent plot from vanishing!
#------------------------------------------------------------------------------
def decodeCommandLine():
	argv = sys.argv[1:]
	if len(argv) < 1: usage()

	try:
		options, filenames = getopt(argv, OPTIONS)
	except GetoptError, m:
		print m
		usage()
	if len(filenames) == 0: usage()
	
	filename = filenames[0]
	if not os.path.exists(filename):
		print "Go boil your head because I can't find \n\t%s" % filename
		sys.exit(0)

	# Check for command line options

	doMasses = True
	title   = "SUSY"
	width   = 1000 # pixels
	maxmass =-1.0
	fixmass = True
	
	for option, value in options:
		if option == '-t':
			title = value
		elif option == '-d':
			doMasses = False
			print "\n\t** This is still experimental! ***\n"
		elif option == '-m':
			try:
				maxmass = atof(value)
				fixmass = False
			except:
				print "\t*** I can't make sense of maxmass( %s )" % value
				sys.exit(0)
		elif option == '-w':
			try:
				width = atoi(value)
			except:
				print "\t*** I can't make sense of width( %s )" % value
				sys.exit(0)
	if doMasses:
		return (filename, title, width, maxmass, fixmass)
	else:
		return (filename, title, width)
#------------------------------------------------------------------------------
def main():

	inputs = decodeCommandLine()

	filename, atitle, awidth = inputs[:3]

	# Read SLHA file
	records  = map(strip, open(filename).readlines())
	
	n = rfind(filename, '.')
	output = filename[:n]
	
	# Plot either masses or decays

	setStyle()
	
	if len(inputs) > 3:
		maxmass, fixmass = inputs[3:]
		masses, maxmass = getMassBlock(records, maxmass)
		if fixmass:
			n = int(1.1 * maxmass/100)
			maxmass = (n+1) * 100
	
		print "Maximum mass: %10.1f" % maxmass

		# Ok plot
		c, h = plotMasses(masses, output, maxmass, title=atitle, width=awidth)
	else:
		decays = getDecayBlocks(records)
		c, h = plotDecays(decays, output, title=atitle, width=awidth)
	#sleep(5)
	gApplication.Run()
#------------------------------------------------------------------------------
main()

