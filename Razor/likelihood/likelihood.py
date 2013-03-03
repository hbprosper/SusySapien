#!/usr/bin/env python
#$Revision: 1.2 $
#----------------------------------------------------------------------
# Created:    Feb-2013 SS
# Updated: 03-Mar-2013 HBP - Add data set to workspace - this is needed
#                            for the BAT rooInterface
#                          - Add uniform prior for all parameters
#----------------------------------------------------------------------
import os,sys,time
from ROOT import *
from array import array
#----------------------------------------------------------------------

def main():

    # Read the arguments

    # Sezen's ROOT style:
    gROOT.SetStyle('Plain')
    gStyle.SetOptStat(kFALSE)
    gStyle.SetPalette(1)
    
    gStyle.SetTextFont(42)
    
    gStyle.SetTitleStyle(0000)
    gStyle.SetTitleBorderSize(0)    


    # Read the RooDataSet from the input file

    rootfilename = 'rzrBTHAD_QCD_MR600.0_R0.2_Had.root'
    #rootfilename = 'rzrBTHAD_data_MR600.0_R0.2_Had.root'
    #rootfilename = 'rzrBTHAD_TTJets_MR600.0_R0.2_Had.root'
    datasetname = 'RMRTree'
    nbins = 10
    nvars = 2
    
    rootfile = TFile(rootfilename)
    dataset = rootfile.Get(datasetname)

    dataset.Print()

    # KDTreeBinning only accepts a dataset that has a size that is an exact multiple
    # of the number of bins
    # We modify the dataset size to fulfil that requirement
    k = int(dataset.numEntries() / nbins)
    datasetsize = k*nbins
    print 'Datasetsize', datasetsize


    # Get the numbers from the RooDataSet
    # original numbers
    lMR = []
    lRsq = []
    # normalized numbers for KDTreeBinning
    lsMR = []
    lsRsq = []

    # Find min and max for MR and Rsq, which will be used for scaling R2 and MR
    # so that they are between -1 and 1
    Rsqmn = TMath.Infinity()
    Rsqmx = -TMath.Infinity()
    MRmn = TMath.Infinity()
    MRmx = -TMath.Infinity()

    for i in range(0, datasetsize):
        row = dataset.get(i)
        MR = row['MR'].getVal()
        Rsq = row['Rsq'].getVal()
        lMR.append(MR)
        lRsq.append(Rsq)
        #print Rsq
        if MR <= MRmn:
            MRmn = MR
        if MR >= MRmx:
            MRmx = MR
        if Rsq <= Rsqmn:
            Rsqmn = Rsq
        if Rsq >= Rsqmx:
            Rsqmx = Rsq 
        #print MR, Rsq
    print 'Rsqmn:', Rsqmn

    # scale MR and Rsq
    for i in range(0, datasetsize):    
        MR = lMR[i]
        Rsq = lRsq[i]
        sMR = 2*MR/(MRmx - MRmn) - (MRmx + MRmn) / (MRmx - MRmn)
        sRsq = 2*Rsq/(Rsqmx - Rsqmn) - (Rsqmx + Rsqmn) / (Rsqmx - Rsqmn)

        lsMR.append(sMR)
        lsRsq.append(sRsq)


    # Put the MR, Rsq list into an array, which will be given as an input
    # to the KDTreeBinning

    ldata = lMR + lRsq
    lsdata = lsMR + lsRsq

    KDdata = array('d')
    KDdata.fromlist(lsdata)

    print len(KDdata), len(lsdata)
    

    # Create the KDTreeBinning object:

    kde = TKDTreeBinning(datasetsize, 2, KDdata, nbins)

    kde.SortBinsByDensity()

    print kde.GetBinMaxDensity(), kde.GetBinMinDensity()

    # look at the KDTB results
    for i in range(nbins):
        #print i, kde.GetBinContent(i)
        minedges = kde.GetBinMinEdges(i)
        maxedges = kde.GetBinMaxEdges(i)
        binarea = (maxedges[0] - minedges[0])*(maxedges[1] - minedges[1])
        print kde.GetBinContent(i), kde.GetBinDensity(i), kde.GetBinContent(i)/kde.GetBinDensity(i), binarea
        #print minedges[0], '< MR <', maxedges[0], ';', minedges[1], '< Rsq <', maxedges[1]


    # Plot

    nbins = kde.GetNBins()
    dim = kde.GetDim()

    binsMinEdges = kde.GetBinsMinEdges()
    binsMaxEdges = kde.GetBinsMaxEdges()

    # convert the bin edges to the actual values
    for i in range(nbins*2):
        if i % 2 == 0:
            binsMinEdges[i] = binsMinEdges[i]*(MRmx - MRmn)/2 + (MRmx + MRmn)/2
            binsMaxEdges[i] = binsMaxEdges[i]*(MRmx - MRmn)/2 + (MRmx + MRmn)/2
        else:
            binsMinEdges[i] = binsMinEdges[i]*(Rsqmx - Rsqmn)/2 + (Rsqmx + Rsqmn)/2
            binsMaxEdges[i] = binsMaxEdges[i]*(Rsqmx - Rsqmn)/2 + (Rsqmx + Rsqmn)/2
        print binsMinEdges[i], binsMaxEdges[i]


    print MRmn, MRmx, Rsqmn, Rsqmx
    #h2pol = TH2Poly("h2PolyBinTest", "KDTree binning", kde.GetDataMin(0), kde.GetDataMax(0), kde.GetDataMin(1), kde.GetDataMax(1))
    h2pol = TH2Poly("h2PolyBinTest", "KDTree binning", MRmn, 1500, Rsqmn, 0.5)
   
   
    for i in range(nbins):
        edgeDim = i*dim
        h2pol.AddBin(binsMinEdges[edgeDim], binsMinEdges[edgeDim + 1], binsMaxEdges[edgeDim], binsMaxEdges[edgeDim + 1])
        h2pol.SetBinContent(i+1, kde.GetBinDensity(i))

    #c = TCanvas('c', 'c', 600, 450)
    #c.SetBottomMargin(0.15)
    #c.SetLeftMargin(0.15)

    h2pol.SetTitle('')
    h2pol.GetXaxis().SetTitle('M_{R} [GeV]')
    h2pol.GetXaxis().CenterTitle()
    h2pol.GetXaxis().SetTitleOffset(1.25)
    h2pol.GetXaxis().SetTitleSize(0.055)
    h2pol.GetXaxis().SetTitleFont(42)
    h2pol.GetXaxis().SetLabelOffset(0.012)
    h2pol.GetXaxis().SetLabelSize(0.050)
    h2pol.GetXaxis().SetLabelFont(42)
    h2pol.GetXaxis().SetNdivisions(10, 5, 0)
    h2pol.GetYaxis().SetTitle("R^{2}")
    h2pol.GetYaxis().CenterTitle()
    h2pol.GetYaxis().SetTitleOffset(1.2)
    h2pol.GetYaxis().SetTitleSize(0.055)
    h2pol.GetYaxis().SetTitleFont(42)
    h2pol.GetYaxis().SetLabelOffset(0.012)
    h2pol.GetYaxis().SetLabelSize(0.050)
    h2pol.GetYaxis().SetLabelFont(42)    
    h2pol.GetYaxis().SetNdivisions(10, 5, 0)

    #h2pol.Draw('COL')
    #h2pol.Draw('same')

    #t1 = TLatex(0.11, 0.95, 'KDTreeBinning: data')           
    #t1.SetTextSize(0.056)
    #t1.SetNDC()
    #t1.Draw("same") 

    #c.Print('datakdtree.pdf')

    #time.sleep(10)

    #sys.exit(0)

    # Build the likelihood

    print '\n *** STARTING LIKELIHOOD *** \n'

    gSystem.Load("libRooRazor2DBackground.so") 

    _MR0 = -161.54
    _R0 = -0.0372
    _B0 = 7.25
    _N0 = 96.34
    _Ntot = 660.

    # Make the workspace
    wspace = RooWorkspace('binnedrazor')

    # The free parameters
    wspace.factory('MR[1900.00000,600.000,4000.000]')
    wspace.factory('Rsq[0.29500,0.04,1.5]')
    
    # Luminosity
    wspace.factory('L[12.07, 0, 100]')
    wspace.var('L').setConstant()
    
    # signal
    wspace.factory('sigma[0.0, 0.0, 10000.0]')
    
    # BG parametersars:
    wspace.factory('MR0[-161.54, -2000, 250.]')
    wspace.factory('R0[-0.0372,-1.,0.]')
    wspace.factory('B0[7.25,0.0001,10.0]')
    wspace.factory('N0[96.34,0.5,150.0]')
    wspace.factory('btot[660.0,0.0,10000.0]')

    # Set of all the parameters
    params = RooArgSet('parameters')
    
    # RooArgSet of all observables, i.e. data counts in each bin
    obs = RooArgSet('obs')
    # RooArgSet of BG nuisance parameters:
    nuis_b = RooArgSet('nuis_b')
    # RooArgSet of signal nuisance parameters:
    nuis_s = RooArgSet('nuis_s')
    # RooArgSet of the parameters of interest for signal:
    poi_s = RooArgSet('poi_s')
    # RooArgSet of the parameters of interest for BG:
    poi_b = RooArgSet('poi_b')    
    # Start creating the factory string expression for the product pdf
    model = 'PROD::model('        

    wspace.factory('MRmn[%(MRmn)s]' % {'MRmn' : MRmn} )
    wspace.factory('MRmx[%(MRmx)s]' % {'MRmx' : MRmx} )
    wspace.factory('Rsqmn[%(Rsqmn)s]' % {'Rsqmn' : Rsqmn} )
    wspace.factory('Rsqmx[%(Rsqmx)s]' % {'Rsqmx' : Rsqmx} )
    wspace.var('MRmn').setConstant()
    wspace.var('MRmx').setConstant()
    wspace.var('Rsqmn').setConstant()    
    wspace.var('Rsqmx').setConstant()

    # make the s:
    sigmalast = 'sigma'
    sigmas = 'sigma'

    # make the bnorm variable:
    bnorm = '1.7e-308 + '  # to avoid summed background = 0
    bslist = ''    

    for i in range(nbins):
        bin = "_%3.3d" % i

        # Get the data counts in each bin
        N = kde.GetBinContent(i)

        # Put the data counts into the workspace
        wspace.factory('N%(bin)s[%(count)d]' % {'bin': bin,  'count': N} )
        # Add the data counts to the obs RooArgSet
        obs.add(wspace.var('N%(bin)s' % {'bin': bin}) )
        # Get the min and max edges for the bins
        minedges = kde.GetBinMinEdges(i)
        maxedges = kde.GetBinMaxEdges(i)

        #print minedges[0], maxedges[0], minedges[1], maxedges[1]

        # Convert the bin edges from the normalized values to the actual values
        MRimn = minedges[0]*(MRmx - MRmn)/2 + (MRmx + MRmn)/2
        MRimx = maxedges[0]*(MRmx - MRmn)/2 + (MRmx + MRmn)/2
        Rsqimn = minedges[1]*(Rsqmx - Rsqmn)/2 + (Rsqmx + Rsqmn)/2
        Rsqimx = maxedges[1]*(Rsqmx - Rsqmn)/2 + (Rsqmx + Rsqmn)/2

        # Write the bin edges into the workspace
        wspace.factory('MR%(bin)s[%(MRimn)s, %(MRimx)s]' % {'bin' : bin, 'MRimn' : MRimn, 'MRimx' : MRimx} )
        wspace.factory('Rsq%(bin)s[%(Rsqimn)s, %(Rsqimx)s]' % {'bin' : bin, 'Rsqimn' : Rsqimn, 'Rsqimx' : Rsqimx} )
        wspace.var('MR%(bin)s' % {'bin' : bin} ).setConstant()
        wspace.var('Rsq%(bin)s' % {'bin' : bin} ).setConstant()    

        # Write the BG yield function into the workspace
        wspace.factory('Razor2DBackground::bfunc%(bin)s('\
                                   'MR%(bin)s,Rsq%(bin)s,'\
                                   'MR0,R0,B0,N0, 1)' % {'bin' : bin})
        if i != nbins - 1:
            bnorm = bnorm + 'bfunc%(bin)s + ' % {'bin' : bin}
            bslist = bslist + 'bfunc%(bin)s, ' % {'bin' : bin}
        else:
            bnorm = bnorm + 'bfunc%(bin)s' % {'bin' : bin}
            bslist = bslist + 'bfunc%(bin)s ' % {'bin' : bin}

    bnorm = 'expr::bnorm("'+bnorm+'", '+bslist+')'
    print bnorm
    wspace.factory(bnorm)

    #wspace.Print()

    #sys.exit(0)

    for i in range(nbins):
        bin = "_%3.3d" % i

        b = 'expr::b%(bin)s("(btot/bnorm)*bfunc%(bin)s", btot, bnorm, bfunc%(bin)s)' \
            % {'bin' : bin}
        wspace.factory(b)

        # Write the Poisson PDF for each bin
        if i < nbins-1:
            sigmalast = sigmalast + ' - sigma%(bin)s' % {'bin': bin}
            sigmas = sigmas + ',sigma%(bin)s' % {'bin': bin}
            wspace.factory('sigma%(bin)s[0.0,0.0,100.0]' % {'bin' : bin})
            # Create the string factory expression for the Poisson pdf for the bin
            s = 'expr::s%(bin)s("sigma%(bin)s * L", sigma%(bin)s, L)' \
                % {'bin' : bin}
            wspace.factory(s)
            # Add the signal counts to the nuis_s RooArgSet
            nuis_s.add(wspace.var('sigma%(bin)s' % {'bin': bin}) )
        else:
            sigmalast = 'expr::sigma%(bin)s("%(sigmalast)s", %(sigmas)s)' \
                        % {'bin' : bin, 'sigmalast' : sigmalast, 'sigmas' : sigmas}
            wspace.factory(sigmalast)
            s = 'expr::s%(bin)s("sigma%(bin)s + L", sigma%(bin)s, L)' \
                % {'bin' : bin}
            wspace.factory(s)
            # Add the signal counts to the nuis_s RooArgSet
            nuis_s.add(wspace.var('sigma'))


        # n = s + b
        n = 'expr::n%(bin)s("s%(bin)s + b%(bin)s", s%(bin)s, b%(bin)s)' \
            % {'bin' : bin}
        wspace.factory(n)

        # Poisson pdf for a single bin
        p = 'Poisson::p%(bin)s(N%(bin)s, n%(bin)s)' \
            % {'bin': bin}
        wspace.factory(p)

        # Add the Poisson pdf to the factory string expression for the product pdf, i.e. model
        if i != nbins - 1:
            model += "p%(bin)s," % {'bin': bin}
        else:
            model += "p%(bin)s" % {'bin': bin}

        #print 'Compare', i, wspace.var('b%(bin)s' % {'bin': bin}).getVal()

    # -------------------------------------------------------
    # Create a set of all parameters
    # -------------------------------------------------------
    print "\ndefineSet: parameters"
    wspace.defineSet('parameters', '%s,%s' % (sigmas, 'MR0,R0,B0,N0,btot'))
    
    # build an n-dimensional uniform prior
    wspace.factory('Uniform::prior(parameters)')

    # -------------------------------------------------------
    # Complete the model pdf string and write the model into
    # the workspace
    # -------------------------------------------------------
    model = model+')'
    print 'MODEL: ', model
    wspace.factory(model)
    
    #mr = wspace.var("N0")
    pdf = wspace.pdf("model")

    print 'Print the value:', pdf.getVal()

    #sys.exit(0)

    # add parameters of interes to the relevant RooArgSet
    poi_s.add(wspace.var('sigma'))
    # Add the set of parameters of interest to the workspace:
    wspace.defineSet('poi_s', poi_s)

    # Add BG nuissance parameters to the relevant RooArgSet
    poi_b.add(wspace.var('MR0'))
    poi_b.add(wspace.var('R0'))
    poi_b.add(wspace.var('B0'))
    poi_b.add(wspace.var('N0'))
    poi_b.add(wspace.var('btot'))
    
    # Add the set of parameters of interest to the workspace:
    wspace.defineSet('poi_b', poi_b)    

    # Add the set of signal nuisance parameters to the workspace:
    wspace.defineSet('nuis_s', nuis_s)

    # Add BG nuissance parameters to the relevant RooArgSet
    nuis_b.add(wspace.var('MR0'))
    nuis_b.add(wspace.var('R0'))
    nuis_b.add(wspace.var('B0'))
    nuis_b.add(wspace.var('N0'))
    nuis_b.add(wspace.var('btot'))
    
    # Add the set of BG nuisance parameters to the workspace:
    wspace.defineSet('nuis_b', nuis_b)

    # Add the set of observables to the workspace:
    wspace.defineSet('obs', obs)

    # Make the RooDataSet for data.
    # It has one entry which consists of data counts in each bin
    data = RooDataSet('data', 'data', obs)
    data.add(obs)
    data.Print()

    # Add RooDataSet to workspace
    getattr(wspace,'import')(data)
    
    # Create model configuration
    modelconfig = RooStats.ModelConfig("Boris")
    modelconfig.SetWorkspace(wspace)
    modelconfig.SetPdf(wspace.pdf("model"))
    modelconfig.SetPriorPdf(wspace.pdf('prior'))
    modelconfig.SetParametersOfInterest(wspace.set('poi_s'))
    modelconfig.SetNuisanceParameters(wspace.set('nuis_b'))

    # Add model configuration to workspace
    getattr(wspace,'import')(modelconfig)

    # Write workspace to file
    wspace.writeToFile("Boris.root")

    wspace.Print()

    sys.exit(0)

    # Use profile likelihood:
    print 'Starting profile lh...'
    plc = RooStats.ProfileLikelihoodCalculator(data, modelconfig)
    plc.SetConfidenceLevel(0.95)
    plInt = plc.GetInterval()
    msglevel = RooMsgService.instance().globalKillBelow()

    RooMsgService.instance().setGlobalKillBelow(RooFit.FATAL)
    plInt.LowerLimit(wspace.var('sigma'))
    RooMsgService.instance().setGlobalKillBelow(msglevel)
    print wspace.var('sigma').getVal()    

    #sys.exit(0)

    # Make the plot for the profile:
    lrplot = RooStats.LikelihoodIntervalPlot(plInt)
    lrplot.Draw()

    


#-----------------------------------------------------------------
try:
    main()
except KeyboardInterrupt:
    print
    print "bye cruel world!"
