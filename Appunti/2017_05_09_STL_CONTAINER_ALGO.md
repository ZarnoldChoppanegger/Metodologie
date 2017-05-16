## STL_VECTO.h ##

Si restituisce l'iteratore nella vector per poter avere la posizione dell'elemento nel caso di riallocazione, poichè vengono invalidati tutti gli iteratori precedenti. Dobbiamo anche ricordarci di ricalcolare la fine del vector perchè è andata da un'altra parte durante la riallocazione.

``` c++
void
insert(iterator __position, size_type __n, const value_type& __x)
{ _M_fill_insert(__position, __n, __x); }
```
Questa funzione del stl_vector, non restituisce iteratore, bisogna ricalcolare tutto. 
I contenitori della STL sono exception safety (base, forte o no throw?). La insert che garanzie fornisce? Per un solo elemento è forte, quello che succede però per motivi di efficienza, quelle che fanno tanti inserimenti non sono implementate con exception safe forte, ma con livello base. Quindi se questa operazione esce con eccezione, noi cosa sappiamo del vector? NULLA!

``` c++
iterator erase (iterator __position) 
{ return _M_erase(__position); }
```
elimina in una posizione precisa.

``` c++
iterator erase(iterator __first, iterator __last)
{ return _M_erase(__first, __last); } 
```
quetsa elimina tutti gli elementi contenuti entro un range, azzera la size, ma non diminuisce la capacità perchè ci è voluta fatica per allocare risorse. Ma se io ho un vector che durante i calcoli diventa grande ma poi alla fine del calcolo diventa piccolo, come posso fare per dargli una capacità sensata? In c++ c'è `shrink_to_fit()`, che implementa `copy & swap`.

``` c++
void
clear() _GLIBCXX_NOEXCEPT
{ _M_erase_at_end(this->_M_impl._M_start); }
```
svuota il vector, ma mantiene capacità.

## STL_DEQUE.h ##

Anche questa permette di fare l'accesso agli elementi in maniera efficiente, in qualunque posizione. Permette di fare rimozioni e inserimenti non solo dall'inizio (front) ma anche dalla fine (back).
Ci sono tanti blocchi di memoria che vengono agginti agli altri senza bisogno di riallocare.

Quello che non c'era nel vector:

``` c++
void push_front(...);

// e anche

void pop_fron();
```
La pop-front si può fare anche nel vector, ma non si mette nel vector con lo stesso nome perchè non è efficiente e non si vuole dare l'impressione che si possa fare facilmente.

La deque non ha il concetto di capacità, infatti c'è `size` ma non `capacity`.

## ST_LIST.h ##

Liste di nodi concatenati, è sempre un contenitore sequenziale con begin() e end(), ma è molto più difficile calcolare la posizione di un nodo, dobbiamo andare di passo in passo seguendo i nodi. Non è efficiente e non ti viene data la possibilità di fare begin() + 30, ,poichè dovrei seguire 30 nodi, passo passo.
Non c'è `capacity`.

``` c++
size_type size() const ...
```
Andare a vedere cosa fa size(), c'è un modo efficiente per trovare la diemensione senza scorrere? NO!

Riescono a fare inseriementi efficienti ovunque, ma solo se ho l'algoritmo che organizza gli iteratori nella posizione nella quale devo lavorare.

``` c++
vois pslice(iterator __position, list&& __x);
```
Ruba tutti gli elementi di una lista e li mette dentro, per poi lascire la lista passata vuota.

``` c++
void remove(const _Tp& __value);

void remove_if(...);
```
Tutte le volte che trova un valore, elemina un nodo. Perchè non c'è nel vector? Potrebbe essere utile.

``` c++
void unique(...)
void reverse();
void sort(_StrictWeakOrdering);
```
Ci sono ordinamenti totali, parziali, deboli, ordinamenti stretti deboli... cosa sono? Se ne parla più avanti.

Perchè nelle liste ci sono questi metodi e nel vector no? Non aveva senso perchè nei vector gli algoritmi generici fuori dalla classe sono efficienti tanto quanto quelli dentro. Nella lista invece i metodi conoscono come è fatto l'oggetto e quindi da' efficienza. É brutto dal punto di vista dell'interfaccia, perchè un utente si deve ricordare tutti i metodi.

I contenitori sono implementati nel modo più simile ma appena l'efficienza cala la somiglianza finisce. L'efficienza viene prima dell'eleganza nel c++.

## Iteratori 3, la minaccia fantasma ##

