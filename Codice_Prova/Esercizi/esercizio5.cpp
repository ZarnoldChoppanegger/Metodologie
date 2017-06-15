#include <algorithm>
#include <iterator>
#include <string>
#include <iostream>
#include <vector>
// 1)
/*
template<typename iter, typename out, typename Unarypred>
iter remove_copy_if(iter first, iter last, out result, Unarypred pred)
{
  for(; first != last ; ++first)
    {
      if (!pred(*first)){
	
	*result= *first;
	++result;
      }
    }
  return result;
}


struct pred{
  bool operator() (const int i)const {
    return i<6;}
};


int main()
{
  pred p;
  std::vector<int> vi {1,3,5,9,7};
    std::remove_copy_if(vi.begin(), vi.end(),
                     std::ostream_iterator<int>(std::cout), p);
    std::cout << '\n';
}

*/

//2)
template<typename iter, typename Unarypred>
iter remove_if(iter first, iter last, Unarypred pred)
{
  first = std::find_if(first,last,pred);
  if(first != last )
    {
    iter i= first;
    last = remove_copy_if(i,last,first,pred);
    }
  return last;
}


struct pred{
  bool operator() (const int i)const {
    if ( i<6)
      return true;
    return false;}
};




int main()
{
  pred p;
  std::vector<int> vi {1,3,5,9,7};
    std::remove_if(vi.begin(), vi.end(), p);
    std::cout << '\n';
}



/* nel 2 vengono utilizzati dei foreward iterator. Rimozione avviene spostando gli elementi nell'intervallo in modo tale che gli elementi da cancellare vengono sovrascritti.*/
