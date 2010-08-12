// ----------------------------------------------------------------------------
// Created: Sun Aug  8 20:51:54 2010 by mkplugins.py
// $Revision: 1.7 $
// ----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"

#include "DataFormats/Scalers/interface/BeamSpotOnline.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<BeamSpotOnline>,
                  "BeamSpotOnline");

#include "DataFormats/Scalers/interface/DcsStatus.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<DcsStatus>,
                  "DcsStatus");

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<EBDigiCollection>,
                  "EBDigiCollection");

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<EEDigiCollection>,
                  "EEDigiCollection");

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<GenEventInfoProduct>,
                  "GenEventInfoProduct");

#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<GenRunInfoProduct>,
                  "GenRunInfoProduct");

#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<HcalNoiseSummary>,
                  "HcalNoiseSummary");

#include "DataFormats/Scalers/interface/L1AcceptBunchCrossing.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<L1AcceptBunchCrossing>,
                  "L1AcceptBunchCrossing");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEmCand.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<L1GctEmCand>,
                  "L1GctEmCand");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtHad.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<L1GctEtHad>,
                  "L1GctEtHad");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtMiss.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<L1GctEtMiss>,
                  "L1GctEtMiss");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtTotal.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<L1GctEtTotal>,
                  "L1GctEtTotal");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFBitCounts.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<L1GctHFBitCounts>,
                  "L1GctHFBitCounts");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFRingEtSums.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<L1GctHFRingEtSums>,
                  "L1GctHFRingEtSums");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHtMiss.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<L1GctHtMiss>,
                  "L1GctHtMiss");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCand.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<L1GctJetCand>,
                  "L1GctJetCand");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCounts.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<L1GctJetCounts>,
                  "L1GctJetCounts");

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<L1GlobalTriggerObjectMapRecord>,
                  "L1GlobalTriggerObjectMapRecord");

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<L1GlobalTriggerReadoutRecord>,
                  "L1GlobalTriggerReadoutRecord");

#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<L1MuGMTReadoutCollection>,
                  "L1MuGMTReadoutCollection");

#include "DataFormats/Scalers/interface/L1TriggerScalers.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<L1TriggerScalers>,
                  "L1TriggerScalers");

#include "DataFormats/Scalers/interface/Level1TriggerScalers.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<Level1TriggerScalers>,
                  "Level1TriggerScalers");

#include "DataFormats/Scalers/interface/LumiScalers.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<LumiScalers>,
                  "LumiScalers");

#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<TrajectorySeed>,
                  "TrajectorySeed");

#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<l1extra::L1EmParticle>,
                  "l1extraL1EmParticle");

#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<l1extra::L1EtMissParticle>,
                  "l1extraL1EtMissParticle");

#include "DataFormats/L1Trigger/interface/L1HFRings.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<l1extra::L1HFRings>,
                  "l1extraL1HFRings");

#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<l1extra::L1JetParticle>,
                  "l1extraL1JetParticle");

#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<l1extra::L1MuonParticle>,
                  "l1extraL1MuonParticle");

#include "DataFormats/PatCandidates/interface/Electron.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<pat::Electron>,
                  "patElectron");

#include "DataFormats/PatCandidates/interface/Hemisphere.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<pat::Hemisphere>,
                  "patHemisphere");

#include "DataFormats/PatCandidates/interface/Jet.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<pat::Jet>,
                  "patJet");

#include "DataFormats/PatCandidates/interface/MET.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<pat::MET>,
                  "patMET");

#include "DataFormats/PatCandidates/interface/MHT.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<pat::MHT>,
                  "patMHT");

#include "DataFormats/PatCandidates/interface/Muon.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<pat::Muon>,
                  "patMuon");

#include "DataFormats/PatCandidates/interface/Photon.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<pat::Photon>,
                  "patPhoton");

#include "DataFormats/PatCandidates/interface/Tau.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<pat::Tau>,
                  "patTau");

#include "DataFormats/METReco/interface/BeamHaloSummary.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::BeamHaloSummary>,
                  "recoBeamHaloSummary");

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::BeamSpot>,
                  "recoBeamSpot");

#include "DataFormats/METReco/interface/CSCHaloData.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::CSCHaloData>,
                  "recoCSCHaloData");

#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::CaloCluster>,
                  "recoCaloCluster");

#include "DataFormats/JetReco/interface/CaloJet.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::CaloJet>,
                  "recoCaloJet");

#include "DataFormats/METReco/interface/CaloMET.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::CaloMET>,
                  "recoCaloMET");

#include "DataFormats/MuonReco/interface/CaloMuon.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::CaloMuon>,
                  "recoCaloMuon");

#include "DataFormats/TauReco/interface/CaloTau.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::CaloTau>,
                  "recoCaloTau");

#include "DataFormats/TauReco/interface/CaloTauDiscriminator.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::CaloTauDiscriminator>,
                  "recoCaloTauDiscriminator");

#include "DataFormats/TauReco/interface/CaloTauTagInfo.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::CaloTauTagInfo>,
                  "recoCaloTauTagInfo");

