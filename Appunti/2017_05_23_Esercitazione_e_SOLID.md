## esercitazione 20060206 ##

### esercizio 3 ###
Gli errori che ci sono sono di progettazione, quelo che voglio è che il codice sia robusto rispetto alle modifiche, se creo una nuova classe deve funzionare. Quindi il codice qui è insensibile rispetto alle modifiche? Se introduco un nuovo generatore di manuali, funziona?
Non funziona perchè nel codice utente dipendiamo dalla tipizzazione **CLOSE WORLD ASSUMPTION (CWA)**. L'utente pretende di conoscere tutti i generatori di manuali da qui alla fine dei tempi. Si vuole codice che si può estendere ma non modificare.
Si risolve facendo in modo che il codice non sia più modificabile.
Non dovresti mai chiederti il tipo, devi lavorare soltanto con l'astrazione. Quindi perchè l'utente si sta chiedendo il tipo? Perchè per esempio per HTML vuole inserire un hyperlink. Ma perchè? Perchè non si offre nella classe astratta il metodo hyperlink e quindi è sbagliata la classe astratta! Lo stesso per il metodo page_break().
Così il codice risulta anche più semplice per l'utente perchè ha un'interfaccia più adeguata.
Se io devo inventare un nuovo generatore di manuali, in un file diverso, farò una nuova interfaccia con l'overriding dei metodi. L'implementazione è a un'altra parte.

``` c++
// Manual_Generator.hpp

#include <string>
using std::string;
class Manual_Generator {
public:
  virtual void put(const string& s) = 0;
  virtual void set_boldface() = 0;
  virtual void reset_boldface() = 0;
  //metodo hyperlink mancante
  virtual void hyperlink(const string& uri, const string& text) = 0;

  virtual void page_break() = 0;
  // ...

  //DISTRUTTORE VIRTUALE!!!
  visrtual ~Manual_Generator() { }
};

//////////////////////////////////////////////////////////////////////////

// HTML_Generator.hpp

class HTML_Generator : public Manual_Generator {
public:
  void put(const string& s);
  void set_boldface();
  void reset_boldface();
  void hyperlink(const string& uri, const string& text);

  virtual void page_break()
  {
    // Nota: usare il tag HR per simulare il cambio pagina in HTML.
    // NON c'è più CWA!!!
   put("<HR>");
  }
  // ...
};

////////////////////////////////////////////////////////////////////////////////

// ASCII_Generator.hpp

class ASCII_Generator : public Manual_Generator {
public:
  void put(const string& s);
  void set_boldface();
  void reset_boldface();
  void page_break();
  // l'utente ci dice come implementare il metodo
  void hyperlink(const string& uri, const string& text)
  {
    put(text);
    put(" (");
    put(uri);
    put(")");
  }
  // ...
};

////////////////////////////////////////////////////////////////////

// Client code

void f(Manual_Generator* mg_p) {
  // ...
  mg_p->hyperlink("http://www.cs.unipr.it/ppl", "PPL");
  // ...
  mp_p->page_break();
  // ...
}
```

________________________________________________________

## SOLID (object-oriented-design) ##
Sono tutti principi, cose verso le quali dovresti provare ad avvicinarti. Valgono in generale per ogni linguaggio.
**SRP** una classe dovrebbe fare solo una sola cosa e bene. Se ne fa di più e meglio separarli. Ogni parte del problema ha la sua parte che la risolve separata in modo logico dalle altre.
"Una classe deve avere solo un motivo per essere modificata"
**open/closed principle (OCP)** le entità software dovrebbero essere aperte per estensioni ma chiuse per modifiche. Meyer è stato il primo a definire il contratto, in EIFEL era possibile dichiarare in modo esplicito precondizioni e invarianti.
>Se noi identifichiamo una dimensione di cambiamento futuro del codice, cioè il codice cambia in tanti modi, ma identifichiamo una direzione nel quale cambieranno (es. nuove figure geometriche, ma non vuoi cambiare codice), allora scrivo codice chiuso alle modifiche e facilmente estendibile. 
OCP è il principio più violato. Ma cosa in particolare?
* ci sono violazioni che hanno natura più sintattica: 
Per vedere una di queste violazioni basta vedere il **DIP**.
I moduli ad alto livello non dovrebbero dipendere da quelli a basso livello ma entrambi devono dipendere dalle astrazioni. <up> dipendenza buona, <down> dipendenza cattiva.
Dove si vede che qualcosa dipende da qualcosa? Inclusioni, se cambio inclusione cambio codice. Quindi non dovremmo includere dettagli implementativi ma astrazioni in questo modo il codice è più generale e bisogna fare sempre meno modifiche. 
>Il codice è robusto quando dipende da concetti astratti
**Perchè?**
Perchè le cose nella vita cambiano, ma se io stabilisco delle regole che sono valide per tutti i casi concreti possibili che soddisfano una situazione atratta, quelli vanno bene per tutte le situazioni possibili. 
>Le astrazioni sono buone perchè sono meno soggette a cambiamenti nel tempo
Il dramma succede quando nel codice utente voglio utilizzare una funzionalità che l'astrazione non mi fornisce e solo allora cambio l'astrazione.

