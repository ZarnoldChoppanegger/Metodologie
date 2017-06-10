Problemi puntatori:
* puntano a qualcosa furi dagli oggetti accessibili, potrebbero essere invalidi
* si potrebbero confrontare ma non fare operazioni, es.
* coppie di iteratori per trattare range, a seconda di quello che ci si chiede su questa coppia le operazioni potrebbero dare errori, (es non confrontare con < e > a meno che non siano random access iterator e devono essere dallo stesso contenitore)
* confrontare puntatori ai membri della classe (non lo ha mai fatto)

**rvalue:** valore che non ha indirizzo, da solo valore
**lvalue:** qualcosa che puo' stare a sinistra dell'assegnamento, che ha una posizione in memoria che posso calcolarne l'indirizzo
**xvalue:** expiring values, valori che stanno per morire, e si possono usare senza preoccuparci di copiarle, gli rubiamo le risorse

Come facciamo a capire quando uno chiama un 'lvalue' o 'rvalue'? Guardiamo se c'e' un indirizzo.

'foo(4)', 4 ha un indirizzo? No allora e' rvalue
'foo(i)', i ha indirizzo? Si allora e' lvalue

'void foo(int i);' L'intero i viene inizializzato. Match perfetto se passi 4.
'void foo(const int i);' Per il chiamante le due funzioni sono identiche! Perche' ho passato per valore!!!

'void foo(const int& i);' Non e' ambiguo
'foo(4)' sarebbe una chiamata ambigua perche' puo' chiamare anche l'ultima funzione perche' promuove rvalue a lvalue, **materializzazione** crea valore in un oggetto temporaneo. Bad practice mettere in overloading due funzioni, una che prende per valore l'altra che prende per riferimento.

``` c++
void bar(int* pi);
void foo(int&&);
int main(){
  int i;
  int ai[100];
  foo(4);
  foo(i);
  
  bar(ai);
  // ai decade da lvalue a rvalue, puro valore
  // non esiste nessuna cella in memoria che contiene l'indirizzo
  // degli elementi, lo calcola al volo
  // la valutazione dell'argomento viene fatta prima della chiamata di funzione
}
```

