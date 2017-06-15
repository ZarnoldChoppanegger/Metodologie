#include <iostream>
#include <iterator>
#include <vector>
#include <string>

double media(int x, int y)
{
  return (x + y) / 2.0;
}

template<typename IterIn1, typename IterIn2,
	 typename IterOut, typename BinPred>
IterIn1 transform(IterIn1 first1, IterIn1 last1, IterIn2 first2,
		  IterOut out, BinPred bin_pred)
{
  for(; first1 != last1; ++first1, ++first2) {
    *out = std::to_string(bin_pred(*first1, *first2));
    ++out;
  }
  return first1;
}


int main() {

  std::vector<int> vi1 {1,2,3,4,5,6,7,8,9};
  std::vector<int> vi2 {9,8,7,6,5,4,3,2,1};

  std::ostream_iterator<std::string> out(std::cout, "\n");
  
  transform(vi1.begin(), vi1.end(), vi2.begin(),
	    out, media);
	    
  return 0;
}