Perchè si chiama principio di inversione delle dipendenze?
Il metodo per risolvere un problema solitamente è top down. Ma si stabiliscono delle relazioni di dipendenza dove moduli ad alto livello dipendono sempre di più da quelli a basso livello poichè implemento dettagli implementativi, è normale e intuitivo (si inseriscono frecce che vanno verso il basso). Se vuoi ottenere gli effetti positivi dell'OCP, tu devi andare a prendere queste dipendenze e invertirle. Come? Ti inveti l'interfacci per il servizio che volevi avere e dici che il tuo meccanismo dipende dall'interfaccia. (es. in codice manuale, si faceva dinamic_cast e si aveva dipendenza verso le dipendenze delle classi derivate).
Nel mondo fuori si fa il contrario, dipendenze verso il basso, perchè? Perchè chi vende vuole che diventi da loro. 
Non si potrà mai fare la cosa fatta bene, ma se un giorno quelle dipendenze cambieranno, allora lì mi chiedo se è il caso di farmi le cose O.O. fatte bene.
Fare le cose bene è un investimento che vale soltanto se un domani torneranno un vantaggio.

* ci sono violazioni che hanno natura più semantica:

## esercitazione 20050920 ##
Il codice `BWare` non lo si può modificare. 
Quello che devo fare è che io devo dipendere da una strazione `Biblio` e che `BSoft` è una particolare implementazione dell'astrazione `Biblio` e lo stesso per `BWare` (`BWare` e `BSoft` is_a `Biblio`). Così abbiamo invertito le dipendenze. Il problema è che per dire che `BSoft` è `Biblio`, bisogna fare la prima come derivata della seconda, ma non è possibile perchè non possiamo modificare il codice sorgente. Quello che vogliamo fare è un **adattatore**, mettiamo uno strato che fa il mapping tra le chiamate dei metodi di una classe. Quindi io non ho bisogno di modificare la classe, ma eredito una classe che al suo interno contiene un oggetto della classe che non posso modificare (passacarte).

Abbiamo generalizzato il codice per decidere quando usare `BSoft` e quando `BWare`. Non si possono mischiare, perchè? Perchè quelli che hanno implementato la libreria hanno dichiarato metodi che prendono come parametri solo oggetti della libreria. 
Ciò che abbiamo fatto è rendere il `codice portabile` (diverso da `portato`, che lo abbiamo fatto) tra le librerie. Poter mischiare le librerie nel loro uso è `l'interoperabilità`, mi serve collaborazione da chi mi fornisce servizi.

