#------------------------------------------------------------------------------
#- File: plotutil.py
#- Description: utilities for plotting
#- Created:     05-Dec-2005, Harrison B. Prosper, Kurtis Johnson
#------------------------------------------------------------------------------
import os
from os  import path
from sys import exit, argv
from string  import *
from math    import *
from array import array
from ROOT import *
#-----------------------------------------------------------------------------
LINEWIDTH= 2
TEXTFONT = 42 # 42
TEXTSIZE = 0.05
NDIVX    = 504
NDIVY    = 505
WIDTH    = 500
HEIGHT   = 500
#-----------------------------------------------------------------------------
def readData(filename, nmax=-1):
    print "reading file %s ..." % filename
    
    records= map(split, open(filename).readlines())
    header = records[0]
    records= records[1:]
    
    dmap = {}
    for index, name in enumerate(header): dmap[name] = index
    if nmax <= 0: nmax = len(records)
    
    nmax = min(nmax, len(records))
    data = map(lambda x: map(atof, x), records[:nmax])
    return (data, dmap)
#-----------------------------------------------------------------------------
def chisq(h1, h2):
    nbin = h1.GetNbinsX()
    chi2 = 0.0
    ND = 0
    for i in xrange(nbin):
        bin = i+1
        d = h1.GetBinContent(bin)
        if d < 5: continue
        f = h2.GetBinContent(bin)
        y = d - f
        ND += 1
        chi2 += y*y/d
    return (chi2, ND) 
#-----------------------------------------------------------------------------
def nameonly(x):
    return os.path.splitext(os.path.basename(x))[0]
#-----------------------------------------------------------------------------
def getargs(args, key, defval):
    if args.has_key(key):
        return args[key]
    else:
        return defval
#-----------------------------------------------------------------------------
def mkcanvas(name, n=None):
    os.system("mkdir -p figures")
    cname  = "figures/fig_%s" % name
    if n != None:
        xoffset = 50*n
        yoffset = 5*n
    else:
        xoffset = 0
        yoffset = 0
    ctitle = cname
    return TCanvas(cname, ctitle, xoffset, yoffset, 500, 500)
#-----------------------------------------------------------------------------
def mklegend(title, nlines, **args):    
    xmin = getargs(args, "xmin", 0.62)
    xmax = getargs(args, "xmax", xmin + 0.25)
    ymax = getargs(args, "ymax", 0.93)
    ymin = ymax - nlines * 0.08
    
    legend = TLegend(xmin, ymin, xmax, ymax, title)
    legend.SetFillColor(0)
    legend.SetFillStyle(4000)
    legend.SetBorderSize(0)
    legend.SetTextSize(TEXTSIZE)
    legend.SetTextFont(TEXTFONT)
    return legend
#-----------------------------------------------------------------------------
def mkhist1(hname,
			xtitle,
			ytitle,
			nbin,
			xmin,
			xmax,
			**args):
	mstyle = getargs(args, "mstyle", 20)
    mcolor = getargs(args, "mcolor", kBlue)
    msize  = getargs(args, "msize",  0.8)
    color  = getargs(args, "color",  kBlack)
    lstyle = getargs(args, "lstyle", 1)
    lwidth = getargs(args, "lwidth", LINEWIDTH)
    ndivx  = getargs(args, "ndivx",  504)
    ndivy  = getargs(args, "ndivy",  504)

    h = TH1F(hname, "", nbin, xmin, xmax)

	h.SetLineStyle(lstyle)
    h.SetLineColor(color)
	h.SetLineWidth(lwidth)

	h.SetMarkerStyle(mstyle)
    h.SetMarkerColor(mcolor)
    h.SetMarkerSize(msize)
    
    h.GetXaxis().CenterTitle()
    h.GetXaxis().SetTitle(xtitle)
    h.GetXaxis().SetTitleOffset(1.3) # 1.3
    h.SetNdivisions(ndivx, "X")
    
    h.GetYaxis().CenterTitle()
    h.GetYaxis().SetTitle(ytitle)
    h.GetYaxis().SetTitleOffset(1.7) # 1.8
    h.SetNdivisions(ndivy, "Y")
    return h
