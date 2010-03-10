#!/usr/bin/env python
# -----------------------------------------------------------------------------
#  File:        analyzer.py
#  Description: Analyzer for ntuples created by Mkntuple
#  Created:     Wed Mar 10 08:10:43 2010 by mkntanalyzer.py
#  $Revision:$
# -----------------------------------------------------------------------------
import os, sys, re
from ROOT import *
from time import sleep
from string import *
from PhysicsTools.LiteAnalysis.AutoLoader import *
# -----------------------------------------------------------------------------
# -- Procedures and functions
# -----------------------------------------------------------------------------
def error(message):
	print "** error ** " + message
	sys.exit(0)

#  Read ntuple filenames from file list

def getFilenames():
	argv = sys.argv
	argc = len(argv)
	filelist = "filelist.txt"
	if argc > 1: filelist = argv[1]

	if not os.path.exists(filelist):
		error("unable to open file: " + filelist)
		
	# Get ntuple file names
	filenames = filter(lambda x: x != "",
					   map(strip, open(filelist).readlines()))
	v = vector("string")()
	for filename in filenames: v.push_back(filename)
	return v
# =============================================================================
def main():

	#  Get names of ntuple files to be processed and open chain of ntuples
  
	filenames = getFilenames()
	stream = itreestream(filenames, "Events")
	if not stream.good(): error("unable to open ntuple file(s)")
  
	#  Get number of events to be read
  
	nevents = stream.size();
	print "Number of events:",nevents
	
	# -------------------------------------------------------------------------
	#  Define variables to be read
    # -------------------------------------------------------------------------
	beamspot_x0	= Double()
	beamspot_y0	= Double()
	beamspot_z0	= Double()
	genmother_charge	= vector("int")(1000)
	genmother_eta	= vector("double")(1000)
	genmother_mass	= vector("double")(1000)
	genmother_pdgId	= vector("int")(1000)
	genmother_phi	= vector("double")(1000)
	genmother_pt	= vector("double")(1000)
	genmother_status	= vector("int")(1000)
	genparticle_charge	= vector("int")(1000)
	genparticle_eta	= vector("double")(1000)
	genparticle_mass	= vector("double")(1000)
	genparticle_pdgId	= vector("int")(1000)
	genparticle_phi	= vector("double")(1000)
	genparticle_pt	= vector("double")(1000)
	genparticle_status	= vector("int")(1000)
	genruninfoproduct_externalXSecLO_value	= Double()
	genruninfoproduct_filterEfficiency	= Double()
	genruninfoproduct_internalXSec_value	= Double()
	muon_caloIso	= vector("float")(500)
	muon_charge	= vector("int")(500)
	muon_ecalIso	= vector("float")(500)
	muon_ecalIsoDeposit_candEnergy	= vector("double")(500)
	muon_energy	= vector("double")(500)
	muon_et	= vector("double")(500)
	muon_eta	= vector("double")(500)
	muon_hcalIso	= vector("float")(500)
	muon_isCaloMuon	= vector("int")(500)
	muon_isGlobalMuon	= vector("int")(500)
	muon_isStandAloneMuon	= vector("int")(500)
	muon_isTrackerMuon	= vector("int")(500)
	muon_p	= vector("double")(500)
	muon_phi	= vector("double")(500)
	muon_pt	= vector("double")(500)
	muon_trackIso	= vector("float")(500)
	ngenmother	= Long()
	ngenparticle	= Long()
	nmuon	= Long()
	triggerbits_HLT_Jet30	= Long()
	triggerbits_HLT_Jet50	= Long()
	triggerbits_HLT_Jet80	= Long()
	triggerbits_value_HLT_L1Jet15	= Long()

	stream.select("recoBeamSpot_offlineBeamSpot.x0", beamspot_x0)
	stream.select("recoBeamSpot_offlineBeamSpot.y0", beamspot_y0)
	stream.select("recoBeamSpot_offlineBeamSpot.z0", beamspot_z0)
	stream.select("GenMother_genParticles.charge", genmother_charge)
	stream.select("GenMother_genParticles.eta", genmother_eta)
	stream.select("GenMother_genParticles.mass", genmother_mass)
	stream.select("GenMother_genParticles.pdgId", genmother_pdgId)
	stream.select("GenMother_genParticles.phi", genmother_phi)
	stream.select("GenMother_genParticles.pt", genmother_pt)
	stream.select("GenMother_genParticles.status", genmother_status)
	stream.select("recoGenParticle_genParticles.charge", genparticle_charge)
	stream.select("recoGenParticle_genParticles.eta", genparticle_eta)
	stream.select("recoGenParticle_genParticles.mass", genparticle_mass)
	stream.select("recoGenParticle_genParticles.pdgId", genparticle_pdgId)
	stream.select("recoGenParticle_genParticles.phi", genparticle_phi)
	stream.select("recoGenParticle_genParticles.pt", genparticle_pt)
	stream.select("recoGenParticle_genParticles.status", genparticle_status)
	stream.select("GenRunInfoProduct_generator.externalXSecLO_value", genruninfoproduct_externalXSecLO_value)
	stream.select("GenRunInfoProduct_generator.filterEfficiency", genruninfoproduct_filterEfficiency)
	stream.select("GenRunInfoProduct_generator.internalXSec_value", genruninfoproduct_internalXSec_value)
	stream.select("patMuon_cleanLayer1Muons.caloIso", muon_caloIso)
	stream.select("patMuon_cleanLayer1Muons.charge", muon_charge)
	stream.select("patMuon_cleanLayer1Muons.ecalIso", muon_ecalIso)
	stream.select("patMuon_cleanLayer1Muons.ecalIsoDeposit_candEnergy", muon_ecalIsoDeposit_candEnergy)
	stream.select("patMuon_cleanLayer1Muons.energy", muon_energy)
	stream.select("patMuon_cleanLayer1Muons.et", muon_et)
	stream.select("patMuon_cleanLayer1Muons.eta", muon_eta)
	stream.select("patMuon_cleanLayer1Muons.hcalIso", muon_hcalIso)
	stream.select("patMuon_cleanLayer1Muons.isCaloMuon", muon_isCaloMuon)
	stream.select("patMuon_cleanLayer1Muons.isGlobalMuon", muon_isGlobalMuon)
	stream.select("patMuon_cleanLayer1Muons.isStandAloneMuon", muon_isStandAloneMuon)
	stream.select("patMuon_cleanLayer1Muons.isTrackerMuon", muon_isTrackerMuon)
	stream.select("patMuon_cleanLayer1Muons.p", muon_p)
	stream.select("patMuon_cleanLayer1Muons.phi", muon_phi)
	stream.select("patMuon_cleanLayer1Muons.pt", muon_pt)
	stream.select("patMuon_cleanLayer1Muons.trackIso", muon_trackIso)
	stream.select("nGenMother_genParticles", ngenmother)
	stream.select("nrecoGenParticle_genParticles", ngenparticle)
	stream.select("npatMuon_cleanLayer1Muons", nmuon)
	stream.select("triggerBits_TriggerResults.HLT_Jet30", triggerbits_HLT_Jet30)
	stream.select("triggerBits_TriggerResults.HLT_Jet50", triggerbits_HLT_Jet50)
	stream.select("triggerBits_TriggerResults.HLT_Jet80", triggerbits_HLT_Jet80)
	stream.select("triggerBits_TriggerResults.value_HLT_L1Jet15", triggerbits_value_HLT_L1Jet15)


    # -------------------------------------------------------------------------
	#  Book histograms etc.
	# -------------------------------------------------------------------------
	kit.setStyle()

	# -------------------------------------------------------------------------
	#  Loop over events
	# -------------------------------------------------------------------------

	for entry in xrange(nevents):
		stream.read(entry)

		#  Find SUSY...
	  
	stream.close()
# =============================================================================
main()

