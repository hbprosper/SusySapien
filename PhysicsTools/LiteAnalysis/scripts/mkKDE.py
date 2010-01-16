#!/usr/bin/env python
#-----------------------------------------------------------------------------
# Filename: mkKDE.py
# Description: Build and test KDE-based discriminant
# Created:  28-Apr-2008 HBP - Ecole Polytechnique
#-----------------------------------------------------------------------------
import sys, os
from sys import exit
from ROOT import *
from math import *
from string import *
from time import sleep, ctime
#-----------------------------------------------------------------------------
argv = sys.argv[1:]
if len(argv) < 3:
    print "Usage:\n\t./mkKDE.py <bkg-file> <sig-file> <name>"
    exit(0)
#-----------------------------------------------------------------------------
from PhysicsTools.LiteAnalysis import *
#-----------------------------------------------------------------------------
def fillandplot(stream, vars, sigkde, bkgkde, canvas, hname, color, fstyle):

    h = kit.histogram(hname, "kde output", "", 50, 0, 1, color, 1, 2)
    h.SetFillColor(color)
    h.SetFillStyle(fstyle)

    out = open(hname+".out","w")
    count = 0
    
    while stream.read():
        entry  = stream.entry()
        weight = stream.get("weight")
        input  = stream.vget(vars)
        S = sigkde.density(input)
        B = bkgkde.density(input)
        D = S+B
        if D > 0:
            y = S/D
            weight = 1
            h.Fill(y, weight)

        if count < 1000:
            out.write("%10d%10.4f\n" % (entry, y))
        count += 1
        
        if entry % 100 == 0:
            canvas.cd()
            h.Draw()
            canvas.Update()

    out.close()
    canvas.cd()
    h.Draw()
    canvas.Update()
    return h
#-----------------------------------------------------------------------------
def mkKDE(stream, vars, name, ntrain):
    d = vvdouble()
    print "building %s.cpp..." % name
    count = 0
    kde = KDE()
    while stream.read():
        kde.add( stream.vget(vars) )
        if count % 500 == 0: print count
        count += 1
        if count >= ntrain: break
    kde.write(name)
    print "done!\n"
    return kde
#-----------------------------------------------------------------------------
def write(name):
    filename = name + "_sig.cpp"
    if not os.path.exists(filename):
        print "Can't find %s" % filename
    S = open(filename).read()

    filename = name + "_bkg.cpp"
    if not os.path.exists(filename):
        print "Can't find %s" % filename
    B = open(filename).read()

    record = '''//-----------------------------------------------------------------------------
// File: %(name)s.cpp
// Description: Kernel Density Estimate of y(x) = s(x)/[s(x) + b(x)]
// Created: %(time)s << " by KDE - PhysicsTools/LiteAnalysis
//-----------------------------------------------------------------------------
#include <vector>
double %(name)s_sig(std::vector<double>&);
double %(name)s_bkg(std::vector<double>&);
//-----------------------------------------------------------------------------
double %(name)s(std::vector<double>& input)
{
  double s = %(name)s_sig(input);
  double b = %(name)s_bkg(input);
  double d = s + b;
  if(d > 0)
    return s / d;
  else
    return -1;
}
''' % {'name': name,
       'time': ctime()}
    open("%s.cpp" % name, "w").write(record)
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
    #-----------------------------------------------------------------------
    # Histograms
    #-----------------------------------------------------------------------
    kit.setStyle()
    os.system("mkdir -p figures")
    
    canvas = TCanvas("figures/fig_%s_train" % dname,
                     "KDE", 700, 0, 500, 500)
    
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

    sig = Slurper(sigfile)
    bkg = Slurper(bkgfile)

    k = min(sig.size(), bkg.size())/ 2
    k = k / 500
    ntrain = k * 500

    print "Number of training events/file %s\n" % ntrain

    #-----------------------------------------------------------------------
    # Define signal and background KDEs
    #-----------------------------------------------------------------------
 
    sigkde = mkKDE(sig, vars, "%s_sig" % dname, ntrain)
    bkgkde = mkKDE(bkg, vars, "%s_bkg" % dname, ntrain)

    # Write discriminant function
    
    write(dname)
    
    # Fill & Plot S/(S+B)

    file = TFile(dname+".root", "RECREATE")
    hs = fillandplot(sig, vars, sigkde, bkgkde, canvas, "hs", kRed, 3004)
    hb = fillandplot(bkg, vars, sigkde, bkgkde, canvas, "hb", kBlue,3005)

    canvas.cd()
    if hs.GetMaximum() > hb.GetMaximum():
        hs.DrawNormalized()
        hb.DrawNormalized("SAME")
    else:
        hb.DrawNormalized()
        hs.DrawNormalized("SAME")
    canvas.Update()
    canvas.SaveAs(".gif")

    file.Write()
    file.Close()
    
    sleep(5)
#-----------------------------------------------------------------------------
main()





