#!/usr/bin/env python
#-----------------------------------------------------------------------------
# File:        train.py
# Description: Created tag-rate function
# Created:     04-Feb-2006 Harrison B. Prosper
#              13-Feb-2006 HBP
#-----------------------------------------------------------------------------
import sys, os
from sys import exit
from ROOT import *
from math import *
from string import *
from time import sleep
from PhysicsTools.LiteAnalysis import *
#-----------------------------------------------------------------------------
NEPOCH = 10000  # Number of epochs
NSTEP  = 10     # Plot every NSTEP epoch
NBIN   = NEPOCH / NSTEP
KBIN   = 50
NTRAIN = 1000   # Number of training events/file
NHIDDEN= 10     # Number of hidden nodes
#-----------------------------------------------------------------------------
def readData(filename):
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
def plotHist(c, hs, hb, nn):

    b = nn.histogram(0)
    s = nn.histogram(1)

    sd = vdouble(len(s))
    bd = vdouble(len(b))
    for i in xrange(len(s)):
        sd[i] = s[i]
        bd[i] = b[i]
    kit.setContents(hs, sd)
    kit.setContents(hb, bd)

    c.cd()
    if hs.GetMaximum() > hb.GetMaximum():
        hs.Draw()
        hb.Draw("SAME")
    else:
        hb.Draw()
        hs.Draw("SAME")
    c.Update()
#-----------------------------------------------------------------------------
def plotSeries(epoch, 
               c, 
               htrain, htest, 
               powertrain, powertest):
    c.cd()
    htrain.Fill(epoch, powertrain)
    htest.Fill(epoch, powertest)
    htrain.Draw("L")
    htest.Draw("L SAME")
    c.Update()
#-----------------------------------------------------------------------------
def main():
    argv = sys.argv[1:]
    if len(argv) < 3:
        print "Usage:\n\t./trainjetnet.py <bkg-file> <sig-file> <net-name>"
        exit(0)

    bkgfile = argv[0]
    if not os.path.exists(bkgfile):
        print "Can't find ", bkgfile
        exit(0)
        
    sigfile = argv[1]
    if not os.path.exists(sigfile):
        print "Can't find ", sigfile
        exit(0)

    netname = argv[2]
    #-----------------------------------------------------------------------
    # Histograms
    #-----------------------------------------------------------------------

    kit.setStyle()
    os.system("mkdir -p figures")
    
    canvas = TCanvas("figures/fig_%s_train" % netname,
                     "Training Jetnet", 700, 0, 500, 500)
    
    hs = kit.histogram("hs", "net output", "", KBIN, 0, 1, kRed, 1, 2)
    hs.SetFillColor(kRed)
    hs.SetFillStyle(3004)
    
    hb = kit.histogram("hb", "net output", "", KBIN, 0, 1, kBlue, 1, 2)
    hb.SetFillColor(kBlue)
    hb.SetFillStyle(3005)
    
    # Power vs epoch number

    cepoch = TCanvas("figures/fig_%s_power" % netname,
                     "Power Time Series", 0, 0, 600, 300)
    
    htrain = kit.histogram("htrain", "Epoch", "Power", 
                           NBIN, 0, NEPOCH, kRed, 1, 2)
    htrain.SetMaximum(1)
    
    htest  = kit.histogram("htest", "Epoch", "Power", 
                           NBIN, 0, NEPOCH, kBlue, 1, 2)
    htest.SetMaximum(1)

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

    varmap, sig = readData(sigfile)
    varmap, bkg = readData(bkgfile)

    ntrain = min( min(len(sig), len(bkg)) / 2 , NTRAIN)
    ntest  = ntrain
    print "Number of training events/file %s\n" % ntrain 
    #-----------------------------------------------------------------------
    # Define network
    #-----------------------------------------------------------------------
    nn = jetnet.NN(vars, NHIDDEN)
    
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
    # POWER: Chip's number

    print "%4s %-20s%-20s%-20s" % ("", "RMS", "Power", "<Power>")

    apwr1 = 0.0
    apwr2 = 0.0
    avpwr1 = 0.0
    avpwr2 = 0.0
    count = 0
    for epoch in xrange(NEPOCH):

        nn.train(epoch)

        if epoch % NSTEP == 0:
            count += 1
            
            rms0 = nn.test(jetnet.NN.kTRAINING, 0.5, KBIN)
            pwr0 = nn.power()
            auc0 = 1-nn.area()

            rms1 = nn.test(jetnet.NN.kTESTING, 0.5, KBIN)
            pwr1 = nn.power()
            auc1 = 1-nn.area()
        
            plotHist(canvas, hs, hb, nn)        
            plotSeries(epoch, cepoch, htrain, htest, pwr0, pwr1)
        
            print "%4d %-10.4f%-10.4f%-10.4f%-10.4f%-10.4f%-10.4f" % \
                  (epoch, rms0, rms1, pwr0, pwr1, avpwr1, avpwr2)

            # Compute running average of test power

            if epoch > 200:
                count += 1
                apwr1 += pwr1
                apwr2 += pwr1*pwr1
                
                if count > 2:
                    avpwr1 = apwr1/count
                    avpwr2 = apwr2/count
                    avpwr2 = sqrt(avpwr2-avpwr1**2)/sqrt(float(count))
                    if pwr1 < avpwr1:
                        break
    nn.end()

    #-----------------------------------------------------------------------
    # Save weights and network function
    #-----------------------------------------------------------------------
    nn.save(netname)
    canvas.SaveAs(".gif")
    cepoch.SaveAs(".gif")
    os.system("rm -rf *.net")
#-----------------------------------------------------------------------------
main()

