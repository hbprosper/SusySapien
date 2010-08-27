#$Revision: 1.1 $ example.py
#------------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
							fileNames =
							cms.untracked.vstring("file:reco.root"
												  )
							)

#-------------------------------------------------------------------------
# Created: Sun Feb  7 22:35:29 2010 by mkntuplecfi.py
# $Revison:$
#-------------------------------------------------------------------------
process.demo =\
cms.EDAnalyzer("Mkntuple",
               ntupleName = cms.untracked.string("ntuple_muskim.root"),
               analyzerName = cms.untracked.string("analyzer_muskim.cc"),

               buffers =
               cms.untracked.
               vstring(
	'edmEvent',
    'HcalNoiseSummary',
    'recoCaloJet',
    'recoCaloMET',
    'recoCaloMET1',
    'recoHcalNoiseRBX',
    'recoVertex',
	'TriggerResultsAddon'
    ),
               edmEvent =
               cms.untracked.
               vstring(
    'edmEvent',
    #---------------------------------------------------------------------
    '   int   id().run()',
	'   int   id().event()',
	'   int   luminosityBlock()',
	'   int   bunchCrossing()'
    ),			   
               HcalNoiseSummary =
               cms.untracked.
               vstring(
    'HcalNoiseSummary                hcalnoise                         1',
    #---------------------------------------------------------------------
    '   bool   passHighLevelNoiseFilter()',
    '   bool   passLooseNoiseFilter()'
    ),
               recoCaloJet =
               cms.untracked.
               vstring(
    'recoCaloJet                     ak5CaloJets                     500',
    #---------------------------------------------------------------------
    ' double   energy()',
    ' double   eta()',
    '  float   etaetaMoment()',
    ' double   phi()',
    '  float   phiphiMoment()',
    ' double   pt()',
    '  float   emEnergyFraction()',
    '    int   n90()'
    ),
               recoCaloMET =
               cms.untracked.
               vstring(
    'recoCaloMET                     met                             500',
    #---------------------------------------------------------------------
    ' double   et()',
    ' double   sumEt()'
    ),
               recoCaloMET1 =
               cms.untracked.
               vstring(
    'recoCaloMET                     metNoHF                         500',
    #---------------------------------------------------------------------
    ' double   et()',
    ' double   sumEt()'
    ),
               recoHcalNoiseRBX =
               cms.untracked.
               vstring(
    'recoHcalNoiseRBX                hcalnoise                       500',
    #---------------------------------------------------------------------
    ' double   caloTowerEmE()',
    ' double   caloTowerHadE()',
    '    int   idnumber()'
    ),
               recoVertex =
               cms.untracked.
               vstring(
    'recoVertex                      offlinePrimaryVertices          500',
    #---------------------------------------------------------------------
    ' double   chi2()',
    ' double   ndof()',
    ' size_t   tracksSize()',
    ' double   x()',
    ' double   y()',
    ' double   z()'
    ),
               TriggerResultsAddon =
               cms.untracked.
               vstring(
    "TriggerResultsAddon             TriggerResults::HLT               1",
    #---------------------------------------------------------------------
    '   bool   value("HLT_Jet15U")   Jet15U',
	'   bool   value("HLT_Jet30U")   Jet30U',
	'   bool   value("HLT_Jet50U")   Jet50U',
	'   bool   value("HLT_L1Jet6U")  L1Jet6U'	
    )			   			   
               )

process.p = cms.Path(process.demo)
