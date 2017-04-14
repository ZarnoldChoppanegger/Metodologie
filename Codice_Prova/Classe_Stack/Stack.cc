#include "Stack.hh"
#include <iostream>

Stack::value_type*
Stack::make_copy(const value_type* src,
                 size_type src_size, size_type dst_capacity) {
  assert(dst_capacity >= src_size);
  // Allochiamo la risorsa.
  value_type* dst = new value_type[dst_capacity];
  try {
    // Proviamo a copiare gli elementi da `src' a `dest'.
    for (size_type i = 0; i < src_size; ++i)
      dst[i] = src[i];
  }
  catch (...) {
    // Qualcosa e` andato storto.
    // Deallochiamo la risorsa ...
    delete[] dst;
    // ... e rilanciamo l'eccezione per essere neutrali.
    throw;
  }
  return dst;
}

bool
Stack::OK() const {
  if (capacity_ == 0) {
#ifndef NDEBUG
    std::cerr << "Lo Stack e` dichiarato avere una capacita` nulla!"
	      << std::endl;
#endif // NDEBUG
    return false;
  }
  if (size_ > capacity_) {
#ifndef NDEBUG
    std::cerr << "Lo Stack e` dichiarato avere una dimensione "
	      << "superiore alla capacita`!" << std::endl;
#endif // NDEBUG
    return false;
  }
  if (vec_ == 0) {
#ifndef NDEBUG
    std::cerr << "Non vi sono risorse allocate per lo Stack!"
	      << std::endl;
#endif // NDEBUG
    return false;
  }
  // Tutto ok.
  return true;
}
