#ifndef FUNCTIONMEMBER_H
#define FUNCTIONMEMBER_H
//-----------------------------------------------------------------------------
//
// Package:    PhysicsTools/Mkntuple
//             FunctionMember.h
// Description: Recursively invoke methods:
//              object(.|->)method1(..)[(.|->)method2(..)] etc.
//           
// Original Author:  Harrison B. Prosper
//         Created:  Tue Dec  8 15:40:26 CET 2009
// $Id: FunctionMember.h,v 1.2 2010/09/19 14:09:53 prosper Exp $
//
//-----------------------------------------------------------------------------
#include <vector>
#include <string>
#include "Reflex/Member.h"
#include "PhysicsTools/Mkntuple/interface/rfx.h"
//-----------------------------------------------------------------------------
/// Describe attributes of a function or data member.
struct FunctionDescriptor
{
  std::string        classname;          /// Name of declaring class
  std::string        expression;         /// Function expression
  Reflex::Member     method;             /// Model of function
  std::vector<rfx::ValueThing*>  values; /// Argument descriptors
  std::vector<void*> args;               /// Arguments
  Reflex::Type       returntype;         /// Function return type

  Reflex::Object     scratch;        /// Memory for simple return types
  void*              memory;         /// Memory for objects returned by value 
  bool               deallocate;     /// True if we need to deallocate memory

  bool               datamember;     /// True if this is a data member
  bool               simple;         /// True if return type is simple
  bool               pointer;        /// True if return type is a pointer
  bool               reference;      /// True if return type is a reference
  bool               smartpointer;   /// True if return type is a smart pointer

  Reflex::Member     isAvailable;        /// Model isAvailable()
  Reflex::Member     isNull;             /// Model isNull()
  Reflex::Member     get;                /// Model get()
};

/** Model a function member (a method) or a data member of a C++ class.
    Given the fully scoped name of a class (that is, the name together with
    any namespaces) and the expression for a simple or compound method, this
    class can be used to invoke the method. However, there is an important 
    restriction: the method must return a simple type (such as <i>float</i>,
    <i>double</i>, <i>int</i>, <i>std::string</i> etc.) and its arguments must 
    likewise comprise simple types. The code uses the Root Reflex classes.
    The interface is designed for a non-expert. For compound methods that 
    return pointers in intermediate steps, a pointer could of course be
    null. If that is the case, a warning is issued using the MessageLogger.
    ``Smart" pointers that define the methods <i>isAvailable()</i>, 
    <i>isNull()</i> and <i>get()</i> are also handled appropriately.
    <p>
    <i>Examples of simple and compound methods:</i>
    - Simple method - pt()
    - Compound method - gsfTrack()->numberOfValidHits()
    <p>
    <i>Example</i>
    The code fragment
    \code
    FunctionMember f("pat::Muon", "gsfTrack()->numberOfValidHits()");
    :  :
    int nhits = (int)f(muon);
    \endcode
    is equvalent to the direct call
    \code
    int nhits = muon.gsfTrack()->numberOfValidHits();
    \endcode
 */
class FunctionMember
{
public:
  ///
  FunctionMember();
    
  /** Model a function member or a data member.
      @param classname - name of class containing method to be invoked
      @param expression - method to be called (given without the return type)
   */
  FunctionMember(std::string classname, std::string expression);

  virtual ~FunctionMember();

   
  /** Call the method.
      @param address - address of object for which the method is to be called
      @return address of returned value
   */
  void*  invoke(void* address);

   /** Call the method.
      @param address - address of object for which the method is to be called
      @return the return value cast to a double
   */
  double operator()(void* address);

  /// String representation of decoded method.
  std::string  str();

  ///
  std::ostream& operator<<(std::ostream& os);

private:
  std::string classname_;
  std::string expression_;
  std::vector<FunctionDescriptor> fd_;
  int debug_;
};

//-----------------------------------------------------------------------------
// Test classes
//-----------------------------------------------------------------------------
template <class X>
class Atemp
{
public:
  Atemp() {}
  ~Atemp() {}

  void speak() { x_.say(); }

private:
  X x_;
};

class Aclass
{
public:
  Aclass() : val(0) {}
  Aclass(const Aclass& x) {val = x.val; }
  ~Aclass() {}
  void say() { std::cout << "I'm an Aclass thing" << std::endl; } 
  void set(double x)  { val = x; }
  double avalue;
  double value(){return val; }
private:
  double val;
};

class Bclass
{
public:
  Bclass() : val(24) {}
  ~Bclass() {}
  void say() { std::cout << "I'm a Bclass thing" << std::endl; } 
  void set(double x) {val = x;}
  double value(){return val;}

private:
  double val;
};


class ATestBase
{
public:
  ATestBase() : val(0) {}
  virtual ~ATestBase() {}
  virtual Aclass* ptrToA() const
  {
    return (Aclass*)0;
  }

  virtual double value() {return val += 3;}
private:
  double val;
  
};

class BTest : public Atemp<Bclass>
{
public:
  BTest() : Atemp<Bclass>() {}
  ~BTest() {}
  void hello() {std::cout << "hello from BTest" << std::endl;}
};

class ATest : public ATestBase
{
public:
  ATest(std::string message)
    : ATestBase(),
      message_(message) {}

  ATest(Aclass& a, Bclass& b)
    : ATestBase(),
      ptrToB(&b),
      a_(&a),
      b_(b),
      count_(0)
  {}

  ~ATest() {}

  double  avalue;

  Aclass* ptrToA() const { return a_; }
  Aclass& refToA() const { return *a_; }
  Aclass  toA()    const { return *a_; }

  Bclass* ptrToB;

  Bclass& getB() 
  {
    b_.say();
    return b_; 
  }

  const Bclass  getB(std::string s) const 
  { 
    std::cout << "getB: \"" << s << "\"" << std::endl;
    return b_; 
  }

  double value(int i, std::string s, double d) 
  {
    count_++;
    std::cout << "\tcount: " << count_ << std::endl;
    return count_; 
  }

  double method1() const
  {
    std::cout << "method1: " << message_ << std::endl;
    return 1;
  }
  double method2(std::string message) const 
  {
    std::cout << "method2: " << message_ << ": " << message << std::endl;
    return 2;
  }


  double method() const
  {
    return 0;
  }

  template <typename X>
  double method() const
  {
    X o;
    o.say();
    return 0;
  }

  template <typename X, typename Y>
  double method() const
  {
    X x;
    x.say();
    Y y;
    y.say();
    return 0;
  }

private:
  std::string message_;
  Aclass* a_;
  Bclass  b_;
  int count_;
};


#endif