``` c++
// Biblio.hh
// Interfaccia astratta e si usa solo questa

class Biblio {
private:
  // ...
public:
  virtual void s1();
  virtual void s2(const Biblio& y, int n);
  virtual void s3(int n);
  virtual ~Biblio() { };
  // ...
};

/////////////////////////////////////////////////////

class BSoft {
private:
  // ...
public:
  void s1();
  void s2(const BSoft& y, int n);
  void s3(int n);
  // ...
};

////////////////////////////////////////////////////

#include "Biblio.hh"

// NON dipende più da dettagli implementativi

void f(Biblio& x, const Biblio& y, int n) {
  // ...
  if (n > 0)
    x.s3(n);
  else {
    x.s1();
    n = 5;
  }
  // ...
  x.s2(y, n);
  // ...
}

////////////////////////////////////////////////////

class BWare {
private:
  // ...
public:
  void w1();
  void w2_1(const BWare& y);
  void w2_2(int n);
  void w3(int n);
  // ... ecc.
};

////////////////////////////////////////////////

// BsoftAdapter.hh
// CODICE MIO LO POSSO MODIFICARE

#include "Biblio.hpp"
#include "BSoft.hpp"

class BSoft_Adapter : public Biblio {
private:
  // ...
  BSoft& bs;
public:
 // dettagli non serve scriverlo
  explicit BSoft_Adapter(BSoft& s) : bs(s) { }

  void s1() {
    bs.b1();
  }
 
  void s2(const Biblio& y, int n) {
    // non funziona perchè y è un oggetto della classe
    // astratta, ma chi ci garantisce che y sia effettivamente
    // un BSoft_Adapter, potrebbe essere anche un BWare_Adapter
    // per risolvere il problema potrei...
    const BSoft_Adapter& bsy = dynamic_cast<const BSoft_Adapter&>(y);
    // se non è un BSoft_Adapter, con i riferimenti non può dare riferimento
    // nullo, quindi lancera eccezione, ma è comunque a tempo di esecuzione
    // questo controllo. L'altra possibilità è fare come nella STL, cioè undefine behaviour...
    const BSoft_Adapter& bsy = static_cast<const BSoft_Adapter&>(y);
    // si fa down cast, che è pericoloso    
    
    bs.s2(y.bs, n);
  }

  void s3(int n) {
    bs.s3(n);
  }
  // ...
};

////////////////////////////////////////////////////

// BWareAdapter.hh
// CODICE MIO LO POSSO MODIFICARE

#include "Biblio.hpp"
#include "BSoft.hpp"

class BWare_Adapter : public Biblio {
private:
  // ...
  BWare& bw;
public:
  // dettagli non serve scriverlo
  explicit BWare_Adapter(BWare& w) : bw(w) { }
  
  void s1() {
    bw.w1();
  }

  void s2(const Biblio& y, int n) {
    // funziona solo se Biblio è un adattatore per BWare
    const BWare_Adapter& bwy = dinamic_cast<const BWare_Adapter&>(y);
    // non dobbiamo sentirci in colpa perchè siamo nei dettagli implementativi
    // il punto è che questo codice funziona soltanto quando lavoro con x e y,
    // nella funzione f. Devono essere due oggetti di tipo Biblio ma concordi
    // entrambi BWare o BSoft. Ma così non abbiamo massima flessibilità! Vero.
    
    bw.w2_1(y.bw);
    bw.w2_2(n);
  }

  void s3(int n) {
    bw.w3(n);
  }
  // ...
};

////////////////////////////////////////////////////
```

Lo `stl_stack.h` è un esempio di adattatore.


L'ereditarietà in passato è stata usata male (abusata). L'ereditarietà con senso ha due usi:
* uso canonico, quello accettato da tutti è quello visto negli esempi, ovvero per definire l'interfaccia astratta che può essere implementata in modi diversi e il meccanismo dell'overriding decide quale chiamare, se non ci fosse non si potrebbe fare overriding. (SERVE EREDITARIETÀ)

altri usi (es. gerarchia classi tag iteratori, uso tecnico di implementazione)

* uso ereditarità per estendere una classe esistente (in java c'è concetto di interfaccia e classe, che è più bello). Uso ereditarietà come estensione, eredito classe per avere più funzionalità, estendendo l'interfaccia. Oppure eredito per estendere l'implementazione. Ma c'è davvero bisogno di usare l'ereditarietà? No, posso semplicemente usare il contenimento, voglio estendere una classe? Al suo interno creo un oggetto della classe da estendere. L'ereditarietà a volte viene comoda perchè non devo scrivere i metodi passacarte, ma dal punto di vista concettuale l'ereditarietà non da nulla. (NON È OBBLIGATORIA L'EREDITARIETÀ)

Poi quando eredito posso fare conversione da tipo attuale a tipo base (upcast) ma a volte non voglio farlo! Potrei fare ereditarietà `private`.
Comunque quando sto per fare ereditarietà è meglio pensarci bene.

Nella STL quanto viene usata la parola chiave `virtual`?
Le eccezioni sono il caso interessante nel quale usare il poliformismo (guardare `/usr/include/c++/5.4.0/exception`).
Tutte le eccezioni hanno metodo virtuale what(). Io catturo eccezione, quale eccezione non lo so.

Passare quando si ha ereditarietà solo per riferimento e no per valore.Per valore viene fatta una copia del pezzettino solo della classe base (quando il parametro e del tipo della classe base).
