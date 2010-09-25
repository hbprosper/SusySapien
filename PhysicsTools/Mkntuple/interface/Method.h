#ifndef METHOD_H
#define METHOD_H
//-----------------------------------------------------------------------------
//
// Package:    PhysicsTools/Mkntuple
//             Method.h
//
// Original Author:  Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
// $Id: Method.h,v 1.8 2010/09/15 13:46:46 prosper Exp $
//
// If using Python, include its header first to avoid annoying compiler
// complaints.
//-----------------------------------------------------------------------------
#include <Python.h>
#include <boost/python/type_id.hpp>
#include <string>
#include <iostream>
#include "PhysicsTools/Mkntuple/interface/FunctionMember.h"
//-----------------------------------------------------------------------------
/// Model function methods.

template <typename T>
class Method : public FunctionMember
{
public:
  ///
  Method() {}
  
  ///
  Method(std::string expression)
    : FunctionMember(boost::python::type_id<T>().name(), expression),
      name_(boost::python::type_id<T>().name()+std::string("::")+expression)
  {}
  
  virtual ~Method() {}

  ///
  std::string name() const { return name_; }
  
  ///
  double operator()(const T& object)
  {
    void* address = (void*)(&object);
    void* raddr = invoke(address);
    if ( raddr == 0 ) 
      return 0;
    else
      return *static_cast<double*>(raddr); 
    }
  
  ///
  double operator()(T& object) { return (*this)((const T)(object)); }
  
private:
  std::string name_;
};

#endif
