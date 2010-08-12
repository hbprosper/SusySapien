// -*- C++ -*-
//
// Package:    PhysicsTools
// Class:      kit
// 
/**\class kit kit.cc 
   PhysicsTools/LiteAnalysis/src/kit.cc

   Description: Some simple utilities

   Implementation:
   Common sense and a sense of beauty.
*/
//
// Original Author:  Harrison B. Prosper
//         Created:  Wed Jun 20 19:53:47 EDT 2007
//         Updated:  Sat Oct 25 2008 - make matchInDeltaR saner
// $Id: kit.cc,v 1.5 2010/03/11 17:44:12 prosper Exp $
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
#include "CLHEP/Random/RandGamma.h"
#include "HepMC/SimpleVector.h"
#include "HepMC/GenVertex.h"
#include "HepPID/ParticleName.hh"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/Utilities/interface/Exception.h"
//-----------------------------------------------------------------------------
using namespace std;
using namespace reco;
using namespace ROOT::Reflex;
//-----------------------------------------------------------------------------
extern "C"
{
  void hepnam_(int&, char*, int);
}
//----------------------------------------------------------------------------

const int MAXDEPTH=10;

void 
kit::set(double y, double* x) {*x=y;}

float
kit::deltaPhi(float phi1, float phi2)
{
  float deltaphi = phi2 - phi1;
  if ( fabs(deltaphi) > M_PI ) deltaphi = 2 * M_PI - fabs(deltaphi);
  return deltaphi;
}

float 
kit::deltaR(float eta1, float phi1, float eta2, float phi2)
{
  float deltaeta = eta1 - eta2;
  float deltaphi = deltaPhi(phi1, phi2);
  return sqrt(deltaeta*deltaeta + deltaphi*deltaphi);
}

std::vector<kit::MatchedPair>
kit::deltaR(std::vector<TLorentzVector>& v1,
            std::vector<TLorentzVector>& v2)
{
  if ( v1.size() == 0 || v2.size() == 0 ) return vector<kit::MatchedPair>();

  vector<kit::MatchedPair> mp(v1.size(), kit::MatchedPair());
  vector<kit::MatchedPair> vp(v2.size(), kit::MatchedPair());

  for(unsigned i=0; i < v1.size(); i++)
    {
      for(unsigned j=0; j < v2.size(); j++)
        {
          vp[j].first = i;
          vp[j].second = j;
          vp[j].distance = v1[i].DeltaR(v2[j]);
        }
      std::sort(vp.begin(), vp.end());
      mp[i].first = i;
      mp[i].second = vp[0].second;
      mp[i].distance = vp[0].distance;
    }
  return mp;
}

std::vector<kit::MatchedPair>
kit::deltaR(std::vector<math::XYZVector>& p1,
            std::vector<math::XYZVector>& p2)
{
  std::vector<TLorentzVector> v1;
  std::vector<TLorentzVector> v2;
  for(unsigned j=0; j < p1.size();j++) v1.push_back(kit::lorentzVector(p1[j]));
  for(unsigned j=0; j < p2.size();j++) v2.push_back(kit::lorentzVector(p2[j]));
  return kit::deltaR(v1, v2);
}

std::vector<kit::MatchedPair>
kit::deltaR(std::vector<PtThing>& v1,
            std::vector<PtThing>& v2)
{
  if ( v1.size() == 0 || v2.size() == 0 ) return vector<kit::MatchedPair>();

  vector<kit::MatchedPair> mp(v1.size(), kit::MatchedPair());
  vector<kit::MatchedPair> vp(v2.size(), kit::MatchedPair());

  for(unsigned i=0; i < v1.size(); i++)
    {
      for(unsigned j=0; j < v2.size(); j++)
        {
          vp[j].first = i;
          vp[j].second = j;
          vp[j].distance = v1[i].deltaR(v2[j]);
        }
      std::sort(vp.begin(), vp.end());
      mp[i].first = i;
      mp[i].second = vp[0].second;
      mp[i].distance = vp[0].distance;
    }
  return mp;
}

std::string 
kit::particleName(reco::GenParticle& p) 
{  
  return HepPID::particleName( p.pdgId() );
}

// Extract name of a file without extension
string 
kit::nameonly(string filename)
{
  int i = filename.rfind("/");
  int j = filename.rfind(".");
  if ( j < 0 ) j = filename.size();
  return filename.substr(i+1,j-i-1);
}

string 
kit::strip(string line)
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
kit::truncate(string s, string substr, int direction)
{
  if ( direction > 0 )
    {
      int i = s.rfind(substr);
      if ( i >= 0 )
        return s.substr(0,i);
      else
        return s;
    }
  else
    {
      int i = s.find(substr);
      if ( i >= 0 )
        return s.substr(i+1, s.size()-i-1);
      else
        return s;
    }
}


void
kit::bisplit(string str, string& left, string& right, string delim,
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

void 
kit::split(string str, vector<string>& vstr)
{
  vstr.clear();
  istringstream stream(str);
  while ( stream )
    {
      string str;
      stream >> str;
      if ( stream ) vstr.push_back(str);
    }
}

string
kit::replace(string& str, string oldstr, string newstr)
{
  return string(TString(str).ReplaceAll(oldstr, newstr).Data());
}

string 
kit::shell(string cmd)
{
  FILE* f = popen(cmd.c_str(),"r");
  int buffsize=8192;
  char s[8192];
  int n = fread(s,1,buffsize,f);
  pclose(f);
  string result = kit::strip(string(s).substr(0,n));
  return result;
}


vector<string>
kit::regex_findall(string& str, string expr)
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
kit::regex_search(string& str, string expr) 
{
  boost::regex regexp(expr);
  boost::smatch match;
  if ( boost::regex_search(str, match, regexp) )
    return match[0];
  else
    return "";
}

string
kit::regex_sub(string& str, string expr, string rstr) 
{
  boost::regex regexp(expr);
  boost::smatch match;
  if ( boost::regex_search(str, match, regexp) )
    return kit::replace(str, match[0], rstr);
  else
    return str;
}

// Needed for classGetterInfo

string subbasicstr(string& str)
{
  return kit::replace(str, "std::basic_string<char>", "std::string");
}

bool skipmethod(string& str)
{
  string expr("TClass|TBuffer|TMember|operator|__get");
  vector<string> v = kit::regex_findall(str, expr);
  return v.size() > 0;
}


string vsqueeze(string& str)
{
  string s = kit::replace(str, " >", ">");
  return kit::replace(s, ">>", "> >");
}

// ---------------------------------------------------------------------------
// Reflex stuff
// ---------------------------------------------------------------------------

void 
kit::getScopes(std::string classname, 
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
      kit::getScopes(basename, names, depth);
    }
}

Reflex::Member
kit::getMethod(std::string classname,
               std::string methodname,
               std::string args)
{
  using namespace Reflex;
  vector<string> names;
  kit::getScopes(classname, names);

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

bool
kit::methodValid(Member& method)
{
  return method.DeclaringType().Name() != "";
}


void
kit::decodeArguments(std::string  args,
                     std::string& argsregex,
                     std::vector<kit::ValueThing*>& vars)
{
  //DB
  //cout << "decodeArguments - args(" << args << ")" << endl;

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

  vector<int> vartype(5,-1);
                      
  argsregex = string("");
  string delim("[(]");
  for(unsigned i=0; i < arglist.size(); i++)
    {
      string str = kit::strip(arglist[i]);
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
            << "kit::decodeArguments - failed on: " << args << endl;
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
          vars.push_back(new kit::Value<string>(str));
          break;

        case 1:
          inp >> d;
          vars.push_back(new kit::Value<double>(d));
          break;

        case 2:
          inp >> l;
          vars.push_back(new kit::Value<long>(l));
          break;

        case 3:
          inp >> b;
          vars.push_back(new kit::Value<bool>(b));
          break;

        case 4:
          break;
        };
    }
  if ( argsregex != "" ) argsregex += "[)]";
}

