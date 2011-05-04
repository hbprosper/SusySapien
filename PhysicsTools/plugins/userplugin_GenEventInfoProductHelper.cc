// ----------------------------------------------------------------------------
// Created: Wed Feb 16 01:43:26 2011 by mkuserplugin.py
// Author:      Harrison B. Prosper      
//$Revision: 1.1 $
// ----------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/UserBuffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
#include "PhysicsTools/TheNtupleMaker/interface/GenEventInfoProductHelper.h"
typedef UserBuffer<GenEventInfoProduct, GenEventInfoProductHelper, true>
GenEventInfoProductHelper_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenEventInfoProductHelper_t,
                  "GenEventInfoProductHelper");
