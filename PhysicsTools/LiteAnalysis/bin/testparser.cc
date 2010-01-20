//-----------------------------------------------------------------------------
// File: testparser.cc
// Created: 03-Nov-2009 Harrison B. Prosper
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "CommonTools/Utils/src/ExpressionPtr.h"
#include "CommonTools/Utils/src/ExpressionBase.h"
#include "CommonTools/Utils/interface/expressionParser.h"
#include "PhysicsTools/LiteAnalysis/interface/ATest.h"
//-----------------------------------------------------------------------------
using namespace std;

void fatal(string message)
{
  cout << "** error ** " << message << endl;
  exit(0);
}

using namespace std;

namespace {

  // Model a function that can be instantiated.
  // This class is basically a copy of StringObjectFunction by Luca Lista. 
  // It models functions of the form
  //
  //  <type> function()
  //
  // where <type> is a simple type like float.

  template <typename T>
  struct SimpleFunction 
  {
    SimpleFunction() {}
    
    SimpleFunction(const std::string& expr) 
      : type_(ROOT::Reflex::Type::ByTypeInfo(typeid(T))) 
    {
      if(!reco::parser::expressionParser<T>(expr, expr_)) 
        {
          throw edm::Exception(edm::errors::Configuration,
                               "Sorry, I'm too stupid to understand \"" 
                               + expr + "\"");
        }
    }

    double operator()(const T & t) const 
    {
      using namespace ROOT::Reflex;
      Object o(type_, const_cast<T *>(& t));
      return expr_->value(o);
    }
  
  private:
    reco::parser::ExpressionPtr expr_;
    ROOT::Reflex::Type type_;
  };
}

//-----------------------------------------------------------------------------
int 
main(int argc, char** argv)
{
  cout << "testparser" << endl;

  AutoLibraryLoader::enable();

  const ATest t1("Hello World 1");
  cout << "ATest direct" << endl;
  t1.method1();
  string method1("method1()");
  SimpleFunction<ATest> f1(method1);
  cout << "      indirect" << endl;
  double y1 = f1(t1);

  const ATest t2("The time has come the walrus said");
  cout << "ATest direct" << endl;
  t2.method2("Hello World");
  string method2("method2(\"Hello World\")");
  SimpleFunction<ATest> f2(method2);
  cout << "      indirect" << endl;
  double y2 = f2(t2);


  int y = (int)((y1+y2)/1000);
  return y;
}