void*
kit::invokeMethod(Member& method, void* address, vector<void*>& args)
{
  void* raddr=0;
  Type dtype = method.DeclaringType();
  if ( dtype.Name() == "" )
    { 
      //cout << "** invokeMethod - declaring type not found! " << endl;
      return raddr;
    }

  if ( address == 0 )
    { 
      //cout << "** invokeMethod - object address is zero " << endl;
      return raddr;
    }

  Type   rtype = method.TypeOf().ReturnType().FinalType();

  Object object(dtype, address);
  Object* tmp = new Object();
  Object* ret = new Object(rtype, tmp);

  //   cout << "** invokeMethod - method  (" << method.Name() << ")" 
  //        << endl;
  //   cout << "                   rtype  (" << rtype.Name() << ")" << endl;
  //   cout << "                  address (" << address << ")" << endl;

  method.Invoke(object, ret, args);
  raddr = ret->Address();
  delete tmp;
  delete ret;

  if ( rtype.IsPointer() )
    {
      raddr = *static_cast<void**>(raddr);
      //DB
      //cout << "** invokeMethod - POINTER ** " << raddr << endl;
    }
  else if ( rtype.IsReference() )
    {
      //DB
      //cout << "** invokeMethod - REFERENCE ** " << raddr << endl;
    }

  //DB
  // cout << "** invokeMethod - " << method.Name() << " = " 
  //        << RED << *static_cast<double*>(raddr) << BLACK << endl;
  return raddr;
}

std::string
kit::getReturnClass(Member& method)
{
  Type retype = method.TypeOf().ReturnType().FinalType();
  if ( retype.IsPointer() )
    return retype.ToType().Name(SCOPED);
  else
    return retype.Name(SCOPED);
}

bool
kit::returnsPointer(Member& method)
{
  Type retype = method.TypeOf().ReturnType().FinalType();
  return retype.IsPointer();
}

Member
kit::getisNull(Member& method)
{
  string rname = kit::getReturnClass(method);
  return kit::getMethod(rname, "isNull");
}


Member
kit::decodeMethod(std::string classname,
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
        << "unable to get name from: " << RED << methodrecord << BLACK << endl;
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
        << RED << methodrecord << BLACK << endl;
    }
  string methodargs = what[0];
  //DB
  //cout << "decodeMethod - args( " << methodargs << " )" << endl;
  
  // Now get argument regex
  string argsregex("");
  kit::decodeArguments(methodargs, 
                       argsregex,
                       values);
  if ( argsregex == "" )
    {
      throw cms::Exception("DecodeFailure")
        << "Method - "
        << "unable to decode arguments: " 
        << RED << methodargs << BLACK << endl;
    }
  //DB
  //cout << "decodeMethod - argsregex( " << argsregex << ") " << endl;
  
  // Search for method that matches both name and signature
  Member method = kit::getMethod(classname, methodname, argsregex);
  if ( !kit::methodValid(method) ) return method;

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

kit::Method::Method()
  : classname1_(""),
    methodrecord_(""),
    debug_(0),
    method1_(Member()),
    classname2_(""),
    method2_(Member()),
    simple_(true),
    useCINT_(false)
{}

// Model an instantiable method using the Reflex tools

kit::Method::Method(std::string classname, 
                    std::string methodrecord,
                    int debug)
  : classname1_(classname),
    methodrecord_(methodrecord),
    debug_(debug),
    method1_(Member()),
    classname2_(""),
    method2_(Member()),
    simple_(true),
    useCINT_(false)
{
  if ( debug_ > 0 )
    cout << "Method - classname<" << classname << ">\n" 
         << "         method   <" << methodrecord << ">" << endl;

  if ( classname == "" ) 
    throw cms::Exception("InvalidClassname")
      << "null classname " << endl;

  // method could be of the form
  // y = method1(...)->method2(...) or
  // y = method1(...).method2(...)
  boost::regex expr("(?<=[)]) *([-][>]|[.]) *(?=[a-zA-Z])");
  boost::smatch what;
  simple_ = !boost::regex_search(methodrecord, what, expr);

  // If method is compound, split it into its components
  methodrecord1_ = methodrecord;
  methodrecord2_ = "";
  if ( !simple_ )
    {
      string delim(what[0]);
      if ( debug_ > 0 )
        cout << "Method - delim<" << delim << ">" << endl;

      kit::bisplit(methodrecord, methodrecord1_, methodrecord2_, delim);
    }

  if ( debug_ > 0 )
    cout << "Method - method1<" << methodrecord1_ << ">\n" 
         << "         method2<" << methodrecord2_ << ">" << endl;
  
  //----------------------------------------------
  // method 1
  //----------------------------------------------
  method1_ = kit::decodeMethod(classname1_, methodrecord1_, values1_, args1_);

  // If this is a simple method, we're done
  if ( simple_ ) return;
  
  //----------------------------------------------
  // method 2 - 2nd part of a compound method
  //----------------------------------------------
  classname2_ = kit::getReturnClass(method1_);

  if ( debug_ > 0 )
    cout << "Method - return class<" << classname2_ << ">" << endl;
  if ( classname2_ == "" )
    {
      throw cms::Exception("getReturnClassFailure")
        << " can't get return class for method " << method1_.Name() << endl;
    }

  // Object returned could be:
  // 1. a simple pointer
  // 2. an object that has an isNull() method
  // 3. neither of the above

  // If this class contains isNull then
  // set up commands to invoke method using CINT

  Reflex::Member isNull = kit::getisNull(method1_);
  if ( methodValid( isNull ) )
    {
      useCINT_ = true;

      char cmd[512];
      sprintf(cmd, "%s* o = (%s*)0x%s", 
              classname1_.c_str(), classname1_.c_str(), "%x");
      declareobject_ = string(cmd);

      sprintf(cmd, "o->%s.get()", methodrecord1_.c_str());
      invokeget_ = string(cmd);

      if ( debug_ > 0 )
        {
          cout << "Method " 
               << GREEN
               << endl << "\t" << declareobject_  
               << endl << "\t" << invokeget_
               << BLACK
               << endl;
        }
    }
  method2_ = kit::decodeMethod(classname2_, methodrecord2_, values2_, args2_);
  // If we did not find the method
  if ( !methodValid(method2_) )
    {
      if ( debug_ > 0 )
        cout << "** warning ** Method - failed to find method: " 
             << methodrecord2_ << endl;

      // We did not find the requested method in this class, so check
      // for operator->. It may return the appropriate class

      vector<ValueThing*> vals;
      vector<void*> args;
      Reflex::Member pointer = kit::decodeMethod(classname2_, 
                                                 "operator->()", 
                                                 vals, args);

      if ( methodValid(pointer) )
        {
          //D
          if ( debug_ > 0 )
            cout << "              Method - search for operator-> " << endl;

          // get type of returned object
          string classname = kit::getReturnClass(pointer);
          if ( classname == "" )
            {
              throw cms::Exception("ReturnTypeNotFound")
                << "unable to get return type of operator-> of class "
                << endl << "\t" << classname2_ << endl;
            }
          classname2_ = classname;

          if ( debug_ > 0 )
            cout << "              Method - operator-> returns: " 
                 << classname2_ << endl;

          // okay, now try again to find the desired method
          method2_ = kit::decodeMethod(classname2_, methodrecord2_, 
                                       values2_, args2_);

          // If we still can't find method, bail out
          if ( !methodValid(method2_) )
            {
              throw cms::Exception("MethodNotFound")
                << "failed to find method " << methodrecord2_ << endl
                << "\t in class " << classname2_ << endl;
            }
        }
    }
}

