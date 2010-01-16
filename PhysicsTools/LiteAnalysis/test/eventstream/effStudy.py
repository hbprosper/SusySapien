#!/usr/bin/env python
#-----------------------------------------------------------------------------
# Filename: effStudy.py
# Description: Compare generated and reconstructed electrons in the
#              region |eta| < 1.48 (the CMS barrel region). This code
#              assumes that the link zjets.root pointing to an actual
#              Z+jets file exists.
#
# Created:  26-Oct-2007 HBP
#-----------------------------------------------------------------------------
import os
import sys
from sys import exit
from ROOT import *
from math import *
from string import *
from time import sleep
from plotutil import plothist
from PhysicsTools.LiteAnalysis import *
#-----------------------------------------------------------------------------
# Constants
#-----------------------------------------------------------------------------
FILELIST = "file.list"
MATCHCUT = 0.05
JETETCUT = 10.0
ELEETCUT = 10.0
ETACUT   = 1.48
#-----------------------------------------------------------------------------
# Selectors and mappers
#-----------------------------------------------------------------------------
def jetSelector(p):
    return (p.et() > JETETCUT) and (abs(p.eta()) < ETACUT)

def electronSelector(p):
    return (p.et() > ELEETCUT) and (abs(p.eta()) < ETACUT)

def genelectronSelector(p):
    id  = abs(p.pdg_id())
    return (id == kit.ELECTRON)

def fromGenParticle(p):
    return p

def fromRecoElectron(p):
    return p.trackMomentumAtVtx()

def fromRecoElectronOut(p):
    return p.trackMomentumOut()

def lzmap(func, p):
    v = vTLorentzVector()
    for x in p: v.push_back( kit.lorentzVector( func(x) ) )
    return v
#-----------------------------------------------------------------------------
def fillPt(h1, h2, p, matches):
    for x in p:
        h1.Fill(x.Pt())
        
    for m in matches:
        if m.distance < MATCHCUT:
            x = p[m.first]
            h2.Fill(x.Pt())
