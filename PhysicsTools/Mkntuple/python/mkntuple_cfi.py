#-------------------------------------------------------------------------
# Created: Fri Jan 15 01:43:28 2010 by mkntuplecfi.py
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
    "patElectron                     cleanLayer1Electrons            500",
    #---------------------------------------------------------------------
    "double   energy()",
    "double   eta()",
    "double   phi()",
    "double   pt()",
    "double ecalIsoDeposit_candEnergy()",
    "double ecalIsoDeposit_eta()",
    "double ecalIsoDeposit_phi()",
    "double gsfTrack_chi2()",
    "double gsfTrack_d0()",
    "double gsfTrack_dxy()",
    "double gsfTrack_normalizedChi2()",
    "double hcalIsoDeposit_candEnergy()",
    "double hcalIsoDeposit_eta()",
    "double hcalIsoDeposit_phi()",
    "double track_d0()",
    "double track_dxy()",
    "float   caloEnergy()",
    "float   ecalIso()",
    "float   hcalIso()",
    "float   trackIso()",
    "int   charge()",
    "/"
    )
               )
