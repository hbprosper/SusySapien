// -------------------------------------------------------------------------
// File::   plugin_edmErrorSummaryEntry.cc
// Created: Tue May  3 03:49:10 2011 by mkplugins.py
// $Revision: 1.2 $
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
#include "FWCore/MessageLogger/interface/ErrorSummaryEntry.h"

typedef Buffer<edm::ErrorSummaryEntry, false> edmErrorSummaryEntry_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmErrorSummaryEntry_t,
                  "edmErrorSummaryEntry");
