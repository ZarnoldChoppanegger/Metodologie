#include "Razionale.hpp"
#include <iostream>
#include <cassert>

#define NDEBUG

Razionale::Razionale(int num, int den) :
  num_{num}, den_{den} {

    assert(check_inv());

    semplifica();

    assert(check_inv());
} 

Razionale Razionale::operator+() const {
  return *this;
}

Razionale Razionale::operator-() const {
  auto tmp = *this;
  tmp.num_ *= -1;

  assert(tmp.check_inv());
  
  return tmp;
}

Razionale& Razionale::operator++(){
  num_ += den_;

  this->semplifica();
  
  assert(check_inv());
  return *this;
}

Razionale& Razionale::operator--(){
  num_ -= den_;
  return *this;
}

Razionale Razionale::operator++(int){
  auto tmp = *this;
  ++(*this);
  return tmp;
}

Razionale Razionale::operator--(int){
  auto tmp = *this;
  --(*this);
  return tmp;
}

Razionale& Razionale::operator+=(const Razionale& y){
  assert(y.check_inv());

  num_ = (num_ * y.den_) + (den_ * y.num_);
  den_ *= y.den_;
  this->semplifica();
  
  assert(this->check_inv());
}

Razionale& Razionale::operator-=(const Razionale& y){
  assert(y.check_inv());

  num_ = (num_ * y.den_) - (den_ * y.num_);
  den_ *= y.den_;
  this->semplifica();

  assert(this->check_inv());
}

Razionale& Razionale::operator*=(const Razionale& y){
  assert(y.check_inv());

  num_ *= y.num_;
  den_ *= y.den_;
  this->semplifica();

  assert(this->check_inv());
}

Razionale& Razionale::operator/=(const Razionale& y){
  assert(y.check_inv());

  num_ /= y.num_;
  den_ /= y.den_;
  this->semplifica();

  assert(this->check_inv());
}

bool Razionale::operator==(const Razionale& y) const{
  assert(y.check_inv());

  if(num_ == y.num_ && den_ == y.den_) return true;

  return false;
}

bool Razionale::operator!=(const Razionale& y) const{
  return !((*this) == y);
}

std::ostream& Razionale::print(std::ostream& out) const{

  out << num_ << '/' << den_;

  return out;
}

void Razionale::semplifica(){

  int val = mcd(num_, den_);

  num_ /= val;
  den_ /= val;
}


bool Razionale::check_inv() const{

  if(den_ == 0)
    return false;

  if(num_ == 0)
    return (den_ == 1);

  if(den_ < 0)
    return false;

  if(!comprimi(abs(num_), den_))
    return false;

  return true;
}


int Razionale::mcd(int x, int y){
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


bool Razionale::comprimi(int x, int y){

  int val = mcd(x, y);
  
  if(val == 0 || val == y || val == 1)
    return true; //valori non ulteriormente semplificabili

  else return false;
}


int Razionale::abs(int x){
  
  if(x < 0) return -x;

  else return x;
}

Razionale operator+(Razionale x, const Razionale& y){
  x += y;

  return x;
}

Razionale operator-(Razionale x, const Razionale& y){
  x -= y;
  
  return x;
}

std::ostream& operator<<(std::ostream& out, const Razionale& x){
  x.print(out);

  return out;
}