kit::Method::~Method() 
{ 
  for(unsigned i=0; i < values1_.size(); i++) delete values1_[i];
  for(unsigned i=0; i < values2_.size(); i++) delete values2_[i];
}

void*
kit::Method::raddress(void* address)
{
  debug_ = 1;
  if ( debug_ > 0 ) cout << endl;

  void* raddr=0;

  // If the returned object has an isNull() method, then
  // use the CINT interpreter to return the object it points to
  if ( useCINT_ )
    {
      if ( debug_ > 0 )
        cout << "Method::raddress - " 
             << RED   << " use CINT" << endl
             << GREEN 
             << declareobject_ << endl
             << invokeget_ << endl
             << BLACK << endl;

      gROOT->ProcessLine(Form(declareobject_.c_str(), address));
      raddr = (void*)gROOT->ProcessLineFast(invokeget_.c_str());

      if ( debug_ > 0 )
        cout << "Method::raddress - get() returns " 
             << RED << raddr << BLACK << endl;
  
      if ( raddr == 0 ) return raddr;
    }
  else
    {
      // Call method 1
      raddr = kit::invokeMethod(method1_, address, args1_);
    }

  if ( simple_ )
    {
      if ( debug_ > 0 )
        //DB
        cout << "Method() - simple method: " << methodrecord_ << endl;

      // simple method with a (presumed) simple return type, so 
      // address cannot be zero
      if ( raddr == 0 )
        {
          // shouldn't happen!
          throw cms::Exception("nullReturn") 
            << RED << " for method " << methodrecord_ << BLACK << endl;
        }
    }
  else
    {
      // This is a compound method, address could be zero if the
      // return type is a pointer or smart pointer
      if ( debug_ > 0 )
        //DB
        cout << "Method() - compound method: " << methodrecord_ << endl;

      if ( raddr == 0 ) return raddr;

      if ( debug_ > 0 )
        //DB
        cout << "Method() - invoke method: " 
             << GREEN << classname2_ << BLACK << "::" 
             << BLUE  << method2_.Name() << BLACK << endl;
      
      // invoke second part of compound method
      raddr = invokeMethod(method2_, raddr, args2_);

      // Must be simple return type, so address cannot be zero
      if ( raddr == 0 )
        {
          // shouldn't happen!
          throw cms::Exception("nullReturn") 
            << RED << " for " 
            << classname2_ << "::" << method2_.Name() << BLACK << endl;
        }
    }
  return raddr;
}

double
kit::Method::operator()(void* addr)
{ 
  void* raddr = raddress(addr);
  if ( raddr == 0 ) 
    return -999999;
  else
    return *static_cast<double*>(raddr); 
}

std::string
kit::Method::str()
{
  ostringstream os;
  os << "DeclaringType1 : " << method1_.DeclaringType().Name() << endl;
  os << "   MethodName1 : " << method1_.Name() << endl;
  os << "     Signature1: " << method1_.TypeOf().Name(SCOPED) << endl;
  if ( values1_.size() > (unsigned)0 )
    {
      os << "     ArgValues: " << endl;
      for(unsigned i=0; i < values1_.size(); i++)
        {
          os << "         " 
             << i << " " << args1_[i]
             << ": " << values1_[i]->str()  << endl;
        }
    }
  if ( !methodValid(method2_) ) return os.str();

  os << "DeclaringType2 : " << method2_.DeclaringType().Name() << endl;
  os << "   MethodName2 : " << method2_.Name() << endl;
  os << "     Signature2: " << method2_.TypeOf().Name(SCOPED) << endl;
  if ( values2_.size() > (unsigned)0 )
    {
      os << "     ArgValues: " << endl;
      for(unsigned i=0; i < values2_.size(); i++)
        {
          os << "         " 
             << i << " " << args2_[i]
             << ": " << values2_[i]->str()  << endl;
        }
    }
  return os.str();
}


std::ostream&
kit::Method::operator<<(std::ostream& os)
{
  os << str() << endl;
  return os;
}

/** Recursively scan class and return all "getter" methods.
 */

kit::ClassMethods::ClassMethods()
  : classname_(""),
    debug_(0),
    baseclasses_(vector<string>()),
    getters_(vector<string>()),
    setters_(vector<string>()),
    signatures_(map<string, int>())
{}

kit::ClassMethods::ClassMethods(string classname, int debug)
  : classname_(classname),
    debug_(debug),
    baseclasses_(vector<string>()),
    getters_(vector<string>()),
    setters_(vector<string>()),
    signatures_(map<string, int>())
{
  getmethods_(classname);
}

kit::ClassMethods::~ClassMethods() {}

void
kit::ClassMethods::getmethods_(string classname, int depth)
{
  using namespace Reflex;

  string spaces = "                                                          ";
  string tab = spaces.substr(0, 2*depth);
  if ( debug_ > 0 )
    cout << tab << "CLASS(" << classname  << ")" << endl;

  if ( depth > 10 ) 
    {
      cout << "ClassMethods::getmethods - yikes, lost in the trees!" 
           << endl;
      exit(0);
    }

  Type thing;
  Type c = thing.ByName(classname);
  int nfunctions = c.FunctionMemberSize();
  
  if ( debug_ > 0 )
    cout << tab << "  number of functions: " << nfunctions << endl;
 
  for(int i=0; i < nfunctions; i++)
    {
      Member m = c.FunctionMemberAt(i);
      if ( !m.IsPublic() ) continue;
      if ( !m.IsFunctionMember() ) continue;
      if ( m.IsConstructor() ) continue;
      if ( m.IsDestructor() ) continue;

      // we have a public method, decode it

      string mname = m.Name();
      string mtype = m.TypeOf().Name(SCOPED);

      if ( debug_ > 2 )
        cout << tab << "\t  "
             << "NAME(" << mname << ") "
             << "MTYPE(" << mtype << ")" << endl;

      // simplify edm::Refs

      boost::regex expr1("edm::Ref<std::vector<.+?(?=> >)");  
      boost::smatch match1;
      if ( boost::regex_search(mtype, match1, expr1) )
        {
          boost::regex expr2("edm::Ref<std::vector<.+?(?=>,)");  
          boost::smatch match2;
          if ( boost::regex_search(mtype, match2, expr2) )
            mtype = kit::replace(mtype, match1[0], match2[0]);
        }

      // replace basic_str with std::string

      mtype = kit::strip( subbasicstr(mtype) );

      // split method type into return type and argument list
      string rtype, args;
      kit::bisplit(mtype, rtype, args, "(");
      args = "(" + args;
      rtype = kit::strip(rtype);

      if ( debug_ > 2 )
        cout << tab << "\t "
             << " RTYPE(" << rtype << ")" 
             << " ARGS(" << args << ")" 
             << endl;

      args = kit::replace(args, "(void)", "()");
      string signature = mname + args;

      // skip if this method already included

      if ( signatures_.find(signature) != signatures_.end() ) continue;
      signatures_[signature] = i;
      if ( debug_ > 2 )
        cout << tab << "\t  signature(" << signature << ")" << endl;

      // skip Root serialization methods
      string method = rtype + " " + signature;
      if ( skipmethod(method) )
        {
          if ( debug_ > 1 ) 
            cout << tab << "\t** skip: " << method << endl << endl;
          continue;
        }

      string record 
        = rtype + "|"
        + mname + "|"
        + args + "|"
        + classname;

      rtype = kit::strip(rtype);
      if ( rtype.substr(0, 4) == "void" )
        setters_.push_back(record);
      else
        getters_.push_back(record);

      if ( debug_ > 0 )
        cout << tab << "\t  METHOD(" << record << ")" 
             << endl << endl;
    }
  
  // Loop over base classes, if present

  int nbase = c.BaseSize();
  if ( debug_ > 0 )
    cout << tab << "\t number of base classes: " << nbase << endl;

  for(int i=0; i < nbase; i++)
    {
      Type b = c.BaseAt(i).ToType();
      string basename = b.Name(SCOPED);
      baseclasses_.push_back(basename);
      getmethods_(basename, depth+1);
    }
}