#-----------------------------------------------------------------------------
def main():
    print "\n\tExample: electron efficiency study\n"

    timer = TStopwatch()
    
    #--------------------------------------
    # Open file or chain of CMS root files
    #--------------------------------------
    print "opening file: %s" % FILELIST
    #timer.Start()

    if not os.path.exists(FILELIST):fatal("unable to open file: %s" % FILELIST)
        
    records = map(strip, open(FILELIST).readlines())

    print "Files:"
    filename = vstring()
    for record in records:
        filename.push_back(record)
        print "\t%s" % record

    stream = EventStream(filename)
    if not stream.good(): fatal("unable to open file: %s" % filename[0])

    #timer.Stop()
    #rt1 = timer.RealTime()
    #cp1 = timer.CpuTime()
    
    nevent = stream.size()
    print "Number of events: %d\n" % nevent
            
    #--------------------------------------
    # Select objects to be accessed
    #--------------------------------------
    particles = GenParticleList()
    if not stream.select(particles): fatal("GenParticleList select failed")
        
    electrons = vector("reco::PixelMatchGsfElectron")()
    if not stream.select(electrons, "pixel"): fatal("electron select failed")

    jets = vector("reco::CaloJet")()
    if not stream.select(jets, "iterativecone7"): fatal("jet select failed")

    #--------------------------------------
    # Setup canvases etc.
    #--------------------------------------
    kit.setStyle()
    
    cdeltar= kit.canvas("deltaR",1)
    hdeltar= kit.histogram("hdeltaR","min #DeltaR","",50,0,0.05)

    czmass = kit.canvas("zmass",2)
    hzmass = kit.histogram("hzmass", "m_{Z} [GeV/c^{2}]","",50,0,200)
    hzmassr= kit.histogram("hzmassr","m_{Z} [GeV/c^{2}]","",50,0,200,
                           kBlue,1,3)
    hzm    = vector("TH1F*")()
    hzm.push_back(hzmass)
    hzm.push_back(hzmassr)
    
    cpte   = kit.canvas("ept",3)
    hpte1  = kit.histogram("hpte1","p_{T}(e) [GeV/c]","",20,0,250)
    hpte2  = kit.histogram("hpte2","p_{T}(e) [GeV/c]","",20,0,250)

    cptjet = kit.canvas("jetet",5)
    hptjet = kit.histogram("hptjet","p_{T}(jet) [GeV/c]","",50,0,250)

    cnjet  = kit.canvas("njet",6)
    hnjet  = kit.histogram("hnjet","n_{jet}","",10,0,10)

    czpt   = kit.canvas("zpt",7)
    hzpt   = kit.histogram("hzpt","p_{T}(Z)","",50,0,250)
        
    #--------------------------------------
    # Loop over events
    #--------------------------------------
    out = open("electrons.txt","w")
    record = "%10s%10s" % ("zmass", "zmassR")
    for k in xrange(2):
        j = k+1
        format = 7*'%10s'
        record += format % ( "energy%d"  % j,  "eta%d" % j,  "phi%d" % j,
                             "class%d"   % j,
                             "energyR%d" % j,  "etaR%d" % j, "phiR%d" % j)

        s = "nclus%d" % j
        record += '%10s' % s

        format = 3*'%10s'
        for i in xrange(5):
            n = i+1
            record += format % ("energyR%d%d" % (j,n),
                                "etaR%d%d" % (j,n),
                                "phiR%d%d" % (j,n))
    record += '\n'
    out.write(record)
    
    bcs = vector("reco::BasicCluster")()
    count = 0
    while stream.read():

        # Select events with both electrons in barrel

        ele = filter(electronSelector, electrons)
        if len(ele) < 2: continue
        count += 1

        # Select jets

        jet = filter(jetSelector, jets)

        # Get Z boson & daughters
        
        Zboson = particles.find(kit.ZBOSON)        
        if not Zboson: continue
        Zd = kit.daughters(Zboson)
        
        # Map to 4-vectors

        Z = kit.lorentzVector(Zboson)
        l = lzmap(fromGenParticle, Zd)
        L = lzmap(fromRecoElectron, ele)

        # Match between truth and reco
        
        matches = kit.matchInDeltaR(l, L)

        #--------------------------------------
        # Fill histograms
        #--------------------------------------
        if L.size() > 1:
            LL = L[0] + L[1]
            hzmassr.Fill(LL.M())

        if matches.size() > 0: hdeltar.Fill( matches[0].distance )

        hzmass.Fill(Z.M())
        hzpt.Fill(Z.Pt())
        hnjet.Fill(len(jet))
        for j in jet: hptjet.Fill(j.et())
        
        # Fill pts for barrel leptons

        fillPt(hpte1, hpte2, l, matches)

        #--------------------------------------
        # Plot histograms
        #--------------------------------------
        if count % 50 == 0:
            print count

            kit.plot(cdeltar, hdeltar)
            kit.plot(czmass, hzm)
            kit.plot(cpte, hpte1)
            kit.plot(cptjet, hptjet)
            kit.plot(cnjet, hnjet)
            kit.plot(czpt, hzpt)

        # Write out detailed info about electrons

        if (matches.size() > 1) and (matches[1].distance < MATCHCUT):
            
            record = "%10.2f%10.2f" % (Z.M(), LL.M())
            
            for k in xrange(2):

                e = l[matches[k].first]
                E = L[matches[k].second]
                P = ele[matches[k].second]  
                format = 6*'%10.3f' + '%10d'
                record += format % (e.Energy(), e.Eta(), e.Phi(),
                                    E.Energy(), E.Eta(), E.Phi(),
                                    P.classification())
                
                kit.clusters(P.superCluster(), bcs)
                record += '%10d' % bcs.size()

                format = 3*'%10.3f'
                for n in xrange(4):
                    if n < bcs.size():
                        record += format % (bcs[n].energy(),
                                            bcs[n].eta(),
                                            bcs[n].phi())
                    else:
                        record += format % (0,0,0)
            record += '\n'
            out.write(record)

    out.close()

    # Plot everything once more

    kit.plot(cdeltar, hdeltar, True)
    czmass.cd()
    hzmass.Draw()
    hzmassr.Draw("same")
    czmass.Update()
    czmass.SaveAs(".gif")

    kit.plot(czmass,hzm, True)
    kit.plot(cpte,  hpte1, True)
    kit.plot(cptjet,hptjet, True)
    kit.plot(cnjet, hnjet, True)
    kit.plot(czpt,  hzpt, True)
    
    # Construct efficiency histograms
    
    heffe = kit.divideHistograms(hpte2, hpte1, "Efficiency")
    heffe.SetMinimum(0)
    heffe.SetMaximum(1)
    ceffe = kit.canvas("eeff", 8)
    kit.plot(ceffe, heffe, True, True)

    raw_input("hit return to continue ")
    
#---------------------------------------------------------------------------
main()




