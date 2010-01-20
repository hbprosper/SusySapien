// ----------------------------------------------------------------------------
// Created: Tue Jan 19 21:44:13 2010 by mkplugins.py
// $Revision: 1.1$
//
// Define all Buffer plugins. Need to use typedefs otherwise the
// DEFINE macro gets confused by classes with multiple template
// arguments.
// ----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEmCand.h"
typedef Buffer<L1GctEmCand, L1GctEmCand> L1GctEmCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEmCand_t, "L1GctEmCand");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtHad.h"
typedef Buffer<L1GctEtHad, L1GctEtHad> L1GctEtHad_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtHad_t, "L1GctEtHad");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtMiss.h"
typedef Buffer<L1GctEtMiss, L1GctEtMiss> L1GctEtMiss_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtMiss_t, "L1GctEtMiss");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtTotal.h"
typedef Buffer<L1GctEtTotal, L1GctEtTotal> L1GctEtTotal_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtTotal_t, "L1GctEtTotal");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctFibreWord.h"
typedef Buffer<L1GctFibreWord, L1GctFibreWord> L1GctFibreWord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctFibreWord_t, "L1GctFibreWord");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFBitCounts.h"
typedef Buffer<L1GctHFBitCounts, L1GctHFBitCounts> L1GctHFBitCounts_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHFBitCounts_t, "L1GctHFBitCounts");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFRingEtSums.h"
typedef Buffer<L1GctHFRingEtSums, L1GctHFRingEtSums> L1GctHFRingEtSums_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHFRingEtSums_t, "L1GctHFRingEtSums");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctInternEmCand.h"
typedef Buffer<L1GctInternEmCand, L1GctInternEmCand> L1GctInternEmCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctInternEmCand_t, "L1GctInternEmCand");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctInternEtSum.h"
typedef Buffer<L1GctInternEtSum, L1GctInternEtSum> L1GctInternEtSum_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctInternEtSum_t, "L1GctInternEtSum");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCand.h"
typedef Buffer<L1GctJetCand, L1GctJetCand> L1GctJetCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctJetCand_t, "L1GctJetCand");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCounts.h"
typedef Buffer<L1GctJetCounts, L1GctJetCounts> L1GctJetCounts_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctJetCounts_t, "L1GctJetCounts");

#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
typedef Buffer<TrajectorySeed, TrajectorySeed> TrajectorySeed_t;
DEFINE_EDM_PLUGIN(BufferFactory, TrajectorySeed_t, "TrajectorySeed");

#include "DataFormats/Common/interface/TriggerResults.h"
typedef Buffer<edm::TriggerResults, edm::TriggerResults> edmTriggerResults_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmTriggerResults_t, "edmTriggerResults");

#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "PhysicsTools/LiteAnalysis/dataformats/adapters/al1extraL1EmParticle.h"
typedef Buffer<l1extra::L1EmParticle, al1extraL1EmParticle> l1extraL1EmParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, l1extraL1EmParticle_t, "l1extraL1EmParticle");

#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
#include "PhysicsTools/LiteAnalysis/dataformats/adapters/al1extraL1EtMissParticle.h"
typedef Buffer<l1extra::L1EtMissParticle, al1extraL1EtMissParticle> l1extraL1EtMissParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, l1extraL1EtMissParticle_t, "l1extraL1EtMissParticle");

#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "PhysicsTools/LiteAnalysis/dataformats/adapters/al1extraL1JetParticle.h"
typedef Buffer<l1extra::L1JetParticle, al1extraL1JetParticle> l1extraL1JetParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, l1extraL1JetParticle_t, "l1extraL1JetParticle");

