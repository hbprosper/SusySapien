// -------------------------------------------------------------------------
// File::   plugins08.cc
// Created: Thu Apr 12 04:32:19 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/TopObjects/interface/TtSemiEvtSolution.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/HLTPathStatus.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1Track.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/EventHypothesis.h"
#include "DataFormats/PatCandidates/interface/GenericParticle.h"
#include "DataFormats/PatCandidates/interface/Hemisphere.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/MHT.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/PFParticle.h"
#include "DataFormats/PatCandidates/interface/Particle.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/TauJetCorrFactors.h"
#include "DataFormats/PatCandidates/interface/TriggerAlgorithm.h"
#include "DataFormats/PatCandidates/interface/TriggerCondition.h"
#include "DataFormats/PatCandidates/interface/TriggerFilter.h"
#include "FWCore/MessageLogger/interface/ErrorSummaryEntry.h"
// -------------------------------------------------------------------------

typedef Buffer<TtSemiEvtSolution, false>
TtSemiEvtSolution_t;
DEFINE_EDM_PLUGIN(BufferFactory, TtSemiEvtSolution_t,
                  "TtSemiEvtSolution");
				  
typedef Buffer<csc::L1Track, false>
cscL1Track_t;
DEFINE_EDM_PLUGIN(BufferFactory, cscL1Track_t,
                  "cscL1Track");
				  
typedef Buffer<edm::ErrorSummaryEntry, false>
edmErrorSummaryEntry_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmErrorSummaryEntry_t,
                  "edmErrorSummaryEntry");
				  
typedef Buffer<edm::HLTPathStatus, false>
edmHLTPathStatus_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmHLTPathStatus_t,
                  "edmHLTPathStatus");
				  
typedef Buffer<edm::Ptr<reco::Candidate>, false>
edmPtrrecoCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmPtrrecoCandidate_t,
                  "edmPtrrecoCandidate");
				  
typedef Buffer<pat::CompositeCandidate, false>
patCompositeCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, patCompositeCandidate_t,
                  "patCompositeCandidate");
				  
typedef Buffer<pat::Electron, false>
patElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, patElectron_t,
                  "patElectron");
				  
typedef Buffer<pat::EventHypothesis, false>
patEventHypothesis_t;
DEFINE_EDM_PLUGIN(BufferFactory, patEventHypothesis_t,
                  "patEventHypothesis");
				  
typedef Buffer<pat::GenericParticle, false>
patGenericParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, patGenericParticle_t,
                  "patGenericParticle");
				  
typedef Buffer<pat::Hemisphere, false>
patHemisphere_t;
DEFINE_EDM_PLUGIN(BufferFactory, patHemisphere_t,
                  "patHemisphere");
				  
typedef Buffer<pat::Jet, false>
patJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, patJet_t,
                  "patJet");
				  
typedef Buffer<pat::JetCorrFactors::CorrectionFactor, false>
patJetCorrFactorsCorrectionFactor_t;
DEFINE_EDM_PLUGIN(BufferFactory, patJetCorrFactorsCorrectionFactor_t,
                  "patJetCorrFactorsCorrectionFactor");
				  
typedef Buffer<pat::MET, false>
patMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMET_t,
                  "patMET");
				  
typedef Buffer<pat::MHT, false>
patMHT_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMHT_t,
                  "patMHT");
				  
typedef Buffer<pat::Muon, false>
patMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMuon_t,
                  "patMuon");
				  
typedef Buffer<pat::PFParticle, false>
patPFParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, patPFParticle_t,
                  "patPFParticle");
				  
typedef Buffer<pat::Particle, false>
patParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, patParticle_t,
                  "patParticle");
				  
typedef Buffer<pat::Photon, false>
patPhoton_t;
DEFINE_EDM_PLUGIN(BufferFactory, patPhoton_t,
                  "patPhoton");
				  
typedef Buffer<pat::Tau, false>
patTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTau_t,
                  "patTau");
				  
typedef Buffer<pat::TauJetCorrFactors::CorrectionFactor, false>
patTauJetCorrFactorsCorrectionFactor_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTauJetCorrFactorsCorrectionFactor_t,
                  "patTauJetCorrFactorsCorrectionFactor");
				  
typedef Buffer<pat::TriggerAlgorithm, false>
patTriggerAlgorithm_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerAlgorithm_t,
                  "patTriggerAlgorithm");
				  
typedef Buffer<pat::TriggerCondition, false>
patTriggerCondition_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerCondition_t,
                  "patTriggerCondition");
				  
typedef Buffer<pat::TriggerFilter, false>
patTriggerFilter_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerFilter_t,
                  "patTriggerFilter");
				  