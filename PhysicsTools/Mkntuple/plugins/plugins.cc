// ----------------------------------------------------------------------------
// Created: Sun Feb  7 20:09:59 2010 by mkplugins.py
// $Revision: 1.3 $
// ----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"

#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<HcalNoiseSummary>,
                  "HcalNoiseSummary");

#include "DataFormats/Scalers/interface/L1AcceptBunchCrossing.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<L1AcceptBunchCrossing>,
                  "L1AcceptBunchCrossing");

#include "DataFormats/Scalers/interface/L1TriggerScalers.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<L1TriggerScalers>,
                  "L1TriggerScalers");

#include "DataFormats/Scalers/interface/LumiScalers.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<LumiScalers>,
                  "LumiScalers");

#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<TrajectorySeed>,
                  "TrajectorySeed");

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

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::BeamSpot>,
                  "recoBeamSpot");

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

#include "DataFormats/EgammaCandidates/interface/Conversion.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::Conversion>,
                  "recoConversion");

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

#include "DataFormats/VertexReco/interface/Vertex.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::Vertex>,
                  "recoVertex");

#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<reco::VertexCompositeCandidate>,
                  "recoVertexCompositeCandidate");

#include "DataFormats/HLTReco/interface/TriggerEvent.h"
DEFINE_EDM_PLUGIN(BufferFactory, Buffer<trigger::TriggerEvent>,
                  "triggerTriggerEvent");
