## Poliformismo dinamico ##

Tutto quello fatto fino ad ora si chiama **poliformismo statico**, statico poichè le decisioni su come si comporta il codice vengono prese a tempo di compilazione. Prima tipicamente per un linguaggio O.O si aveva in mente un'altra forma di poliformismo, quello **dinamico**. Come si introduce questo polimorfismo?  --Bruce Heckel. 

**stringstream:** utile quando ho un numero grande e lo voglio convertire in stringa

**modificare** prendo qualcosa che funziona e lo cambio per farlo funzionare in un altro modo 
**estendere** qualcosa che funzionava in un modo in certi contesti, ora deve funzionare nello stesso modo ma anche in altri contesti diversi

Vogliamo risolvere un problema di estendibilità. Se modifichiamo possiamo cambiare il comportamento del programma, rischiando di rompere qualcosa. Quindi ci si chiede se per caso non abbiamo sbagliato a progettare qualcosa. Specie quando ho un punto centralizzato di modifiche e devo anche riconpilare tutto il codice. Io volgio che se l'utente vuole aggiungere una sola cosa, non ricompilo il codice, voglio solo fare del linking. Quindi, non c'è un modo per aggiungere funioni aggiungendo solo codice? Come codice non open, do exe e librerie con cui fare linking. **Codice facilmente estendibile ma non facilmente modificato**.
Poliformismo dinamico, vuol dire scrivere codice una volta sola e poi trovo un modo per decidere quale è la particolare funzione che vogliamo chiamare.

Nell'esempio ogni oggetto viene implementato in una classe a parte, quindi separati tra loro. 

Metodo `virtual`, il mio utente quando chiamerà quel metodo, significa dammi il metodo dell'oggetto concreto che hai usato per implementare l'oggetto astratto. `=0` significa che è una funzione virtuale **pura**, ovvero nella classe base non vi è alcuna implementazione di default, interfaccia totalmente astratta. 
La classe derivata implicitamente si ha conversione a puntatore a classe base, relazione `is-a`, un cane implementa interfaccia di un animale (genere, verso, nome) e quindi è un animale.
Nella classe derivata, ridichiare le funzioni della classe base per poter dare l'implementazione di questi metodi. 
Quindi se ho 5000 oggetti, ho 5000 implementazioni separate, se devo modificare uno di questi, mi basta toccare solo quello senza rischiare di rompere tutto. Scrivo codice senza bisogno di toccare le altre interfacce.

**Meccanismi di compilazione** devono tenere traccia delle dipendenze. Voglio ricompilare il meno possibile. Devi compilare solo i file che sono stati modificati più recentemente rispetto all'ultima implementazione che hai fatto. Quindi, a un file contribuiscono più file sorgenti, e si tiene traccia di tutte le dipendenze e si vede tutti i file sorgente e confronto il timestamp dell'ultima modifica alle dipendenze. 
`CXXFLAGS` flag per comandi di compilazione cpp
`touch` tocca file, modificagli il time-stamp

Una **classe dinamica** in c++ è itechettata con `virtual`, con la quale si può fare poliformismo dinamico. All'interno di classi dinamiche si possono distinguere quelle che sono classi **astratte** e classi **concrete**. 
Se una classe dinamica ha un membro puro, che non ha ridefinito, quella è una classe atratte. In realtà non è completamente astratta, ha ancora delle parti che sono concrete, ma siccome c'è almeno un pezzettino che è astratto, allora è astratta. Nella realtà si richiede che tutta sia astratta.
Le classi dinamiche astratte non permetto di creare oggetti di quel tipo, es:

``` c++
struct Dinamica_Astratta {
virtual void bar() = 0;
void foo() { }
virtual ~Dinamica_Astratta() { }
};
```
Questo perchè se non abbiamo implementazioni dei metodi allora ci ritroviamo con undefine behaviour.
Le classi atratte servono ad implementare classi concrete.

``` c++
struct Dinamica_Non_Piu_Astratta : public Dinamica_Astratta {
  virtual void bar() { }
};
```
È una classe che al suo interno contiene una `Dinamica_Astratta`.

