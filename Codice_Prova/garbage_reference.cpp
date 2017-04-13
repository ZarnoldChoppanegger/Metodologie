#include <iostream>

int& foo(){
  //int x = 7;
  int* x = new int(7);
  return *x;
}

int main()
{
  int y = foo();

  std::cout << y << '\n';

  return 0;
}
