#$Revision: 1.3 $
import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
							fileNames =
							cms.untracked.vstring("file:pat.root"
												  )
							)
process.load("PhysicsTools.Mkntuple.pat_cfi")

process.p = cms.Path(process.demo)