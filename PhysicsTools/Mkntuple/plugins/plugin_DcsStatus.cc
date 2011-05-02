// -------------------------------------------------------------------------
// File::   plugin_DcsStatus.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"

typedef Buffer<DcsStatus, false> DcsStatus_t;
DEFINE_EDM_PLUGIN(BufferFactory, DcsStatus_t,
                  "DcsStatus");