#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
typedef Buffer<l1extra::L1MuonParticle, l1extra::L1MuonParticle> l1extraL1MuonParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, l1extraL1MuonParticle_t, "l1extraL1MuonParticle");

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "PhysicsTools/LiteAnalysis/dataformats/adapters/apatElectron.h"
typedef Buffer<pat::Electron, apatElectron> patElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, patElectron_t, "patElectron");

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "PhysicsTools/LiteAnalysis/dataformats/adapters/apatJet.h"
typedef Buffer<pat::Jet, apatJet> patJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, patJet_t, "patJet");

#include "DataFormats/PatCandidates/interface/MET.h"
#include "PhysicsTools/LiteAnalysis/dataformats/adapters/apatMET.h"
typedef Buffer<pat::MET, apatMET> patMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMET_t, "patMET");

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "PhysicsTools/LiteAnalysis/dataformats/adapters/apatMuon.h"
typedef Buffer<pat::Muon, apatMuon> patMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMuon_t, "patMuon");

#include "DataFormats/PatCandidates/interface/Photon.h"
#include "PhysicsTools/LiteAnalysis/dataformats/adapters/apatPhoton.h"
typedef Buffer<pat::Photon, apatPhoton> patPhoton_t;
DEFINE_EDM_PLUGIN(BufferFactory, patPhoton_t, "patPhoton");

#include "DataFormats/PatCandidates/interface/Tau.h"
#include "PhysicsTools/LiteAnalysis/dataformats/adapters/apatTau.h"
typedef Buffer<pat::Tau, apatTau> patTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTau_t, "patTau");

#include "DataFormats/EgammaReco/interface/BasicCluster.h"
typedef Buffer<reco::BasicCluster, reco::BasicCluster> recoBasicCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBasicCluster_t, "recoBasicCluster");

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
typedef Buffer<reco::BeamSpot, reco::BeamSpot> recoBeamSpot_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBeamSpot_t, "recoBeamSpot");

#include "DataFormats/JetReco/interface/CaloJet.h"
typedef Buffer<reco::CaloJet, reco::CaloJet> recoCaloJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloJet_t, "recoCaloJet");

#include "DataFormats/METReco/interface/CaloMET.h"
typedef Buffer<reco::CaloMET, reco::CaloMET> recoCaloMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloMET_t, "recoCaloMET");

#include "DataFormats/MuonReco/interface/CaloMuon.h"
typedef Buffer<reco::CaloMuon, reco::CaloMuon> recoCaloMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloMuon_t, "recoCaloMuon");

#include "DataFormats/TauReco/interface/CaloTau.h"
typedef Buffer<reco::CaloTau, reco::CaloTau> recoCaloTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloTau_t, "recoCaloTau");

#include "DataFormats/TauReco/interface/CaloTauTagInfo.h"
typedef Buffer<reco::CaloTauTagInfo, reco::CaloTauTagInfo> recoCaloTauTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloTauTagInfo_t, "recoCaloTauTagInfo");

#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "PhysicsTools/LiteAnalysis/dataformats/adapters/arecoConversion.h"
typedef Buffer<reco::Conversion, arecoConversion> recoConversion_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoConversion_t, "recoConversion");

#include "DataFormats/EgammaCandidates/interface/Electron.h"
typedef Buffer<reco::Electron, reco::Electron> recoElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoElectron_t, "recoElectron");

#include "DataFormats/EgammaReco/interface/ElectronPixelSeed.h"
typedef Buffer<reco::ElectronPixelSeed, reco::ElectronPixelSeed> recoElectronPixelSeed_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoElectronPixelSeed_t, "recoElectronPixelSeed");

#include "DataFormats/JetReco/interface/GenJet.h"
typedef Buffer<reco::GenJet, reco::GenJet> recoGenJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGenJet_t, "recoGenJet");

#include "DataFormats/METReco/interface/GenMET.h"
typedef Buffer<reco::GenMET, reco::GenMET> recoGenMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGenMET_t, "recoGenMET");

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
typedef Buffer<reco::GenParticle, reco::GenParticle> recoGenParticle_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGenParticle_t, "recoGenParticle");

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
typedef Buffer<reco::GsfElectron, reco::GsfElectron> recoGsfElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGsfElectron_t, "recoGsfElectron");

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
typedef Buffer<reco::GsfTrack, reco::GsfTrack> recoGsfTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGsfTrack_t, "recoGsfTrack");

