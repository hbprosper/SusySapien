#ifndef ATEST_H
#define ATEST_H

#include <string>
#include <iostream>

class Aclass
{
public:
  Aclass() {}
  ~Aclass() {}
  void say() { std::cout << "I'm an Aclass thing" << std::endl; } 
};

class Bclass
{
public:
  Bclass() {}
  ~Bclass() {}
  void say() { std::cout << "I'm an Bclass thing" << std::endl; } 
};


class ATest
{
public:
  ATest(std::string message)
  : message_(message) {}
  ~ATest() {}

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

};



#endif 
