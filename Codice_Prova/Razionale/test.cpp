#include <iostream>
#include "Razionale.hpp"

bool test1(){

  Razionale r1;
  Razionale r2;

  std::cout << r1 << ' ' << r2 << '\n';
  return (r1 != r2);
}

bool test2(){
  Razionale r1 = 25;
  Razionale r2 {18};

  std::cout << r1 << ' ' << r2 << '\n';
  return (r1 <= r2);
}

bool test3(){
  Razionale r1 {4, 2};
  Razionale r2 {r1 + 1};

  std::cout << r1 << ' ' << r2 << '\n';
  return (r1 == r2);
}

bool test4(){
  Razionale r1;

  // oltre 44 fa wrap around
  // denominatore diventa < 0
  // rompe invariante
  for(long i = 0; i >= -44; --i)
    r1 += i + Razionale{i, (i * -1) + 1};

  std::cout << r1 << '\n';

  return r1 == 0;
}

int main(){
  if(!test1()) std::cout << "test1 OK\n";
  if(!test2()) std::cout << "test2 OK\n";
  if(!test3()) std::cout << "test3 OK\n";
  if(!test4()) std::cout << "test4 OK\n";

  return 0;
}
