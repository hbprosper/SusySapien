#-------------------------------------------------------------------------
# Created: Sat Feb 13 21:09:33 2010 by ntuplecfi.py
#-------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
demo =\
cms.EDAnalyzer("Mkntuple",
               ntupleName = cms.untracked.string("ntuple.root"),

               buffers =
               cms.untracked.
               vstring(
    'recoBeamSpot',
    'recoCaloJet',
    'recoMuon',
    'recoTrack',
    'recoTrack1',
    'recoTrack2'
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
               recoCaloJet =
               cms.untracked.
               vstring(
    'recoCaloJet                     antikt5CaloJets                 500',
    #---------------------------------------------------------------------
    ' double   energy()',
    '  float   energyFractionHadronic()',
    ' double   et()',
    ' double   eta()',
    ' double   p()',
    ' double   phi()',
    ' double   pt()',
    '  float   emEnergyFraction()',
    '  float   jetArea()'
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
    ' double   track()->d0()'
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
    ' double   d0()',
    ' double   normalizedChi2()'
    ),
               recoTrack1 =
               cms.untracked.
               vstring(
    'recoTrack                       globalMuons                     500',
    #---------------------------------------------------------------------
    '    int   charge()',
    ' double   eta()',
    ' double   p()',
    ' double   phi()',
    ' double   pt()',
    ' double   chi2()',
    ' double   d0()',
    ' double   normalizedChi2()'
    ),
               recoTrack2 =
               cms.untracked.
               vstring(
    'recoTrack                       standAloneMuons                 500',
    #---------------------------------------------------------------------
    '    int   charge()',
    ' double   eta()',
    ' double   p()',
    ' double   phi()',
    ' double   pt()',
    ' double   chi2()',
    ' double   d0()',
    ' double   normalizedChi2()'
    )
               )
