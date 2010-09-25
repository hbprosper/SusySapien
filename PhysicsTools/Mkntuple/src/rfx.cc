// -*- C++ -*-
//
// Package:    PhysicsTools
// Class:      rfx
// 
/**\class rfx rfx.cc 
   PhysicsTools/Mkntuple/src/rfx.cc

   Description: Some Reflex utilities

   Implementation:
   Common sense and a sense of beauty.
*/
//
// Original Author:  Harrison B. Prosper
//         Created:  Wed Jun 20 19:53:47 EDT 2007
//         Updated:  Sat Oct 25 2008 - make matchInDeltaR saner
// $Id: kit.cc,v 1.14 2010/09/23 09:10:24 prosper Exp $
//
//
//-----------------------------------------------------------------------------
#include <Python.h>
#include <boost/python.hpp>
#include <boost/regex.hpp>

#include <sstream>
#include <cassert>
#include <string>
#include <cmath>
#include <iomanip>
#include <string>
#include <algorithm>
#include <map>
//-----------------------------------------------------------------------------
#include "TROOT.h"
#include "TSystem.h"
#include "TString.h"
#include "TList.h"
#include "TClass.h"
#include "TIterator.h"
#include "TFile.h"
#include "TInterpreter.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "PhysicsTools/Mkntuple/interface/rfx.h"
#include "PhysicsTools/Mkntuple/interface/colors.h"
//-----------------------------------------------------------------------------
using namespace std;
using namespace ROOT::Reflex;
//----------------------------------------------------------------------------
static bool DBinvokeMethod = 
  getenv("DBinvokeMethod")>0 ? (bool)getenv("DBinvokeMethod") : false; 
  
string 
rfx::strip(string line)
{
  int l = line.size();
  if ( l == 0 ) return string("");
  int n = 0;
  while (((line[n] == 0)    ||
          (line[n] == ' ' ) ||
          (line[n] == '\n') ||
          (line[n] == '\t')) && n < l) n++;
  
  int m = l-1;
  while (((line[m] == 0)    ||
          (line[m] == ' ')  ||
          (line[m] == '\n') ||
          (line[m] == '\t')) && m > 0) m--;
  return line.substr(n,m-n+1);
}

string
rfx::replace(string& str, string oldstr, string newstr)
{
  return string(TString(str).ReplaceAll(oldstr, newstr).Data());
}

void
rfx::bisplit(string str, string& left, string& right, string delim,
             int direction)
{
  left  = str;
  right = "";
  int i = 0;
  if ( direction > 0 )
    i = str.rfind(delim);
  else      
    i = str.find(delim);
  if ( i > 0 )
    {
      left  = str.substr(0, i);
      right = str.substr(i+delim.size(), str.size()-i-delim.size());
    }
}

vector<string>
rfx::regex_findall(string& str, string expr)
{
  vector<string> v;
  // Get all matches
  boost::regex regexp(expr);
  int start = 0;
  int end = (int)str.size();
  for (;;)
    {
      boost::smatch matches;
      if ( boost::regex_search(str.substr(start, end), 
                               matches, regexp,
                               boost::match_partial) )
        {
          v.push_back(matches[0]);
          int index=0;
          start 
            += (int)(matches.position(index)) 
            +  (int)(matches[0].length()) + 1; 
          if ( start >= end ) break;
        }
      else
        break;
    }
  return v;
} 

string
rfx::regex_search(string& str, string expr) 
{
  boost::regex regexp(expr);
  boost::smatch match;
  if ( boost::regex_search(str, match, regexp) )
    return match[0];
  else
    return "";
}

string
rfx::regex_sub(string& str, string expr, string rstr) 
{
  boost::regex regexp(expr);
  boost::smatch match;
  if ( boost::regex_search(str, match, regexp) )
    return replace(str, match[0], rstr);
  else
    return str;
}

// ---------------------------------------------------------------------------
// Reflex stuff
// ---------------------------------------------------------------------------

bool 
rfx::isCompoundMethod(std::string expression, std::string& delim)
{
  // method could be of the form
  // y = method1(...)->method2(...) or
  // y = method1(...).method2(...)
  // y = method1(...).datamember
  // y = datamember.method2(...)
  // y = datamember->method2(...)
  boost::regex expr("(?<=[)a-zA-Z_0-9]) *([-][>]|[.]) *(?=[a-zA-Z])");
  boost::smatch what;
  bool yes = boost::regex_search(expression, what, expr);
  if ( yes ) delim = what[0];
  return yes;
}

