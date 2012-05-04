// -------------------------------------------------------------------------
// File::   plugins01.cc
// Created: Thu Apr 26 00:40:14 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/EcalRawData/interface/ESListOfFEDS.h"
#include "DataFormats/EcalRawData/interface/EcalListOfFEDS.h"
#include "DataFormats/HcalCalibObjects/interface/HOCalibVariables.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloEmCand.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEmCand.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtHad.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtMiss.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtTotal.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctFibreWord.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFBitCounts.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFRingEtSums.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHtMiss.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctInternEmCand.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctInternEtSum.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctInternHFData.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctInternHtMiss.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctInternJetData.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCand.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCounts.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtFdlWord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtPsbWord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtTechnicalTrigger.h"
#include "DataFormats/L1Trigger/interface/L1TriggerError.h"
#include "DataFormats/Scalers/interface/BeamSpotOnline.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/Scalers/interface/L1AcceptBunchCrossing.h"
#include "SimDataFormats/CaloHit/interface/HFShowerLibraryEventInfo.h"
#include "SimDataFormats/CaloHit/interface/HFShowerPhoton.h"
// -------------------------------------------------------------------------

std::string BeamSpotOnline_n("BeamSpotOnline");
typedef Buffer<BeamSpotOnline,
               &BeamSpotOnline_n, COLLECTION>
BeamSpotOnline_t;
DEFINE_EDM_PLUGIN(BufferFactory, BeamSpotOnline_t,
                  "BeamSpotOnline");
				  
std::string CaloTower_n("CaloTower");
typedef Buffer<CaloTower,
               &CaloTower_n, COLLECTION>
CaloTower_t;
DEFINE_EDM_PLUGIN(BufferFactory, CaloTower_t,
                  "CaloTower");
				  
std::string DcsStatus_n("DcsStatus");
typedef Buffer<DcsStatus,
               &DcsStatus_n, COLLECTION>
DcsStatus_t;
DEFINE_EDM_PLUGIN(BufferFactory, DcsStatus_t,
                  "DcsStatus");
				  
std::string ESListOfFEDS_n("ESListOfFEDS");
typedef Buffer<ESListOfFEDS,
               &ESListOfFEDS_n, COLLECTION>
ESListOfFEDS_t;
DEFINE_EDM_PLUGIN(BufferFactory, ESListOfFEDS_t,
                  "ESListOfFEDS");
				  
std::string EcalListOfFEDS_n("EcalListOfFEDS");
typedef Buffer<EcalListOfFEDS,
               &EcalListOfFEDS_n, COLLECTION>
EcalListOfFEDS_t;
DEFINE_EDM_PLUGIN(BufferFactory, EcalListOfFEDS_t,
                  "EcalListOfFEDS");
				  
std::string HFShowerLibraryEventInfo_n("HFShowerLibraryEventInfo");
typedef Buffer<HFShowerLibraryEventInfo,
               &HFShowerLibraryEventInfo_n, COLLECTION>
HFShowerLibraryEventInfo_t;
DEFINE_EDM_PLUGIN(BufferFactory, HFShowerLibraryEventInfo_t,
                  "HFShowerLibraryEventInfo");
				  
std::string HFShowerPhoton_n("HFShowerPhoton");
typedef Buffer<HFShowerPhoton,
               &HFShowerPhoton_n, COLLECTION>
HFShowerPhoton_t;
DEFINE_EDM_PLUGIN(BufferFactory, HFShowerPhoton_t,
                  "HFShowerPhoton");
				  
std::string HOCalibVariables_n("HOCalibVariables");
typedef Buffer<HOCalibVariables,
               &HOCalibVariables_n, COLLECTION>
HOCalibVariables_t;
DEFINE_EDM_PLUGIN(BufferFactory, HOCalibVariables_t,
                  "HOCalibVariables");
				  
std::string L1AcceptBunchCrossing_n("L1AcceptBunchCrossing");
typedef Buffer<L1AcceptBunchCrossing,
               &L1AcceptBunchCrossing_n, COLLECTION>
L1AcceptBunchCrossing_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1AcceptBunchCrossing_t,
                  "L1AcceptBunchCrossing");
				  
std::string L1CaloEmCand_n("L1CaloEmCand");
typedef Buffer<L1CaloEmCand,
               &L1CaloEmCand_n, COLLECTION>
L1CaloEmCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1CaloEmCand_t,
                  "L1CaloEmCand");
				  
std::string L1CaloRegion_n("L1CaloRegion");
typedef Buffer<L1CaloRegion,
               &L1CaloRegion_n, COLLECTION>
L1CaloRegion_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1CaloRegion_t,
                  "L1CaloRegion");
				  
std::string L1GctEmCand_n("L1GctEmCand");
typedef Buffer<L1GctEmCand,
               &L1GctEmCand_n, COLLECTION>
L1GctEmCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEmCand_t,
                  "L1GctEmCand");
				  
std::string L1GctEtHad_n("L1GctEtHad");
typedef Buffer<L1GctEtHad,
               &L1GctEtHad_n, COLLECTION>
L1GctEtHad_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtHad_t,
                  "L1GctEtHad");
				  
std::string L1GctEtMiss_n("L1GctEtMiss");
typedef Buffer<L1GctEtMiss,
               &L1GctEtMiss_n, COLLECTION>
