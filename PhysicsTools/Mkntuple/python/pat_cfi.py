#-------------------------------------------------------------------------
# Created: Sun Feb  7 22:35:29 2010 by mkntuplecfi.py
# $Revison:$
#-------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
demo =\
cms.EDAnalyzer("Mkntuple",
               ntupleName = cms.untracked.string("ntuple.root"),

               buffers =
               cms.untracked.
               vstring(
    "recoBeamSpot",
    "patMuon",
    "recoGenParticle"
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
               patMuon =
               cms.untracked.
               vstring(
    "patMuon                         cleanLayer1Muons                500",
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
               recoGenParticle =
               cms.untracked.
               vstring(
    "recoGenParticle                 genParticles                    500",
    #---------------------------------------------------------------------
    "    int   charge()",
    " double   energy()",
    " double   et()",
    " double   eta()",
    " double   p()",
    " double   phi()",
    " double   pt()",
    " double   px()",
    " double   py()",
    " double   pz()",
    " double   mass()",
    "    int   pdgId()",
    "    int   status()"
    )
               )
