#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>

// Scrivere l’implementazione dell’algoritmo della STL remove_copy_if, che ha come parametri una
// sequenza di input ed una sequenza di output, specificate mediante iteratori, ed un predicato unario.
// L’algoritmo copia nella sequenza di output, mantenendone l’ordine, gli elementi della sequenza di
// input che non soddisfano il predicato; l’algoritmo restituisce l’iteratore corrispondente alla fine
// della sequenza di output.

template <typename IterIn, typename IterOut, typename UnaryPredicate>
IterOut my_remove_copy_if(IterIn first, IterIn last, IterOut out, UnaryPredicate pred)
{
  for(; first != last; ++first) {
    if(!pred(*first)) {
      (*out) = (*first);
      ++out;
    }
  }
  return out;
}

// Utilizzando l’algoritmo precedente, implementare l’analogo algoritmo remove_if, che invece di scri-
// vere su di una sequenza di output modifica direttamente la sequenza di input. Motivare brevemente
// quali siano le categorie di iteratori utilizzabili in ognuno dei due algoritmi.

template <typename IterIn, typename UnaryPredicate>
IterIn my_remove_if(IterIn first, IterIn last, UnaryPredicate pred)
{
  first = std::find_if(first, last, pred);

  if(first != last) {
    IterIn i = first;

    last = remove_copy_if(i, last, first, pred);
  }

  return last;    
}

// Nel primo algoritmo gli iteratori della prima sequenza devono essere almeno InputIterator
// mentre per la seconda sequenza deve essere almeno un OutputIterator
// Nel secondo algoritmo gli iteratori devono essere almeno dei ForwardIterator poiché vi sono
// sia operazioni di lettura che scrittura.

template <typename T>
struct Equal_To {
  using value_type = T;
XF
  Equal_To(value_type x) : val_{x} {}

  bool operator()(value_type x) { return x == val_; }

private:
  T val_;
};

template <typename Iter>
void printAll(Iter first, Iter last)
{
  for(; first != last; ++first)
    std::cout << *first << ' ';
  std::cout << '\n';
}

int main()
{
  std::vector<int> v1 = {1,5,4,3,5,6};

  Equal_To<int> eq_to{5};

  v1.erase(my_remove_if(std::begin(v1), std::end(v1), eq_to),
	      std::end(v1));

  printAll(std::begin(v1), std::end(v1));

  return 0;
}
