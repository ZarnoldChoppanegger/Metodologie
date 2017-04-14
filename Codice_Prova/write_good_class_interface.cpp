#include <iostream>

/* Questo codice è basato su questo articolo
 * ( https://herbsutter.com/2013/05/20/gotw-4-class-mechanics/ )
 * nel quale vengono indicate le linee guida principali per scrivere
 * una buona interfaccia per una classe, questa infatti deve essere
 * costruita in tal modo che l'utente sia portato in modo naturale
 * ad usarla correttamente, e quindi che sia difficile scrivere qualcosa
 * di sbagliato inavvertitamente, così l'utente è protetto da sorprese non volute
 */

namespace N{
  
  class Int{
  public:

    
    /* Manca il COSTRUTTORE di default e questo rende l'uso della classe
     * fastidioso. Le soluzioni sono quelle di inizializzare i parametri
     * ai valori di default, così il costruttore può essere invocato senza parametri,
     * oppure forzare il compilatore ad introdurre anche il costruttore di default
     * Int() = default;
     */
    Int(int x = 0) : m_int{x} { }

    
    /* OPERATORE += il quale ritorna il riferimeto
     * all'oggetto modificato, questo serve per poter concatenare le operazioni.
     * L'oggetto che viene passato deve essere solo letto, quindi viene passato come
     * riferimento a costante (per motivi di efficienza).
     */
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
    

    /* Il PREINCREMENTO deve ritornare un riferimento, permettendo a chi usa la classe
     * di usare l'operatore in modo più intuitivo ed efficiente
     */
    Int& operator++(){
      ++m_int;
      return *this;
    }
    

    /* POSTINCEMENTO, implementato mediante preincremento
     * il postincemento prima valuta il valore dell'oggetto
     * e poi lo incrementa!
     */
    Int operator++(int){
      Int temp = *this;
      ++*this;
      return temp;
    }


    /* Funzione membro costante che permette di implementare la stampa.
     * Guarda operator<< per capire meglio.
     */
    std::ostream& print(std::ostream& os) const {
      os << m_int;
      return os;
    }

    
  private:
    int m_int;
  };

  
  /* Questo operatore non va definito come funzione membro 
   * (nessun operatore binario va definito come funzione membro)
   * poichè l'operatore non funzionerà in modo così naturale come l'utente si aspetterebbe.
   * Si avrà asimmetria nell'uso nel caso si sommino due tipi diversi, ad esempio:
   * (double + complex) ma non possibile (complex + double).
   * Questo operatore va implementato mediante l'uso dell'operatore +=, infatti è sempre
   * preferibile scrivere a op= b piuttosto che a = a op b. È più pulito ed efficiente, poichè
   * va ad operare direttamente sull'oggetto di sinistra e ritorna solo un riferimento.
   * Notare come l'operando di sinistra (da modificare), sia passato per valore, mentre l'operando
   * di destra per riferimento costante poichè non è necessario modificarlo ma solo legerlo.
   */
  Int operator+(Int x, const Int&y){
    x += y;
    return x;
  }


  /* OPERATORE << non deve essere una funzione membro, altrimenti la chiamata avverrebbe in questo
   * modo (my_obj << cout), ed è lo stesso motivo per il quale anche l'operatore binario + non viene
   * implementato come funzione membro. Lo stream viene passato per riferimento, poichè bisogna
   * modificarlo e anche eventualmente poterlo concatenare. L'oggetto da stampare invece viene passato
   * solo per riferimento costante, poichè biogna solo stamparlo.
   * Questo operatore viene implementato per mezzo della funzione membro print() 
   * dell'oggeto da stampare. Normalmente le funzioni che non sono membri della classe dovrebbero
   * essere implementate mediante funzioni membro costanti della classe.
   */
  std::ostream& operator<<(std::ostream& out, const Int& x){
    return x.print(out);
  }
  
} // namespace N

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
