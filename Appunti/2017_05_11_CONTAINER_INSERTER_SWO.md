``` c++
/*
  Leggere da std::cin tutte le stringe
  e stamparle in std::cout ordinate
  e senza ripetizioni
 */

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

bool reverse_order(const std::string& a, const std::string& b) {
  return a > b;
}

bool lenght_order(const std::string& a, const std::string& b) {
  return a.size() < b.size();
}

// oggetto funzione
struct Reverse_Order {
  Reverse_Order(const std::string& a, const std::string& b) { }
  bool operator()() {return a > b;}
}

int main()
{
  std::vector<std::string> vs;
  // Leggi da std::cin in vs
  // prende input e lo usa come container
  std::istream_iterator<std::string> first(std::cin);
  // se creo un istream_iterator di default
  // questo è l'iteratore 1 dopo la fine
  // quindi l'end(); è una convenzione
  std::istream_iterator<std::string> last;

  // il mio vector prima era vuoto
  // e quando provo a scrivere la prima posizione è end()
  // l'iteratore di output sovrascrive, non creano posto,
  // se non c'è niente undefine behaviour

  // non c'è deduzione automatica di template di classe
  
  //std::back_insert_iterator<std::vector>std::string>> bi_iter(vs);

  // come lo risolvi? Con funzione std che permette di creare l'oggetto
  std::copy(first, last, std::back_insert(vs) /*bi_iter*/);
  
  // Ordina vs
  // sort prende solo random_access_iterators
  std::sort(vs.begin(), vs.end());
  // Elimina i duplicati da vs (ordinato)
  // unique prende una sequenza la scorre
  // quando trova due elementi adicenti identici
  // elimina la seconda, cioè sposta elementi
  // all'interno della sequenza, li compatta all'inizio
  // non li rimuove veramente la unique restituisce
  // come risultato un iteratore

  // se volessi ordine inverso?
  std::sort(vs.begin(), vs.end(), reverse_order);

  // std::vector<std::string>::iterator i = std::unique(vs.begin(), vs.end());

  //oppure da c++ 11...
  auto i = std::unique(vs.begin(), vs.end());
  //se io voglio veramente eliminare questi elementi per davvero...
  vs.erase(i, vs.end());
  
  // Stampa su std::cout
  // non possiamo vedere lo std::out
  // anche esso come un contenitore
  // e quindi i voglio copiare da out a vs
  // Per fare questo ci sono gli standard stream iterators
  // iteratori che si possono usare sugli stream e scriverci dentro
  /* /urs/include/c++/5.4.0/bits/stream_iterator.h */
  // oggetto che prende lo stream e lo mostra come contenitore
  std::ostream_iterator<std::string> out (std::cout, "\n");
  std::copy(vs.begin(), vs.end(), out);
  
  return 0;
}

//VARIANTE
// contare quante volte le duplicate sono duplicate

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

int main()
{

  std::map<std::string, unsigned> m;
  
  // Leggi da std::cin in vs
  std::istream_iterator<std::string> first(std::cin);
  std::istream_iterator<std::string> last;

  for ( ; first != last; ++first) {
    // meglio???
    ++m[*first];
    // auto i = m.find(*first) == end();
    // if (i == m.end())
    //   m.insert(std::make_pair(*first, 1));
    // else
    //   ++(i->second);
  }
  
   // Stampa su std::cout
  std::ostream_iterator<std::string> out (std::cout, "\n");
  // la copy proverebbe ad usare operator<<() sulle mappe
  // quindi non si può fare copy così sulle mappe, ma invece...
  std::copy(m.begin(), m.end(), out);

  //si fa così output
  for (auto i = m.begin(); i != m.end(); ++i) {
    std::cout << '(' << i->first << ", " << i->second << ")\n";
  }
  
  return 0;
}
```

`std::copy(first, last, vs.begin());`

Come risolvere problema di scrittura su container vuoto:
1. faccio resize(200) ma è difficile azzeccare dimensione giusta, brutto
2. uso la classe templatica, chiamati **inseritori** (**inserter(s)**). Dove si possono fare gli inserimenti? In coda, in cima, e in una posizione qualunque, quindi ci sono `front_inserter`, `back_inserter`, `inserter`

Sono identiche le istanze delle funzioni con predicato funzione da quelle con predicato oggetto funzione? No. 
es.

``` c++
std::sort(vs.begin(), vs.end(), straight_order);
std::sort(vs.begin(), vs.end(), reverse_order);
```
Quante versioni istanzio di sort? Una! Perchè? Perchè le due funzioni restituiscono lo stesso tipo, anche se diverse.
Con l'oggetto funzione vengono generate più copie per ogni specifico ordinamento, e quindi se la funzione è piccola, il compilatore può fare inlining ed eliminare overhead di chiamata di funzione. C'è il prezzo che la dimensione del codice sorgente diventa più grande (code bloating). Con le funzioni si hanno puntatori a funzione e quindi niente inlining.
Nella STL esiste la possibilità di istanziare oggetti funzione che fanno cose comuni, ad esempio:

``` c++
std::less<std::string>();
```
crea un oggetto funzione che implementa il `<` per il tipo stringa, evitandoci di scrivere l'oggetto funzione da noi. C'è anche:

``` c++
std::greater<std::string>();
```

**Strict Weak Ordering:** l'ordinamento non deve essere per forza totale (cioè < o >). Non vuol dire che se sono = (hanno stessa lunghezza) allora sono effettivamente uguali. Se due elementi sono inconfrontabili, decido che per me sono equivalenti. Se questi elementi li metto in una classe di equivalenza, allora ottengo un ordinamento totale (es. [tutte stringe lunge 1] < [tutte stringe lunghe 2]).

Quando non abbiamo ordinamenti parziali che non vanno bene?
es. quando abbiamo elementi che non possono essere confrontabili tra loro, ma almeno due di questi possono essere confrontati tra loro (A !conf C, B !conf C, A conf B).

## CONTENITORI ASSOCIATIVI ##
Accedo agli elementi mediante chiavi e queste sono ordinate (SWO). È il caso della `map<key, mapped>`, `multimap`, `set<key>`, `multiset<key>`. Il criterio di confronto è `operator<(key)`. Queste funzionano quando si ha SWO. 

SUNTO:
* possiamo utilizzare delle cose che non sono contenitori come tali, come? usando delle classi adattatori (istream_iterator) e utilizzare algo stl su stream.
* anche se compilano bene, il codice che usa output iterator, schianta perchè gli iteratori di output di default usano modalità ovewrite, ma se non abbiamo elementi scriviamo su locazioni non valide. Perciò bisogna usare gli inserter che inseriscono elementi, passandogli gli iteratori e funziona su tutti i contenitori sequenziali. Hanno funzione per poterli costruire con autodeduzione.