TLorentzVector 
kit::lorentzVector(reco::GenParticle& p)
{ 
  return TLorentzVector(p.px(), p.py(), p.pz(), p.energy());
}

TLorentzVector 
kit::lorentzVector(reco::GenParticle* p)
{ 
  return kit::lorentzVector(*p);
}

TVector3 
kit::vector3(reco::GenParticle& p)
{ 
  return TVector3(p.px(), p.py(), p.pz());
}

TLorentzVector 
kit::lorentzVector(math::XYZVector& q)
{ 
  TLorentzVector v(q.X(),q.Y(),q.Z(),q.R());
  return v;
}

TVector3 
kit::vector3(math::XYZVector& q)
{ 
  TVector3 v(q.X(), q.Y(), q.Z());
  return v;
}

/*
  vector<HepMC::GenParticle*>
  kit::daughters(HepMC::GenParticle* part)
  {
  vector<HepMC::GenParticle*> vp;
  HepMC::GenVertex* v = part->end_vertex();
  if ( v )
  {
  HepMC::GenVertex::particles_out_const_iterator p;
  HepMC::GenVertex::particles_out_const_iterator start = 
  v->particles_out_const_begin();
  HepMC::GenVertex::particles_out_const_iterator end = 
  v->particles_out_const_end();
  for ( p = start; p != end; ++p ) vp.push_back(*p);
  }
  return vp;
  }
*/

std::vector<reco::GenParticle*>
kit::daughters(reco::GenParticle& p, bool skipsame) 
{
  std::vector<reco::GenParticle*> vp;
  int nd = p.numberOfDaughters();
  if ( nd == 0 ) return vp;

  for(int i=0; i < nd; i++)
    {
      if ( p.daughter(i) == 0 )
        {
          cout << "\t** kit::daughters - no daughters found, while expecting " 
               << nd << endl;
          continue;
        }
      reco::GenParticle* d = dynamic_cast<reco::GenParticle*>(p.daughter(i));
      if ( d == 0 ) continue;

      if ( skipsame )
        if ( p.pdgId() == d->pdgId() ) continue;
      vp.push_back( d );
    }
  return vp;
}

std::vector<reco::GenParticle*>
kit::daughters(reco::GenParticle* p, bool skipsame)
{
  return kit::daughters(*p, skipsame);
} 

int 
kit::find(vector<reco::GenParticle>& vp, int id, int start) 
{
  for(unsigned i=start; i < vp.size(); i++)
    {
      if ( vp[i].pdgId() != id ) continue;
      return i;
    }
  return -1;
}

int 
kit::find(vector<reco::GenParticle*>& vp, int id, int start) 
{
  for(unsigned i=start; i < vp.size(); i++)
    {
      if ( vp[i]->pdgId() != id ) continue;
      return i;
    }
  return -1;
}


std::string
kit::table(vector<reco::GenParticle>& vp)
{
  std::ostringstream os;

  char record[256];
  sprintf(record, 
          "%6s %-10s%9s%10s%10s%10s%10s%4s", 
          "index", "name", "pdgid","px","py","pz","e","stat");
  os << record << endl;

  for(unsigned index=0; index < vp.size(); index++)
    {
      reco::GenParticle& p = vp[index];
      sprintf(record, 
              "%6d %-10s%9d %9.2e %9.2e %9.2e %9.2e%4d", 
              index, 
              kit::particleName(p).substr(0,10).c_str(),
              p.pdgId(), 
              p.px(), p.py(), p.pz(), p.energy(), 
              p.status());
      os << record << endl;

      vector<reco::GenParticle*> d = kit::daughters(p);
      int count = 0;
      int maxcount = 5;
      string delim("\t\t\t");
      for(unsigned i=0; i < d.size(); i++)
        {
          os << delim << kit::particleName(d[i]).substr(0,10);
          delim = " ";
          count++;
          if (count == maxcount)
            {
              count = 0;
              os << endl;
              delim = "\t\t\t";
            }
        }
      os << endl;
    }
  return os.str();
}

/*
  std::ostream& operator<<(std::ostream& os, vector<reco::GenParticle*>& vp)
  {
  return os << kit::table(vp);
  }

  std::ostream& operator<<(std::ostream& os, reco::GenParticle* p)
  {
  return os << kit::tree(os, p, 1);
  }
*/

std::string
kit::tree(reco::GenParticle& p,
          int printlevel, int maxdepth, int depth)
{
  std::ostringstream os;
  kit::tree(os, 
            p,
            printlevel, maxdepth, depth);
  return os.str();
}

//---------------------------------------------------------------------------
// Purpose: Print StdHep event in a tree format
// Created: 06-Sep-2004 Harrison B. Prosper
//---------------------------------------------------------------------------
void 
kit::tree(std::ostream& stream,
          reco::GenParticle& p,
          int           printlevel,
          int           maxdepth,
          int           depth)
{
  if ( depth >  (maxdepth <= 0 ? MAXDEPTH : maxdepth) ) return;    
  
  int ppid   = p.pdgId();
  int status = p.status();
  float px   = p.px();
  float py   = p.py();
  float pz   = p.pz();
  float pt   = p.pt();
  float e    = p.energy();

  if ( status < 4 )
    {
      depth++;  
      stream << "  ";
      if ( depth > 1 ) for (int i = 1; i < depth; i++) stream << ". ";
  
      stream.setf(std::ios::left);  // left justify

      stream << std::setw(10) << kit::particleName(p);

      if ( printlevel > 0 )
        {
          stream.setf(std::ios::right);  // right justify  
          stream.precision(4);
          stream << "  "	    
                 << pt << "  "
                 << "("
                 << e  << ", "  
                 << px << ", "
                 << py << ", "
                 << pz
                 << ")";
          stream << " " << status;
        }
      if ( printlevel > 1 ) stream << " " << ppid;
      stream << std::endl;
    }

  std::vector<reco::GenParticle*> d = kit::daughters(p);
  if ( d.size() == 0 ) return;

  // Loop over daughters
  for (unsigned i=0; i < d.size(); i++)
    kit::tree(stream, 
              *d[i],
              printlevel,
              maxdepth,
              depth);
}


