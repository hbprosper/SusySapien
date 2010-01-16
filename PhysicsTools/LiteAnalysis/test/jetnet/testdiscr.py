#!/usr/bin/env python
#-----------------------------------------------------------------------------
# File:        testdiscr.py
# Description: test a trained network
# Created:     04-Feb-2006 Harrison B. Prosper
#              13-Feb-2006 HBP
#-----------------------------------------------------------------------------
import sys, os
from sys import exit
from ROOT import *
from math import *
from string import *
from time import sleep
#-----------------------------------------------------------------------------
argv = sys.argv[1:]
if len(argv) < 4:
    print "Usage:\n\t./testdiscr.py <bkg-file> <sig-file> <name> <title>" \
          "\n\t\tneeds input.var\n"
    exit(0)
#-----------------------------------------------------------------------------
from PhysicsTools.LiteAnalysis import *
#-----------------------------------------------------------------------------
KBIN = 50
#-----------------------------------------------------------------------------
def fillandplot(hname, filename, discriminant, vars, canvas, color, lstyle):
    h = kit.histogram(hname, "network output", "", KBIN, 0, 1, color, 1, 2)
    h.SetMinimum(0)
    h.SetFillColor(color)
    h.SetFillStyle(lstyle)

    stream = Slurper(filename)
    ntotal = stream.size()
    print filename, ntotal

    w = 0.0
    while stream.read():
        entry  = stream.entry()
        weight = stream.get("weight")
        input  = stream.vget(vars)

        w += weight
        y = eval(discriminant)
        h.Fill(y, weight)
            
        if entry % 100 == 0:
            canvas.cd()
            h.Draw()
            canvas.Update()

    canvas.cd()
    h.Draw()
    canvas.Update()
    sleep(1)
    return (w, h)

def lumiscale(z, lumi, y):
    f = sqrt(lumi)
    for i in range(len(y)): z[i] = f * y[i]
#-----------------------------------------------------------------------------
def main():

    bkgfile = argv[0]
    if not os.path.exists(bkgfile):
        print "Can't find ", bkgfile
        exit(0)
        
    sigfile = argv[1]
    if not os.path.exists(sigfile):
        print "Can't find ", sigfile
        exit(0)

    dname = argv[2]
    title = argv[3]
    
    #-----------------------------------------------------------------------
    # Load discriminant
    #-----------------------------------------------------------------------
    sigfunc = "%s_sig.cpp" % dname
    bkgfunc = "%s_bkg.cpp" % dname
    if os.path.exists(sigfunc) and os.path.exists(bkgfunc):
        command = ".L %s+" % sigfunc
        gROOT.ProcessLine(command)            
        command = ".L %s+" % bkgfunc
        gROOT.ProcessLine(command)
    command = ".L %s.cpp+" % dname
    gROOT.ProcessLine(command)
    discriminant = "%s(input)"  % dname

    #-----------------------------------------------------------------------
    # Get variable names and data
    #-----------------------------------------------------------------------
    vlist = filter(lambda x: x != "",
                   map(strip, open("input.var").readlines()))
    vars = vstring()
    print "Variables"
    for x in vlist:
        print "\t%s" % x
        vars.push_back(x)

    #-----------------------------------------------------------------------
    # Fill & Plot
    #-----------------------------------------------------------------------
    file = TFile("%s_test.root" % dname, "RECREATE")

    kit.setStyle()
    os.system("mkdir -p figures")

    canvas = TCanvas("figures/fig_%s_test" % dname,
                     title, 100, 0, 500, 500)
    
    ws, hs = fillandplot("hs", sigfile, discriminant, vars, canvas,kRed,3004)
    wb, hb = fillandplot("hb", bkgfile, discriminant, vars, canvas,kBlue,3005)
    
    canvas.cd()
    if hs.GetMaximum() > hb.GetMaximum():
        hs.DrawNormalized()
        hb.DrawNormalized("same")
    else:
        hb.DrawNormalized()
        hs.DrawNormalized("same")
    canvas.Update()
    canvas.SaveAs(".gif")

    #-----------------------------------------------------------------------
    # 
    #-----------------------------------------------------------------------
    c = TCanvas("figures/fig_%s_test_unscaled" % dname,
                title, 300, 100, 500, 500)

    ymax = hs.GetMaximum()
    hs.SetMinimum(ymax/1000)
    hb.SetMinimum(ymax/1000)

    c.cd()
    gPad.SetLogy(1)
    hb.GetYaxis().SetTitle("cross section [fb]")
    hb.Draw()
    
    hs.Draw("same")
    c.Update()
    c.SaveAs(".gif")

    print "Signal cross section:     %10.2f fb" % ws
    print "Background cross section: %10.2f fb" % wb
    #-----------------------------------------------------------------------
    csignif = TCanvas("figures/fig_%s_test_signif" % dname,
                      title, 500, 200, 500, 500)

    cs = kit.contents(hs)
    cb = kit.contents(hb)
    x  = vdouble(len(cs))
    y  = vdouble(len(cs))
    z  = vdouble(len(cs))

    step = 1.0/KBIN
    for i in range(len(x)):
        x[i] = (i+0.5)*step
        s = cs[i]
        b = cb[i]
        if b > 0:
            y[i] = sqrt(2*((s+b)*log(1.0+s/b)-s))
        else:
            y[i] = 0.0
        
    g1 = kit.graph(x, y, "network output",
                   "#sqrt{2[(s+b)ln(1+s/b)-s]}",
                   0.5,1,0,5,  kRed)

    lumiscale(z, 10, y)
    g10 = kit.graph(x, z, "", "", 0.5,1,0,5, kGreen)

    lumiscale(z, 20, y)
    g20 = kit.graph(x, z, "", "", 0.5,1,0,5, kBlue)
    
    lumiscale(z, 30, y)
    g30 = kit.graph(x, z, "", "", 0.5,1,0,5, kBlack)
    
    legend = kit.legend("discovery potential", 4, 0.4,0.7)
    legend.AddEntry(g30, "L = 30 fb^{-1}")
    legend.AddEntry(g20, "L = 20 fb^{-1}")
    legend.AddEntry(g10, "L = 10 fb^{-1}")
    legend.AddEntry(g1,  "L =  1 fb^{-1}")
    
    csignif.cd()

    g1.Draw("AC")
    g10.Draw("C same")
    g20.Draw("C same")
    g30.Draw("C same")
    legend.Draw("same")
    
    csignif.Update()
    csignif.SaveAs(".gif")

    file.Write()
    sleep(10)
#-----------------------------------------------------------------------------
main()

