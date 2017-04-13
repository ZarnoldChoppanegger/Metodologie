/* In questo codice si prova a capire quando
 * viene chiata una funzione costante in caso
 * di overload */

#include <iostream>

namespace N{
  class C{
  public:
    C(int x = 0);

    //funzione non costante
    int& get();
    //funzione costante
    //funzioni costanti hanno senso se si ritorna un riferimento (o puntatore) e non una copia
    const int& get() const;

  private:
    int m_x;
  };

  C::C(int x) : m_x(x) {}

  int& C::get() {
    std::cout << "Non const called\n";
    return m_x;
  }
  
  const int& C::get() const {
    std::cout << "Const called\n";
    return m_x;
  }
}//namespace N



int main()
{
  N::C c1(1);
  const N::C c2(2);

  //anche se assegnato a una constante viene chiamata la funzione non costante
  const int cx = c1.get();
  //la funzione costante viene chiamata se l'oggetto è costante
  int vx = c2.get();

  std::cout << c2.get() << "  " << cx << "  " << ++vx << '\n';
  
  return 0;
}

/* La risoluzione dell'overload non avviene guardando il tipo di ritorno
 * ma solo il nome della funzione e il tipo e numero dei parametri.
 * In questo caso la chiamata di funzione quindi può essere decisa solo
 * in base all'oggetto chiamante. Quindi se l'oggetto è costante, verrà
 * chiamata la funzione costante e viceversa.
 */