std::string
kit::tree(int    index,
          int    nhep,
          int*   pdgid,
          int*   status,
          double p[][5],
          int    d[][2],
          int printlevel, int maxdepth, int depth)
{
  std::ostringstream os;
  kit::tree(os, 
            index,
            nhep,
            pdgid,
            status,
            p,
            d,
            printlevel, maxdepth, depth);
  return os.str();
}

//---------------------------------------------------------------------------
// Purpose: Print StdHep event in a tree format
// Created: 06-Sep-2004 Harrison B. Prosper
//---------------------------------------------------------------------------
void 
kit::tree(std::ostream& stream,
          int    index,
          int    nhep,
          int*   pdgid,
          int*   status,
          double p[][5],
          int    d[][2],
          int           printlevel,
          int           maxdepth,
          int           depth)
{
  if ( index >  nhep - 1 ) return;
  if ( depth >  (maxdepth <= 0 ? MAXDEPTH : maxdepth) ) return;    
  
  int ppid   = pdgid[index];
  int stat   = status[index];
  float px   = p[index][0];
  float py   = p[index][1];
  float pz   = p[index][2];
  float e    = p[index][3];
  float pt   = sqrt(px*px+py*py);

  if ( stat < 4 )
    {
      depth++;  
      stream << "  ";
      if ( depth > 1 ) for (int i = 1; i < depth; i++) stream << ". ";
  
      stream.setf(std::ios::left);  // left justify

      stream << std::setw(10) << kit::particleName(ppid);

      if ( printlevel > 0 )
        {
          stream.setf(std::ios::right);  // right justify  
          stream.precision(4);
          stream << "  "	    
                 << pt << "  "
                 << "("
                 << e  << ", "  
                 << px << ", "
                 << py << ", "
                 << pz
                 << ")";
          stream << " " << stat;
        }
      if ( printlevel > 1 ) stream << " " << ppid;
      stream << std::endl;
    }

  if (d[index][0] == 0) return;

  // Loop over daughters
  for (int i=d[index][0]; i <= d[index][1]; i++)
    kit::tree(stream, 
              i,
              nhep,
              pdgid,
              status,
              p,
              d,
              printlevel,
              maxdepth,
              depth);
}

std::string 
kit::particleName(reco::GenParticle* p) 
{  
  return kit::particleName( *p );
}

std::string 
kit::particleName(int id) 
{
  return HepPID::particleName(id);
}

// ---------------------------------------------
// Some histograms utilities
// ---------------------------------------------
namespace {
  const int LINEWIDTH= 2;
  const int TEXTFONT = 42;
  const int NDIV     = 505;
  const int WIDTH    = 500;
  const int HEIGHT   = 500;
  const double TEXTSIZE = 0.04;
  const double MARKERSIZE = 1.0;
}

void 
kit::setStyle()
{
  gROOT->SetStyle("Pub");
  TStyle* style = gROOT->GetStyle("Pub");
  style->SetFrameBorderMode(0);
  style->SetCanvasBorderSize(0);
  style->SetCanvasBorderMode(0);
  style->SetCanvasColor(0);
  style->SetPadBorderMode(0);
  style->SetPadColor(0);

  // Margins

  style->SetPadTopMargin(0.05);
  style->SetPadBottomMargin(0.16);
  style->SetPadLeftMargin(0.20);
  style->SetPadRightMargin(0.10);

  // For the Global title:

  style->SetOptTitle(0);
  style->SetTitleFont(TEXTFONT);
  style->SetTitleColor(1);
  style->SetTitleTextColor(1);
  style->SetTitleFillColor(10);
  style->SetTitleFontSize(0.05);

  // For the axis titles:

  style->SetTitleColor(1, "XYZ");
  style->SetTitleFont(TEXTFONT, "XYZ");
  style->SetTitleSize(0.05, "XYZ");
  style->SetTitleXOffset(0.9);
  style->SetTitleYOffset(1.25);

  // For the axis labels:

  style->SetLabelColor(1, "XYZ");
  style->SetLabelFont(TEXTFONT, "XYZ");
  style->SetLabelOffset(0.007, "XYZ");
  style->SetLabelSize(0.05, "XYZ");

  // For the axis:

  style->SetAxisColor(1, "XYZ");
  style->SetStripDecimals(kTRUE);
  style->SetTickLength(0.03, "XYZ");
  style->SetNdivisions(NDIV, "XYZ");
  style->SetPadTickX(1);  
  style->SetPadTickY(1);

  style->cd();
}

vector<double> 
kit::contents(TH1* hist)
{
  vector<double> c(hist->GetNbinsX());
  for(int i=0; i < hist->GetNbinsX(); i++)
    c[i] = hist->GetBinContent(i+1);
  return c;
}

vector<double> 
kit::cdf(TH1* hist)
{
  vector<double> c(hist->GetNbinsX());
  c[0] = hist->GetBinContent(1);
  for(int i=1; i < hist->GetNbinsX(); i++)
    c[i] = c[i-1]+hist->GetBinContent(i+1);
  return c;
}

void
kit::setContents(TH1* hist, vector<double>& c)
{
  int n = hist->GetNbinsX();
  int nbin = n < (int)c.size() ? n : c.size();
  for(int i=0; i < nbin; i++) hist->SetBinContent(i+1, c[i]);
}

void
kit::setErrors(TH1* hist, vector<double>& err)
{
  int n = hist->GetNbinsX();
  int nbin = n < (int)err.size() ? n : err.size();
  for(int i=0; i < nbin; i++) hist->SetBinError(i+1, err[i]);
}

TH1F*
kit::divideHistograms(TH1* N, TH1* D, string ytitle)
{
  int nN = N->GetNbinsX();
  int nD = D->GetNbinsX();
  int nbin = nN < nD ? nN : nD;
  vector<double> n = kit::contents(N);
  vector<double> d = kit::contents(D);

  string nname(N->GetName());
  string dname(D->GetName());
  string hname = nname + "_" + dname;

  double xmin = N->GetXaxis()->GetXmin();
  double xmax = N->GetXaxis()->GetXmax();

  TH1F* h = new TH1F(hname.c_str(), "", nbin, xmin, xmax);

  // Set some reasonable defaults

  h->SetLineColor(kBlack);
  h->SetMarkerSize(1);
  h->SetMarkerStyle(20);

  h->GetXaxis()->CenterTitle();
  h->GetXaxis()->SetTitle(N->GetXaxis()->GetTitle());
  h->GetXaxis()->SetTitleOffset(1.3);
  h->SetNdivisions(504, "X");
  h->SetMarkerSize(1.0);

  h->GetYaxis()->CenterTitle();
  h->GetYaxis()->SetTitle(ytitle.c_str());
  h->GetYaxis()->SetTitleOffset(1.8);
  h->SetNdivisions(510, "Y");

  for(int i=0; i < nbin; i++) 
    {
      int bin = i+1;
      double p = 0;
      double e = 0;
      if ( (d[i] > 0) && (n[i] > 0) ) 
        {
          p = n[i] / d[i];
          // Use rough error calculation for now
          e = sqrt(d[i]*p*(1-p))/d[i];
        }
      h->SetBinContent(bin, p);
      h->SetBinError(bin, e);
    }
  return h;
}



