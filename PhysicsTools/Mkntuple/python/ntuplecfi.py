#-------------------------------------------------------------------------
# Created: Tue Aug 24 22:06:57 2010 by /home/harry/CMS/cmssw/CMSSW_3_6_1/src/PhysicsTools/Mkntuple/test/ntuplecfi.py
#-------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
demo =\
cms.EDAnalyzer("Mkntuple",
               ntupleName = cms.untracked.string("ntuple.root"),
               analyzerName = cms.untracked.string("analyzer.cc"),

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
	'triggerBits'
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
    'HcalNoiseSummary                hcalnoise',
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
               triggerBits =
               cms.untracked.
               vstring(
    "triggerBits                     TriggerResults::HLT                1",
    #---------------------------------------------------------------------
    '   bool   value("HLT_Jet15U")   Jet15U',
	'   bool   value("HLT_Jet30U")   Jet30U',
	'   bool   value("HLT_Jet50U")   Jet50U',
	'   bool   value("HLT_L1Jet6U")   L1Jet6U'	
    )			   			   
			   
               )
