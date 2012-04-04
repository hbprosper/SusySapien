// -------------------------------------------------------------------------
// File::   plugins06.cc
// Created: Tue Apr  3 21:40:06 2012 by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------

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
#include "DataFormats/MuonSeed/interface/L2MuonTrajectorySeed.h"
#include "DataFormats/Scalers/interface/L1AcceptBunchCrossing.h"
#include "DataFormats/Scalers/interface/L1TriggerRates.h"
#include "DataFormats/Scalers/interface/L1TriggerScalers.h"
// -------------------------------------------------------------------------

typedef Buffer<L1AcceptBunchCrossing, false>
L1AcceptBunchCrossing_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1AcceptBunchCrossing_t,
                  "L1AcceptBunchCrossing");
				  
typedef Buffer<L1CaloEmCand, false>
L1CaloEmCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1CaloEmCand_t,
                  "L1CaloEmCand");
				  
typedef Buffer<L1CaloRegion, false>
L1CaloRegion_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1CaloRegion_t,
                  "L1CaloRegion");
				  
typedef Buffer<L1GctEmCand, false>
L1GctEmCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEmCand_t,
                  "L1GctEmCand");
				  
typedef Buffer<L1GctEtHad, false>
L1GctEtHad_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtHad_t,
                  "L1GctEtHad");
				  
typedef Buffer<L1GctEtMiss, false>
L1GctEtMiss_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtMiss_t,
                  "L1GctEtMiss");
				  
typedef Buffer<L1GctEtTotal, false>
L1GctEtTotal_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctEtTotal_t,
                  "L1GctEtTotal");
				  
typedef Buffer<L1GctFibreWord, false>
L1GctFibreWord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctFibreWord_t,
                  "L1GctFibreWord");
				  
typedef Buffer<L1GctHFBitCounts, false>
L1GctHFBitCounts_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHFBitCounts_t,
                  "L1GctHFBitCounts");
				  
typedef Buffer<L1GctHFRingEtSums, false>
L1GctHFRingEtSums_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHFRingEtSums_t,
                  "L1GctHFRingEtSums");
				  
typedef Buffer<L1GctHtMiss, false>
L1GctHtMiss_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctHtMiss_t,
                  "L1GctHtMiss");
				  
typedef Buffer<L1GctInternEmCand, false>
L1GctInternEmCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctInternEmCand_t,
                  "L1GctInternEmCand");
				  
typedef Buffer<L1GctInternEtSum, false>
L1GctInternEtSum_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctInternEtSum_t,
                  "L1GctInternEtSum");
				  
typedef Buffer<L1GctInternHFData, false>
L1GctInternHFData_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctInternHFData_t,
                  "L1GctInternHFData");
				  
typedef Buffer<L1GctInternHtMiss, false>
L1GctInternHtMiss_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctInternHtMiss_t,
                  "L1GctInternHtMiss");
				  
typedef Buffer<L1GctInternJetData, false>
L1GctInternJetData_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctInternJetData_t,
                  "L1GctInternJetData");
				  
typedef Buffer<L1GctJetCand, false>
L1GctJetCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctJetCand_t,
                  "L1GctJetCand");
				  
typedef Buffer<L1GctJetCounts, false>
L1GctJetCounts_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GctJetCounts_t,
                  "L1GctJetCounts");
				  
typedef Buffer<L1GlobalTriggerObjectMap, false>
L1GlobalTriggerObjectMap_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerObjectMap_t,
                  "L1GlobalTriggerObjectMap");
				  
typedef Buffer<L1GtFdlWord, false>
L1GtFdlWord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GtFdlWord_t,
                  "L1GtFdlWord");
				  
typedef Buffer<L1GtPsbWord, false>
L1GtPsbWord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GtPsbWord_t,
                  "L1GtPsbWord");
				  
typedef Buffer<L1GtTechnicalTrigger, false>
L1GtTechnicalTrigger_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GtTechnicalTrigger_t,
                  "L1GtTechnicalTrigger");
				  
typedef Buffer<L1MuGMTCand, false>
L1MuGMTCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1MuGMTCand_t,
                  "L1MuGMTCand");
				  
typedef Buffer<L1MuRegionalCand, false>
L1MuRegionalCand_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1MuRegionalCand_t,
                  "L1MuRegionalCand");
				  
typedef Buffer<L1TriggerError, false>
L1TriggerError_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1TriggerError_t,
                  "L1TriggerError");
				  
typedef Buffer<L1TriggerRates, false>
L1TriggerRates_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1TriggerRates_t,
                  "L1TriggerRates");
				  
typedef Buffer<L1TriggerScalers, false>
L1TriggerScalers_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1TriggerScalers_t,
                  "L1TriggerScalers");
				  
typedef Buffer<L2MuonTrajectorySeed, false>
L2MuonTrajectorySeed_t;
DEFINE_EDM_PLUGIN(BufferFactory, L2MuonTrajectorySeed_t,
                  "L2MuonTrajectorySeed");
				  