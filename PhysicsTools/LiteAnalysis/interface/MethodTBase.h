#ifndef METHODTBASE_H
#define METHODTBASE_H
//
// Package:    PhysicsTools/LiteAnalysis
//             MethodTBase.h
//
// Original Author:  Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
// $Id: MethodTBase.h,v 1.1 2010/09/18 21:01:08 prosper Exp $
//
//-----------------------------------------------------------------------------
#include <string>
#include "CommonTools/Utils/src/ExpressionPtr.h"
#include "Reflex/Type.h"
#include "Reflex/Object.h"
//-----------------------------------------------------------------------------
///
bool isCompoundMethod(std::string expression, std::string& delim);

/** Model function members, that is, methods.
    This is the base class of the template class MethodT. The functions that 
    can be modeled must return simple types and have either no arguments or
    arguments comprising simple types. The functions can be of the form:
    - y = method1(...)->method2(...) or
    - y = method1(...).method2(...) or
    - y = method1(..).variable
*/
class MethodTBase 
{
public:
  ///
  MethodTBase();
  
  ///
  MethodTBase(std::string classname, 
              std::string expression,
              reco::parser::ExpressionPtr exprPtr);

  ~MethodTBase();

  ///
  std::string name() const;

  ///
  double invoke(ROOT::Reflex::Object& o, void* address);
  
private:
  std::string classname_;
  std::string expression_;
  ROOT::Reflex::Type type_;
  reco::parser::ExpressionPtr expr1_;

  std::string expression1_;
  std::string expression2_;

  bool compoundMethod_;
  bool checkReturn_;
  bool checkisNull_;

  std::string setAddress_;
  std::string callMethod1_;
  std::string compoundCall_;

  int debug_;
};

#endif
