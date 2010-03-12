#-------------------------------------------------------------------------
# Created: Fri Mar 12 17:24:35 2010 by ntuplecfi.py
#-------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
demo =\
cms.EDAnalyzer("Mkntuple",
               ntupleName = cms.untracked.string("ntuple.root"),
               analyzerName = cms.untracked.string("analyzer.cc"),

               buffers =
               cms.untracked.
               vstring(
    'GenEventInfoProduct',
    'GenRunInfoProduct',
    'recoBeamSpot',
    'patJet',
    'patMuon',
    'recoGenParticle'
    ),
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
    'recoBeamSpot                    offlineBeamSpot                   1',
    #---------------------------------------------------------------------
    ' double   x0()',
    ' double   y0()',
    ' double   z0()'
    ),
               patJet =
               cms.untracked.
               vstring(
    'patJet                          cleanLayer1JetsAK5              500',
    #---------------------------------------------------------------------
    ' double   energy()',
    ' double   et()',
    ' double   eta()',
    ' double   phi()',
    '   bool   isCaloJet()'
    ),
               patMuon =
               cms.untracked.
               vstring(
    'patMuon                         cleanLayer1Muons                500',
    #---------------------------------------------------------------------
    '    int   charge()',
    ' double   energy()',
    ' double   eta()',
    ' double   phi()',
    ' double   pt()',
    '  float   caloIso()',
    '  float   ecalIso()',
    ' double   ecalIsoDeposit()->candEnergy()',
    '  float   hcalIso()',
    '  float   trackIso()'
    ),
               recoGenParticle =
               cms.untracked.
               vstring(
    'recoGenParticle                 genParticles                    500',
    #---------------------------------------------------------------------
    '    int   charge()',
    ' double   et()',
    ' double   eta()',
    ' double   phi()',
    ' double   mass()',
    '    int   pdgId()',
    '    int   status()'
    )
               )
