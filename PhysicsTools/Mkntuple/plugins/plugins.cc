// ----------------------------------------------------------------------------
// Created: Fri Oct  8 19:16:16 2010 by mkplugins.py
// $Revision: 1.14 $
// ----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"

#include "DataFormats/Scalers/interface/BeamSpotOnline.h"
typedef Buffer<BeamSpotOnline, false> BeamSpotOnline_t;
DEFINE_EDM_PLUGIN(BufferFactory, BeamSpotOnline_t,
                  "BeamSpotOnline");

#include "DataFormats/Scalers/interface/DcsStatus.h"
typedef Buffer<DcsStatus, false> DcsStatus_t;
DEFINE_EDM_PLUGIN(BufferFactory, DcsStatus_t,
                  "DcsStatus");

#include "DataFormats/Scalers/interface/L1AcceptBunchCrossing.h"
typedef Buffer<L1AcceptBunchCrossing, false> L1AcceptBunchCrossing_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1AcceptBunchCrossing_t,
                  "L1AcceptBunchCrossing");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEmCand.h"
typedef Buffer<L1GctEmCand, false> L1GctEmCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEmCand_t,
                  "L1GctEmCand");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtHad.h"
typedef Buffer<L1GctEtHad, false> L1GctEtHad_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtHad_t,
                  "L1GctEtHad");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtMiss.h"
typedef Buffer<L1GctEtMiss, false> L1GctEtMiss_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtMiss_t,
                  "L1GctEtMiss");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtTotal.h"
typedef Buffer<L1GctEtTotal, false> L1GctEtTotal_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtTotal_t,
                  "L1GctEtTotal");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFBitCounts.h"
typedef Buffer<L1GctHFBitCounts, false> L1GctHFBitCounts_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHFBitCounts_t,
                  "L1GctHFBitCounts");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFRingEtSums.h"
typedef Buffer<L1GctHFRingEtSums, false> L1GctHFRingEtSums_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHFRingEtSums_t,
                  "L1GctHFRingEtSums");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHtMiss.h"
typedef Buffer<L1GctHtMiss, false> L1GctHtMiss_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHtMiss_t,
                  "L1GctHtMiss");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCand.h"
typedef Buffer<L1GctJetCand, false> L1GctJetCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctJetCand_t,
                  "L1GctJetCand");

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCounts.h"
typedef Buffer<L1GctJetCounts, false> L1GctJetCounts_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctJetCounts_t,
                  "L1GctJetCounts");

#include "DataFormats/Scalers/interface/L1TriggerScalers.h"
typedef Buffer<L1TriggerScalers, false> L1TriggerScalers_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1TriggerScalers_t,
                  "L1TriggerScalers");

#include "DataFormats/Scalers/interface/Level1TriggerScalers.h"
typedef Buffer<Level1TriggerScalers, false> Level1TriggerScalers_t;
DEFINE_EDM_PLUGIN(BufferFactory, Level1TriggerScalers_t,
                  "Level1TriggerScalers");

#include "DataFormats/Scalers/interface/LumiScalers.h"
typedef Buffer<LumiScalers, false> LumiScalers_t;
DEFINE_EDM_PLUGIN(BufferFactory, LumiScalers_t,
                  "LumiScalers");

#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
typedef Buffer<TrajectorySeed, false> TrajectorySeed_t;
DEFINE_EDM_PLUGIN(BufferFactory, TrajectorySeed_t,
                  "TrajectorySeed");

#include "FWCore/MessageLogger/interface/ErrorSummaryEntry.h"
typedef Buffer<edm::ErrorSummaryEntry, false> edmErrorSummaryEntry_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmErrorSummaryEntry_t,
                  "edmErrorSummaryEntry");

#include "DataFormats/PatCandidates/interface/Electron.h"
typedef Buffer<pat::Electron, false> patElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, patElectron_t,
                  "patElectron");

#include "DataFormats/PatCandidates/interface/Hemisphere.h"
typedef Buffer<pat::Hemisphere, false> patHemisphere_t;
DEFINE_EDM_PLUGIN(BufferFactory, patHemisphere_t,
                  "patHemisphere");

#include "DataFormats/PatCandidates/interface/Jet.h"
typedef Buffer<pat::Jet, false> patJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, patJet_t,
                  "patJet");

