#ifndef METHODT_H
#define METHODT_H
//
// Package:    PhysicsTools/LiteAnalysis
//             MethodT.h
//
// Original Author:  Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
// $Id: Method.h,v 1.4 2010/03/10 13:29:58 prosper Exp $
//
// If using Python, include its header first to avoid annoying compiler
// complaints.
#include <Python.h>
#include <boost/python/type_id.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <stdlib.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
#include "CommonTools/Utils/src/ExpressionPtr.h"
#include "CommonTools/Utils/src/ExpressionBase.h"
#include "CommonTools/Utils/interface/expressionParser.h"
#include "TROOT.h"

// This class models functions of the form
//
//  <simple-return-type> function(<simple-type>)
//
// where <simple-return-type> is a simple type like float and <simple-type> 
// is typically void. The function could be compound, for example:
// y = o->track()->pt()

template <typename T>
struct MethodT 
{
  MethodT() {}
  
  MethodT(const std::string expression)
    : expression_(expression),
      expression1_(""),
      expression2_(""),
      classname_(boost::python::type_id<T>().name()),
      type_(ROOT::Reflex::Type::ByTypeInfo(typeid(T))),
      compoundMethod_(false),
      checkReturn_(false)
  {
    if ( getenv("DEBUGMETHOD") > 0 )
      debug_ = atoi(getenv("DEBUGMETHOD"));
    else
      debug_ = 0;

    // method could be of the form
    // y = method1(...)->method2(...) or
    // y = method1(...).method2(...) or
    // y = method1(..).variable
    boost::regex expr("(?<=[)]) *([-][>]|[.]) *(?=[a-zA-Z])");
    boost::smatch what;

    if ( debug_ > 0 )
      std::cout << "Expression( " 
                << BLUE << expression_ <<  BLACK << " )" << std::endl;

    compoundMethod_ = boost::regex_search(expression_, what, expr);
    
    // If method is compound, use CINT to evaluate it
    
    if ( compoundMethod_ )
      {
        if ( debug_ > 0 )
          std::cout << "Method - " 
                    << RED << "compound method" 
                    << BLACK 
                    << std::endl;
        
        std::string delim(what[0]);

        if ( debug_ > 0 )
          std::cout << "         delimeter: (" << delim << ")" << std::endl;

        kit::bisplit(expression_, expression1_, expression2_, delim);

        // Get return type of 1st part of compound method
        // and determine if it is a pointer 

        std::vector<kit::ValueThing*> values;
        std::vector<void*> args;
        Reflex::Member method = kit::decodeMethod(classname_,
                                                  expression1_,
                                                  values,
                                                  args);
        if ( !kit::memberValid(method) )
          throw edm::Exception(edm::errors::Configuration,
                               "I'm too stupid to decode \"" 
                               + RED + expression1_ + BLACK + "\"\n");

        // Command to set address of object
        char cmd[1024];

        sprintf(cmd, "gROOT->Reset(); %s* o = (%s*)0x%s", 
                classname_.c_str(), classname_.c_str(), "%x");
        setAddress_ = std::string(cmd);

        // Check return type 
        if ( kit::returnsPointer(method) )
          {
            // Returns a simple pointer
            checkReturn_ = true;
            sprintf(cmd, "o->%s", expression1_.c_str());
          }
        else
          {
            Reflex::Member isNull = kit::getisNull(method); 
            if ( kit::memberValid(isNull) )
              {
                checkReturn_ = true;
                sprintf(cmd, "o->%s.get()", expression1_.c_str());
              }
            else
              {
                sprintf(cmd, "%s", "");
              }
          }
        callMethod1_ = std::string(cmd);

        sprintf(cmd, "kit::set(o->%s, x)", expression_.c_str());
        compoundCall_ = std::string(cmd);
      }
    else
      {
        if ( debug_ > 0 )
          std::cout << "Method - " 
                    << RED << "simple method" 
                    << BLACK 
                    << std::endl;

        // This is a simple method, so use
        // expression parser to evaluate it

        expression1_ = expression_;
        if(!reco::parser::expressionParser<T>(expression1_, expr1_)) 
          {
            throw edm::Exception(edm::errors::Configuration,
                                 "Since I'm a cyber ignoramous, "
                                 "I'm too stupid to understand \"" 
                                 + RED + expression1_ + BLACK + "\"\n");
          }
      }

    if ( debug_ > 0 )
      std::cout << "Method - 1: " << expression1_ << std::endl
                << "         2: " << expression2_ << std::endl;
    
  }
  
  std::string name() const
  {
    return std::string(boost::python::type_id<T>().name()) 
      + "/" + expression_;
  }

  double operator()(const T& t) const 
  {
    using namespace ROOT::Reflex;

    if ( !compoundMethod_ )
      {
        if ( debug_ > 0 )
          //DB
          std::cout << "simpleMethod(" << classname_ << "): " 
                    << GREEN 
                    << expression_
                    << BLACK
                    << std::endl;

        Object o(type_, const_cast<T*>(&t));
        double x = expr1_->value(o);

        if ( debug_ > 0 )
          //DB
          std::cout << "  return: " 
                    << RED 
                    << x
                    << BLACK
                    << std::endl;
        return x;
      }
    else
      {
        if ( debug_ > 0 )
          std::cout << "compoundMethod(" << classname_ << "): " 
                    << RED 
                    << expression_
                    << BLACK
                    << std::endl
                    << " setAddress: " 
                    << RED 
                    << setAddress_
                    << BLACK
                    << std::endl;

        gROOT->ProcessLine(Form(setAddress_.c_str(), &t)); 
        if ( checkReturn_ )
          {
            if ( debug_ > 0 )
              //DB
              std::cout << "   checkReturn: " 
                        << RED 
                        << callMethod1_
                        << BLACK
                        << std::endl;
            void* y = 0;
            try
              {
                y = (void*)gROOT->ProcessLineFast(callMethod1_.c_str());
              }
            catch (...)
              {}

            if ( y == 0 )
              {
                double x = -99999;
            
                if ( debug_ > 0 )
                  //DB
                  std::cout << "     return: " 
                            << RED 
                            << x
                            << BLACK
                            << std::endl;
                return x;
              }
          }

        // We have a valid pointer, so proceed

        double x = 0;
        gROOT->ProcessLine(Form("double* x = (double*)0x%x", &x));

        if ( debug_ > 0 )
          //DB
          std::cout << "     callCompound: " 
                    << RED 
                    << compoundCall_
                    << BLACK
                    << std::endl;

        try
          {
            gROOT->ProcessLineFast(compoundCall_.c_str());
          }
        catch (...)
          {}

        if ( debug_ > 0 )
          //DB
          std::cout << "       return: " 
                    << RED 
                    << x
                    << BLACK
                    << std::endl;
        return x;
      }
  }
  
private:

  std::string expression_;
  std::string expression1_;
  std::string expression2_;
  std::string classname_;
  ROOT::Reflex::Type type_;
  reco::parser::ExpressionPtr expr1_;

  bool compoundMethod_;
  bool checkReturn_;

  std::string setAddress_;
  std::string callMethod1_;
  std::string compoundCall_;

  int debug_;
};

#endif