void 
rfx::getScopes(std::string classname, 
               std::vector<std::string>& names, 
               int depth)
{
  depth++;
  if ( depth > 20 )
    {
      throw cms::Exception("LostInTrees")
        << "\tgetScopes: yikes! I'm lost in the trees" << std::endl;
    }
  names.push_back(classname);

  Type c = Type::ByName(classname);
  for (unsigned i=0; i < c.BaseSize(); i++)
    {
      string basename = c.BaseAt(i).ToType().Name(SCOPED);
      rfx::getScopes(basename, names, depth);
    }
}

Reflex::Member
rfx::getMethod(std::string classname,
               std::string methodname,
               std::string args)
{
  using namespace Reflex;
  vector<string> names;
  rfx::getScopes(classname, names);

  boost::regex expr(args);
  boost::smatch what;

  //DB
  //   cout << "getMethod - \n"
  //        << "  classname<" << classname << ">\n"
  //        << "    method<" << methodname << ">\n" 
  //        << "      args<" << args << ">" 
  //        << endl;

  for(unsigned i=0; i < names.size(); i++)
    {
      //DB
      //cout << i << "\tSCOPE( " << names[i] << " )" << endl;

      Type t = Type::ByName(names[i]); 

      for(unsigned j=0; j < t.FunctionMemberSize(); j++)
        {
          Member m = t.FunctionMemberAt(j);
          if ( !m.IsPublic() ) continue;
          if ( !m.IsFunctionMember() ) continue;
          if ( m.IsConstructor() ) continue;
          if ( m.IsDestructor() ) continue;
 
          // Check method name
          string name = m.Name();

          //DB
          //cout << "\t\t" << name << endl;
          if ( methodname != name ) continue;

          // Now check signature
          string signature = m.TypeOf().Name(SCOPED);
          //cout << "\t\t\t" << signature << endl;
          //cout << "\t\t\t" << args << endl;

          // May need to make this a bit more sophisticated

          if ( boost::regex_search(signature, what, expr) ) 
            {
              //cout << "\t\t\t\t** matched **" << endl;
              // We have a match
              return m;
            }
        }
    }
  return Member();
}

Reflex::Member
rfx::getDataMember(std::string classname,
                   std::string datumname)
{
  using namespace Reflex;
  vector<string> names;
  rfx::getScopes(classname, names);

  for(unsigned i=0; i < names.size(); i++)
    {
      //DB
      //cout << i << "\tSCOPE( " << names[i] << " )" << endl;

      Type t = Type::ByName(names[i]); 

      for(unsigned j=0; j < t.DataMemberSize(); j++)
        {
          Member m = t.DataMemberAt(j);
          if ( !m.IsPublic() ) continue;
 
          // Check member name
          string name = m.Name();

          //DB
          //cout << "\t\t" << name << endl;
          if ( datumname == name )
            {
              return m;
            }
        }
    }
  return Member();
}

bool
rfx::memberValid(Member& m)
{
  return m.DeclaringType().Name() != "";
}

int
rfx::simpleType(string name)
{
  // Codes:
  // 0 non-simple 
  // 1 float
  // 2 integer
  // 3 string
  // 4 void

  vector<boost::regex> regex;

  // Regex for reals
  regex.push_back(boost::regex("double|float"));

  // Regex for integers
  regex.push_back(boost::regex("(unsigned )?(short|int|long|size_t)"));

  // Regex for strings
  regex.push_back(boost::regex("string"));

  // Regex for bools
  regex.push_back(boost::regex("bool"));

  // Regex for voids
  regex.push_back(boost::regex("void"));

  int code=0;
  for(unsigned int i=0; i < regex.size(); ++i)
    {
      boost::smatch what;
      if ( boost::regex_search(name, what, regex[i]) )
        {
          code = i + 1;
          break;
        }
    }
  return code;
}

