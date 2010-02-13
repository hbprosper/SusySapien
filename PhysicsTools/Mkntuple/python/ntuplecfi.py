#-------------------------------------------------------------------------
# Created: Sat Feb 13 18:27:17 2010 by ntuplecfi.py
#-------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
demo =\
cms.EDAnalyzer("Mkntuple",
               ntupleName = cms.untracked.string("ntuple.root"),

               buffers =
               cms.untracked.
               vstring(
    'recoBeamSpot',
    'recoMuon',
    'recoTrack'
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
    '   bool   isTrackerMuon()'
    ),
               recoTrack =
               cms.untracked.
               vstring(
    'recoTrack                       generalTracks                   500',
    #---------------------------------------------------------------------
    '    int   charge()',
    ' double   eta()',
    ' double   p()',
    ' double   phi()',
    ' double   pt()',
    ' double   chi2()',
    ' double   d0()'
    )    'recoTrack                       standAloneMuons                 500',
    #---------------------------------------------------------------------
    '    int   charge()',
    ' double   eta()',
    ' double   p()',
    ' double   phi()',
    ' double   pt()',
    ' double   chi2()',
    ' double   d0()'
    )
               )
