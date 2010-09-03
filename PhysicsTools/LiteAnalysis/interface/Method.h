#ifndef METHOD_H
#define METHOD_H
//
// Package:    PhysicsTools/LiteAnalysis
//             Method.h
//
// Original Author:  Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
// $Id: Method.h,v 1.4 2010/03/10 13:29:58 prosper Exp $
//
// If using Python, include its header first to avoid annoying compiler
// complaints.
//-----------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <string>
#include "Reflex/Member.h"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
//-----------------------------------------------------------------------------
/// Model function methods.
class Method
{
public:
  ///
  Method();
    
  ///
  Method(std::string classname, std::string methodrecord);

  ~Method();

  ///
  void*  invoke(void* address);

  ///
  double operator()(void* address);

  ///
  Reflex::Member& method(int code)
  { return code == 1 ? method1_ : method2_; }

  ///
  std::vector<kit::ValueThing*>& arguments(int code)
  { return code == 1 ? values1_ : values2_; }

  ///
  std::string  str();

  ///
  std::ostream& operator<<(std::ostream& os);

private:
  std::string classname1_;
  std::string methodrecord_;
  int debug_;

  std::string methodrecord1_;
  Reflex::Member method1_;
  std::vector<kit::ValueThing*> values1_;
  std::vector<void*> args1_;

  std::string classname2_;
  std::string methodrecord2_;
  Reflex::Member method2_;
  std::vector<kit::ValueThing*> values2_;
  std::vector<void*> args2_;

  bool simplemethod_;
  bool smartpointer_;
  bool datamember1_;
  bool datamember2_;

  Reflex::Member isnullmethod_;
  Reflex::Member getmethod_;
};

#endif