#include "DataFormats/CastorReco/interface/CastorCluster.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::CastorCluster>,
                  "recoCastorCluster");

#include "DataFormats/CastorReco/interface/CastorEgamma.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::CastorEgamma>,
                  "recoCastorEgamma");

#include "DataFormats/CastorReco/interface/CastorJet.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::CastorJet>,
                  "recoCastorJet");

#include "DataFormats/CastorReco/interface/CastorTower.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::CastorTower>,
                  "recoCastorTower");

#include "DataFormats/EgammaCandidates/interface/Conversion.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::Conversion>,
                  "recoConversion");

#include "DataFormats/METReco/interface/EcalHaloData.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::EcalHaloData>,
                  "recoEcalHaloData");

#include "DataFormats/EgammaReco/interface/ElectronSeed.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::ElectronSeed>,
                  "recoElectronSeed");

#include "DataFormats/JetReco/interface/GenJet.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::GenJet>,
                  "recoGenJet");

#include "DataFormats/METReco/interface/GenMET.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::GenMET>,
                  "recoGenMET");

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::GenParticle>,
                  "recoGenParticle");

#include "DataFormats/METReco/interface/GlobalHaloData.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::GlobalHaloData>,
                  "recoGlobalHaloData");

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::GsfElectron>,
                  "recoGsfElectron");

#include "DataFormats/EgammaCandidates/interface/GsfElectronCore.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::GsfElectronCore>,
                  "recoGsfElectronCore");

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::GsfTrack>,
                  "recoGsfTrack");

#include "DataFormats/GsfTrackReco/interface/GsfTrackExtra.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::GsfTrackExtra>,
                  "recoGsfTrackExtra");

#include "DataFormats/EgammaReco/interface/HFEMClusterShape.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::HFEMClusterShape>,
                  "recoHFEMClusterShape");

#include "DataFormats/METReco/interface/HcalHaloData.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::HcalHaloData>,
                  "recoHcalHaloData");

#include "DataFormats/METReco/interface/HcalNoiseRBX.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::HcalNoiseRBX>,
                  "recoHcalNoiseRBX");

#include "DataFormats/METReco/interface/MET.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::MET>,
                  "recoMET");

#include "DataFormats/MuonReco/interface/Muon.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::Muon>,
                  "recoMuon");

#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::PFBlock>,
                  "recoPFBlock");

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::PFCandidate>,
                  "recoPFCandidate");

#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::PFCluster>,
                  "recoPFCluster");

#include "DataFormats/JetReco/interface/PFJet.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::PFJet>,
                  "recoPFJet");

#include "DataFormats/METReco/interface/PFMET.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::PFMET>,
                  "recoPFMET");

#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::PFRecHit>,
                  "recoPFRecHit");

#include "DataFormats/TauReco/interface/PFTau.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::PFTau>,
                  "recoPFTau");

#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::PFTauDiscriminator>,
                  "recoPFTauDiscriminator");

#include "DataFormats/TauReco/interface/PFTauTagInfo.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::PFTauTagInfo>,
                  "recoPFTauTagInfo");

#include "DataFormats/EgammaCandidates/interface/Photon.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::Photon>,
                  "recoPhoton");

#include "DataFormats/EgammaCandidates/interface/PhotonCore.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::PhotonCore>,
                  "recoPhotonCore");

#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::PreshowerCluster>,
                  "recoPreshowerCluster");

#include "DataFormats/EgammaReco/interface/PreshowerClusterShape.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::PreshowerClusterShape>,
                  "recoPreshowerClusterShape");

#include "DataFormats/RecoCandidate/interface/RecoChargedRefCandidate.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::RecoChargedRefCandidate>,
                  "recoRecoChargedRefCandidate");

#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::RecoEcalCandidate>,
                  "recoRecoEcalCandidate");

#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::SecondaryVertexTagInfo>,
                  "recoSecondaryVertexTagInfo");

#include "DataFormats/BTauReco/interface/SoftLeptonTagInfo.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::SoftLeptonTagInfo>,
                  "recoSoftLeptonTagInfo");

#include "DataFormats/EgammaReco/interface/SuperCluster.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::SuperCluster>,
                  "recoSuperCluster");

#include "DataFormats/TrackReco/interface/Track.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::Track>,
                  "recoTrack");

#include "DataFormats/TrackReco/interface/TrackExtra.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::TrackExtra>,
                  "recoTrackExtra");

#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::TrackIPTagInfo>,
                  "recoTrackIPTagInfo");

#include "DataFormats/JetReco/interface/TrackJet.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::TrackJet>,
                  "recoTrackJet");

#include "DataFormats/VertexReco/interface/Vertex.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::Vertex>,
                  "recoVertex");

#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::VertexCompositeCandidate>,
                  "recoVertexCompositeCandidate");

#include "DataFormats/HLTReco/interface/TriggerEvent.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<trigger::TriggerEvent>,
                  "triggerTriggerEvent");
