// -------------------------------------------------------------------------
// File::   plugins03.cc
// Created: Thu Apr 26 00:40:14 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/FatJet.h"
#include "AnalysisDataFormats/CMGTools/interface/GenericTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/Hemisphere.h"
#include "AnalysisDataFormats/CMGTools/interface/MultiObject.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/NamedObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/Photon.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"
#include "AnalysisDataFormats/CMGTools/interface/VBFEventT1T2.h"
#include "AnalysisDataFormats/TopObjects/interface/TtDilepEvtSolution.h"
#include "AnalysisDataFormats/TopObjects/interface/TtHadEvtSolution.h"
#include "AnalysisDataFormats/TopObjects/interface/TtSemiEvtSolution.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
// -------------------------------------------------------------------------

std::string TrajectorySeed_n("TrajectorySeed");
typedef Buffer<TrajectorySeed,
               &TrajectorySeed_n, COLLECTION>
TrajectorySeed_t;
DEFINE_EDM_PLUGIN(BufferFactory, TrajectorySeed_t,
                  "TrajectorySeed");
				  
std::string TtDilepEvtSolution_n("TtDilepEvtSolution");
typedef Buffer<TtDilepEvtSolution,
               &TtDilepEvtSolution_n, COLLECTION>
TtDilepEvtSolution_t;
DEFINE_EDM_PLUGIN(BufferFactory, TtDilepEvtSolution_t,
                  "TtDilepEvtSolution");
				  
std::string TtHadEvtSolution_n("TtHadEvtSolution");
typedef Buffer<TtHadEvtSolution,
               &TtHadEvtSolution_n, COLLECTION>
TtHadEvtSolution_t;
DEFINE_EDM_PLUGIN(BufferFactory, TtHadEvtSolution_t,
                  "TtHadEvtSolution");
				  
std::string TtSemiEvtSolution_n("TtSemiEvtSolution");
typedef Buffer<TtSemiEvtSolution,
               &TtSemiEvtSolution_n, COLLECTION>
TtSemiEvtSolution_t;
DEFINE_EDM_PLUGIN(BufferFactory, TtSemiEvtSolution_t,
                  "TtSemiEvtSolution");
				  
std::string cmgAbstractPhysicsObject_n("cmg::AbstractPhysicsObject");
typedef Buffer<cmg::AbstractPhysicsObject,
               &cmgAbstractPhysicsObject_n, COLLECTION>
cmgAbstractPhysicsObject_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgAbstractPhysicsObject_t,
                  "cmgAbstractPhysicsObject");
				  
std::string cmgBaseJet_n("cmg::BaseJet");
typedef Buffer<cmg::BaseJet,
               &cmgBaseJet_n, COLLECTION>
cmgBaseJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgBaseJet_t,
                  "cmgBaseJet");
				  
std::string cmgBaseMET_n("cmg::BaseMET");
typedef Buffer<cmg::BaseMET,
               &cmgBaseMET_n, COLLECTION>
cmgBaseMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgBaseMET_t,
                  "cmgBaseMET");
				  
std::string cmgDiElectron_n("cmg::DiElectron");
typedef Buffer<cmg::DiElectron,
               &cmgDiElectron_n, COLLECTION>
cmgDiElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgDiElectron_t,
                  "cmgDiElectron");
				  
std::string cmgDiFatJet_n("cmg::DiFatJet");
typedef Buffer<cmg::DiFatJet,
               &cmgDiFatJet_n, COLLECTION>
cmgDiFatJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgDiFatJet_t,
                  "cmgDiFatJet");
				  
std::string cmgDiHemisphere_n("cmg::DiHemisphere");
typedef Buffer<cmg::DiHemisphere,
               &cmgDiHemisphere_n, COLLECTION>
cmgDiHemisphere_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgDiHemisphere_t,
                  "cmgDiHemisphere");
				  
std::string cmgDiJet_n("cmg::DiJet");
typedef Buffer<cmg::DiJet,
               &cmgDiJet_n, COLLECTION>
cmgDiJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgDiJet_t,
                  "cmgDiJet");
				  
std::string cmgDiMuon_n("cmg::DiMuon");
typedef Buffer<cmg::DiMuon,
               &cmgDiMuon_n, COLLECTION>
cmgDiMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgDiMuon_t,
                  "cmgDiMuon");
				  
std::string cmgDiPFJet_n("cmg::DiPFJet");
typedef Buffer<cmg::DiPFJet,
               &cmgDiPFJet_n, COLLECTION>
cmgDiPFJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgDiPFJet_t,
                  "cmgDiPFJet");
				  
std::string cmgDiPhoton_n("cmg::DiPhoton");
typedef Buffer<cmg::DiPhoton,
               &cmgDiPhoton_n, COLLECTION>
