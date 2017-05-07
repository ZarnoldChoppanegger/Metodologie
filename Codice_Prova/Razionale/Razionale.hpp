#ifndef RAZIONALE_HH
#define RAZIONALE_HH

#include <iosfwd>

class Razionale{

public:
  //costruttore di default
  Razionale(long num = 0, long den = 1);

  //rule of five, costruttore e assegnamento per copia e spostamento
  //vanno bene anche quelli di default
  Razionale(const Razionale&) = default;
  Razionale(Razionale&&) = default;
  Razionale& operator=(const Razionale&) = default;
  Razionale& operator=(Razionale&&) = default;

  //operatori aritmetici unari
  Razionale operator+() const;
  Razionale operator-() const;

  //incremento e decremento prefissi
  Razionale& operator++();
  Razionale& operator--();

  //incremento e decremento postfissi
  Razionale operator++(int x = 0);
  Razionale operator--(int x = 0);

  //operatori binari con assegnamento
  Razionale& operator+=(const Razionale& y);
  Razionale& operator-=(const Razionale& y);
  Razionale& operator*=(const Razionale& y);
  Razionale& operator/=(const Razionale& y);

  long numerator() const;
  long denominator() const;

  // funzione di stampa
  std::ostream& print(std::ostream& os) const;

  ~Razionale() { };
  
private:
  long num_;
  long den_;

  // calcola mcd tra due numeri
  static long gcd(long x, long y);

  // calcola il valore assoluto di x
  static long abs(long x);

  // testa l'invariante della classe:
  // den != 0
  // il numero deve essere normalizzato
  bool  test_invariant() const;

  // normalizza il numero
  void normalize();

  // vedi se x/y sono in forma normalizzata
  static bool is_normalize(long x, long y);
};

//operatori di confronto
bool operator==(const Razionale& x, const Razionale& y);
bool operator!=(const Razionale& x, const Razionale& y);
bool operator<(const Razionale& x, const Razionale& y);
bool operator>(const Razionale& x, const Razionale& y);
bool operator<=(const Razionale& x, const Razionale& y);
bool operator>=(const Razionale& x, const Razionale& y);


//operatori aritmetici binari
Razionale operator+(Razionale x, const Razionale& y);
Razionale operator-(Razionale x, const Razionale& y);
Razionale operator*(Razionale x, const Razionale& y);
Razionale operator/(Razionale x, const Razionale& y);

//operatore di stampa
std::ostream& operator<<(std::ostream& os, const Razionale& r_out);

#endif //RAZIONALE_HH
