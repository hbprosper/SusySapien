// ----------------------------------------------------------------------------
// Created: Fri Oct  8 18:15:16 2010 by mkuserplugin.py
// Author:      Harrison B. Prosper      
//$Revision: 1.10 $
// ----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/UserBuffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "PhysicsTools/Mkntuple/interface/L1GlobalTriggerReadoutRecordHelper.h"
typedef UserBuffer<L1GlobalTriggerReadoutRecord, L1GlobalTriggerReadoutRecordHelper, true>
L1GlobalTriggerReadoutRecordHelper_t;
DEFINE_EDM_PLUGIN(BufferFactory, L1GlobalTriggerReadoutRecordHelper_t,
                  "L1GlobalTriggerReadoutRecordHelper");
