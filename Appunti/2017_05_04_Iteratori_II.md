## Iteratori ##

Andare a vedere stl_algo.h funzione sort.

Algoritmo introspettivo: è algoritmo che tiene traccia di quello che fa dentro se stesso. Es quicksort tiene traccia di quello che fa, se arriva a un tempo troppo lungo allora cambia l'argoritmo di ordinamento, per esempio se l'array diventa piccolo allora passa all'insertion sort.

**set_union**, procedura di merge che dà l'insieme ordinato delle due unioni, funziona come il merge sort. 
* operatore < usato sempre così il tipo sottostante deve implementare solo il <.
* algoritmi che vanno a scrivere su una sequenza di output restituiscono il punto nel quale sono arrivati a scrivere.
* **_Compare** predicato che implementa il confronto, in questo modo non c'è più il bisogno di usare il <. La relazione di ordinamento non è banale: per esempio cosa ci dice che se confronto due stringe con < e > allora sono una minore dell'altra? Non è detto che il criterio di confronto sia totale. Quindi devo stare attento al criterio di confronto che viene usato.

* **equal**, quando faccio confronto di uguaglianza tra elementi strutturati, vorrei spesso sapere dove non sono uguali, allora si usa algoritmo **mismatch**, questo restituisce la posizione del primo elemento diverso. Quello che restituisce però è una **coppia di elementi**, quindi come faccio a restituirli in c++? Spesso quando scrivo algoritmi generici ho bisogno di restituire oggetti generici, per questo nella STL nell'header **utility** vengono implementate delle strutture di supporto, come **std::pair<Iter1, Iter2>**

```c++
template <typename Iter1, typename Iter2>
std::pair<Iter1, Iter2>
mismatch(Iter1 first1, Iter last1, Iter2 first2) {
  for( ; first1 != last1; ++first1, ++first2) {
    if (!(*first1 == *first2))
	 // nella classe non c'è deduzione automatica dei tipi
	 // diventa lungo da scrivere
	 // ma nelle funzioni funziona la deduzione, quindi...
	 return std::pair<Iter1, Iter2>(first1, first2);
	 // uso una funznione che crea la coppia a partire dai sue due argomenti
	 return std::make_pair(first1, first2);
	 }
  return std::make_pair(first1, first2);
}
```

Come è fatta la classe che deve fornire una coppia? stl_pair.h

Nella pair tutti i campi sono pubblici, inclusi anche i dati membro perchè non ci sono invarianti da soddisfare, quindi non faccio incapsulamento!

**template variadici** è un pacchetto lungo quanto vuoi di argomenti templatici, la tupla è una generalizzazione del concetto di coppia, lunga quanto vuoi che inizializzi a tempo di compilazione.

Esiste la versione di mismatch con il confronto:

``` `c++
template<typename Iter1, typename Iter2>
mismatch(Iter1 first1, Iter1 last2, Iter2 first2, BindPred equal) {
  for( ; first1 != last1; ++first1, ++first2) {
    if(!(equal(*first1, *first2)))
	  return std::make_pair(first1, first2);
   }
   return std::make_pair(first1, first2);
}
```
Convenzioni STL (sono 3):
* non so cosa siano i contenitori, lavoro sugli iteratori;
* se assumo che le sequenze abbiano la stessa lunghezza e quindi non metto il secondo iteratore di fine
* se faccio scrittura non si mette mai l'ultimo iteratore(???)

**Tipi membro:** **alias di tipi che vengono forniti all'utente per potersi riferire a quella classe!** `value_type` alias per riferirsi al tipo dei valori contenuti ad esempio in un vector.
**Tipi proxy** si comportano come valuetype ma in maniera diversa.
* **const_reference** è il tipo di riferimento a costante, quindi è l'oggetto che viene riferito costante, corrisponde a costante di value_type.
* **iterator**, iteratore sugli elementi del vector, non mi interessa come è implementato. È un iteratore a value_type che consente di modificare gli elementi, corrisponde a concetto di puntatore sul quale però posso fare operazioni(andare avanti e indietro):
* **const_iterator**, permette solo lettura.
* **reverese_iterator**, permette di andare all'indietro sulla sequenza come se andassi avanti, rigira l'ordinamento, assomiglia all'iteratore.
* **const_reverse_iterator**
* **difference_type**, corrisponde più o meno a size_type, ma con segno, serve a indicare distanza tra due elementi di un vector.

Di metodi begin nel vector ce ne sono due, uno const per essere chiamato da oggetti costanti e l'altro non costante. Ciò che cambia è il tipo di iteratore fornito come risultato, quindi non ti dà un iterator se l'oggetto è const ma solo un const_iterator. L'overloading viene usato per fare la cosa giusta. La stessa identica cosa viene fatta anche per end(). È fondamentale perchè gli algoritmi devono lavorare su coppie di iteratori dello stesso tipo, non può essere uno const e l'altro no. 

Ci sono anche cbegin e cend che ritornano iteratori costanti, sono stati introdotti dal c++11, perchè?

``` `c++
	std::vector<int> vi;
	
	// se faccio:
	std::vector<int>::const_iterator first = vi.begin();
	std::vector<int>::const_iterator last = vi.end();
	// non è permesso poiche` i tipi sono diversi
	// cioè const_iterator != iterator, tipi diversi
	
	//si può fare così, ma è lungo da scrivere
	std::vector<int>::const_iterator first = vi.cbegin();
	std::vector<int>::const_iterator last = vi.cend();
	
	//se avessi scritto
	auto first = vi.begin();
	// non c'è autodeduzione del tipo e quindi 
	// non capisce che deve essere costante
	
	stampa_tutti<int>::const_iterator last = vi.cend();