cmgDiPhoton_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgDiPhoton_t,
                  "cmgDiPhoton");
				  
std::string cmgDiTau_n("cmg::DiTau");
typedef Buffer<cmg::DiTau,
               &cmgDiTau_n, COLLECTION>
cmgDiTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgDiTau_t,
                  "cmgDiTau");
				  
std::string cmgElectron_n("cmg::Electron");
typedef Buffer<cmg::Electron,
               &cmgElectron_n, COLLECTION>
cmgElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgElectron_t,
                  "cmgElectron");
				  
std::string cmgFatJet_n("cmg::FatJet");
typedef Buffer<cmg::FatJet,
               &cmgFatJet_n, COLLECTION>
cmgFatJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgFatJet_t,
                  "cmgFatJet");
				  
std::string cmgGenJet_n("cmg::GenJet");
typedef Buffer<cmg::GenJet,
               &cmgGenJet_n, COLLECTION>
cmgGenJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgGenJet_t,
                  "cmgGenJet");
				  
std::string cmgGenParticle_n("cmg::GenParticle");
typedef Buffer<cmg::GenParticle,
               &cmgGenParticle_n, COLLECTION>
cmgGenParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgGenParticle_t,
                  "cmgGenParticle");
				  
std::string cmgHemisphere_n("cmg::Hemisphere");
typedef Buffer<cmg::Hemisphere,
               &cmgHemisphere_n, COLLECTION>
cmgHemisphere_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgHemisphere_t,
                  "cmgHemisphere");
				  
std::string cmgMuE_n("cmg::MuE");
typedef Buffer<cmg::MuE,
               &cmgMuE_n, COLLECTION>
cmgMuE_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgMuE_t,
                  "cmgMuE");
				  
std::string cmgMultiObject_n("cmg::MultiObject");
typedef Buffer<cmg::MultiObject,
               &cmgMultiObject_n, COLLECTION>
cmgMultiObject_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgMultiObject_t,
                  "cmgMultiObject");
				  
std::string cmgMuon_n("cmg::Muon");
typedef Buffer<cmg::Muon,
               &cmgMuon_n, COLLECTION>
cmgMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgMuon_t,
                  "cmgMuon");
				  
std::string cmgNamedObject_n("cmg::NamedObject");
typedef Buffer<cmg::NamedObject,
               &cmgNamedObject_n, COLLECTION>
cmgNamedObject_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgNamedObject_t,
                  "cmgNamedObject");
				  
std::string cmgPFCand_n("cmg::PFCand");
typedef Buffer<cmg::PFCand,
               &cmgPFCand_n, COLLECTION>
cmgPFCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgPFCand_t,
                  "cmgPFCand");
				  
std::string cmgPFJet_n("cmg::PFJet");
typedef Buffer<cmg::PFJet,
               &cmgPFJet_n, COLLECTION>
cmgPFJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgPFJet_t,
                  "cmgPFJet");
				  
std::string cmgPhoton_n("cmg::Photon");
typedef Buffer<cmg::Photon,
               &cmgPhoton_n, COLLECTION>
cmgPhoton_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgPhoton_t,
                  "cmgPhoton");
				  
std::string cmgQuadMuon_n("cmg::QuadMuon");
typedef Buffer<cmg::QuadMuon,
               &cmgQuadMuon_n, COLLECTION>
cmgQuadMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgQuadMuon_t,
                  "cmgQuadMuon");
				  
std::string cmgTau_n("cmg::Tau");
typedef Buffer<cmg::Tau,
               &cmgTau_n, COLLECTION>
cmgTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgTau_t,
                  "cmgTau");
				  
std::string cmgTauE_n("cmg::TauE");
typedef Buffer<cmg::TauE,
               &cmgTauE_n, COLLECTION>
cmgTauE_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgTauE_t,
                  "cmgTauE");
				  
std::string cmgTauMu_n("cmg::TauMu");
typedef Buffer<cmg::TauMu,
               &cmgTauMu_n, COLLECTION>
cmgTauMu_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgTauMu_t,
                  "cmgTauMu");
				  
std::string cmgTriggerObject_n("cmg::TriggerObject");
typedef Buffer<cmg::TriggerObject,
               &cmgTriggerObject_n, COLLECTION>
cmgTriggerObject_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgTriggerObject_t,
                  "cmgTriggerObject");
				  
std::string cmgVBFEventT1T2patJetpatJet_n("cmg::VBFEventT1T2<pat::Jet,pat::Jet>");
typedef Buffer<cmg::VBFEventT1T2<pat::Jet,pat::Jet>,
               &cmgVBFEventT1T2patJetpatJet_n, COLLECTION>
cmgVBFEventT1T2patJetpatJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, cmgVBFEventT1T2patJetpatJet_t,
                  "cmgVBFEventT1T2patJetpatJet");
				  