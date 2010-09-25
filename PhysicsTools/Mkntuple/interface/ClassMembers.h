#ifndef CLASSMEMBERS_H
#define CLASSMEMBERS_H
// -*- C++ -*-
//
// Package:    PhysicsTools
// Class:      kit
// 
/**\class ClassMembers 
   PhysicsTools/Mkntuple/src/ClassMembers.cc

 Description: Use ROOT Reflex to return function and data members of a class
 
 Implementation:
     As simple as possible
*/
//
// Original Author:  Harrison B. Prosper
//         Created:  December 2009
// $Id: ClassMembers.h,v 1.1 2010/09/03 04:56:58 prosper Exp $
//
//$Revision: 1.1 $
//-----------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <string>
//-----------------------------------------------------------------------------
#include "Reflex/Object.h"
#include "Reflex/Base.h"
#include "Reflex/Type.h"
//-----------------------------------------------------------------------------
///
class ClassMembers
{
public:
  ///
  ClassMembers();
    
  ///
  ClassMembers(std::string classname, int debug=0);


  ~ClassMembers();

  ///
  std::vector<std::string>& baseclasses() { return baseclasses_; }

  ///
  std::vector<std::string>& getters() { return getters_; }

  ///
  std::vector<std::string>& datamembers() { return datamembers_; }

  ///
  std::vector<std::string>& setters() { return setters_; }

private:
  std::string classname_;
  int debug_;
  std::vector<std::string> baseclasses_;
  std::vector<std::string> getters_;
  std::vector<std::string> setters_;
  std::vector<std::string> datamembers_;
  std::map<std::string, int> signatures_;

  void  get_getters_(std::string classname, int depth=0);
  void  get_datamembers_(std::string classname, int depth=0);
};


#endif
