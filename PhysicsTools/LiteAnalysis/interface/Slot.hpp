#ifndef SLOT_HPP
#define SLOT_HPP
/////////////////////////////////////////////////////////////////////////
// File:    Slot.hpp
// Purpose: Model a Slot for use in Signal/Slot communication.
// Created: Summer-2002 Harrison B. Prosper
// Updated: 05-Jun-2008 HBP Adapt to CMS
/////////////////////////////////////////////////////////////////////////
//$Revision: 1.2 $

#include <string>
#include <TQObject.h>

/**
 */
class Slot : public TQObject
{
private:

  UInt_t      _object;
  std::string _method;

public:

  /** RootCint requires a default constructor      
   */

  Slot();

  /**
   */
  Slot(UInt_t object, const char *method);

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

  UInt_t      receiver() const {return _object;}
  const char* method()   const {return _method.c_str();}

 // Make this class known to Root

#ifdef __WITH_CINT__
  ClassDef(Slot,0)
#endif
};

#endif
