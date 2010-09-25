#ifndef RFX_H
#define RFX_H
// -*- C++ -*-
//
// Package:    PhysicsTools
// Class:      rfx
// 
/**\class rfx rfx.cc 
   PhysicsTools/Mkntuple/src/rfx.cc

 Description: A class of Reflex utilities. These functions are placed in a 
 class so that Reflex can handle overloading automatically.
 
 Implementation:
     As simple as possible
*/
//
// Original Author:  Harrison B. Prosper
//         Created:  Fri Apr 04 2008
// $Id: kit.h,v 1.12 2010/09/23 09:10:23 prosper Exp $
//
//$Revision: 1.12 $
//-----------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <stdio.h>
//-----------------------------------------------------------------------------
#include "Reflex/Object.h"
#include "Reflex/Base.h"
#include "Reflex/Type.h"
//-----------------------------------------------------------------------------
namespace {
  std::vector<void*> DEFARGS;
}

struct rfx
{

  ///
  struct ValueThing
  {
    virtual ~ValueThing() {}
    virtual std::string str()=0;
    virtual void*       address()=0;
  };

  ///
  template <typename X>
  struct Value : public ValueThing
  {
    Value(X& x) : x_(x) {}
    ~Value() {}

    std::string str() 
    {
      std::ostringstream os;
      os << x_;
      return os.str();
    }
    void* address() { return static_cast<void*>( &x_ ); }

    X x_;
  };

  ///
  static
  bool           isCompoundMethod(std::string expression, std::string& delim);

  ///
  static
  void           getScopes(std::string classname, 
                           std::vector<std::string>& names, 
                           int depth=0);
  ///
  static
  Reflex::Member getMethod(std::string classname,
                           std::string methodname,
                           std::string args=std::string("(void)"));

  ///
  static
  Reflex::Member getDataMember(std::string classname,
                               std::string membername);
  ///
  static
  bool           memberValid(Reflex::Member& member);

  ///
  static
  bool           returnsPointer(Reflex::Member& method);

  ///
  static
  int            simpleType(std::string name);

  ///
  static
  Reflex::Member getReturnedObjectMethod(Reflex::Member& method,
                                         std::string name);

  ///
  static
  Reflex::Member getisAvailable(Reflex::Member& method);

  ///
  static
  Reflex::Member getisNull(Reflex::Member& method);

  ///
  static
  void           decodeArguments(std::string  args,
                                 std::string& argsregex,
                                 std::vector<rfx::ValueThing*>& values);

  ///
  static
  Reflex::Member decodeMethod(std::string classname,
                              std::string methodrecord,
                              std::vector<rfx::ValueThing*>& values,
                              std::vector<void*>& args);
  ///
  static
  void*          datamemberValue(std::string& classname, void* address, 
                                 std::string& membername);
  ///
  static
  void*          invokeMethod(Reflex::Member& method, 
                              void* address, 
                              void* memory,
                              std::vector<void*>& args=DEFARGS);
  ///
  static
  void           deallocateMemory(Reflex::Member& member, void* address);

  ///
  static
  std::string    returnTypeName(Reflex::Member& method);

  ///
  static
  Reflex::Type   returnType(Reflex::Member& method, int& code);

  ///
  static
  std::string    strip(std::string line);

  ///
  static
  std::string    replace(std::string& str, 
                         std::string oldstr, std::string newstr);
  ///
  static
  void           bisplit(std::string  s, 
                         std::string& left, 
                         std::string& right, 
                         std::string  delim,
                         int direction=1);
  ///
  static
  std::vector<std::string> regex_findall(std::string& str, std::string expr);
  
  ///
  static
  std::string              regex_search(std::string& str, std::string expr); 

  ///
  static
  std::string              regex_sub(std::string& str, 
                                     std::string expr, 
                                     std::string rstr); 
};


#endif
