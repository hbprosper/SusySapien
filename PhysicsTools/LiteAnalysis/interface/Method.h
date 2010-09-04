#ifndef METHOD_H
#define METHOD_H
//-----------------------------------------------------------------------------
//
// Package:    PhysicsTools/LiteAnalysis
//             Method.h
//
// Original Author:  Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
// $Id: Method.h,v 1.5 2010/09/03 01:54:12 prosper Exp $
//
// If using Python, include its header first to avoid annoying compiler
// complaints.
//-----------------------------------------------------------------------------
#include <Python.h>
#include <boost/python/type_id.hpp>
#include <string>
#include "PhysicsTools/LiteAnalysis/interface/FunctionMember.h"
//-----------------------------------------------------------------------------
/// Model function methods.
template <typename T>
class Method : public FunctionMember
{
public:
  ///
  Method() {}
    
  ///
  Method(std::string methodexpression)
    : FunctionMember(boost::python::type_id<T>().name(), methodexpresion),
      methodexpr_(methodexpression)

  virtual ~Method();

  std::string name() const
  {
    return std::string(boost::python::type_id<T>().name()) 
      + std::string("::") + methodexpression_;
  }

  double operator()(const T& o) const 
  {
    return (*this)((void*)(&o));
  }
private:
  std::string methodexpression_;
};

#endif