Con le classe dinamice posso avere polimorfismo dinamico. A tempo di esecuzione quindi non è mai l'oggetto della classe base e basta, ma vi è anche quella derivata che viene passata. Il compilatore però non ne ha alcuna idea dell'oggetto passato.Quando fa risoluzione dell'overloading e vede un metodo virtuale, il compilatore genererà del codice che permetterà di andare a chiamare la funzione con l'implementazione giusta ma lui non sa quale sia questa. 
**RTS** supporto a tempo di compilazione, infilate dal compilatore che entrano in gioco in certi contesti e fanno operazioni come decidere quale sia la funzione invocata. Questo come fa però a capire quale funzione chiamare, tra le classi concrete? C'è qualcosa di implicito... si va a vedere la size di classe astratta e di quella concreta. In quest'ultima vediamo che abbiamo delle informazioni ulteriori, su come sono implementati i metodi, distruttore, v-table (tabella delle implementazioni dei metodi virtuali). Quindi il RTS entrerà nella classe, a tempo di esecuzione andrà nella v-table e lì troverà l'indirizzo della funzione da chiamare. È una chiamata indiretta di funzione. `=0` infatti indica all' RTS che non c'è ancora implementazione e quindi il compilatore mette il puntatore della funzione a 0.
Nella classe dinamica quindi ci sono informazioni supplementari a quelle che abbiamo messo noi in modo esplicito.
`overriding` prendere il soppravvento su qualcosa con una nuova implementazione. Quando c'è polimorfismo dinamico oltre a risoluzione overloading vi è anche la **risoluzione dell'overriding**, il primo fatto a tempo di compilazione il secondo a tempo di esecuzione. Trova il tipo della classe derivata e chiama il metodo giusto (overriding).
Il codice dipende solo dall'interfaccia della classe astratta, non mi interessa l'implementazione per fare le operazioni. Un po' di modifiche le devo fare se voglio fare, ma il minimo. Poi faccio il linking di tutti i file oggetto.

## prova 20050201 ##

**exe4**
Il codice funziona, ma vogliamo ristrutturarlo per semplificare il lavoro in futuro.
* classe astratta sporcata con dettagli di implemementazione
* l'enumerazione non ha senso, è come se pretendessi di sapere già tutte le schede che farà il marketing
* marco le funzioni virtuali e siccome non riesco a darne un'implementazione le segno come virtuali pure. (anche distruttore virtuale, poi si vedrà perchè).
* e abbiamo anche fatto l'implementazione dei metodi separati

**Come mai il distruttore di una classe astratta dinamica, è opportuno che sia marcato virtual?**
Che cosa succede quando lavoriamo con oggetti di quella classe lì?
Si supponga che la classe concreta  gestisca risorse. Se la classe non avesse un distruttore virtuale, il compilatore avrebbe chiamato quello della classe base (astratta) e non deallocherebbe alcuna risorsa, le mie classi concrete potrebbero avere tanti dati, che voglio rilasciare. Il distruttore parte dalla classe derivata e poi successivamente invoca anche quelli delle classi base, per questo il distruttore deve avere una minima implementazione e non può essere virtuale puro. 

### Morale tecnica ###
Classi dinamiche ereditano metodi virtuali. Se la classe dinamica è un'interfaccia ha i metodi virtuali senza implementazione eventualmente. Con la classe astratta implemento classi concrete.

### Risoluzione dell'overriding ###
1. overloading / overriding / hiding: con il primo si hanno n funzioni nello stesso scope che si differenziano per il tipo e/o numero degli argomenti e il compilatore decide quale chiamare. Con l'hiding, es. faccio classe derivata che implementa lo stesso metodo della classe base, la nuova funzione nasconde quella della classe base. L'overriding una implementazione ha prevalenza su un'altra. Quest'ultimo ha a che vedere solo con il tempo di esecuzione.
   * serve derivazione per overriding (public), non mi basta il duck_type_system, il sistema deve essere a conoscenza dell'interfaccia che implementa.
   * tipo statico / tipo dinamico: io guardo il tipo, il compilatore dice classe astratta, il supporto a run time mi dice la classe concreta. La risoluzione dell'overriding ha senso quando si lavora con tipi diversi, tra tipo statico e dinamico.
   * metodi virtuali: per avere overriding devo avere dei metodi virtuali, non tutti se non ho bisogno di overriding.
   * chiamate non qualificate: noi potremmo qualificare una chiamata dando il nome della classe per quella chiamata. es:
   nella catena delle derivazioni si va fino all'ultima classe derivata e si chiama il metodo di quella

``` c++
void (Cane& c) {
  c.verso();
  // insisto che volgio l'implementazione della classe Cane
  // disabilito l'implementazione RTS
  c.Cane::verso();
}
```
Alle volte l'overriding si disabilita per errore, facendo hiding (dimenticandomi il const, aggiungendo parametri). Con lo standard nuovo, aggiungi `override` e indichi al compilatore che volevi fare override, indicandoti che hai fatto errore