Classificare gli iteratori a seconda di quello che possono fare, **categorie di iteratori**:
1. **Random access iterator(s)**, sono quelli dei (std::vector, std::deque, std::string, array). Danno un accesso efficiente a una qualsiasi posizione della struttura. Che operazioni possiamo fare? Tutto quello che ha senso fare sugli iteratori, quindi:
    * `++i`, `i++`(non efficiente), `--i`, `i--`(non efficiente) [spostarsi di un passo]
    * `i + n`, `i - n`, `i += n`, `i -= n`,  [spostarsi di n posizioni, calcolando nuovo iteratore temporaneo o incrementando iteratore]
    * `i - j` [differenza tra due iteratori]
    * `==`, `!=`, [confrontare] 
    * `<`, `<=`, `>`, `>=` [ vedere l'ordine, chi viene prima e dopo, sulla lista non è possibile]
    * `*i`, `i[n]` [dereferenziazione]

2. **Bidirectional iterator(s)** , std::list:
    * `++i`, `i++`, `--i`, `i--`
    * `==`, `!=`
    * `*i`
  
3. **Forward iterator(s)** iteratori unidirezionali, std::forward_list (usata solitamente solo quando liste corte):
    * `++i`, `i++`
    * `==`, `!=`
    * `*i`

4. **Input iterator(s)** la differenza sta nel quando e come posso invocarli. Che differenza c'è tra un forward iterator che sia const e un input iterator? Sono due categorie diverse perchè hanno proprietà diverse, ma quali? Gli iteratori di input sono molto deboli per quanto riguarda l'invalidazione: tutte le volte che ho un iteratore di input e mi sposto di un passo, l'elemento che c'era dietro non è più disponibile, è andato perso, questo perchè vengono usati solitamente su stream (std::istream). Prendo due iteratori che si riferiscono alla stessa posizione, se uno dei due lo sposto, questo "distrugge" l'elemento e si rimane con un dangling pointer. Con il forward non succede questo.
    * `++i`, `i++`
    * `==`, `!=`
    * `*i` (solo lettura)

* **output iterator(s)** è una gerarchia funzionale, gli output iterator stanno da un'altra parte. scrivi un elemento e non puoi più leggerlo, l'unica cosa che puoi fare è spostarti alla prossima posizione per scrivere nuovamente. Questo lo si usa solitamente per depotenziare un contenitore.
  * `++i`, `i++`
  * `*i` (solo scrittura)
  * `==` (? non sa se si possono confrontare...controllare)


Iterator Category |         Ability                |    Providers
------------------------- |  ---------------------------- |  ---------------------------
Input iterator        |    Reads forward      |    istream
Output iterator |    Writes forward |            ostream, inserter
Forward iterator  |  Reads/writes forward  |  forward_list, unordered_[multi]set, unordered_[multi]map
Bidirectional it. |  Reads/writes forward/backward |   list, [multi]set, [multi]map
Random access it. |  Reads/writes with random access |  vector, deque string, array 


Devo implementare un algoritmo, questo però può essere implementato in maniera diversa in base a agli iteratori che mi capitano sotto mano, cosa faccio? Ad esempio, per la lista per calcolare la dimensione, si calcola la distanza tra il primo e l'ultimo elemento:

``` c++
template<typename Iter>
size_t
distance(Iter first, Iter last) {
  // se random iterator...
  return last - first;
  
  // se input iterator o forward...
  size_t d = 0;
  while(first != last) {
    ++first;
	++d;
	}
	
	return d;
	
	//per i bidirectional? bella domanda...
}
```
Come faccio a scegliere tra un'implementazione e l'altra? Devo fare introspezione, chiedere all'iteratore di che tipo è. Poi scelgo l'implementazione corretta.

#### Algoritmo più semplice da implementare ####

``` c++
template<typename Iter, typename T>
Iter
advance(Iter i, T n) {
  //se random iterator
  return  i + n;
  
  //se input iterator
  while (n > 0) { ++i; --n; }
  return i;
}
```
Negli iteratori ci sono gli **Iterator traits**, è una calsse della stl, che dice quale è la categoria dell'iteratore, quale è il value_type.

### STL_ITERATOR.h  ###

Perchè ci sono i traits e non lo chiedo direttamente agli iteratori? Più tardi...
* `difference_type` per rappresentare distanze tra elementi.

Poi ci sono delle struct che non hanno nulla all'interno ma rappresentano dei tag che indicano la categoria dell'iteratore. I forward, bidiretional e random acces c'è ereditarietà, perchè se non c'è niente? ereditano la possibilità se gli viene dato un `forward_iterator_tag` convertirlo facilmente in `input_iterator_tag`, (relazione is\_a) lo stesso anche i bidirectional e `random_access_iterator_tag`. 
Gli iteratori quando sono implementati come classi devono implementare i traits. 

Gli iterator traits sono passacarte, quale è l'utilità? Perchè a volte non fanno passacarte, perchè ci sono tipi built-in che non sono classi, **come i puntatori**. Quindi gli si mette un "tutore", cioè la classe traits. 
Negli iterator traits si hanno **specializzazioni parziali di template**, ad esempio la specializzazione della classe è `const _Tp*` che è ancora un template a sua volta. 

``` c++
template <typename RAIter>
typename std::iterator_traits<RAIter>::difference_type
distance(RAIter first, RAIter last) {
	return last - first;
}

template <typename InIter>
typename std::iterator_traits<RAIter>::difference_type
distance(InIter first, InIter last) {
  typename std::iterator_traits<RAIiter>::difference_type;
  while (first != last ) { ++first; ++d; }
  return d;
}
```

** andare a vedere /usr/include/c++/5.4.0/bits/stl_iterator_base_funcs.h **

`__advance` ha senso su tutti gli iterators tranne che sugli output iterators. Son state fatte solo tre funzioni però perchè c'è ereditarietà tra gli iteratori. 
(Provare a vedere quali sono quelle che sono candidate, utilizzabili e migliori, passandogli i vari iteratori.) 
La versione per il forward non si è messa perchè non ha senso. Questo è un meccanismo di dispatchin detto `tag dispatching`, cioè basato sul tag e non sul valore, questo non mi interessa. L'overloading poi mi sceglierà l'implementazione corretta, succede a compile-time perciò è molto efficiente.

> Le categorie degli iteratori permette di fornire versioni alternative per avere quella più efficiente.

## appello_20050920 ##

**void f(const vect& v)**
non compila il programma perchè:
* andiamo a lavorare su vettore v che è costante, begin() e end() sono quelli marcati const che restituiscono un `const_iterator` e quindi chiamiamo la find(...) che prende un input_iterator.Questa restituisce un iteratore dello stesso tipo passato, cioè un const_iterator. Il problema è quando proviamo ad inizializzare un iteratore `iter i_end` a partire da un const_iterator. Posso fare il contrario. La chiamata è legittima ma l'inizializzazione degli iteratori no. Quindi per risolverlo basta metterli `const_iterator`.
Non funziona bene perchè devo controllare che la find() abbia effettivamente trovato l'elemento e quindi confrontare il risultato con end().

**void g(vect& v)**
Problema nella scrittura `*i = "cacao"`, se v vuoto scrivo in una posizione sbagliata, quindi controllare che non sia vuoto.
`v.insert(i, "vaniglia");` metti vaniglia in prima posizione, spostando gli altri, dopo cicla per i != i_end, problemi? 
1. con la insert, sposto la end che non è stato ricalcolato
2. la insert invalida tutti gli iteratori che son stati calcolati

Ci sono iteratori in gcc per il debugging che tengono traccia di quando sono validi e invalidi

**exe 4**

**Corretto:**

``` c++
#include <list>
using namespace std;
//contenitore di contenitori di tipo T, avrei potuto chiamarlo come volevo
template <typename Cont_1_Cont2_T, typename Cont3_U>
void f(const Cont_1_Cont2_T& c1c2t, Cont3_U& c3u) {
  //siccome so che è un contenitore della STL so che ha iteratori a costanti
  for (typename Cont1_Cont2_T::const_iterator c1c2t_i = c1c2t.begin(),
	 c1c2t_end = c1c2t.end(); c1c2t_i != c1c2t_end; ++c1c2t_i) {

     //nome del tipo contenuto dentro Cont1_Cont2_T
    typedef typename Cont1_Cont2_T::value_type Cont2_T;
    for (typename Cont2_T::const_iterator i = c1c2t_i->begin(),
	   i_end = c1c2t_i->end(); i != i_end; ++i)
      c3u.push_back(*i); //funziona? si perchè i container hanno metodo push_back
  }
}
```
Con questo si ha astrazione anche sul contenitore, posso usare qualsiasi concatenazione di contenitori

**richiesta 2, generalizzare di più**

``` c++
#include <list>
using namespace std;
//contenitore di contenitori di tipo T, avrei potuto chiamarlo come volevo
template <typename Iter1, typename Out>
void f(Iter1 first, Iter1 last, Out, out) {

  for ( ; first != last; ++first) {
    //Nome tipo elementi puntati da iteratore
    //standard nuovo
    const auto cont2 = *first;
    //altrimenti
    typename typedef std::iterato_traits<Iter1>::value_type Cont2;
    typename typedef Cont2::const_iterator Iter2 Iter2;
    
    const Cont2& cont2 = *first;
    //devo fare il ciclo sulla sequenza puntata
    for (Iter2 first2 = first->begin(), last2 = first1->end(); first2 != last2; ++first2) {
      *out = *first2;
      ++out;
    }
  }
}
```
> Morale: è difficile fare cose semplici in maniera generica

Un problema che si ha è che si chiama metodo begin(), quindi funzionerà solo su oggetti che implementano metodi begin(). Ma ci sono strutture, come gli array del C, che non implementano il metodo begin(). Quindi queste funzioni funzioneranno solo per contenitori che implementano begin() e end(). Allora si implementano delle funzioni templatiche esterne.

**vedi /usr/include/c++/5.4.0/bits/range_access.h** metodi begin() e end().
