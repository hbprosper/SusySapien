// -------------------------------------------------------------------------
// File::   plugin_GenRunInfoProduct.cc
// Created: Tue May  3 03:49:10 2011 by mkplugins.py
// $Revision: 1.15 $
// -------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/Buffer.h"
#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

typedef Buffer<GenRunInfoProduct, true> GenRunInfoProduct_t;
DEFINE_EDM_PLUGIN(BufferFactory, GenRunInfoProduct_t,
                  "GenRunInfoProduct");
