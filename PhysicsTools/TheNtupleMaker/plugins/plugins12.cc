// -------------------------------------------------------------------------
// File::   plugins12.cc
// Created: Sat May  5 16:09:43 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "DataFormats/HLTReco/interface/HLTPrescaleTable.h"
#include "DataFormats/HLTReco/interface/HLTResult.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/HepMCCandidate/interface/PdfInfo.h"
#include "DataFormats/JetReco/interface/FFTJetProducerSummary.h"
#include "DataFormats/JetReco/interface/JPTJetCollection.h"
#include "DataFormats/JetReco/interface/PFClusterJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/PattRecoPeak.h"
#include "DataFormats/JetReco/interface/PattRecoTree.h"
#include "DataFormats/JetReco/interface/TrackJetCollection.h"
#include "DataFormats/METReco/interface/EcalHaloData.h"
#include "DataFormats/METReco/interface/GlobalHaloData.h"
#include "DataFormats/METReco/interface/HcalHaloData.h"
#include "DataFormats/METReco/interface/PhiWedge.h"
#include "DataFormats/TauReco/interface/JetPiZeroAssociation.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminatorByIsolation.h"
// -------------------------------------------------------------------------

std::string recoEcalHaloData_n("reco::EcalHaloData");
typedef Buffer<reco::EcalHaloData,
               &recoEcalHaloData_n, SINGLETON>
recoEcalHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoEcalHaloData_t,
                  "recoEcalHaloData");
				  
std::string recoFFTJetProducerSummary_n("reco::FFTJetProducerSummary");
typedef Buffer<reco::FFTJetProducerSummary,
               &recoFFTJetProducerSummary_n, SINGLETON>
recoFFTJetProducerSummary_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoFFTJetProducerSummary_t,
                  "recoFFTJetProducerSummary");
				  
std::string recoGlobalHaloData_n("reco::GlobalHaloData");
typedef Buffer<reco::GlobalHaloData,
               &recoGlobalHaloData_n, SINGLETON>
recoGlobalHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoGlobalHaloData_t,
                  "recoGlobalHaloData");
				  
std::string recoHLTResult16unsignedshort_n("reco::HLTResult<16,unsigned short>");
typedef Buffer<reco::HLTResult<16,unsigned short>,
               &recoHLTResult16unsignedshort_n, SINGLETON>
recoHLTResult16unsignedshort_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHLTResult16unsignedshort_t,
                  "recoHLTResult16unsignedshort");
				  
std::string recoHLTResult24unsignedshort_n("reco::HLTResult<24,unsigned short>");
typedef Buffer<reco::HLTResult<24,unsigned short>,
               &recoHLTResult24unsignedshort_n, SINGLETON>
recoHLTResult24unsignedshort_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHLTResult24unsignedshort_t,
                  "recoHLTResult24unsignedshort");
				  
std::string recoHLTResult8unsignedshort_n("reco::HLTResult<8,unsigned short>");
typedef Buffer<reco::HLTResult<8,unsigned short>,
               &recoHLTResult8unsignedshort_n, SINGLETON>
recoHLTResult8unsignedshort_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHLTResult8unsignedshort_t,
                  "recoHLTResult8unsignedshort");
				  
std::string recoHcalHaloData_n("reco::HcalHaloData");
typedef Buffer<reco::HcalHaloData,
               &recoHcalHaloData_n, SINGLETON>
recoHcalHaloData_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoHcalHaloData_t,
                  "recoHcalHaloData");
				  
std::string recoJPTJetRefVector_n("reco::JPTJetRefVector");
typedef Buffer<reco::JPTJetRefVector,
               &recoJPTJetRefVector_n, SINGLETON>
recoJPTJetRefVector_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoJPTJetRefVector_t,
                  "recoJPTJetRefVector");
				  
std::string recoJetPiZeroAssociation_n("reco::JetPiZeroAssociation");
typedef Buffer<reco::JetPiZeroAssociation,
               &recoJetPiZeroAssociation_n, SINGLETON>
recoJetPiZeroAssociation_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoJetPiZeroAssociation_t,
                  "recoJetPiZeroAssociation");
				  
std::string recoPFClusterJetRefVector_n("reco::PFClusterJetRefVector");
typedef Buffer<reco::PFClusterJetRefVector,
               &recoPFClusterJetRefVector_n, SINGLETON>
recoPFClusterJetRefVector_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFClusterJetRefVector_t,
                  "recoPFClusterJetRefVector");
				  