```
Quando la gente implementava i const_iterator metteva anche un costruttore per fare la  conversione da iteratore non costante a costante. Allora, hanno dato due funzioni supplementari per consentire al programmatore di farsi dare l'iteratore costante. Così evito costo della copia e conversione.

Guarda costruttore di vector che permette di inizializzare a partire da qualsiasi struttura usando gli iteratori.

``` `c++
int main() {
  
  int a[] = { 2, 3, 4, 7, 12, 14, 18 };
  
  //in automatico copio tutti gli elementi dall'array al vettore
  std::vector<int> vi(a, a + 7);
  
  const char* p = "Hello, world!";
  
  std::vector<int> vc(p, p + 7);
  
  //lo stesso anche per le stringe,
  //sono pseudo contenitori
  
  stampa_tutti(vi.begin(), vi.end());
}
```
Il costruttore è flessibilissimo, è davvero generico, può essere costruito a partire da qualsiasi sequenza.

Nel vector c'è anche metodo **reserve** che permette di allocare una determinata dimensione che è importante se sappiamo di dover allocare molti elementi ed evitare riallocazione. Questo è drammatico per sistemi hard-real-time, durante l'operazione critica voglio essere sicuro che l'allocazione avviene in certi momenti.

Nel vector l'operator[] non controlla se la posizine data è giusta, non lancia eccezione. Per fare controlli si usa la funzione at(), che nessuno usa per performance scarse.
Membro data(): permette di lavorare sul contenuto di un vector anche con quelle funzioni che usano array grezzi.
Nella vector non c'è push_front, perchè? Perchè non è efficiente e darebbe al programmatore l'idea che sarebbe efficiente. C'è la insert che è più complicata da usare e ti da l'impressione che sia più complesso inserire elementi in una posizione arbitraria. insert() restituisce un iteratore, perchè? Quale è il suo valore? Ho già l'iteratore, l'ho dato io! Non è sempre uguale, per esempio quando rialloca, la posizione di memoria cambia. Questo ti dice che tutte le volte che vai a fare un inserimento in un vector, a meno che non usi reserve, ci potrebbe essere stata una riallocazione quindi tutti gli iteratori potrebbero essere **stati invalidati**, sono diventati dei dangling iterators ed è una fonte di errori più comuni quando si usano vectors. 

``` `c++
for(auto i = vi.begin(), i_end = vi.end(); i != i_end; ++i) {
  if(*i == 'l')
    vi.insert(i, 'L');
  }
```
Questo codice è pieno di undefine behaviour, perchè non so quale è la capacità del vector, io faccio inserimenti se rialloca invalido TUTTI gli iteratori che sto usando (i, i_end).

Migliore:

``` `c++
for(auto i = vi.begin(), i_end = vi.end(); i != i_end; ++i) {
  if(*i == 'l') {
    vi.insert(i, 'L');
	++i; //elemento già controllato, vai avanti
	i_end = vi.end();
  }
}
```
