#include <iostream>

namespace N{
  class Int{
  public:
    Int(int x = 0) : m_int{x} { }
    Int& operator+=(const Int& y){
      m_int += y.m_int;
      return *this;
    }
    // non permette concatenazione, schifo...
    /*
    void operator+=(const Int& y){
      m_int += y.m_int;
    }
    */

    //preincremento
    Int& operator++(){
      ++m_int;
      return *this;
    }

    //postincemento, implementato mediante preincremento
    //il postincemento prima valuta il valore dell'oggetto
    //e poi lo incrementa!
    Int operator++(int){
      Int temp = *this;
      //std::cout << "temp = " << temp.m_int << '\n';
      ++*this;
      //std::cout << "temp = " << temp.m_int << '\n';
      return temp;
    }

    std::ostream& print(std::ostream& os) const {
      os << m_int;
      return os;
    }
  private:
    int m_int;
  };

  Int operator+(Int x, const Int&y){
    x += y;
    return x;
  }

  std::ostream& operator<<(std::ostream& out, const Int& x){
    return x.print(out);
  }
}

using N::Int;

int main()
{
  Int i1{1};
  Int i2 = 2;

  i1 += i2 += 3;

  std::cout << "i1 += i2 += 3 " << i1 << '\n';

  i1 = i1 + i2;

  std::cout << "i1 = i1 + i2 " << i1 << '\n';

  ++i1;

  std::cout << "++i1 " << i1 << '\n';

  i2++;

  std::cout << "i2++ " << i2 << '\n';

  return 0;
}
