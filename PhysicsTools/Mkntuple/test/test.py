#$Revision: 1.2 $
import FWCore.ParameterSet.Config as cms

process = cms.Process("TestBed")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
							fileNames =
							cms.untracked.vstring("file:pat.root")
							)
process.testbed = cms.EDAnalyzer("TestBed")

process.p = cms.Path(process.testbed)