#include "DataFormats/PatCandidates/interface/MET.h"
typedef Buffer<pat::MET, false> patMET_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMET_t,
                  "patMET");

#include "DataFormats/PatCandidates/interface/MHT.h"
typedef Buffer<pat::MHT, false> patMHT_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMHT_t,
                  "patMHT");

#include "DataFormats/PatCandidates/interface/Muon.h"
typedef Buffer<pat::Muon, false> patMuon_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMuon_t,
                  "patMuon");

#include "DataFormats/PatCandidates/interface/Photon.h"
typedef Buffer<pat::Photon, false> patPhoton_t;
DEFINE_EDM_PLUGIN(BufferFactory, patPhoton_t,
                  "patPhoton");

#include "DataFormats/PatCandidates/interface/Tau.h"
typedef Buffer<pat::Tau, false> patTau_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTau_t,
                  "patTau");

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

#include "DataFormats/CastorReco/interface/CastorCluster.h"
typedef Buffer<reco::CastorCluster, false> recoCastorCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorCluster_t,
                  "recoCastorCluster");

#include "DataFormats/CastorReco/interface/CastorEgamma.h"
typedef Buffer<reco::CastorEgamma, false> recoCastorEgamma_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorEgamma_t,
                  "recoCastorEgamma");

#include "DataFormats/CastorReco/interface/CastorJet.h"
typedef Buffer<reco::CastorJet, false> recoCastorJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCastorJet_t,
                  "recoCastorJet");

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

#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
typedef Buffer<reco::PFCluster, false> recoPFCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFCluster_t,
                  "recoPFCluster");

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

#include "DataFormats/TauReco/interface/PFTauTagInfo.h"
typedef Buffer<reco::PFTauTagInfo, false> recoPFTauTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTauTagInfo_t,
                  "recoPFTauTagInfo");

#include "DataFormats/EgammaCandidates/interface/Photon.h"
typedef Buffer<reco::Photon, false> recoPhoton_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPhoton_t,
                  "recoPhoton");

#include "DataFormats/EgammaCandidates/interface/PhotonCore.h"
typedef Buffer<reco::PhotonCore, false> recoPhotonCore_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPhotonCore_t,
                  "recoPhotonCore");

#include "DataFormats/ParticleFlowReco/interface/PreId.h"
typedef Buffer<reco::PreId, false> recoPreId_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreId_t,
                  "recoPreId");

#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
typedef Buffer<reco::PreshowerCluster, false> recoPreshowerCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreshowerCluster_t,
                  "recoPreshowerCluster");

#include "DataFormats/EgammaReco/interface/PreshowerClusterShape.h"
typedef Buffer<reco::PreshowerClusterShape, false> recoPreshowerClusterShape_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPreshowerClusterShape_t,
                  "recoPreshowerClusterShape");

#include "DataFormats/RecoCandidate/interface/RecoChargedRefCandidate.h"
typedef Buffer<reco::RecoChargedRefCandidate, false> recoRecoChargedRefCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoChargedRefCandidate_t,
                  "recoRecoChargedRefCandidate");

#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
typedef Buffer<reco::RecoEcalCandidate, false> recoRecoEcalCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoRecoEcalCandidate_t,
                  "recoRecoEcalCandidate");

#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
typedef Buffer<reco::SecondaryVertexTagInfo, false> recoSecondaryVertexTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSecondaryVertexTagInfo_t,
                  "recoSecondaryVertexTagInfo");

#include "DataFormats/BTauReco/interface/SoftLeptonTagInfo.h"
typedef Buffer<reco::SoftLeptonTagInfo, false> recoSoftLeptonTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSoftLeptonTagInfo_t,
                  "recoSoftLeptonTagInfo");

#include "DataFormats/EgammaReco/interface/SuperCluster.h"
typedef Buffer<reco::SuperCluster, false> recoSuperCluster_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoSuperCluster_t,
                  "recoSuperCluster");

#include "DataFormats/TrackReco/interface/Track.h"
typedef Buffer<reco::Track, false> recoTrack_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrack_t,
                  "recoTrack");

#include "DataFormats/TrackReco/interface/TrackExtra.h"
typedef Buffer<reco::TrackExtra, false> recoTrackExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackExtra_t,
                  "recoTrackExtra");