#include "DataFormats/METReco/interface/MET.h"
typedef Buffer<reco::MET, reco::MET> recoMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMET_t, "recoMET");

#include "DataFormats/MuonReco/interface/Muon.h"
typedef Buffer<reco::Muon, reco::Muon> recoMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuon_t, "recoMuon");

#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
typedef Buffer<reco::PFBlock, reco::PFBlock> recoPFBlock_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFBlock_t, "recoPFBlock");

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
typedef Buffer<reco::PFCandidate, reco::PFCandidate> recoPFCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFCandidate_t, "recoPFCandidate");

#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
typedef Buffer<reco::PFCluster, reco::PFCluster> recoPFCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFCluster_t, "recoPFCluster");

#include "DataFormats/JetReco/interface/PFJet.h"
typedef Buffer<reco::PFJet, reco::PFJet> recoPFJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFJet_t, "recoPFJet");

#include "DataFormats/METReco/interface/PFMET.h"
typedef Buffer<reco::PFMET, reco::PFMET> recoPFMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFMET_t, "recoPFMET");

#include "DataFormats/TauReco/interface/PFTau.h"
typedef Buffer<reco::PFTau, reco::PFTau> recoPFTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTau_t, "recoPFTau");

#include "DataFormats/TauReco/interface/PFTauTagInfo.h"
typedef Buffer<reco::PFTauTagInfo, reco::PFTauTagInfo> recoPFTauTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTauTagInfo_t, "recoPFTauTagInfo");

#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "PhysicsTools/LiteAnalysis/dataformats/adapters/arecoPhoton.h"
typedef Buffer<reco::Photon, arecoPhoton> recoPhoton_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPhoton_t, "recoPhoton");

#include "DataFormats/EgammaCandidates/interface/PhotonID.h"
typedef Buffer<reco::PhotonID, reco::PhotonID> recoPhotonID_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPhotonID_t, "recoPhotonID");

#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
typedef Buffer<reco::PreshowerCluster, reco::PreshowerCluster> recoPreshowerCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreshowerCluster_t, "recoPreshowerCluster");

#include "DataFormats/EgammaReco/interface/PreshowerClusterShape.h"
typedef Buffer<reco::PreshowerClusterShape, reco::PreshowerClusterShape> recoPreshowerClusterShape_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreshowerClusterShape_t, "recoPreshowerClusterShape");

#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
typedef Buffer<reco::SecondaryVertexTagInfo, reco::SecondaryVertexTagInfo> recoSecondaryVertexTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSecondaryVertexTagInfo_t, "recoSecondaryVertexTagInfo");

#include "DataFormats/BTauReco/interface/SoftLeptonTagInfo.h"
typedef Buffer<reco::SoftLeptonTagInfo, reco::SoftLeptonTagInfo> recoSoftLeptonTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSoftLeptonTagInfo_t, "recoSoftLeptonTagInfo");

#include "DataFormats/EgammaReco/interface/SuperCluster.h"
typedef Buffer<reco::SuperCluster, reco::SuperCluster> recoSuperCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSuperCluster_t, "recoSuperCluster");

#include "DataFormats/TrackReco/interface/Track.h"
typedef Buffer<reco::Track, reco::Track> recoTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrack_t, "recoTrack");

#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
typedef Buffer<reco::TrackIPTagInfo, reco::TrackIPTagInfo> recoTrackIPTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackIPTagInfo_t, "recoTrackIPTagInfo");

#include "DataFormats/VertexReco/interface/Vertex.h"
typedef Buffer<reco::Vertex, reco::Vertex> recoVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoVertex_t, "recoVertex");