std::string recoPFJetRefVector_n("reco::PFJetRefVector");
typedef Buffer<reco::PFJetRefVector,
               &recoPFJetRefVector_n, SINGLETON>
recoPFJetRefVector_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFJetRefVector_t,
                  "recoPFJetRefVector");
				  
std::string recoPFTauDiscriminator_n("reco::PFTauDiscriminator");
typedef Buffer<reco::PFTauDiscriminator,
               &recoPFTauDiscriminator_n, SINGLETON>
recoPFTauDiscriminator_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTauDiscriminator_t,
                  "recoPFTauDiscriminator");
				  
std::string recoPFTauDiscriminatorByIsolation_n("reco::PFTauDiscriminatorByIsolation");
typedef Buffer<reco::PFTauDiscriminatorByIsolation,
               &recoPFTauDiscriminatorByIsolation_n, SINGLETON>
recoPFTauDiscriminatorByIsolation_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTauDiscriminatorByIsolation_t,
                  "recoPFTauDiscriminatorByIsolation");
				  
std::string recoPattRecoTreedoublerecoPattRecoPeakdouble_n("reco::PattRecoTree<double,reco::PattRecoPeak<double> >");
typedef Buffer<reco::PattRecoTree<double,reco::PattRecoPeak<double> >,
               &recoPattRecoTreedoublerecoPattRecoPeakdouble_n, SINGLETON>
recoPattRecoTreedoublerecoPattRecoPeakdouble_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPattRecoTreedoublerecoPattRecoPeakdouble_t,
                  "recoPattRecoTreedoublerecoPattRecoPeakdouble");
				  
std::string recoPattRecoTreefloatrecoPattRecoPeakfloat_n("reco::PattRecoTree<float,reco::PattRecoPeak<float> >");
typedef Buffer<reco::PattRecoTree<float,reco::PattRecoPeak<float> >,
               &recoPattRecoTreefloatrecoPattRecoPeakfloat_n, SINGLETON>
recoPattRecoTreefloatrecoPattRecoPeakfloat_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPattRecoTreefloatrecoPattRecoPeakfloat_t,
                  "recoPattRecoTreefloatrecoPattRecoPeakfloat");
				  
std::string recoPdfInfo_n("reco::PdfInfo");
typedef Buffer<reco::PdfInfo,
               &recoPdfInfo_n, SINGLETON>
recoPdfInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPdfInfo_t,
                  "recoPdfInfo");
				  
std::string recoPhiWedge_n("reco::PhiWedge");
typedef Buffer<reco::PhiWedge,
               &recoPhiWedge_n, SINGLETON>
recoPhiWedge_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPhiWedge_t,
                  "recoPhiWedge");
				  
std::string recoTrackJetRefVector_n("reco::TrackJetRefVector");
typedef Buffer<reco::TrackJetRefVector,
               &recoTrackJetRefVector_n, SINGLETON>
recoTrackJetRefVector_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoTrackJetRefVector_t,
                  "recoTrackJetRefVector");
				  
std::string triggerHLTPrescaleTable_n("trigger::HLTPrescaleTable");
typedef Buffer<trigger::HLTPrescaleTable,
               &triggerHLTPrescaleTable_n, SINGLETON>
triggerHLTPrescaleTable_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerHLTPrescaleTable_t,
                  "triggerHLTPrescaleTable");
				  
std::string triggerTriggerEvent_n("trigger::TriggerEvent");
typedef Buffer<trigger::TriggerEvent,
               &triggerTriggerEvent_n, SINGLETON>
triggerTriggerEvent_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerTriggerEvent_t,
                  "triggerTriggerEvent");
				  
std::string triggerTriggerEventWithRefs_n("trigger::TriggerEventWithRefs");
typedef Buffer<trigger::TriggerEventWithRefs,
               &triggerTriggerEventWithRefs_n, SINGLETON>
triggerTriggerEventWithRefs_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerTriggerEventWithRefs_t,
                  "triggerTriggerEventWithRefs");
				  
std::string triggerTriggerFilterObjectWithRefs_n("trigger::TriggerFilterObjectWithRefs");
typedef Buffer<trigger::TriggerFilterObjectWithRefs,
               &triggerTriggerFilterObjectWithRefs_n, SINGLETON>
triggerTriggerFilterObjectWithRefs_t;
DEFINE_EDM_PLUGIN(BufferFactory, triggerTriggerFilterObjectWithRefs_t,
                  "triggerTriggerFilterObjectWithRefs");
				  