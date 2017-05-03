#ifndef RAZIONALE_HPP
#define RAZIONALE_HPP

#include <iosfwd>

class Razionale{

public:
  Razionale(int num = 0, int den = 1);

  //costruttori e assegnameto per copia e spostamento
  Razionale(const Razionale&) = default;
  Razionale& operator=(const Razionale&) = default;
  Razionale(Razionale&& ) = default;
  Razionale& operator=(Razionale&& ) = default;

  //operatori aritmetici unari
  Razionale operator+() const;
  Razionale operator-() const;

  //incremento e decremento prefissi
  Razionale& operator++();
  Razionale& operator--();

  //incremento e decremento postfissi
  Razionale operator++(int =0);
  Razionale operator--(int =0);

  //operatori binari con assegnamento
  Razionale& operator+=(const Razionale& y);
  Razionale& operator-=(const Razionale& y);
  Razionale& operator*=(const Razionale& y);
  Razionale& operator/=(const Razionale& y);

  //operatori di confronto
  bool operator==(const Razionale& y) const;
  bool operator!=(const Razionale& y) const;
  bool operator<(const Razionale& y) const;
  bool operator>(const Razionale& y) const;
  bool operator<=(const Razionale& y) const;
  bool operator>=(const Razionale& y) const;

  //metodo stampa
  std::ostream& print(std::ostream& out) const;

  //distruttore
  ~Razionale(){ };
  
private:
  int num_;
  int den_;
  void semplifica();
  bool check_inv() const;
public:
  static int mcd(int x, int y);
  //dice se i due numeri sono semplificabili tra loro
  static bool comprimi(int x, int y);
  static int abs(int x);
};

//operatori binari senza assegnamento
Razionale operator+(Razionale x, const Razionale& y);
Razionale operator-(Razionale x, const Razionale& y);
std::ostream& operator<<(std::ostream& out, const Razionale& x);
  
#endif //RAZIONALE_HPP
