#$Revision: 1.2 $
import FWCore.ParameterSet.Config as cms

process = cms.Process("TestMe")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
							fileNames =
							cms.untracked.vstring("file:pat.root")
							)
process.test   = cms.EDAnalyzer("TestMe")

process.p = cms.Path(process.test)