L1GctEtMiss_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtMiss_t,
                  "L1GctEtMiss");
				  
std::string L1GctEtTotal_n("L1GctEtTotal");
typedef Buffer<L1GctEtTotal,
               &L1GctEtTotal_n, COLLECTION>
L1GctEtTotal_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtTotal_t,
                  "L1GctEtTotal");
				  
std::string L1GctFibreWord_n("L1GctFibreWord");
typedef Buffer<L1GctFibreWord,
               &L1GctFibreWord_n, COLLECTION>
L1GctFibreWord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctFibreWord_t,
                  "L1GctFibreWord");
				  
std::string L1GctHFBitCounts_n("L1GctHFBitCounts");
typedef Buffer<L1GctHFBitCounts,
               &L1GctHFBitCounts_n, COLLECTION>
L1GctHFBitCounts_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHFBitCounts_t,
                  "L1GctHFBitCounts");
				  
std::string L1GctHFRingEtSums_n("L1GctHFRingEtSums");
typedef Buffer<L1GctHFRingEtSums,
               &L1GctHFRingEtSums_n, COLLECTION>
L1GctHFRingEtSums_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHFRingEtSums_t,
                  "L1GctHFRingEtSums");
				  
std::string L1GctHtMiss_n("L1GctHtMiss");
typedef Buffer<L1GctHtMiss,
               &L1GctHtMiss_n, COLLECTION>
L1GctHtMiss_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHtMiss_t,
                  "L1GctHtMiss");
				  
std::string L1GctInternEmCand_n("L1GctInternEmCand");
typedef Buffer<L1GctInternEmCand,
               &L1GctInternEmCand_n, COLLECTION>
L1GctInternEmCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctInternEmCand_t,
                  "L1GctInternEmCand");
				  
std::string L1GctInternEtSum_n("L1GctInternEtSum");
typedef Buffer<L1GctInternEtSum,
               &L1GctInternEtSum_n, COLLECTION>
L1GctInternEtSum_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctInternEtSum_t,
                  "L1GctInternEtSum");
				  
std::string L1GctInternHFData_n("L1GctInternHFData");
typedef Buffer<L1GctInternHFData,
               &L1GctInternHFData_n, COLLECTION>
L1GctInternHFData_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctInternHFData_t,
                  "L1GctInternHFData");
				  
std::string L1GctInternHtMiss_n("L1GctInternHtMiss");
typedef Buffer<L1GctInternHtMiss,
               &L1GctInternHtMiss_n, COLLECTION>
L1GctInternHtMiss_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctInternHtMiss_t,
                  "L1GctInternHtMiss");
				  
std::string L1GctInternJetData_n("L1GctInternJetData");
typedef Buffer<L1GctInternJetData,
               &L1GctInternJetData_n, COLLECTION>
L1GctInternJetData_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctInternJetData_t,
                  "L1GctInternJetData");
				  
std::string L1GctJetCand_n("L1GctJetCand");
typedef Buffer<L1GctJetCand,
               &L1GctJetCand_n, COLLECTION>
L1GctJetCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctJetCand_t,
                  "L1GctJetCand");
				  
std::string L1GctJetCounts_n("L1GctJetCounts");
typedef Buffer<L1GctJetCounts,
               &L1GctJetCounts_n, COLLECTION>
L1GctJetCounts_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctJetCounts_t,
                  "L1GctJetCounts");
				  
std::string L1GlobalTriggerObjectMap_n("L1GlobalTriggerObjectMap");
typedef Buffer<L1GlobalTriggerObjectMap,
               &L1GlobalTriggerObjectMap_n, COLLECTION>
L1GlobalTriggerObjectMap_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerObjectMap_t,
                  "L1GlobalTriggerObjectMap");
				  
std::string L1GtFdlWord_n("L1GtFdlWord");
typedef Buffer<L1GtFdlWord,
               &L1GtFdlWord_n, COLLECTION>
L1GtFdlWord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GtFdlWord_t,
                  "L1GtFdlWord");
				  
std::string L1GtPsbWord_n("L1GtPsbWord");
typedef Buffer<L1GtPsbWord,
               &L1GtPsbWord_n, COLLECTION>
L1GtPsbWord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GtPsbWord_t,
                  "L1GtPsbWord");
				  
std::string L1GtTechnicalTrigger_n("L1GtTechnicalTrigger");
typedef Buffer<L1GtTechnicalTrigger,
               &L1GtTechnicalTrigger_n, COLLECTION>
L1GtTechnicalTrigger_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GtTechnicalTrigger_t,
                  "L1GtTechnicalTrigger");
				  
std::string L1MuGMTCand_n("L1MuGMTCand");
typedef Buffer<L1MuGMTCand,
               &L1MuGMTCand_n, COLLECTION>
L1MuGMTCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1MuGMTCand_t,
                  "L1MuGMTCand");
				  
std::string L1MuRegionalCand_n("L1MuRegionalCand");
typedef Buffer<L1MuRegionalCand,
               &L1MuRegionalCand_n, COLLECTION>
L1MuRegionalCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1MuRegionalCand_t,
                  "L1MuRegionalCand");
				  
std::string L1TriggerError_n("L1TriggerError");
typedef Buffer<L1TriggerError,
               &L1TriggerError_n, COLLECTION>
L1TriggerError_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1TriggerError_t,
                  "L1TriggerError");
				  