void
rfx::decodeArguments(std::string  args,
                     std::string& argsregex,
                     std::vector<rfx::ValueThing*>& vars)
{
  //DB
  //cout << "decodeArguments - ARGS(" << args << ")" << endl;

  // Split string into argument fields

  bool isString = false;
  vector<string> arglist;
  string str("");
  for(unsigned i=1; i < args.size(); i++)
    { 
      if ( isString )
        {
          str += args[i];
          if ( args[i] == '"' ) isString = false;
        }
      else if ( args[i] == '"' )
        {
          str += args[i];
          isString = true;
        }
      else if ( args[i] == ',' )
        {
          arglist.push_back(str);
          str = "";
        }
      else if ( args[i] == ')' )
        {
          arglist.push_back(str);
          str = "";
        }
      else
        {
          str += args[i];
        }
    }

  // Create regex for arguments and note the type

  // Regex for strings
  vector<string> atype;
  vector<boost::regex> expr;
  expr.push_back(boost::regex("\".*\"")); 
  atype.push_back("std::string");

  // Regex for reals
  expr.push_back(boost::regex("[-+]?[0-9]*[.][0-9]*([eE][-+]?[0-9]+)?"));
  atype.push_back("(double|float)");

  // Regex for integers
  expr.push_back(boost::regex("[-+]?[0-9]+"));
  atype.push_back("((unsigned )?(short|int|long|size_t))");

  // Regex for bools
  expr.push_back(boost::regex("\b(false|true)\b"));
  atype.push_back("(bool)");

  // Regex for voids
  expr.push_back(boost::regex("\bvoid\b"));
  atype.push_back("(void)");

  boost::smatch what;

  vector<int> vartype(expr.size(),-1);
                      
  argsregex = string("");
  string delim("[(]");
  for(unsigned i=0; i < arglist.size(); i++)
    {
      string str = rfx::strip(arglist[i]);
      //DB
      //cout << "\targ(" << str << ")" << endl;

      if ( str == "" ) // Check for void
        {
          vartype[i] = 4;
          argsregex += delim + atype[4];
        }
      else
        {
          for(unsigned j=0; j < expr.size(); j++)
            {
              //DB
              //cout << "\texpr(" << expr[j] << ")" << endl;
              if ( boost::regex_search(str, what, expr[j]) )
                {
                  vartype[i] = j;
                  argsregex += delim + atype[j];
                  break;
                }
            }
        }
      // Make sure we had a match
      if ( vartype[i] < 0 )
        {
          throw cms::Exception("ArgDecodeFailure")
            << "rfx::decodeArguments - failed on: " << args << endl;
        }
      delim = ", ";

      //DB
      //cout << "\ttype: " << vartype[i] << endl;

      string s;
      double d;
      long l;
      bool b;
      istringstream inp(str);

      switch (vartype[i])
        {
        case 0:
          vars.push_back(new rfx::Value<string>(str));
          break;

        case 1:
          inp >> d;
          vars.push_back(new rfx::Value<double>(d));
          break;

        case 2:
          inp >> l;
          vars.push_back(new rfx::Value<long>(l));
          break;

        case 3:
          inp >> b;
          vars.push_back(new rfx::Value<bool>(b));
          break;

        case 4:
          break;
        };
    }
  if ( argsregex != "" ) argsregex += "[)]";
}


void*
rfx::invokeMethod(Member& method, 
                  void* address,
                  void* memory, 
                  vector<void*>& args)
{
  if ( DBinvokeMethod )
    cout << "\t\tinvokeMethod - " 
         << BLUE 
         << method.Name()
         << DEFAULT_COLOR
         << endl;

  void* raddr=0;
  Type dtype = method.DeclaringType();
  if ( dtype.Name() == "" )
    { 
      cout << "** invokeMethod - declaring type not found! " << endl;
      return raddr;
    }

  if ( DBinvokeMethod )
    cout << "\t\tinvokeMethod - declaring type: " 
         << BLUE 
         << dtype.Name()
         << DEFAULT_COLOR
         << endl;

  if ( address == 0 )
    { 
      cout << "** invokeMethod - object address is zero " << endl;
      return raddr;
    }

  if ( DBinvokeMethod )
    cout << "\t\tinvokeMethod - address: " 
         << RED 
         << address
         << DEFAULT_COLOR 
         << endl;

  // Well...finally, do the deed!
  Type   rtype = method.TypeOf().ReturnType().FinalType();
  Object object(dtype, address);
  Object returnedObject(rtype, memory);
  method.Invoke(object, &returnedObject, args);
  raddr = returnedObject.Address();

  // Check for pointer, references, etc.
  if ( rtype.IsPointer() || rtype.IsReference() )
    {
      raddr = *static_cast<void**>(raddr);
      if ( DBinvokeMethod )
        {
          if ( rtype.IsPointer() )
            cout << "\t\tinvokeMethod - returns " 
                 << BLUE 
                 << " by POINTER "
                 << RED 
                 << "(" << raddr << ")" 
                 << DEFAULT_COLOR
                 << endl;
          else
            cout << "\t\tinvokeMethod - returns " 
                 << BLUE 
                 << " by REFERENCE "
                 << RED 
                 << "(" << raddr << ")" 
                 << DEFAULT_COLOR
                 << endl;
        }
    }
  else if ( rtype.IsClass() )
    {
      // The method returns by value, so memory contains the
      // address of the returned object
      raddr = memory;

      if ( DBinvokeMethod )
            cout << "\t\tinvokeMethod - returns " 
                 << BLUE 
                 << " by VALUE "
                 << RED 
                 << "(" << raddr << ")" 
                 << DEFAULT_COLOR
                 << endl;
    }
  return raddr;
}