void
kit::saveHistograms(string histfilename, 
                    TDirectory* dir, TFile* hfile, int depth)
{
  // Create output file

  if ( depth == 0 )
    {
      cout << "Saving histograms to " << histfilename << endl;
      hfile = new TFile(histfilename.c_str(), "RECREATE");
    }

  // Important!
  depth++;
  // Check recursion depth 
  if ( depth > 100 )
    {
      cout << "saveHistograms is lost in trees!" << endl;
      exit(0);
    }
  string tab(2*depth, ' ');

  // Loop over all histograms

  TList* list = dir->GetList();
  TListIter* it = (TListIter*)list->MakeIterator();

  while ( TObject* o = it->Next() )
    {
      dir->cd();
      
      if ( o->IsA()->InheritsFrom("TDirectory") )
        {
          TDirectory* d = (TDirectory*)o;
          cout << tab << "BEGIN " << d->GetName() << endl;
          kit::saveHistograms(histfilename, d, hfile, depth);
          cout << tab << "END " << d->GetName() << endl;
        }
      // Note: All histograms inherit from TH1
      else if ( o->IsA()->InheritsFrom("TH1") )
        {
          TH1* h = (TH1*)o;
          cout << tab << o->ClassName() 
               << "\t" << h->GetName()
               << "\t" << h->GetTitle()
               << endl;
          hfile->cd();
          h->Write("", TObject::kOverwrite);
        }
    } // end of loop over keys

  hfile->Close();
  delete hfile;
}


TCanvas*
kit::canvas(string name, int n, int width, int height)
{
  kit::shell("mkdir -p figures");
  string cname  = "figures/fig_" + name;
  string ctitle = cname;
  int xoffset = 40*n;
  int yoffset = 10*n;
  return new TCanvas(cname.c_str(), 
                     ctitle.c_str(), 
                     xoffset, yoffset, width, height);
}
//-----------------------------------------------------------------------------
TLegend*
kit::legend(string title, int nlines, float xmin, float xmax, float ymax)
{    
  float ymin = ymax - nlines * 0.08;
  TLegend* l = new TLegend(xmin, ymin, xmax, ymax, title.c_str());
  l->SetFillColor(0);
  l->SetFillStyle(4000);
  l->SetBorderSize(0);
  l->SetTextSize(TEXTSIZE);
  l->SetTextFont(TEXTFONT);
  return l;
}
//-----------------------------------------------------------------------------
TH1F* 
kit::histogram(string hname,
               string xtitle,
               string ytitle,
               int    nbin,
               float  xmin,
               float  xmax,
               int    color,
               int    lstyle,
               int    lwidth)
{
  TH1F* h = new TH1F(hname.c_str(), "", nbin, xmin, xmax);

  h->SetLineColor(color);
  h->SetLineStyle(lstyle);
  h->SetLineWidth(lwidth);
 
  h->GetXaxis()->CenterTitle();
  h->GetXaxis()->SetTitle(xtitle.c_str());
  h->GetXaxis()->SetTitleOffset(1.3);

  h->GetYaxis()->CenterTitle();
  h->GetYaxis()->SetTitle(ytitle.c_str());
  h->GetYaxis()->SetTitleOffset(1.8);
  return h;
}
//-----------------------------------------------------------------------------
TH2F* 
kit::histogram(string hname,
               string xtitle,
               int    nbinx,
               float  xmin,
               float  xmax,
               string ytitle,
               int    nbiny,
               float  ymin,
               float  ymax,
               int    color,
               int    lstyle,
               int    lwidth)
{
  TH2F* h = new TH2F(hname.c_str(), "", nbinx, xmin, xmax, nbiny, ymin, ymax);

  h->SetLineColor(color);
  h->SetLineStyle(lstyle);
  h->SetLineWidth(lwidth);
 
  h->GetXaxis()->CenterTitle();
  h->GetXaxis()->SetTitle(xtitle.c_str());
  h->GetXaxis()->SetTitleOffset(1.3);

  h->GetYaxis()->CenterTitle();
  h->GetYaxis()->SetTitle(ytitle.c_str());
  h->GetYaxis()->SetTitleOffset(1.8);
  return h;
}
//-----------------------------------------------------------------------------
TGraph* 
kit::graph(vector<double>& x, 
           vector<double>& y,
           string xtitle,
           string ytitle,
           float  xmin,
           float  xmax,
           float  ymin,
           float  ymax,
           int    color,
           int    lstyle,
           int    lwidth)
{
  int nbins = x.size();
    
  TGraph* g = new TGraph(nbins, &x[0], &y[0]);
  g->SetLineColor(color);
  g->SetLineStyle(lstyle);
  g->SetLineWidth(lwidth);

  g->GetXaxis()->CenterTitle();
  g->GetXaxis()->SetTitle(xtitle.c_str());
  g->GetXaxis()->SetTitleOffset(1.3);
  g->GetHistogram()->SetAxisRange(xmin, xmax, "X");

  g->GetYaxis()->CenterTitle();
  g->GetYaxis()->SetTitle(ytitle.c_str());
  g->GetYaxis()->SetTitleOffset(1.8);
  g->GetHistogram()->SetAxisRange(ymin, ymax, "Y");
  return g;
}

void 
kit::plot(TCanvas* canvas, 
          TH1* h, string opt)
{
  if (!canvas) return;
  if (!h) return;

  TString tstr(opt.c_str());
  tstr.ToUpper();
  bool witherrors = tstr.Contains("E");
  bool save = tstr.Contains("S");

  canvas->cd();
  string option("");
  if (witherrors) option = "EP";
  h->Draw(option.c_str());
  canvas->Update();

  if (save) canvas->SaveAs(".gif");
}

void 
kit::plot(TCanvas* canvas, 
          vector<TH1*>& h, string opt)
{
  if (!canvas) return;
  if ( h.size() == 0 ) return;

  TString tstr(opt.c_str());
  tstr.ToUpper();
  bool witherrors = tstr.Contains("E");
  bool save = tstr.Contains("S");

  canvas->cd();
  if (witherrors)
    h[0]->Draw("EP");
  else
    h[0]->Draw();

  for(unsigned i=1; i < h.size(); i++) h[i]->Draw("same");
  canvas->Update();

  if (save) canvas->SaveAs(".gif");
}

// ---- STATISTICS

///////////////////////////////////////////////////////////////////////////////
// File:	poissongamma.cpp
// Description:	x = poissongamma(p,a,d)
// 
//              compute the marginal likelihood:
//              a product of Poisson distributions
//              and a prior that is a product of gamma distributions.
//
//              Model:
//
//              d_i = Sum p_j * a_ji,  i=1..M,   j=1..N
// 
//              D_i  is count corresponding to true mean d_i
//              A_ji is count corresponding to true mean a_ji
//
//              D is a vector of M observed counts (that is, over M bins)
//              p is a vector of N parameters (that is, over N sources)
//              A is a vector of vectors of size N x M counts.
//
//              Simple 1-D Application:
//              
//              d =  xsec * e_a * a + e_b * b 
//
//              where p_1 = xsec * e_a
//                    p_2 = e_b
//                    a_11= a
//                    a_21= b
//
//              and e_a, e_b are scale factors defined by
//              e = sigma^2 / estimate
//
// WARNING: The number of terms grows exponentially with the number of 
//          sources and the maximum bin count. Therefore, this routine
//          really only makes sense for rather few sources with modest
//          bin counts. When the number of sources is > 4 and/or the 
//          maximum observed bin count is large use poissongammamc,
//          which does the calculation by Monte Carlo integration.
//
// Created:     20-Dec-2001 Harrison B. Prosper
//              11-Mar-2004 HBP add additional interfaces
//              07-Jun-2005 HBP increase number of sources to 10! See warning
//                          above.
//              21-Jun-2005 HBP add poissongammamc
///////////////////////////////////////////////////////////////////////////////

