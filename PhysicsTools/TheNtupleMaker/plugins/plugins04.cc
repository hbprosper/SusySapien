// -------------------------------------------------------------------------
// File::   plugins04.cc
// Created: Thu Apr 12 04:32:19 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Candidate/interface/CandMatchMapMany.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/HLTReco/interface/HLTResult.h"
#include "DataFormats/JetReco/interface/DiscretizedEnergyFlow.h"
#include "DataFormats/JetReco/interface/FFTJetPileupSummary.h"
#include "DataFormats/JetReco/interface/FFTJetProducerSummary.h"
#include "DataFormats/METReco/interface/BeamHaloSummary.h"
#include "DataFormats/METReco/interface/CSCHaloData.h"
#include "DataFormats/METReco/interface/EcalHaloData.h"
#include "DataFormats/METReco/interface/GlobalHaloData.h"
#include "DataFormats/METReco/interface/HcalHaloData.h"
#include "DataFormats/MuonReco/interface/MuonTimeExtraMap.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/TriggerPath.h"
#include "DataFormats/TauReco/interface/CaloTauDiscriminator.h"
#include "DataFormats/TauReco/interface/CaloTauDiscriminatorAgainstElectron.h"
#include "DataFormats/TauReco/interface/CaloTauDiscriminatorByIsolation.h"
#include "DataFormats/TauReco/interface/JetPiZeroAssociation.h"
#include "DataFormats/TrackerRecHit2D/interface/ClusterRemovalInfo.h"
// -------------------------------------------------------------------------

typedef Buffer<pat::TriggerObjectStandAloneRefProd, true>
patTriggerObjectStandAloneRefProd_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerObjectStandAloneRefProd_t,
                  "patTriggerObjectStandAloneRefProd");
				  
typedef Buffer<pat::TriggerPathRefProd, true>
patTriggerPathRefProd_t;
DEFINE_EDM_PLUGIN(BufferFactory, patTriggerPathRefProd_t,
                  "patTriggerPathRefProd");
				  
typedef Buffer<reco::BeamHaloSummary, true>
recoBeamHaloSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBeamHaloSummary_t,
                  "recoBeamHaloSummary");
				  
typedef Buffer<reco::BeamSpot, true>
recoBeamSpot_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoBeamSpot_t,
                  "recoBeamSpot");
				  
typedef Buffer<reco::CSCHaloData, true>
recoCSCHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCSCHaloData_t,
                  "recoCSCHaloData");
				  
typedef Buffer<reco::CaloTauDiscriminator, true>
recoCaloTauDiscriminator_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloTauDiscriminator_t,
                  "recoCaloTauDiscriminator");
				  
typedef Buffer<reco::CaloTauDiscriminatorAgainstElectron, true>
recoCaloTauDiscriminatorAgainstElectron_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloTauDiscriminatorAgainstElectron_t,
                  "recoCaloTauDiscriminatorAgainstElectron");
				  
typedef Buffer<reco::CaloTauDiscriminatorByIsolation, true>
recoCaloTauDiscriminatorByIsolation_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCaloTauDiscriminatorByIsolation_t,
                  "recoCaloTauDiscriminatorByIsolation");
				  
typedef Buffer<reco::CandMatchMapMany, true>
recoCandMatchMapMany_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCandMatchMapMany_t,
                  "recoCandMatchMapMany");
				  
typedef Buffer<reco::ClusterRemovalInfo, true>
recoClusterRemovalInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoClusterRemovalInfo_t,
                  "recoClusterRemovalInfo");
				  
typedef Buffer<reco::CompositeCandidate, true>
recoCompositeCandidate_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoCompositeCandidate_t,
                  "recoCompositeCandidate");
				  
typedef Buffer<reco::DiscretizedEnergyFlow, true>
recoDiscretizedEnergyFlow_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoDiscretizedEnergyFlow_t,
                  "recoDiscretizedEnergyFlow");
				  
typedef Buffer<reco::EcalHaloData, true>
recoEcalHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoEcalHaloData_t,
                  "recoEcalHaloData");
				  
typedef Buffer<reco::FFTJetPileupSummary, true>
recoFFTJetPileupSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoFFTJetPileupSummary_t,
                  "recoFFTJetPileupSummary");
				  
typedef Buffer<reco::FFTJetProducerSummary, true>
recoFFTJetProducerSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoFFTJetProducerSummary_t,
                  "recoFFTJetProducerSummary");
				  
typedef Buffer<reco::GlobalHaloData, true>
recoGlobalHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGlobalHaloData_t,
                  "recoGlobalHaloData");
				  
typedef Buffer<reco::HLTResult< 8, unsigned short>, true>
recoHLTResult8unsignedshort_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHLTResult8unsignedshort_t,
                  "recoHLTResult8unsignedshort");
				  
typedef Buffer<reco::HLTResult<16, unsigned short>, true>
recoHLTResult16unsignedshort_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHLTResult16unsignedshort_t,
                  "recoHLTResult16unsignedshort");
				  
typedef Buffer<reco::HLTResult<24, unsigned short>, true>
recoHLTResult24unsignedshort_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHLTResult24unsignedshort_t,
                  "recoHLTResult24unsignedshort");
				  
typedef Buffer<reco::HcalHaloData, true>
recoHcalHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHcalHaloData_t,
                  "recoHcalHaloData");
				  
typedef Buffer<reco::JetPiZeroAssociation, true>
recoJetPiZeroAssociation_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoJetPiZeroAssociation_t,
                  "recoJetPiZeroAssociation");
				  
typedef Buffer<reco::MuonTimeExtraMap, true>
recoMuonTimeExtraMap_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoMuonTimeExtraMap_t,
                  "recoMuonTimeExtraMap");
				  