void*
rfx::datamemberValue(string& classname, void* address, string& membername)
{
  //DB
  //cout << "\t\tCALL datamemberValue: " 
  //     << classname << "::" << membername << endl;

  void* raddr=0;
  if ( address == 0 )
    { 
      //cout << "** invokeMethod - object address is zero " << endl;
      return raddr;
    }

  Type thing;
  Type ctype = thing.ByName(classname);
  //DB
  //cout << "\t\tdeclaring class: " << ctype.Name() << endl;

  // Model instance of class
  Object object(ctype, address);

  // Model datamember and get value
  Object value = object.Get(membername);
  raddr  = value.Address();

  // If this is a pointer, then return address of object pointed to
  Type atype = value.TypeOf();
  if ( atype.IsPointer() || atype.IsReference() ) 
    raddr = *static_cast<void**>(raddr);

  return raddr;
}


void
rfx::deallocateMemory(Member& method, void* address)
{
  method.TypeOf().ReturnType().FinalType().Deallocate(address);
}


std::string
rfx::returnTypeName(Member& method)
{
  Type rtype = method.TypeOf().ReturnType().FinalType();
  if ( rtype.IsPointer() )
    return rtype.ToType().Name(SCOPED);
  else
    return rtype.Name(SCOPED);
}

Type
rfx::returnType(Member& method, int& code)
{
  Type rtype = method.TypeOf().ReturnType().FinalType();
  if ( rtype.IsReference() )
    {
      code = 3;
      return rtype;
    }
  else if ( rtype.IsPointer() )
    {
      code = 2;
      return rtype.ToType();
    }
  else
    {
      code = 1;
      return rtype;
    }
}

bool
rfx::returnsPointer(Member& method)
{
  Type rtype = method.TypeOf().ReturnType().FinalType();
  return rtype.IsPointer();
}

Member
rfx::getReturnedObjectMethod(Member& method, std::string name)
{
  string rname = rfx::returnTypeName(method);
  return rfx::getMethod(rname, name);
}

Member
rfx::getisAvailable(Member& method)
{
  return getReturnedObjectMethod(method, "isAvailable");
}

Member
rfx::getisNull(Member& method)
{
  return getReturnedObjectMethod(method, "isNull");
}


Member
rfx::decodeMethod(std::string classname,
                  std::string methodrecord,
                  std::vector<ValueThing*>& values,
                  std::vector<void*>& args)
{
  //DB
  //cout << "decodeMethod - classname( " << classname << " )" << endl;

  // Regex to extract name of method
  boost::regex exprName("[a-zA-Z][a-zA-Z0-9]*(-[>])? *(?=[(])");

  // Regex to extract arguments
  boost::regex exprArgs("(?<=[a-zA-Z0-9>]) *[(].*[)]");
  
  boost::smatch what;

  // Extract name of method
  if ( !boost::regex_search(methodrecord, what, exprName) )
    {
      throw cms::Exception("RegexFailure")
        << "Method - "
        << "unable to get name from: " 
        << RED << methodrecord << DEFAULT_COLOR << endl;
    }
  string methodname = what[0];
  
  //DB
  //cout << "decodeMethod - name( " << methodname << " )" << endl;
  
  // Extract arguments of method
  if ( !boost::regex_search(methodrecord, what, exprArgs) )
    {
      throw cms::Exception("RegexFailure")
        << "Method - "
        << "unable to get arguments from: " 
        << RED << methodrecord << DEFAULT_COLOR << endl;
    }
  string methodargs = what[0];
  //DB
  //cout << "decodeMethod - args( " << methodargs << " )" << endl;
  
  // Now get argument regex
  string argsregex("");
  rfx::decodeArguments(methodargs, 
                       argsregex,
                       values);
  if ( argsregex == "" )
    {
      throw cms::Exception("DecodeFailure")
        << "Method - "
        << "unable to decode arguments: " 
        << RED << methodargs << DEFAULT_COLOR << endl;
    }
  //DB
  //cout << "decodeMethod - argsregex( " << argsregex << ") " << endl;
  
  // Search for method that matches both name and signature
  Member method = rfx::getMethod(classname, methodname, argsregex);
  if ( !rfx::memberValid(method) ) return method;

  // We have a method, so get address of arguments and associated
  // values
  for(unsigned i=0; i < values.size(); i++)
    {
      void* addr = values[i]->address();
      //DB
      //cout << "decodeMethod arg[" << i << "]: " 
      //     << addr << ": "
      //     << values[i]->str() << endl;
      args.push_back( addr );
    }
  return method;
}
