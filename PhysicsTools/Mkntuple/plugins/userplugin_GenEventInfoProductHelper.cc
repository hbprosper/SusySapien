// ----------------------------------------------------------------------------
// Created: Wed Feb 16 01:43:26 2011 by mkuserplugin.py
// Author:      Harrison B. Prosper      
//$Revision: 1.11 $
// ----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/UserBuffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "PhysicsTools/Mkntuple/interface/GenEventInfoProductHelper.h"
typedef UserBuffer<GenEventInfoProduct, GenEventInfoProductHelper, true>
GenEventInfoProductHelper_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenEventInfoProductHelper_t,
                  "GenEventInfoProductHelper");
