#-------------------------------------------------------------------------
# Created: Sat Jan 16 21:45:40 2010 by mkntuplecfi.py
#-------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
demo =\
cms.EDAnalyzer("Mkntuple",
               debugLevel = cms.untracked.int32(0),

               ntupleName = cms.untracked.string("ntuple.root"),
               variables  =
               cms.untracked.
               vstring(
    "recoBeamSpot                    offlineBeamSpot                   1",
    #---------------------------------------------------------------------
    "double   x0()",
    "double   x0Error()",
    "double   y0()",
    "double   y0Error()",
    "double   z0()",
    "double   z0Error()",
    "/",
    "patJet                          cleanLayer1Jets                 500",
    #---------------------------------------------------------------------
    "double   energy()",
    "double   et()",
    "double   eta()",
    "double   phi()",
    'float   bDiscriminator("simpleSecondaryVertexBJetTags")',
    "float   emEnergyFraction()",
    "float   energyFractionHadronic()",
    "float   jetArea()",
    "float   towersArea()",
    "/",
    "patMET                          layer1METs                      500",
    #---------------------------------------------------------------------
    "bool   isCaloMET()",
    "bool   isRecoMET()",
    "double   energy()",
    "double   etFractionHadronic()",
    "double   eta()",
    "double   metSignificance()",
    "double   phi()",
    "double   pt()",
    "/",
    "patMuon                         cleanLayer1Muons                500",
    #---------------------------------------------------------------------
    "double   energy()",
    "double   et()",
    "double   eta()",
    "double   phi()",
    "double   pt()",
    "float   caloIso()",
    "float   ecalIso()",
    "float   hcalIso()",
    "float   trackIso()",
    "int   charge()",
    "/",
    "recoTrack                       generalTracks                   500",
    #---------------------------------------------------------------------
    "double   chi2()",
    "double   d0()",
    "double   dz()",
    "double   eta()",
    "double   normalizedChi2()",
    "double   phi()",
    "double   pt()",
    "int   charge()",
    "/"
    )
               )