#------------------------------------------------------------------------------
def mkhist2(hname,
			xtitle,
			ytitle,
			nxbin,
			xmin,
			xmax,
			nybin,
			ymin,
			ymax,
			**args):
	mstyle = getargs(args, "mstyle", 20)
    mcolor = getargs(args, "mcolor", kBlue)
    msize  = getargs(args, "msize",  0.8)
    color  = getargs(args, "color",  kBlack)
    lstyle = getargs(args, "lstyle", 1)
    lwidth = getargs(args, "lwidth", LINEWIDTH)
    ndivx  = getargs(args, "ndivx",  504)
    ndivy  = getargs(args, "ndivy",  504)

    h = TH2F(hname, "", nxbin, xmin, xmax, nybin, ymin, ymax)

	h.SetLineStyle(lstyle)
    h.SetLineColor(color)
    h.SetLineWidth(lwidth)

	h.SetMarkerStyle(mstyle)
    h.SetMarkerColor(mcolor)
    h.SetMarkerSize(msize)
    
    h.GetXaxis().CenterTitle()
    h.GetXaxis().SetTitle(xtitle)
    h.GetXaxis().SetTitleOffset(1.3) # 1.3
    h.SetNdivisions(ndivx, "X")
    
    h.GetYaxis().CenterTitle()
    h.GetYaxis().SetTitle(ytitle)
    h.GetYaxis().SetTitleOffset(1.7) # 1.8
    h.SetNdivisions(ndivy, "Y")
    return h
#------------------------------------------------------------------------------
def mkgraph(x, y,
			xtitle,
			ytitle,
			xmin,
			xmax,
			ymin=-1,
			ymax=-1,
			**args):
	color  = getargs(args, "color",   1)
	lstyle = getargs(args, "lstyle",  1)
	lwidth = getargs(args, "lwidth",  LINEWIDTH)
	ndivx  = getargs(args, "ndivx",   504)
	ndivy  = getargs(args, "ndivy",   504)
	nbins  = len(x)

	g = TGraph(nbins, x, y)
	g.SetLineColor(color)
	g.SetLineWidth(lwidth)
	g.SetLineStyle(lstyle)

	g.GetXaxis().CenterTitle()
	g.GetXaxis().SetTitle(xtitle)
	g.GetXaxis().SetTitleOffset(1.3)
	g.GetHistogram().SetAxisRange(xmin, xmax, "X")
	g.GetHistogram().SetNdivisions(ndivx, "Y")

	g.GetYaxis().CenterTitle()
	g.GetYaxis().SetTitle(ytitle)
	g.GetYaxis().SetTitleOffset(1.8)

	if ymin < ymax:
		g.GetHistogram().SetAxisRange(ymin, ymax, "Y")
		g.GetHistogram().SetNdivisions(ndivy, "Y")
	return g
#------------------------------------------------------------------------------
def scalehist(hist):
    ymax =-1.0
    for h in hist:
        y = h.GetMaximum()
        if y > ymax: ymax = y
    if ymax < 1:
        step = 0.2
    elif ymax >= 1 and ymax < 10:
        step = 2
    elif ymax >= 10 and ymax < 100:
        step = 10
    elif ymax >= 100 and ymax < 1000:
        step = 50
    elif ymax >= 1000:
        step = 100

    k = int(ymax / step)
    ymax = step * (k+1)
    for h in hist:
        h.SetMaximum(ymax)
#------------------------------------------------------------------------------
def plothist(canvas,
             hist,
             **args):
    canvas.cd()

    if args.has_key("logy"):
        gPad.SetLogy(1)
    else:
        gPad.SetLogy(0)
        
    if args.has_key("logx"):
        gPad.SetLogx(1)
    else:
        gPad.SetLogx(0)

    normalize = getargs(args, "normalize", False)
    errors = getargs(args, "errors", False)

    if errors:
        option = "E"
    else:
        option = ""
        
    for h in hist:
        if normalize:
            h.DrawNormalized(option)
        else:
            h.Draw(option)

        if errors:
            option = "E same"
        else:
            option = "same"
    
    if args.has_key("legend"): args["legend"].Draw()

    canvas.Update()
    canvas.SaveAs(".gif")
