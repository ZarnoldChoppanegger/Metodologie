#include "Razionale.hpp"
#include <iostream>
#include <string>
#include <sstream>

/*bool test1(){
  Razionale r1;
  Razionale r2;

  return(r1 != r2);
}

bool test2(){
  Razionale r;

  std::stringstream ss;

  s << r;

  const std::string& s = ss.str();

  return (s != 0);
}
*/

bool test3(){
  Razionale r1{-13, 20};
  auto r2 = r1;
  r1 += 1;
  ++r2;

  std::cout << r1 << ' ' << r2 << '\n';
  
  return (r1 != r2); 
}

int main()
{
  //std::cout << "Test 1: " << test1();
  //std::cout << "Test 2: " << test2();
  std::cout << "Test 3: " << test3();
  
  return 0;
}
