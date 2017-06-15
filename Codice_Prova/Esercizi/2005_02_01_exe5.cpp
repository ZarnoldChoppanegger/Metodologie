#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include <iostream>

template<typename T>
struct Not_starts_with{
  
public:
  Not_starts_with(const T& val) : value_{val} { }
  
  template<typename U>
  bool operator()(const U& x) { return *(std::begin(x)) != value_;}
  
private:
  T value_;
};

void f(const std::vector<std::string>& vs, const char c) {
  std::vector<std::string> tmp(vs);

  Not_starts_with<char> nsw(c);
  
  tmp.erase(std::remove_if(tmp.begin(), tmp.end(), nsw),
	          tmp.end());

  std::unique(tmp.begin(), tmp.end());

  for(auto x : tmp)
    std::cout << x << '\n';
}

void f_inv(const std::vector<std::string>& vs, const char c) {
  std::vector<std::string> tmp;

  std::copy(vs.crbegin(), vs.crend(),
	    std::back_inserter(tmp));

  f(tmp, c);
}

int main()
{
  std::vector<std::string> vs = {"ciao", "aaaaa", "bbbb", "wwww", "cccc"};

  f(vs, 'c');

  f_inv(vs, 'c');

  return 0;
}
