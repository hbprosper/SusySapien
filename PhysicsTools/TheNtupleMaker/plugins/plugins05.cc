// -------------------------------------------------------------------------
// File::   plugins05.cc
// Created: Tue Apr  3 22:26:00 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EcalScDetId.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/EcalRawData/interface/ESListOfFEDS.h"
#include "DataFormats/EcalRawData/interface/EcalListOfFEDS.h"
#include "DataFormats/HcalCalibObjects/interface/HOCalibVariables.h"
#include "DataFormats/HepMCCandidate/interface/PdfInfo.h"
#include "DataFormats/JetReco/interface/PattRecoPeak.h"
#include "DataFormats/JetReco/interface/PattRecoTree.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1CSCSPStatusDigi.h"
#include "DataFormats/METReco/interface/PhiWedge.h"
#include "DataFormats/Scalers/interface/BeamSpotOnline.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/SiPixelRawData/interface/SiPixelRawDataError.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminatorByIsolation.h"
#include "SimDataFormats/CaloHit/interface/HFShowerLibraryEventInfo.h"
#include "SimDataFormats/CaloHit/interface/HFShowerPhoton.h"
// -------------------------------------------------------------------------

typedef Buffer<reco::PFTauDiscriminator, true>
recoPFTauDiscriminator_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTauDiscriminator_t,
                  "recoPFTauDiscriminator");
				  
typedef Buffer<reco::PFTauDiscriminatorByIsolation, true>
recoPFTauDiscriminatorByIsolation_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPFTauDiscriminatorByIsolation_t,
                  "recoPFTauDiscriminatorByIsolation");
				  
typedef Buffer<reco::PattRecoTree<double,reco::PattRecoPeak<double> >, true>
recoPattRecoTreedoublerecoPattRecoPeakdouble_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPattRecoTreedoublerecoPattRecoPeakdouble_t,
                  "recoPattRecoTreedoublerecoPattRecoPeakdouble");
				  
typedef Buffer<reco::PattRecoTree<float,reco::PattRecoPeak<float> >, true>
recoPattRecoTreefloatrecoPattRecoPeakfloat_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPattRecoTreefloatrecoPattRecoPeakfloat_t,
                  "recoPattRecoTreefloatrecoPattRecoPeakfloat");
				  
typedef Buffer<reco::PdfInfo, true>
recoPdfInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPdfInfo_t,
                  "recoPdfInfo");
				  
typedef Buffer<reco::PhiWedge, true>
recoPhiWedge_t;
DEFINE_EDM_PLUGIN(BufferFactory, recoPhiWedge_t,
                  "recoPhiWedge");
				  
typedef Buffer<std::map<int, std::vector<SiPixelRawDataError> >, true>
stdmapintstdvectorSiPixelRawDataError_t;
DEFINE_EDM_PLUGIN(BufferFactory, stdmapintstdvectorSiPixelRawDataError_t,
                  "stdmapintstdvectorSiPixelRawDataError");
				  
typedef Buffer<std::pair< int, std::vector<L1CSCSPStatusDigi> >, true>
stdpairintstdvectorL1CSCSPStatusDigi_t;
DEFINE_EDM_PLUGIN(BufferFactory, stdpairintstdvectorL1CSCSPStatusDigi_t,
                  "stdpairintstdvectorL1CSCSPStatusDigi");
				  
typedef Buffer<std::set<EBDetId>, true>
stdsetEBDetId_t;
DEFINE_EDM_PLUGIN(BufferFactory, stdsetEBDetId_t,
                  "stdsetEBDetId");
				  
typedef Buffer<std::set<EEDetId>, true>
stdsetEEDetId_t;
DEFINE_EDM_PLUGIN(BufferFactory, stdsetEEDetId_t,
                  "stdsetEEDetId");
				  
typedef Buffer<std::set<EcalScDetId>, true>
stdsetEcalScDetId_t;
DEFINE_EDM_PLUGIN(BufferFactory, stdsetEcalScDetId_t,
                  "stdsetEcalScDetId");
				  
typedef Buffer<std::set<EcalTrigTowerDetId>, true>
stdsetEcalTrigTowerDetId_t;
DEFINE_EDM_PLUGIN(BufferFactory, stdsetEcalTrigTowerDetId_t,
                  "stdsetEcalTrigTowerDetId");
				  
typedef Buffer<BeamSpotOnline, false>
BeamSpotOnline_t;
DEFINE_EDM_PLUGIN(BufferFactory, BeamSpotOnline_t,
                  "BeamSpotOnline");
				  
typedef Buffer<CaloTower, false>
CaloTower_t;
DEFINE_EDM_PLUGIN(BufferFactory, CaloTower_t,
                  "CaloTower");
				  
typedef Buffer<DcsStatus, false>
DcsStatus_t;
DEFINE_EDM_PLUGIN(BufferFactory, DcsStatus_t,
                  "DcsStatus");
				  
typedef Buffer<ESListOfFEDS, false>
ESListOfFEDS_t;
DEFINE_EDM_PLUGIN(BufferFactory, ESListOfFEDS_t,
                  "ESListOfFEDS");
				  
typedef Buffer<EcalListOfFEDS, false>
EcalListOfFEDS_t;
DEFINE_EDM_PLUGIN(BufferFactory, EcalListOfFEDS_t,
                  "EcalListOfFEDS");
				  
typedef Buffer<HFShowerLibraryEventInfo, false>
HFShowerLibraryEventInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, HFShowerLibraryEventInfo_t,
                  "HFShowerLibraryEventInfo");
				  
typedef Buffer<HFShowerPhoton, false>
HFShowerPhoton_t;
DEFINE_EDM_PLUGIN(BufferFactory, HFShowerPhoton_t,
                  "HFShowerPhoton");
				  
typedef Buffer<HOCalibVariables, false>
HOCalibVariables_t;
DEFINE_EDM_PLUGIN(BufferFactory, HOCalibVariables_t,
                  "HOCalibVariables");
				  