#include "DataFormats/JetReco/interface/TrackExtrapolation.h"
typedef Buffer<reco::TrackExtrapolation, false> recoTrackExtrapolation_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackExtrapolation_t,
                  "recoTrackExtrapolation");

#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
typedef Buffer<reco::TrackIPTagInfo, false> recoTrackIPTagInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackIPTagInfo_t,
                  "recoTrackIPTagInfo");

#include "DataFormats/JetReco/interface/TrackJet.h"
typedef Buffer<reco::TrackJet, false> recoTrackJet_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackJet_t,
                  "recoTrackJet");

#include "DataFormats/VertexReco/interface/Vertex.h"
typedef Buffer<reco::Vertex, false> recoVertex_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoVertex_t,
                  "recoVertex");

#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
typedef Buffer<reco::VertexCompositeCandidate, false> recoVertexCompositeCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoVertexCompositeCandidate_t,
                  "recoVertexCompositeCandidate");

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
typedef Buffer<GenEventInfoProduct, true> GenEventInfoProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenEventInfoProduct_t,
                  "GenEventInfoProduct");

#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
typedef Buffer<GenRunInfoProduct, true> GenRunInfoProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenRunInfoProduct_t,
                  "GenRunInfoProduct");

#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
typedef Buffer<HcalNoiseSummary, true> HcalNoiseSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, HcalNoiseSummary_t,
                  "HcalNoiseSummary");

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
typedef Buffer<L1GlobalTriggerObjectMapRecord, true> L1GlobalTriggerObjectMapRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerObjectMapRecord_t,
                  "L1GlobalTriggerObjectMapRecord");

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
typedef Buffer<L1GlobalTriggerReadoutRecord, true> L1GlobalTriggerReadoutRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerReadoutRecord_t,
                  "L1GlobalTriggerReadoutRecord");

#include "DataFormats/Common/interface/ConditionsInEdm.h"
typedef Buffer<edm::ConditionsInEventBlock, true> edmConditionsInEventBlock_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmConditionsInEventBlock_t,
                  "edmConditionsInEventBlock");

#include "FWCore/Framework/interface/Event.h"
typedef Buffer<edm::Event, true> edmEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmEvent_t,
                  "edmEvent");

#include "DataFormats/Common/interface/TriggerResults.h"
typedef Buffer<edm::TriggerResults, true> edmTriggerResults_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmTriggerResults_t,
                  "edmTriggerResults");

#include "DataFormats/METReco/interface/BeamHaloSummary.h"
typedef Buffer<reco::BeamHaloSummary, true> recoBeamHaloSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBeamHaloSummary_t,
                  "recoBeamHaloSummary");

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
typedef Buffer<reco::BeamSpot, true> recoBeamSpot_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBeamSpot_t,
                  "recoBeamSpot");

#include "DataFormats/METReco/interface/CSCHaloData.h"
typedef Buffer<reco::CSCHaloData, true> recoCSCHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCSCHaloData_t,
                  "recoCSCHaloData");

#include "DataFormats/TauReco/interface/CaloTauDiscriminator.h"
typedef Buffer<reco::CaloTauDiscriminator, true> recoCaloTauDiscriminator_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloTauDiscriminator_t,
                  "recoCaloTauDiscriminator");

#include "DataFormats/METReco/interface/EcalHaloData.h"
typedef Buffer<reco::EcalHaloData, true> recoEcalHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoEcalHaloData_t,
                  "recoEcalHaloData");

#include "DataFormats/METReco/interface/GlobalHaloData.h"
typedef Buffer<reco::GlobalHaloData, true> recoGlobalHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGlobalHaloData_t,
                  "recoGlobalHaloData");

#include "DataFormats/METReco/interface/HcalHaloData.h"
typedef Buffer<reco::HcalHaloData, true> recoHcalHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHcalHaloData_t,
                  "recoHcalHaloData");

#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
typedef Buffer<reco::PFTauDiscriminator, true> recoPFTauDiscriminator_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTauDiscriminator_t,
                  "recoPFTauDiscriminator");

#include "DataFormats/HLTReco/interface/TriggerEvent.h"
typedef Buffer<trigger::TriggerEvent, true> triggerTriggerEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerTriggerEvent_t,
                  "triggerTriggerEvent");

