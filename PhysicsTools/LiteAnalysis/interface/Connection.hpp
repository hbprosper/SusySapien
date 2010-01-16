#ifndef CONNECTION_HPP
#define CONNECTION_HPP
/////////////////////////////////////////////////////////////////////////
// File:    Connection.hpp
// Purpose: Model a connection between a Signal and a Slot.
// Created: 25-Aug-2002 Harrison B. Prosper
// Updated: 05-Jun-2008 HBP Adapt to CMS 
/////////////////////////////////////////////////////////////////////////
//$Revision: 1.2 $

#include <Python.h>
#include "PhysicsTools/LiteAnalysis/interface/Slot.hpp"
#include <string>
#include <TQObject.h>

/**
 */
class Connection : public TQObject
{
public:
  /**
   */
  Connection(TQObject *sender,   const char *signal, 
	     PyObject *receiver, const char *method);

  /**
   */
  ~Connection();

  bool noArgs(std::string &signal);

  TQObject   *sender()   const {return _sender;}
  const char *signal()   const {return _signal.c_str();}

  PyObject   *receiver() const {return (PyObject *)_slot->receiver();}
  const char *method()   const {return _slot->method();}


private:
  TQObject    *_sender;
  std::string  _signal;
  Slot        *_slot;
};

#endif
