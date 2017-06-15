template <typename Iter, typename T>
void replace(Iter first, Iter last, const T& old_value, const T& new_value)
{
  for(; first != last; ++first){
    if((*first) == old_value)
      (*first) = new_value;
  }
}

// T deve essere dello stesso tipo degli elementi contenuti dalla sequenza
// first e last devono appartenere alla stessa sequenza
// Gli iteratori devono essere almeno forward-iterator perché si legge e scrive sulla sequenza
