// -------------------------------------------------------------------------
// File::   plugins2.cc
// Created: Thu Jul 28 02:06:20 2011 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"

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

#include "DataFormats/EgammaReco/interface/HFEMClusterShape.h"
typedef Buffer<reco::HFEMClusterShape, false> recoHFEMClusterShape_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHFEMClusterShape_t,
                  "recoHFEMClusterShape");

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

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
typedef Buffer<reco::PFCandidate, false> recoPFCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFCandidate_t,
                  "recoPFCandidate");

#include "DataFormats/JetReco/interface/PFJet.h"
typedef Buffer<reco::PFJet, false> recoPFJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFJet_t,
                  "recoPFJet");

#include "DataFormats/METReco/interface/PFMET.h"
typedef Buffer<reco::PFMET, false> recoPFMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFMET_t,
                  "recoPFMET");

#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
typedef Buffer<reco::PFRecHit, false> recoPFRecHit_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFRecHit_t,
                  "recoPFRecHit");

#include "DataFormats/TauReco/interface/PFTau.h"
typedef Buffer<reco::PFTau, false> recoPFTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTau_t,
                  "recoPFTau");

#include "DataFormats/EgammaCandidates/interface/Photon.h"
typedef Buffer<reco::Photon, false> recoPhoton_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPhoton_t,
                  "recoPhoton");

#include "DataFormats/EgammaCandidates/interface/PhotonCore.h"
typedef Buffer<reco::PhotonCore, false> recoPhotonCore_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPhotonCore_t,
                  "recoPhotonCore");

#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
typedef Buffer<reco::PreshowerCluster, false> recoPreshowerCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreshowerCluster_t,
                  "recoPreshowerCluster");
