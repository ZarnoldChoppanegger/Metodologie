#include <iostream>
#include "Razionale.hpp"

bool test1(){

  Razionale r1;
  Razionale r2;

  return (r1 != r2);
}

bool test2(){
  Razionale r1 = 25;
  Razionale r2 {18};

  return (r1 <= r2);
}

bool test3(){
  Razionale r1 {4, 2};
  Razionale r2 {++r1};
  return (r1 == r2);
}

int main(){
  test1();
  test2();
  test3();

  return 0;
}