const int MAXBUF = 10000; // Maximum count per bin
const int MAXSRC = 10;    // Maximum number of sources

// Global variables to avoid memory allocation.

static double cs[MAXSRC][MAXBUF];
static double ns[MAXSRC];
static double si[MAXSRC];
static double x [MAXSRC];
static double y [MAXSRC];

double 
kit::logpoisson(vdouble&	p,  // Weights "p_j" 
                vvdouble&	A,  // Counts  "A_ji" for up to 10 sources
                vdouble&	D,  // Counts  "D_i" for data.
                bool scale)       // Scale p_j if true  
{
  int N = p.size(); // Number of sources (N)
  int M = D.size(); // Number of bins    (M)
  vdouble x(N);

  // Scale counts if requested

  if ( scale )
    {
      for ( int j = 0; j < N; j++ )
        {
          double ns = 0;
          for ( int i = 0; i < M; i++ ) ns += A[j][i];
          x[j] = p[j]/(ns+M);
        }
    }
  else
    for ( int j = 0; j < N; j++ )
      x[j] = p[j];
    
  double q = 0;
  for ( int i = 0; i < M; i++ )
    {
      // Compute Poisson parameter for ith bin
      double d = 0;
      for ( int j = 0; j < N; j++ ) d += x[j] * A[j][i];
      double p = exp(-d);
      int N = (int)D[i];
      if ( N > 0 ) for (int n = 1; n <= N; n++) p *= d / n;
      q += log(p);
    }
  return q;
}

double 
kit::poissongamma(vdouble&	p,  // Weights "p_j" 
                  vvdouble&	a,  // Counts  "A_ji" for up to 10 sources
                  vdouble&	d,  // Counts  "D_i" for data.
                  bool returnlog,    // return log(P) if true
                  bool scale)     // Scale p_j if true  
{
  int N = p.size(); // Number of sources (N)
  int M = d.size(); // Number of bins    (M)

  if ( a.size() != (unsigned int)N )
    {
      std::cout << "**Error - poissongamma - mis-match in number of sources\n"
                << "size(p): " << N << " differs from size(A) = " << a.size()
                << std::endl;
      exit(0);
    }

  if ( a[0].size() != (unsigned int)M )
    {
      std::cout << "**Error - poissongamma - mis-match in number of sources\n"
                << "size(d): " << M << " differs from size(A[0]) = " 
                << a[0].size()
                << std::endl;
      exit(0);
    }

  if ( M < 1 ) return -1.0;
  
  if ( ( N < 2 ) || ( N > MAXSRC ) ) return -2.0;
  
  // Get total counts per source

  for ( int j = 0; j < N; j++ )
    {
      ns[j] = 0;
      for ( int i = 0; i < M; i++ ) ns[j] += a[j][i];
    }

  // Get data count

  double nd = 0.0; 					
  for ( int i = 0; i < M; i++ ) nd += d[i];

  // Scale counts

  for ( int j = 0; j < N; j++ )
    {
      if ( scale )
        x[j] = p[j] / (ns[j]+M);
      else
        x[j] = p[j];
      y[j] = x[j] / (1+x[j]);
    }

  // loop over the M terms of the product,
  // corresponding to the M bins

  double prob;
  if ( returnlog )
    prob = 0.0;
  else
    prob = 1.0;
  for ( int i = 0; i < M; i++)
    {
      double di = d[i];  // data count for bin i
      int D = (int)di;

      // compute terms of sum from zero to D
      
      for ( int j = 0; j < N; j++ )
        {
          si[j]    = a[j][i];  // "A_ji"
          cs[j][0] = pow(1+x[j],-(si[j]+1));
        }
      
      if ( D > 0 )
        {
          for ( int k = 1; k < D+1; k++ )
            for ( int j = 0; j < N; j++ )
              cs[j][k] = cs[j][k-1] * y[j] * (si[j]+k)/k;
        }

      // compute sum

      double sum = 0.0;
      switch (N)
        {
        case 2:
          for (int j = 0; j < D+1; j++)
            sum += 
              cs[0][j] *
              cs[1][D-j];
          break;

        case 3:
          for (int j = 0; j < D+1; j++)
            for (int k = 0; k < D+1-j; k++)
              sum += 
                cs[0][j] *
                cs[1][k] *
                cs[2][D-j-k];
          break;

        case 4:
          for (int j = 0; j < D+1; j++)
            for (int k = 0; k < D+1-j; k++)
              for (int l = 0; l < D+1-j-k; l++)
                sum += 
                  cs[0][j] *
                  cs[1][k] *
                  cs[2][l] *
                  cs[3][D-j-k-l];
          break;

        case 5:
          for (int j = 0; j < D+1; j++)
            for (int k = 0; k < D+1-j; k++)
              for (int l = 0; l < D+1-j-k; l++)
                for (int m = 0; m < D+1-j-k-l; m++)
                  sum += 
                    cs[0][j] *
                    cs[1][k] *
                    cs[2][l] *
                    cs[3][m] *
                    cs[4][D-j-k-l-m];
          break;

        case 6:
          for (int j = 0; j < D+1; j++)
            for (int k = 0; k < D+1-j; k++)
              for (int l = 0; l < D+1-j-k; l++)
                for (int m = 0; m < D+1-j-k-l; m++)
                  for (int m1 = 0; m1 < D+1-j-k-l-m; m1++)
                    sum += 
                      cs[0][j] *
                      cs[1][k] *
                      cs[2][l] *
                      cs[3][m] *
                      cs[4][m1] *
                      cs[5][D-j-k-l-m-m1];
          break;

        case 7:
          for (int j = 0; j < D+1; j++)
            for (int k = 0; k < D+1-j; k++)
              for (int l = 0; l < D+1-j-k; l++)
                for (int m = 0; m < D+1-j-k-l; m++)
                  for (int m1 = 0; m1 < D+1-j-k-l-m; m1++)
                    for (int m2 = 0; m2 < D+1-j-k-l-m-m1; m2++)
                      sum += 
                        cs[0][j] *
                        cs[1][k] *
                        cs[2][l] *
                        cs[3][m] *
                        cs[4][m1] *
                        cs[5][m2] *
                        cs[6][D-j-k-l-m-m1-m2];
          break;

        case 8:
          for (int j = 0; j < D+1; j++)
            for (int k = 0; k < D+1-j; k++)
              for (int l = 0; l < D+1-j-k; l++)
                for (int m = 0; m < D+1-j-k-l; m++)
                  for (int m1 = 0; m1 < D+1-j-k-l-m; m1++)
                    for (int m2 = 0; m2 < D+1-j-k-l-m-m1; m2++)
                      for (int m3 = 0; m3 < D+1-j-k-l-m-m1-m2; m3++)
                        sum += 
                          cs[0][j] *
                          cs[1][k] *
                          cs[2][l] *
                          cs[3][m] *
                          cs[4][m1] *
                          cs[5][m2] *
                          cs[6][m3] *
                          cs[7][D-j-k-l-m-m1-m2-m3];
          break;

        case 9:
          for (int j = 0; j < D+1; j++)
            for (int k = 0; k < D+1-j; k++)
              for (int l = 0; l < D+1-j-k; l++)
                for (int m = 0; m < D+1-j-k-l; m++)
                  for (int m1 = 0; m1 < D+1-j-k-l-m; m1++)
                    for (int m2 = 0; m2 < D+1-j-k-l-m-m1; m2++)
                      for (int m3 = 0; m3 < D+1-j-k-l-m-m1-m2; m3++)
                        for (int m4 = 0; m4 < D+1-j-k-l-m-m1-m2-m3; m4++)
                          sum += 
                            cs[0][j] *
                            cs[1][k] *
                            cs[2][l] *
                            cs[3][m] *
                            cs[4][m1] *
                            cs[5][m2] *
                            cs[6][m3] *
                            cs[7][m4] *
                            cs[8][D-j-k-l-m-m1-m2-m3-m4];
          break;

        case 10:
          for (int j = 0; j < D+1; j++)
            for (int k = 0; k < D+1-j; k++)
              for (int l = 0; l < D+1-j-k; l++)
                for (int m = 0; m < D+1-j-k-l; m++)
                  for (int m1 = 0; m1 < D+1-j-k-l-m; m1++)
                    for (int m2 = 0; m2 < D+1-j-k-l-m-m1; m2++)
                      for (int m3 = 0; m3 < D+1-j-k-l-m-m1-m2; m3++)
                        for (int m4 = 0; m4 < D+1-j-k-l-m-m1-m2-m3; m4++)
                          for (int m5 = 0; m5 < D+1-j-k-l-m-m1-m2-m3-m4; m5++)
                            sum += 
                              cs[0][j] *
                              cs[1][k] *
                              cs[2][l] *
                              cs[3][m] *
                              cs[4][m1] *
                              cs[5][m2] *
                              cs[6][m3] *
                              cs[7][m4] *
                              cs[8][m5] *
                              cs[9][D-j-k-l-m-m1-m2-m3-m4-m5];
          break;
        };
      if ( returnlog )
        prob += log(sum);
      else
        prob *= sum;
    }
  return prob;
}

