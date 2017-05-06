#include "Razionale.hpp"
#include <iostream>
#include <cassert>

#define NDEBUG

Razionale::Razionale(int num, int den)
  :  num_{num}, den_{den}
  {
    assert(den_ == 0);
    normalize();
    assert(test_invariant());
   }

Razionale Razionale::operator+() const {
  return (*this);
}

Razionale Razionale::operator-() const {
  int den = this->den_ * -1;
  int num = this->num_;
  Razionale tmp{den, num};
  return tmp;
}

//prefissi
Razionale& Razionale::operator++() {
  num_ += den_;
  normalize();
  assert(test_invariant());
  return *this;
}

Razionale& Razionale::operator--() {
  num_ -= den_;
  normalize();
  assert(test_invariant());
  return *this;
}

//postfissi
Razionale Razionale::operator++(int x) {
  Razionale tmp{*this};
  normalize();
  assert(test_invariant());
  ++(*this);
  return tmp;
}

Razionale Razionale::operator--(int x) {
  Razionale tmp = *this;
  --(*this);
  normalize();
  assert(test_invariant());
  return tmp;
}

//operatori con assegnamento
Razionale& Razionale::operator+=(const Razionale& y) {
  assert(y.test_invariant());
  num_ = (den_ * y.num_) + (num_ * y.den_);
  den_ *= y.den_;
  normalize();
  assert(test_invariant());
  return *this;
}

Razionale& Razionale::operator-=(const Razionale& y) {
  assert(y.test_invariant());
  num_ = (den_ * y.num_) * (num_ * y.den_);
  den_ *= y.den_;
  normalize();
  assert(test_invariant());
  return *this;
}

Razionale& Razionale::operator*=(const Razionale& y) {
  assert(y.test_invariant());
  num_ *= y.num_;
  den_ *= y.den_;
  normalize();
  assert(test_invariant());
  return *this;
}

Razionale& Razionale::operator/=(const Razionale& y) {
  assert(y.test_invariant());
  num_ /= y.num_;
  den_ /= y.den_;
  normalize();
  assert(test_invariant());
  return *this;
}

std::ostream& print(std::ostream& os) const {
  os << num_ << '/' << den_;
  return os;
}

int Razionale::gcd(int x, int y) {
  x = abs(x);
  y = abs(y);
  //indefinito ma lo si fa per comodità
  if( x == 0 && y == 0) return 0;
  
  else if(x == 0 && y != 0) return abs(y);


  else if(!(x % 2)) {

    if(!(y % 2)) return 2 * mcd(x / 2, y / 2);

    else if(!(y % 1)) return mcd(x / 2, y);
  }

  else if(!(x % 1)) {

    if(!(y % 2)) return mcd(x , y / 2);

    else if(!(y % 1)) {
      if(x >= y) return mcd((x - y) / 2, y);

      else if(x < y) return mcd((y - x) / 2, x);
    }
  }
}

int Razionale::abs(int x) {
  return (x < 0 ? -x : x);
}

bool Razionale::test_invariant() const {

  if (den_ <= 0) return false;

  else if(num_ == 0) return (den_ == 1);

  else if(!is_normalize(abs(num_), den_)) return false;

  return true;
}

bool Razionale::is_normalize(int x, int y) {
  int val = mcd(x, y);
  
  if(val == 0 || val == y || val == 1)
    return true; //valori non ulteriormente semplificabili

  else return false;
}
// funzioni entro la classe finiscono qui

// operatori di confronto
bool Razionale::operator==(const Razionale& x, const Razionale& y) {
  if (x.num_ == y.num_ && x.den_ == y.den_) return true;
  return false;
}

bool Razionale::operator!=(const Razionale& x, const Razionale& y) {
  return !(x == y);
}

bool Razionale::operator<(const Razionale& x, const Razionale& y) {
  float a = x.num_ / x.den_;
  float b = y.num_ / y.den_;

  return a < b;
}


bool Razionale::operator>(const Razionale& x, const Razionale& y) {
  return !(x < y);
}

bool Razionale::operator<=(const Razionale& x, const Razionale& y) {
  return (x == y || x < y);
}

bool Razionale::operator>=(const Razionale& x, const Razionale& y) {
  return (x == y || x > y);
}

Razionale Razionale::operator+(Razional x, const Razionale& y) {
  x += y;
  return x;
}

Razionale Razionale::operator-(Razionale x, const Razionale& y) {
  x -= y;
  return x;
}

Razionale Razionale::operator*(Razionale x, const Razionale& y) {
  x *= y;
  return x;
}

Razionale Razionale::operator/(Razionale x, const Razionale& y) {
  x /= y;
  return x;
}

std::ostream& operator<<(std::ostream& os, const Razionale& r_out) {
  return r_out.print(os);
}
