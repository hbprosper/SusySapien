// -------------------------------------------------------------------------
// File::   plugin_L1GlobalTriggerReadoutRecord.cc
// Created: Tue May  3 03:49:10 2011 by mkplugins.py
// $Revision: 1.15 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

typedef Buffer<L1GlobalTriggerReadoutRecord, true> L1GlobalTriggerReadoutRecord_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerReadoutRecord_t,
                  "L1GlobalTriggerReadoutRecord");
