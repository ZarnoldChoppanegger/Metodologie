/*
  Leggere da std::cin tutte le stringe
  e stamparle in std::cout ordinate
  e senza ripetizioni
*/

#include <vector>
#include <deque>
#include <stack>
#include <iostream>
#include <algorithm>
#include <iterator>

int main()
{
  std::istream_iterator<std::string> cin_begin(std::cin);
  // iteratore di end-of-stream
  std::istream_iterator<std::string> cin_end;

  // vettore nel quale inserire l'input
  // e effettuare le operazioni volute
  //std::vector<std::string> vs;

  std::deque<std::string> vs;
  // inserire l'input nel vector
  std::copy(cin_begin, cin_end, std::back_inserter(vs));

  // ordinare gli elementi
  std::sort(std::begin(vs), std::end(vs));

  // eliminare le ripetizioni
  vs.erase( std::unique(std::begin(vs), std::end(vs)),
  	    std::end(vs));

  // stampare il contenuto del vettore su cout
  std::ostream_iterator<std::string> cout_begin(std::cout, "\n");
  std::copy(std::begin(vs), std::end(vs), cout_begin);

  return 0;
}
