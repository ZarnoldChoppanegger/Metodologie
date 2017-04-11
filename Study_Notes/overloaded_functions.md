## Overloaded functions ##
Il c++ offre la possibilità di definire più funzioni con lo stesso nome ma numero e/o tipo di parametri diverso. Questa funzionalità è più che altro una notazione di convenienza che risulta utile per funzioni con nomi convenzionali come *sqrt, print, open*.  Quando i nomi sono **semanticamente significativi** questa convenienza diventa essenziale come nel caso degli operatori +, *, <<, per i costruttori e nella programmazione generica.

## Automatic Overload Resolution ##
Un compilatore per sapere quale chiamata di funzione effettuare, confronta i parametri formali delle funzioni con quelli attuali  della chiamata e sceglie la funzione in base a questi criteri:

1. **corrispondenza esatta**, cioè quando tra parametri formali e attuali non vi è alcuna conversione o solo conversioni banali (come per esempio nome di array a puntatore, nome funzione a puntatore di funzione, e qualificazione da T a const T);
2. **promozioni**, cioè quando si ha promozione dei parametri attuali a quelli formali (bool a int, char a int, short a int, le controparti unsigned e float a double);
3. **conversioni standard**, cioè quando si ha qualsiasi conversione che implica perdita di informazioni (int a double, double a int, double a long double), da Derived* a Base*, T* a void*, int a unsigned int;
4. **conversione definita da utente**, conversioni effettuate da tipi definiti da utente;

Quindi tra tutte le funzioni **candidate** per la chiamata (quelle trovate in base al nome), si sceglie quella **utilizzabile**, cioè quella con il match di livello più alto. Se si ha più di una funzione utilizzabile la chiamata è rifiutata come ambigua.

**Descrizione fasi:**
* **Prima fase:** scelgo solo in base al nome e alla visibilità 
* **Seconda fase:** controlla tra le funzioni scelte il numero e il tipo dei parametri che ho messo nella chiamata della funzione
* **Terza fase:** si hanno più situazioni:
  * nessuna funzione utilizzabile -> errore
  * una sola funzione utilizzabile -> la uso
  * più funzioni utilizzabili -> si applicano le regole di risoluzione automatica dell'overloading
  
Quando si ha una situazione di questo tipo:

``` c++
Razionale r = 12;
```
le funzioni candidate sono due: il costruttore (non dichiarato explicit) e il costruttore di copia.
In questo caso viene utilizzato il costruttore (non explicit) poichè il costruttore di copia è meno efficiente.

Quando chiamo una funzione, il fatto che questa sia privata, friend, protetta, etc... non ha nulla a che vedere con la risoluzione dell'overload, quindi può essere scelta una funzione non invocabile e quindi impossibile da utilizzare, con errore a compile time.

La cosa più schifosa:

``` c++
class A{
public:
	foo(...)
private:
	foo(...)
}
```

