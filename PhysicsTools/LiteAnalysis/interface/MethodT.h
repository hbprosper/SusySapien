#ifndef METHODT_H
#define METHODT_H
//-----------------------------------------------------------------------------
//
// Package:    PhysicsTools/LiteAnalysis
//             MethodT.h
//
// Original Author:  Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
// $Id: MethodT.h,v 1.2 2010/09/06 05:28:48 prosper Exp $
//
//-----------------------------------------------------------------------------
// If using Python, include its header first to avoid annoying compiler
// complaints.
#include <Python.h>
#include <boost/python/type_id.hpp>
#include "CommonTools/Utils/src/ExpressionPtr.h"
#include "CommonTools/Utils/src/ExpressionBase.h"
#include "CommonTools/Utils/interface/expressionParser.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "PhysicsTools/LiteAnalysis/interface/MethodTBase.h"
#include "Reflex/Object.h"
#include "Reflex/Type.h"
//-----------------------------------------------------------------------------
template <typename T>
reco::parser::ExpressionPtr parserPtr(std::string expression)
{
  std::string delim("");
  reco::parser::ExpressionPtr ptr;
  if ( !isCompoundMethod(expression, delim) )
    if( !reco::parser::expressionParser<T>(expression, ptr) ) 
      {
        throw cms::Exception("ExpressionError",
                             "Since I'm a cyber ignoramous, "
                             "I'm too stupid to understand \"" 
                             + expression "\"\n");
      } 
  return ptr;
}

/** Model function members, that is, methods.
    The functions that 
    can be modeled must return simple types and have either no arguments or
    arguments comprising simple types. The functions can be of the form:
    - y = method1(...)->method2(...) or
    - y = method1(...).method2(...) or
    - y = method1(..).variable
*/
template <typename T>
struct MethodT : public MethodTBase 
{
  MethodT() {}
  
  ///
  MethodT(const std::string expression)
    : MethodTBase(boost::python::type_id<T>().name(),
                  expression,
                  parserPtr<T>(expression)),
      type_(ROOT::Reflex::Type::ByTypeInfo(typeid(T)))
  {}

  ///
  double operator()(const T& t)
  {
    void* address = static_void(void*)(&t);
    ROOT::Reflex::Object object(type_, const_cast<T*>(address));
    return (*this)(object, address);
  }
};

#endif
