#ifndef ATEST_H
#define ATEST_H

#include <string>
#include <iostream>

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
  ~Aclass() {}
  void say() { std::cout << "I'm an Aclass thing" << std::endl; } 
  double value(){return val++;}
private:
  double val;
};

class Bclass
{
public:
  Bclass() : val(24) {}
  ~Bclass() {}
  void say() { std::cout << "I'm a Bclass thing" << std::endl; } 
  double value(int x){val += x; return val;}

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
      a_(&a),
      b_(b),
      count_(0)
  {}

  ~ATest() {}


  Aclass* ptrToA() const { return a_; }

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
//     std::cout << "BEGIN(value)" << std::endl;
//     std::cout << "  index  <" << i << ">" << std::endl;
//     std::cout << "  string <" << s << ">" << std::endl;
//     std::cout << "  double <" << d << ">" << std::endl;
//     std::cout << "END(value)" << std::endl;
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
