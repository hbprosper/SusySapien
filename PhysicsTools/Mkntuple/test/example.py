#$Revision: 1.4 $ example.py
#------------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
							fileNames =
							cms.untracked.vstring("file:pat.root"
												  )
							)

#-------------------------------------------------------------------------
# Created: Sun Feb  7 22:35:29 2010 by mkntuplecfi.py
# $Revison:$
#-------------------------------------------------------------------------
process.demo =\
cms.EDAnalyzer("Mkntuple",

			   # Name of output n-tuple
               ntupleName = cms.untracked.string("ntuple.root"),

			   # List of buffers to allocate.
			   #----------------------------------------------------------
			   # The names are arbitrary so long as each agrees with
			   # the name of a vstring. For example, since the vstring
			   # "buffers" contains the name "recoBeamSpot", the code
			   # expects a vstring of that name to exist.
			   #----------------------------------------------------------
               buffers =
               cms.untracked.
               vstring(
    "recoBeamSpot",
    "patMuon",
	"patElectron",
    "recoGenParticle"
    ),
			   #----------------------------------------------------------
			   # Format of 1st line:
			   #   buffer-name         getByLabel         maximum-count
			   #
			   # Format of subsequent lines:
			   #   [return-type] method [alias]
			   #----------------------------------------------------------
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
    "patMuon                         cleanLayer1Muons                50",
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

               patElectron =
               cms.untracked.
               vstring(
    "patElectron                      cleanLayer1Electrons            50",
    #---------------------------------------------------------------------
    "    int   charge()",
    " double   energy()",
    " double   et()",
    " double   eta()",
    " double   p()",
    " double   phi()",
    " double   pt()",
	" double   gsfTrack()->d0()",
	" double   gsfTrack()->phi()"
    ),

			   recoGenParticle =
               cms.untracked.
               vstring(
    "recoGenParticle                 genParticles                    1000",
    #---------------------------------------------------------------------
    "    int   charge()",
    "    int   pdgId()",
    "    int   status()",
    " double   pt()",
    " double   eta()",
    " double   phi()",
    " double   mass()"
    ),
			   cuts =
			   cms.untracked.
			   string(
	'''
	bool keep = false;
	'''
	)
			   
               )

process.p = cms.Path(process.demo)
