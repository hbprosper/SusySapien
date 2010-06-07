#$Revision: 1.5 $ example.py
#------------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

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

			   # Name of output n-tuple
               ntupleName = cms.untracked.string("ntuple_recodata.root"),

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
    'recoBeamSpot',
	'recoCaloJet',
	'triggerBits'
    ),
			   #----------------------------------------------------------
			   # Format of 1st line:
			   #   buffer-name         getByLabel         maximum-count
			   #
			   # Format of subsequent lines:
			   #   [return-type] method [alias]
			   #----------------------------------------------------------
               recoBeamSpot =
               cms.untracked.
               vstring(
    "recoBeamSpot                    offlineBeamSpot                   1",
    #---------------------------------------------------------------------
    " double   x0()",
    " double   y0()",
    " double   z0()"
    ),
               recoCaloJet =
               cms.untracked.
               vstring(
    "recoCaloJet                     ak5CaloJets                    100",
    #---------------------------------------------------------------------
    " double   energy()",
    " double   et()",
    " double   eta()",
    " double   p()",
    " double   phi()",
    " double   pt()",
    "  float   jetArea()",
    "  float   pileup()"
    ),

               triggerBits =
               cms.untracked.
               vstring(
    "triggerBits                     TriggerResults::HLT                1",
    #---------------------------------------------------------------------
    '   bool   value("HLT_Jet15U")   Jet15U',
	'   bool   value("HLT_Jet30U")   Jet30U',
	'   bool   value("HLT_Jet50U")   Jet50U'
    )			   			   			   
			   
               )

process.p = cms.Path(process.demo)
