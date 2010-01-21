#-------------------------------------------------------------------------
# Created: Wed Jan 20 09:40:29 2010 by mkntuplecfi.py
#$Revision:$
#-------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
demo =\
cms.EDAnalyzer("Mkntuple",
               ntupleName = cms.untracked.string("ntuple.root"),
               variables  =
               cms.untracked.
               vstring(
    "recoBeamSpot                    offlineBeamSpot                   1",
    #---------------------------------------------------------------------
    "  double  x0()",
    "  double  y0()",
    "  double  z0()",
    "/",
    "patElectron                     cleanLayer1Electrons            500",
    #---------------------------------------------------------------------
    "     int  charge()",
    "  double  energy()",
    "  double  et()",
    "  double  eta()",
    "  double  phi()",
    "  double  pt()",
    "   float  caloIso()",
    "   float  ecalIso()",
    "  double  ecalIsoDeposit_candEnergy()",
    "   float  hcalIso()",
    "  double  hcalIsoDeposit_candEnergy()",
    "   float  trackIso()",
    "  double  trackerIsoDeposit_candEnergy()",
    "     int  classification()",
    "  double  gsfTrack_d0()",
    "  double  gsfTrack_phi()",
    "/",
    "patJet                          cleanLayer1Jets                 500",
    #---------------------------------------------------------------------
    "  double  energy()",
    "   float  energyFractionHadronic()",
    "  double  et()",
    "  double  eta()",
    "  double  phi()",
    "    bool  isBasicJet()",
    "    bool  isCaloJet()",
    "    bool  isJet()",
    "/",
    "patMuon                         cleanLayer1Muons                500",
    #---------------------------------------------------------------------
    "     int  charge()",
    "  double  energy()",
    "  double  et()",
    "  double  eta()",
    "  double  phi()",
    "  double  pt()",
    "   float  caloIso()",
    "   float  ecalIso()",
    "  double  ecalIsoDeposit_candEnergy()",
    "   float  hcalIso()",
    "  double  hcalIsoDeposit_candEnergy()",
    "   float  trackIso()",
    "  double  track_d0()",
    "  double  track_phi()",
    "/",
    "recoTrack                       generalTracks                   500",
    #---------------------------------------------------------------------
    "     int  charge()",
    "  double  eta()",
    "  double  phi()",
    "  double  pt()",
    "  double  d0()",
    "  double  dxy()",
    "  double  normalizedChi2()",
    "/",

	"patMuonExtra                    cleanLayer1Muons                500",
    #---------------------------------------------------------------------
	"bool isGlobalMuonPromptTight()",
	"double relIso()",
	"/"
    )
               )
