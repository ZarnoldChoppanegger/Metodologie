#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <vector>
#include <utility>
#include <functional>

template<typename FwdIter, typename BinPred>
FwdIter max_elemento(FwdIter first, FwdIter last, BinPred pred)
{
  if(first != last) {
    FwdIter max = first;
    for(; first != last; ++first){
      if(pred(*max, *first))
	max = first;
    }
    return max;
  }
  return last;
}

int main()
{
  std::istream_iterator<std::string> b_in(std::cin);
  std::istream_iterator<std::string> e_in;

  std::vector<std::string> vs;

  //auto bi = std::back_inserter(vs);

  std::copy(b_in, e_in,
	    std::back_inserter(vs));

  // since C++14
  auto max_string = max_elemento(std::begin(vs), std::end(vs),
			     [&](auto a, auto b) { return a < b;});

  // until C++14
  auto max_string = max_elemento(std::begin(vs), std::end(vs),
			         std::less<std::string>());

  std::cout << "Stringa piú grande (ordine lessicografico): " << *max_string << '\n';

  // C++14
  max_string = max_elemento(std::begin(vs), std::end(vs),
			    [&](auto a, auto b) { return a.size() < b.size();});

  std::cout << "Stringa piú lunga: " << *max_string << '\n';

  return 0;
}
