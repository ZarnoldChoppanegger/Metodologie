#include "Razionale.hpp"
#include <iostream>
//#define NDEBUG
#include <cassert>

Razionale::Razionale(long num, long den)
  :  num_{num}, den_{den}
  {
    assert(den_ != 0);
    normalize();
    assert(test_invariant());
   }

Razionale Razionale::operator+() const {
  return (*this);
}

Razionale Razionale::operator-() const {
  long den = this->den_ * -1;
  long num = this->num_;
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
Razionale Razionale::operator++(int) {
  Razionale tmp{*this};
  normalize();
  assert(test_invariant());
  ++(*this);
  return tmp;
}

Razionale Razionale::operator--(int) {
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
  num_ = (den_ * y.num_) - (num_ * y.den_);
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
  num_ *= y.den_;
  den_ *= y.num_;
  normalize();
  assert(test_invariant());
  return *this;
}

long Razionale::numerator() const {
  return num_;
}

long Razionale::denominator() const {
  return den_;
}

std::ostream& Razionale::print(std::ostream& os) const {
  os << num_ << '/' << den_;
  return os;
}

long Razionale::gcd(long x, long y) {
  x = abs(x);
  y = abs(y);
  //indefinito ma lo si fa per comodità
  if( x == 0 && y == 0) return 0;
  
  else if(x == 0 && y != 0) return abs(y);


  else if(!(x % 2)) {

    if(!(y % 2)) return 2 * gcd(x / 2, y / 2);

    else if(!(y % 1)) return gcd(x / 2, y);
  }

  else if(!(x % 1)) {

    if(!(y % 2)) return gcd(x , y / 2);

    else if(!(y % 1)) {
      if(x >= y) return gcd((x - y) / 2, y);

      else if(x < y) return gcd((y - x) / 2, x);
    }
  }
}

long Razionale::abs(long x) {
  return (x < 0 ? -x : x);
}

bool Razionale::test_invariant() const {

  if (den_ <= 0) return false;

  else if(num_ == 0) return (den_ == 1);

  else if(!is_normalize(abs(num_), den_)) return false;

  return true;
}

void Razionale::normalize() {
  long val = gcd(abs(num_), den_);

  if(val == 1) return;
  
  num_ /= val;
  den_ /= val;

  assert(test_invariant());
}

bool Razionale::is_normalize(long x, long y) {
  long val = gcd(x, y);
  
  if(val == 0 || val == y || val == 1)
    return true; //valori non ulteriormente semplificabili

  else return false;
}
// funzioni entro la classe finiscono qui

// operatori di confronto
bool operator==(const Razionale& x, const Razionale& y) {
  if (x.numerator() == y.numerator() &&
      x.denominator() == y.denominator()) return true;
  return false;
}

bool operator!=(const Razionale& x, const Razionale& y) {
  return !(x == y);
}

bool operator<(const Razionale& x, const Razionale& y) {
  float a = x.numerator() / x.denominator();
  float b = y.numerator() / y.denominator();

  return a < b;
}


bool operator>(const Razionale& x, const Razionale& y) {
  return !(x < y);
}

bool operator<=(const Razionale& x, const Razionale& y) {
  return (x == y || x < y);
}

bool operator>=(const Razionale& x, const Razionale& y) {
  return (x == y || x > y);
}

Razionale operator+(Razionale x, const Razionale& y) {
  x += y;
  return x;
}

Razionale operator-(Razionale x, const Razionale& y) {
  x -= y;
  return x;
}

Razionale operator*(Razionale x, const Razionale& y) {
  x *= y;
  return x;
}

Razionale operator/(Razionale x, const Razionale& y) {
  x /= y;
  return x;
}

std::ostream& operator<<(std::ostream& os, const Razionale& r_out) {
  return r_out.print(os);
}
