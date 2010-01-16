///////////////////////////////////////////////////////////////////////
// File:    Slot.cpp
// Purpose: Models a slot to which a signal can be connected.
// Created: Summer-2002 Harrison B. Prosper
// Updated: 05-Jun-2008 HBP Adapt to CMS
///////////////////////////////////////////////////////////////////////
//$Revision: 1.2 $

#include <Python.h>
#include <iostream>
#include <iomanip>
#include "PhysicsTools/LiteAnalysis/interface/Slot.hpp"

using namespace std;

// Make this class known to Root

#ifdef __WITH_CINT__
ClassImp(Slot)
#endif

// RootCint requires a default constructor

Slot::Slot() {}

Slot::Slot(UInt_t object, const char *method)  	   
  : _object(object),
    _method(method)
{}

Slot::~Slot() 
{}

// SLOTS
////////

void Slot::handleSignal(Int_t id)
{
  PyObject *result = PyObject_CallMethod((PyObject*)_object,
					 (char*)(_method.c_str()),
					 "(i)", id);
  if ( PyErr_Occurred() ) PyErr_Clear();

  // Decrement reference count. Use XDECREF to ignore NULL references
  
  Py_XDECREF(result);
}

void Slot::handleSignal()
{
  PyObject *result = PyObject_CallMethod((PyObject*)_object,
					 (char*)(_method.c_str()), NULL);
  if ( PyErr_Occurred() ) PyErr_Clear();

  // Decrement reference count. Use XDECREF to ignore NULL references
  
  Py_XDECREF(result);
}


