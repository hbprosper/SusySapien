//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: Mkntuple
// Description: Mkntuple helper class for L1GlobalTriggerReadoutRecord
// Created:     Fri Oct  8 18:15:16 2010
// Author:      Harrison B. Prosper      
//$Revision: 1.10 $
//-----------------------------------------------------------------------------
#include "PhysicsTools/Mkntuple/interface/L1GlobalTriggerReadoutRecordHelper.h"
#include "PhysicsTools/Mkntuple/interface/CurrentEvent.h"
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
// This constructor is called once per job
L1GlobalTriggerReadoutRecordHelper::L1GlobalTriggerReadoutRecordHelper()
  : HelperFor<L1GlobalTriggerReadoutRecord>() {}
    
L1GlobalTriggerReadoutRecordHelper::~L1GlobalTriggerReadoutRecordHelper() {}

// -- Called once per event
void L1GlobalTriggerReadoutRecordHelper::analyzeEvent()
{
  // get pointer to event setup
  const edm::EventSetup* eventsetup = CurrentEvent::instance().getsetup();

  if ( eventsetup == 0 )
    throw cms::Exception("NullEventSetupPointer");

  // A pointer to the event is available by default
  if ( event == 0 )
    throw cms::Exception("NullEventPointer");

  l1gtutils.retrieveL1EventSetup(*eventsetup);
}

// -- Access Methods
bool 
L1GlobalTriggerReadoutRecordHelper::value(std::string name, int code) const
{
  const string triggername(name);
  int err=0;
  if ( code < 0 )
    return (bool)l1gtutils.decisionBeforeMask(*event, triggername, err);
  else
    return (bool)l1gtutils.decisionBeforeMask(*event, triggername, err);
}

int
L1GlobalTriggerReadoutRecordHelper::prescale(std::string name) const
{
  const string triggername(name);
  int err=0;
  return (int)l1gtutils.prescaleFactor(*event, triggername, err);
}
