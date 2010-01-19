#include "PhysicsTools/Mkntuple/interface/pluginfactory.h"
//-----------------------------------------------------------------------------
BufferThing::~BufferThing() {}
MkntuplePlugin::~MkntuplePlugin() {}
EDM_REGISTER_PLUGINFACTORY(BufferFactory, "BufferFactory");
EDM_REGISTER_PLUGINFACTORY(MkntuplePluginFactory, "MkntuplePluginFactory");