// For simple 1-D application
//
// 
double 
kit::poissongamma1D(double xsec,
                    double Lum,
                    double sigmaLum,
                    double Bkg,
                    double sigmaBkg,
                    double Datum)
{
  // Compute scale factors and effective counts
  double eA = sigmaLum * sigmaLum / Lum;
  double Aeff = Lum / eA;

  if ( Bkg > 0.0 )
    {
      double eB = sigmaBkg * sigmaBkg / Bkg;
      double Beff = Bkg / eB;
      
      vdouble  p(2); p[0] = xsec * eA; p[1] = eB;
      vdouble  a(1), b(1), D(1); a[0] = Aeff; b[0] = Beff;
      vvdouble A(2); A[0] = a; A[1] = b; D[0] = Datum;

      return kit::poissongamma(p, A, D, false, false);
    }
  else
    {
      vdouble  p(1); p[0] = xsec * eA;
      vdouble  a(1), D(1); a[0] = Aeff;
      vvdouble A(1); A[0] = a; D[0] = Datum;

      return kit::poissongamma(p, A, D, false, false);
    }
}


double 
kit::poissongamma2(double	p1,
                   double     p2,
                   vdouble&   A1,
                   vdouble&   A2,
                   vdouble&	D,
                   bool returnlog,
                   bool scale)
{
  vdouble  p(2); p[0] = p1; p[1] = p2;
  vvdouble A(2); A[0] = A1; A[1] = A2;
  return poissongamma(p, A, D, returnlog, scale);
}

double 
kit::poissongamma5(double	p1,
                   double     p2,
                   double     p3,
                   double     p4,
                   double     p5,
                   vdouble&   A1,
                   vdouble&   A2,
                   vdouble&   A3,
                   vdouble&   A4,
                   vdouble&   A5,
                   vdouble&	D,
                   bool returnlog,
                   bool scale)
{
  vdouble  p(5); p[0] = p1; p[1] = p2; p[2] = p3; p[3] = p4; p[4] = p5;
  vvdouble A(5); A[0] = A1; A[1] = A2; A[2] = A3; A[3] = A4; A[4] = A5;
  return kit::poissongamma(p, A, D, returnlog, scale);
}


//-----------------------------------------------------------------------------
// Compute Poisson X gamma density by Monte Carlo integration
//-----------------------------------------------------------------------------
//extern "C"
//{
//  float rngama_(float& p);
//}

static int TOTAL=0;
static vector<double> YIELD;

void 
kit::initpoissongammamc(vvdouble& a, // Counts  "A_ji" 
                        int total)   // Number of MC sampling points
{
  int N = a.size();    // Number of sources (N)
  int M = a[0].size(); // Number of bins    (M)
  TOTAL = total;
  YIELD.clear();
  YIELD.reserve(N*M*TOTAL);
  for ( int i = 0; i < M; i++ )
    for (int k = 0; k < TOTAL; k++)
      for ( int j = 0; j < N; j++ )
        {
          //float q = (float)(a[j][i] + 1);
          //YIELD.push_back(rngama_(q));
          //YIELD.push_back(rngama_(q));
        }
}

double 
kit::poissongammamc(vdouble&	p,  // Weights "p_j" 
                    vvdouble&	a,  // Counts  "A_ji"
                    vdouble&	d,  // Counts  "D_i" for data.
                    bool returnlog,
                    bool scale)   // Scale    p_j if true  
{
  int N = p.size(); // Number of sources (N)
  int M = d.size(); // Number of bins    (M)
  
  if ( a.size() != (unsigned int)N )
    {
      std::cout << "**Error - poissongamma - mis-match in number of sources\n"
                << "size(p): " << N << " differs from size(A) = " << a.size()
                << std::endl;
      exit(0);
    }

  if ( a[0].size() != (unsigned int)M )
    {
      std::cout << "**Error - poissongamma - mis-match in number of sources\n"
                << "size(d): " << M << " differs from size(A[0]) = " 
                << a[0].size()
                << std::endl;
      exit(0);
    }
  
  if ( M < 1 ) return -1.0;
  if ( ( N < 2 ) || ( N > MAXSRC ) ) return -2.0;
  
  if (YIELD.size() == 0) kit::initpoissongammamc(a);

  // Get total counts per source
  for ( int j = 0; j < N; j++ )
    {
      ns[j] = 0;
      for ( int i = 0; i < M; i++ ) ns[j] += a[j][i];
    }
  
  // Scale counts
  
  for ( int j = 0; j < N; j++ )
    {
      if ( scale )
        x[j] = p[j] / (ns[j]+M);
      else
        x[j] = p[j];
    }
  
  // Loop over each point in the swarm of points
  // that represents the prior
  
  int    index = 0;
  double prob;
  if ( returnlog )
    prob = 0.0;
  else
    prob = 1.0;
  for ( int i = 0; i < M; i++ )
    {
      double P = 0;
      for (int k = 0; k < TOTAL; k++)
        {
          // Compute Poisson parameter for ith bin
          double a = 0;
          for ( int j = 0; j < N; j++ )
            { 
              a += x[j] * YIELD[index];
              index++;
            }

          // Compute Poisson probability
          double q = exp(-a);
          int D = (int)d[i];
          if ( D > 0 ) for ( int n = 1; n <= D; n++ ) q *= a / n;

          P += q;
        }
      P /= TOTAL;
      if ( returnlog )
        prob += log(P);
      else
        prob *= P;
    }
  return prob;
}

