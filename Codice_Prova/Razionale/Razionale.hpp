#ifndef RAZIONALE_HH
#define RAZIONALE_HH

#include <iosfwd>

class Razionale{

public:
  //costruttore di default
  /*constexpr*/ Razionale(int num = 0, int den = 1);

  //rule of five, costruttore e assegnamento per copia e spostamento
  //vanno bene anche quelli di default
  /*constexpr*/ explicit Razionale(const Razionale&) = default;
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

  // funzione di stampa
  std::ostream& print(std::ostream& os) const;

  ~Razionale() { };
  
private:
  int num_;
  int den_;

  // calcola mcd tra due numeri
  static int gcd(int x, int y);

  // calcola il valore assoluto di x
  static int abs(int x);

  // testa l'invariante della classe:
  // den != 0
  // il numero deve essere normalizzato
  bool  test_invariant() const;

  // normalizza il numero
  void normalize();

  // vedi se x/y sono in forma normalizzata
  static bool is_normalize(int x, int y);
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
