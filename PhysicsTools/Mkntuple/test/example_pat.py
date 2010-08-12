#$Revision: 1.1 $ example.py
#------------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
							fileNames =
							cms.untracked.vstring("file:pat.root"
												  )
							)

#-------------------------------------------------------------------------
# Created: Sun Feb  7 22:35:29 2010 by mkntuplecfi.py
# $Revison:$
#-------------------------------------------------------------------------
process.demo =\
cms.EDAnalyzer("Mkntuple",

			   # Name of output n-tuple file
               ntupleName = cms.untracked.string("ntuple_pat.root"),

			   # Name of analyzer
			   analyzerName = cms.untracked.string("analyzer.cc"),
			   			   
			   # List of buffers to allocate.
			   #----------------------------------------------------------
			   # The names are arbitrary so long as each agrees with
			   # the name of a vstring. For example, since the vstring
			   # "buffers" contains the name "recoBeamSpot", the code
			   # expects a vstring of that name to exist.
			   #----------------------------------------------------------
               buffers =
               cms.untracked.
               vstring(
	'GenEventInfoProduct',
    'GenRunInfoProduct',
    'recoBeamSpot',
	'patMET',
    'patMuon',
	'patElectron',
    'GParticle',
	'triggerBits'
    ),
			   #----------------------------------------------------------
			   # Format of 1st line:
			   #   buffer-name         getByLabel         maximum-count
			   #
			   # Format of subsequent lines:
			   #   [return-type] method [alias]
			   #----------------------------------------------------------
               GenEventInfoProduct =
               cms.untracked.
               vstring(
    'GenEventInfoProduct             generator                         1',
    #---------------------------------------------------------------------
    ' double   weight()'
    ),
               GenRunInfoProduct =
               cms.untracked.
               vstring(
    'GenRunInfoProduct               generator                         1',
    #---------------------------------------------------------------------
    ' double   externalXSecLO().value()',
    ' double   externalXSecNLO().value()',
    ' double   filterEfficiency()',
    ' double   internalXSec().value()'
    ),			   
               recoBeamSpot =
               cms.untracked.
               vstring(
    "recoBeamSpot                    offlineBeamSpot                   1",
    #---------------------------------------------------------------------
    " double   x0()",
    " double   y0()",
    " double   z0()"
    ),
               patMET =
               cms.untracked.
               vstring(
    "patMET                         layer1METsAK5                50",
    #---------------------------------------------------------------------
    " double   et()",
    " double   phi()",
    " double   pt()",
	" double   genMET()->pt()"
    ),

               patMuon =
               cms.untracked.
               vstring(
    "patMuon                         cleanLayer1Muons                50",
    #---------------------------------------------------------------------
    "    int   charge()",
    " double   energy()",
    " double   et()",
    " double   eta()",
    " double   p()",
    " double   phi()",
    " double   pt()",
    "  float   caloIso()",
    "  float   ecalIso()",
    " double   ecalIsoDeposit()->candEnergy()",
    "  float   hcalIso()",
    "  float   trackIso()",
    "   bool   isCaloMuon()",
    "   bool   isGlobalMuon()",
    "   bool   isStandAloneMuon()",
    "   bool   isTrackerMuon()"
    ),

               patElectron =
               cms.untracked.
               vstring(
    "patElectron                      cleanLayer1Electrons            50",
    #---------------------------------------------------------------------
    "    int   charge()",
    " double   energy()",
    " double   et()",
    " double   eta()",
    " double   p()",
    " double   phi()",
    " double   pt()",
	" double   gsfTrack()->d0()",
	" double   gsfTrack()->phi()"
    ),

			   GParticle =
               cms.untracked.
               vstring(
    "GParticle                 genParticles                    4000",
    #---------------------------------------------------------------------
    "    int   charge()",
    "    int   pdgId()",
    "    int   status()",
    " double   pt()",
    " double   eta()",
    " double   phi()",
    " double   mass()",
	"    int   firstMother()",
	"    int   lastMother()",
	"    int   firstDaughter()",
	"    int   lastDaughter()"
    ),
               triggerBits =
               cms.untracked.
               vstring(
    "triggerBits                     TriggerResults                     1",
    #---------------------------------------------------------------------
    '   bool   value("HLT_L1Jet15")',
	'   bool   value("HLT_Jet30")',
	'   bool   value("HLT_Jet50")',
	'   bool   value("HLT_Jet80")',
	'   bool   value("HLT_Mu9")'
    )			   			   			   
               )

process.p = cms.Path(process.demo)