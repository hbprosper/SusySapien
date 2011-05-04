#$Revision: 1.1 $ example.py
#------------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
							fileNames =
							cms.untracked.vstring("file:pat.root")
							)
process.me = \
cms.EDAnalyzer("TestMethod",
               patElectronLabel = cms.untracked.string("cleanPatElectrons"),
			   patMuonLabel     = cms.untracked.string("cleanPatMuons"),
			   patJetLabel      = cms.untracked.string("cleanPatJets")
               )

process.p = cms.Path(process.me)
