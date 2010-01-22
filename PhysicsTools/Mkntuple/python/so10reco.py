#-------------------------------------------------------------------------
# Created: Thu Jan 21 22:29:51 2010 by mkntuplecfi.py
#-------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
demo =\
cms.EDAnalyzer("Mkntuple",
               ntupleName = cms.untracked.string("so10reco_ntuple.root"),
               variables  =
               cms.untracked.
               vstring(
    "recoBeamSpot                    offlineBeamSpot                   1",
    #---------------------------------------------------------------------
    "  double  x0()",
    "  double  y0()",
    "  double  z0()",
    "/",
    "recoGenParticle                 genParticles                  4000",
    #---------------------------------------------------------------------
    "     int  charge()",
    "  double  energy()",
    "  double  et()",
    "  double  eta()",
    "  double  p()",
    "  double  phi()",
    "  double  pt()",
    "    bool  longLived()",
    "  double  mass()",
    "     int  pdgId()",
    "     int  status()",
    "/",
    "recoGsfElectron                 gsfElectrons                    500",
    #---------------------------------------------------------------------
    "     int  charge()",
    "  double  energy()",
    "  double  et()",
    "  double  eta()",
    "  double  phi()",
    "  double  pt()",
    "    bool  isEnergyScaleCorrected()",
    "    bool  isMomentumCorrected()",
    "   float  ecalEnergy()",
    "   float  fbrem()",
    "/",
    "recoMET                         htMetIC5                        500",
    #---------------------------------------------------------------------
    "  double  energy()",
    "  double  et()",
    "  double  phi()",
    "/",
    "recoMET                         htMetKT4                        500",
    #---------------------------------------------------------------------
    "  double  energy()",
    "  double  et()",
    "  double  phi()",
    "/",
    "recoMuon                        muons                           500",
    #---------------------------------------------------------------------
    "     int  charge()",
    "  double  energy()",
    "  double  et()",
    "  double  eta()",
    "  double  phi()",
    "  double  pt()",
    "    bool  isCaloMuon()",
    "    bool  isGlobalMuon()",
    "    bool  isStandAloneMuon()",
    "    bool  isTrackerMuon()",
    "/",
    "recoSuperCluster                hybridSuperClusters             500",
    #---------------------------------------------------------------------
    "  double  energy()",
    "  double  eta()",
    "  double  etaWidth()",
    "  double  phi()",
    "  double  phiWidth()",
    "  size_t  clustersSize()",
    "  double  preshowerEnergy()",
    "  double  rawEnergy()",
    "  size_t  size()",
    "/"
    )
               )