#------------------------------------------------------------------------------
CMSstyle = TStyle("CMSstyle","CMS Style")
def setStyle():
    
    # For the canvas:
    CMSstyle.SetCanvasBorderMode(0)
    CMSstyle.SetCanvasColor(kWhite)
    CMSstyle.SetCanvasDefH(500) #Height of canvas
    CMSstyle.SetCanvasDefW(500) #Width of canvas
    CMSstyle.SetCanvasDefX(0)   #Position on screen
    CMSstyle.SetCanvasDefY(0)

    # For the Pad:
    CMSstyle.SetPadBorderMode(0)
    CMSstyle.SetPadColor(kWhite)
    CMSstyle.SetPadGridX(kFALSE)
    CMSstyle.SetPadGridY(kFALSE)
    CMSstyle.SetGridColor(kGreen)
    CMSstyle.SetGridStyle(3)
    CMSstyle.SetGridWidth(1)
    
    # For the frame:
    CMSstyle.SetFrameBorderMode(0)
    CMSstyle.SetFrameBorderSize(1)
    CMSstyle.SetFrameFillColor(0)
    CMSstyle.SetFrameFillStyle(0)
    CMSstyle.SetFrameLineColor(1)
    CMSstyle.SetFrameLineStyle(1)
    CMSstyle.SetFrameLineWidth(1)
    
    # For the histo:
    CMSstyle.SetHistLineColor(1)
    CMSstyle.SetHistLineStyle(0)
    CMSstyle.SetHistLineWidth(1)

    CMSstyle.SetEndErrorSize(2)
    CMSstyle.SetErrorX(0.)
    
    CMSstyle.SetMarkerSize(0.1)
    CMSstyle.SetMarkerStyle(20)

    #For the fit/function:
    CMSstyle.SetOptFit(1)
    CMSstyle.SetFitFormat("5.4g")
    CMSstyle.SetFuncColor(2)
    CMSstyle.SetFuncStyle(1)
    CMSstyle.SetFuncWidth(1)

    #For the date:
    CMSstyle.SetOptDate(0)

    # For the statistics box:
    CMSstyle.SetOptFile(0)
    CMSstyle.SetOptStat("")
    # To display the mean and RMS:
    #CMSstyle.SetOptStat("mr") 
    CMSstyle.SetStatColor(kWhite)
    CMSstyle.SetStatFont(TEXTFONT)
    CMSstyle.SetStatFontSize(TEXTSIZE)
    CMSstyle.SetStatTextColor(1)
    CMSstyle.SetStatFormat("6.4g")
    CMSstyle.SetStatBorderSize(1)
    CMSstyle.SetStatH(0.2)
    CMSstyle.SetStatW(0.3)
    
    ## # Margins:
    CMSstyle.SetPadTopMargin(0.05)
    CMSstyle.SetPadBottomMargin(0.16)
    CMSstyle.SetPadLeftMargin(0.20)
    CMSstyle.SetPadRightMargin(0.10)

    # For the Global title:
    CMSstyle.SetOptTitle(0)
    CMSstyle.SetTitleFont(TEXTFONT)
    CMSstyle.SetTitleColor(1)
    CMSstyle.SetTitleTextColor(1)
    CMSstyle.SetTitleFillColor(10)
    CMSstyle.SetTitleFontSize(TEXTSIZE*1.1)

    # For the axis titles:
    CMSstyle.SetTitleColor(1, "XYZ")
    CMSstyle.SetTitleFont(TEXTFONT, "XYZ")
    CMSstyle.SetTitleSize(TEXTSIZE*1.1, "XYZ") # 0,05
    CMSstyle.SetTitleXOffset(0.9) # 0.9
    CMSstyle.SetTitleYOffset(1.25) # 1.25

    # For the axis labels:
    CMSstyle.SetLabelColor(1, "XYZ")
    CMSstyle.SetLabelFont(TEXTFONT, "XYZ")
    CMSstyle.SetLabelOffset(0.007, "XYZ")
    CMSstyle.SetLabelSize(TEXTSIZE, "XYZ")

    # For the axis:
    CMSstyle.SetAxisColor(1, "XYZ")
    CMSstyle.SetStripDecimals(kTRUE)
    CMSstyle.SetTickLength(0.03, "XYZ")
    CMSstyle.SetNdivisions(510, "XYZ")
    # To get tick marks on the opposite side of the frame
    CMSstyle.SetPadTickX(1)  
    CMSstyle.SetPadTickY(1)

    # Change for log plots:
    CMSstyle.SetOptLogx(0)
    CMSstyle.SetOptLogy(0)
    CMSstyle.SetOptLogz(0)

    # Postscript options:
    CMSstyle.SetPaperSize(20.,20.)
    CMSstyle.cd()
    
