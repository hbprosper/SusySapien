#-------------------------------------------------------------------------
# Created: Mon Feb 15 20:59:39 2010 by ntuplecfi.py
#-------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
demo =\
cms.EDAnalyzer("Mkntuple",
               ntupleName = cms.untracked.string("ntuple.root"),

               buffers =
               cms.untracked.
               vstring(
    'recoBeamSpot',
    'patMuon',
    'recoGenParticle'
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
               patMuon =
               cms.untracked.
               vstring(
    'patMuon                         cleanLayer1Muons                500',
    #---------------------------------------------------------------------
    '    int   charge()',
    ' double   energy()',
    ' double   et()',
    ' double   eta()',
    ' double   p()',
    ' double   phi()',
    ' double   pt()'
    ),
               recoGenParticle =
               cms.untracked.
               vstring(
    'recoGenParticle                 genParticles                    500',
    #---------------------------------------------------------------------
    '    int   charge()',
    ' double   energy()',
    ' double   px()',
    ' double   py()',
    ' double   pz()',
    '    int   pdgId()',
    '    int   status()'
    )
               )
