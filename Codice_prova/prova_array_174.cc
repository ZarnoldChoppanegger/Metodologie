#include <iostream>

void foo(int n){
  int v1[n];

  for(int i = 0; i < n; ++i)
    {
      v1[i] = i;
      std::cout << v1[i] << " ";
    }
}

int main()
{
  constexpr int n = 10;

  foo(n);

  return 0;
}
  
