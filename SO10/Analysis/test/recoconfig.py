#$Revision: 1.1 $ example.py
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
# Created: Thu Feb 18 22:39:24 2010 by ntuplecfi.py
#-------------------------------------------------------------------------
process.demo =\
cms.EDAnalyzer("Mkntuple",
               ntupleName = cms.untracked.string("ntuple_reco.root"),

               buffers =
               cms.untracked.
               vstring(
    'recoBeamSpot',
    'recoMuon',
    'recoGenParticle',
    'GenMother'
    ),
               recoBeamSpot =
               cms.untracked.
               vstring(
    'recoBeamSpot                    offlineBeamSpot                   1',
    #---------------------------------------------------------------------
    ' double   x0()',
    ' double   y0()',
    ' double   z0()'
    ),

               recoMuon =
               cms.untracked.
               vstring(
    'recoMuon                        muons                           500',
    #---------------------------------------------------------------------
    '    int   charge()',
    ' double   energy()',
    ' double   et()',
    ' double   eta()',
    ' double   p()',
    ' double   phi()',
    ' double   pt()',
    '   bool   isCaloMuon()',
    '   bool   isGlobalMuon()',
    '   bool   isStandAloneMuon()',
    '   bool   isTrackerMuon()',
	' double   track()->chi2()',
	'  float   isolationR03().sumPt',
	'  float   isolationR03().emEt',
	'  float   isolationR03().hadEt',
	'  int     isolationR03().nTracks'
    ),

               recoGenParticle =
               cms.untracked.
               vstring(
    "recoGenParticle                 genParticles                   4000",
    #---------------------------------------------------------------------
    "    int   charge()",
    "    int   pdgId()",
    "    int   status()",
    " double   pt()",
    " double   eta()",
    " double   phi()",
    " double   mass()"
    ),

               GenMother =
               cms.untracked.
               vstring(
    "GenMother                         genParticles                  4000",
    #---------------------------------------------------------------------
    "    int   charge()",
    "    int   pdgId()",
    "    int   status()",
    " double   pt()",
    " double   eta()",
    " double   phi()",
    " double   mass()"
    )
               )

process.p = cms.Path(process.demo)
