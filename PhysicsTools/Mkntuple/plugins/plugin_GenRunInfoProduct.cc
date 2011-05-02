// -------------------------------------------------------------------------
// File::   plugin_GenRunInfoProduct.cc
// Created: Sun May  1 05:53:33 2011 by mkplugins.py
// $Revision: 1.14 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

typedef Buffer<GenRunInfoProduct, true> GenRunInfoProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenRunInfoProduct_t,
                  "GenRunInfoProduct");
