#include <iostream>
#include <string>
#include <stdexcept>

class Vector {
public:
  Vector(int n) : elem{new double[n]}, sz {n} {}
  double& operator[](int i) {
    if(i<0 || size()<i) throw std::out_of_range {"Vector::operator[]"};
    return elem[i];}
  int size() {return sz;}
  void stampa() {
    for(int i = 0; i < size() ; ++i)
      std::cout<<this->elem[i];}

private:
  double* elem;
  int sz;

};

int main() {
  try{
  Vector v(6);
  std::cout<<"vettore in posizione 8= "<<v[7];
  }
  catch(const std::out_of_range& oor) {std::cerr<<"errore"<<oor.what();}
}
