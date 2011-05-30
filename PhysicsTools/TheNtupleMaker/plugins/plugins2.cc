// -------------------------------------------------------------------------
// File::   plugins2.cc
// Created: Mon May 30 06:05:59 2011 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"

#include "DataFormats/PatCandidates/interface/Photon.h"
typedef Buffer<pat::Photon, false> patPhoton_t;
DEFINE_EDM_PLUGIN(BufferFactory, patPhoton_t,
                  "patPhoton");

#include "DataFormats/PatCandidates/interface/Tau.h"
typedef Buffer<pat::Tau, false> patTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTau_t,
                  "patTau");

#include "DataFormats/JetReco/interface/BasicJet.h"
typedef Buffer<reco::BasicJet, false> recoBasicJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBasicJet_t,
                  "recoBasicJet");

#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
typedef Buffer<reco::CaloCluster, false> recoCaloCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloCluster_t,
                  "recoCaloCluster");

#include "DataFormats/JetReco/interface/CaloJet.h"
typedef Buffer<reco::CaloJet, false> recoCaloJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloJet_t,
                  "recoCaloJet");

#include "DataFormats/METReco/interface/CaloMET.h"
typedef Buffer<reco::CaloMET, false> recoCaloMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloMET_t,
                  "recoCaloMET");

#include "DataFormats/MuonReco/interface/CaloMuon.h"
typedef Buffer<reco::CaloMuon, false> recoCaloMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloMuon_t,
                  "recoCaloMuon");

#include "DataFormats/TauReco/interface/CaloTau.h"
typedef Buffer<reco::CaloTau, false> recoCaloTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloTau_t,
                  "recoCaloTau");

#include "DataFormats/TauReco/interface/CaloTauTagInfo.h"
typedef Buffer<reco::CaloTauTagInfo, false> recoCaloTauTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloTauTagInfo_t,
                  "recoCaloTauTagInfo");

#include "DataFormats/CastorReco/interface/CastorTower.h"
typedef Buffer<reco::CastorTower, false> recoCastorTower_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorTower_t,
                  "recoCastorTower");

#include "DataFormats/EgammaCandidates/interface/Conversion.h"
typedef Buffer<reco::Conversion, false> recoConversion_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoConversion_t,
                  "recoConversion");

#include "DataFormats/EgammaReco/interface/ElectronSeed.h"
typedef Buffer<reco::ElectronSeed, false> recoElectronSeed_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoElectronSeed_t,
                  "recoElectronSeed");

#include "DataFormats/JetReco/interface/GenJet.h"
typedef Buffer<reco::GenJet, false> recoGenJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGenJet_t,
                  "recoGenJet");

#include "DataFormats/METReco/interface/GenMET.h"
typedef Buffer<reco::GenMET, false> recoGenMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGenMET_t,
                  "recoGenMET");

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
typedef Buffer<reco::GenParticle, false> recoGenParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGenParticle_t,
                  "recoGenParticle");

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
typedef Buffer<reco::GsfElectron, false> recoGsfElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGsfElectron_t,
                  "recoGsfElectron");

#include "DataFormats/EgammaCandidates/interface/GsfElectronCore.h"
typedef Buffer<reco::GsfElectronCore, false> recoGsfElectronCore_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGsfElectronCore_t,
                  "recoGsfElectronCore");

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
typedef Buffer<reco::GsfTrack, false> recoGsfTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGsfTrack_t,
                  "recoGsfTrack");

#include "DataFormats/GsfTrackReco/interface/GsfTrackExtra.h"
typedef Buffer<reco::GsfTrackExtra, false> recoGsfTrackExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGsfTrackExtra_t,
                  "recoGsfTrackExtra");

#include "DataFormats/EgammaReco/interface/HFEMClusterShape.h"
typedef Buffer<reco::HFEMClusterShape, false> recoHFEMClusterShape_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHFEMClusterShape_t,
                  "recoHFEMClusterShape");

#include "DataFormats/METReco/interface/HcalNoiseRBX.h"
typedef Buffer<reco::HcalNoiseRBX, false> recoHcalNoiseRBX_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHcalNoiseRBX_t,
                  "recoHcalNoiseRBX");

#include "DataFormats/JetReco/interface/JPTJet.h"
typedef Buffer<reco::JPTJet, false> recoJPTJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoJPTJet_t,
                  "recoJPTJet");

#include "DataFormats/METReco/interface/MET.h"
typedef Buffer<reco::MET, false> recoMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMET_t,
                  "recoMET");

#include "DataFormats/MuonReco/interface/Muon.h"
typedef Buffer<reco::Muon, false> recoMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuon_t,
                  "recoMuon");

#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
typedef Buffer<reco::PFBlock, false> recoPFBlock_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFBlock_t,
                  "recoPFBlock");

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
typedef Buffer<reco::PFCandidate, false> recoPFCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFCandidate_t,
                  "recoPFCandidate");
