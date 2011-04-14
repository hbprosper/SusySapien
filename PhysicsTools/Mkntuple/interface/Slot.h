#ifndef SLOT_H
#define SLOT_H
/////////////////////////////////////////////////////////////////////////
// File:    Slot.h
// Purpose: Model a Slot for use in Signal/Slot communication.
// Created: Summer-2002 Harrison B. Prosper
// Updated: 05-Jun-2008 HBP Adapt to CMS
//          14-Apr-2011 HBP changed unsigned long
/////////////////////////////////////////////////////////////////////////
//$Revision: 1.1 $

#include <string>
#include <vector>
#include <TQObject.h>

/**
 */
class Slot : public TQObject
{
private:

  UInt_t      _object;
  std::string _mstr;
  std::vector<char> _method;

public:

  /** RootCint requires a default constructor      
   */

  Slot();

  /**
   */
  Slot(unsigned long object, const char *method);

  /**
   */
  ~Slot();

  // SLOTS
  ////////

  /**
  */
  void handleSignal(Int_t id);

  /**
   */
  void handleSignal();

  unsigned long  receiver() const {return _object;}
  const char*    method()   const {return _mstr.c_str();}

 // Make this class known to Root

#ifdef __WITH_CINT__
  ClassDef(Slot,0)
#endif
};

#endif
