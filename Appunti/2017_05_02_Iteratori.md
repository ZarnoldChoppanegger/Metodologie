## Correzione non completa ##

*exe 2 conversioni e promozioni* 
*  conversione qualificazione, prende un **riferimento costante** e l'oggetto passato non è costante, non vale se faccio passaggio per valore
* lvalue->rvalue passa per copia e causa leggere il valore dell'lvalue e creare una copia

*exe3*
Aveva 3 concetti da applicare:
1. la const correctness, ogni volta che un parametro viene passato non deve essere modificato, vanno marcate queste cose come const. Dove:
   * metodo chiavi_accoppiate, const parapetri e funzione
   * check_inv, funzione costante
   * codifica, decodifica, firma_digitale, parametri in input marcati const, output no const, metodi marcati const perchè non modificano this
   * chiave_pubblica, metodo const

2. incapsulare i dati membro
   * private k_priv e k_pubbl, opzionale anche i metodi genera_chiavi, chiavi_accoppiate, check_inv

3. metodo per generare le chiavi non ha bisogno di lavorare sull'oggetto this, quindi tanto vale dichiarare il metodo static, lo stesso anche per chiavi_accoppiate

*exe5*

``` c++
struct Lock{
private:
  Scheda& s;
  //Scheda* s;

  //disabilito assegnamento e copia
  Lock(const Lock&) = delete;
  Lock& operator=(const Lock&) = delete;
public:
  //non si passa per const la scheda, sbagliato perchè non sappiamo se la scheda
  //deve essere modificata, nel testo era modificabile
  Lock(Scheda& scheda) : s(scheda) /*s(&scheda)*/ { get_lock_for(scheda); }

  //potevo aggiungere anche get
  
  ~Lock() { release_lock_for(s); }
};

void foo(...){
  {
    Lock lock1(s1);

    read_data(s1, data1);
    //release_lock_for(s1); rilasciato alla fine del blocco
  }

  Lock lock2(s2);
  read_data(s2, data2);

  Lock lock1(s1);
  write_data(s1, data2);
  write_data(s2, data1);
}
```
##  STL  ##

### Iteratori  ###

funzione che stampa tutti gli elementi di gualsiasi contenitore che ti dà l'iteratore
``` c++
template <typename Iter>
void stampa_tutti(iter first, Iter last) {
  for (Iter p = first; p!= last; ++p)
    std::cout << *p;
}
```

### Algoritmi ###
Si inventano un po' di algoritmi

Predicati su sequenze

Non predicati

find: trova in una sequenza la prima occorrenza del valore v.

Uso

``` c++
void gelati(const std::vector<std::string>& vg) {
  find(vg.begin(),  vg.end(), "menta");
}
```

Il risultato cos'è? Non voglio sapere cos'è il gusto menta ma dov'è!
Se io vado a cercare un elemento in una sequenza lunga mi aspetto mi restituisca una posizione.
Se non lo trovo? Devo ritornare una posizione che indica che non l'ho trovato

``` c++
void gelati(std::vector<std::string>& vg) {
  std::vector<std::string>iterator res = find(vg.begin(),  vg.end(), "menta");
  
  if(res != vg.end())  //undefine behaviour se vado oltre
	*res = "cioccolato";
  }
```
La find:

``` c++
template <typename Iter, typename Value>
Iter 
find(Iter first, Iter last, const Value& value);
```
Questa non sa niente di contenitori, ci sono solo gli iteratori.

``` c++
template <typename Iter, typename Value>
Iter 
find(Iter first, Iter last,  const Value& value) {
  for ( ; first != last; ++first) {
    if (*first == value)
	  return first;
  }
  
  return last;
}
    
```
Per convenzione gli iteratori si passano sempre per valore e non per riferimento perchè si sa che sono piccoli. Quindi quello che mi arriva è una copia di quello passato e lo posso modificare tranquillamente. Non ha senso passare per valore, il valore (Value), si passa **sempre** per **riferimento a costante**.
Se non trovo nessun valore ritorno quello dopo la sequenza (last).
Quand'è che questo algoritmo funziona? Quando first e last sono fatti bene, presi dallo stesso contenitore, quindi trovarsi in un range valido!
**range valido** quando vengono presi dallo stesso iteratore e in ordine. 
**Value** non serve per forza il tipo contenuto nella sequenza, basta che possa essere confrontato con il tipo della sequenza, usando conversione ovviamente. Questo è più generico, qualsiasi cosa posso confrontare va bene.
L'utente è obbligato a dare degli iteratori ben formati. È difficile scrivere asserzioni con uso generico.

Uso:

``` c++
int main() {
  
  std::vector<std::string> vg;
  vg:push_back("arancia");
  vg:push_back("banana");
  vg:push_back("cioccolato");
  vg:push_back("zenzero");
  
  std::string ag[] = { "arancia", "cioccolato"};
  
  auto res = find(vg.begin(), vg.end(), "menta");
  auto res = find(ag, ag + 2, "menta");
  
  if(*res != vg.end())
    *res = "cioccolato";
	
	stampa_tutti(vg.begin(), vg.end());
	stampa_tutti(ag, ag + 2);	
```
si sono inventati un modo per avere iteratori su qualsiasi contenitore.

Nella STL quando bisogna fare confronto di ordinamento si usa operato<=, quello che non è minore di nessun altro.

``` c++
template <typename Iter>
Iter max_elem(Iter first, Iter last) {
  if(first == last) return last;
  
  Iter current_max = first;
  ++first;
  for ( ; first != last; ++first) {
    //se è più piccolo di quello che ottengo con current_max
    if(*current_max < *first) //allora
	  current_max = first;
  }
  return current_max;
}
```
Se la sequenza è vuota vuol dire che first = last, non entro nel ciclo e restituisco current_max. 

E per il minimo? Usiamo >? No, perchè vogliamo chiedere il meno possibile ai tipi istanziati

Chi non ha capito fa così:
``` c++
template <typename Container>
typename Container::iterator
max_elem(const Container& cont){
  auto first = cont.begin();
  auto last = cont.end();
  
  ... 
  }
```
Non è quello che volevamo perchè funziona con un solo contenitore! Si perde di generalità! Vado a lavorare su tutto il contenitore, con gli iteratori invece posso lavorare su sottoinsiemi del contenitore, è più flessibile. Si perde generalità lavorando su contenitori, sia perchè non posso lavorare su altri sia perchè non posso lavorare su sottosequenze di questi.

``` c++
//un predicato è un qualcosa che soddisfa il concetto di funzione
//qualsiasi cosa che si comporti come una funzione
//funzioni predicate unarie, funzioni che le chiamo su un valore
//e lo convertono a un valore che è vero o falso
template <typename Iter, typename UnaryPredicate>
Iter
find_if(Iter first, Iter last, UnaryPredicate pred){
  for( ; first != last; ++first) {
    if(pred(*first))
	   return first;
	}
	return last;
}
```
Parametrizzo rispetto al comportamento del predicato. Quali sono i parametri che posso usare nel predicato?
* puntatori a funzione
* lamba expression
* oggetti funzione, oggetti che si possono comportare come funzioni

Esempio oggetti funzione:

``` c++
struct Lungo_Sei {
  bool operator() (const std::string& s) const {
    return s.size() == 6;
  }
};
```
Quindi potrei usarlo così:

``` c++
auto b = find_if(vg.begin(), vg.end(), lungo_sei);
//istanzia find_if<std::vector<std::string>::iterator, 
//                            bool (*)(const std::string&)>
```
Con oggetto funzione:

``` c++
auto b = find_if(vg.begin(), vg.end(), Lungo_Sei());
//istanzia find_if<std::vector<std::string>::iterator, 
//                            bool (*)(const std::string&)>
```

Molti degli algoritmi generici della STL hanno anche il predicato. Si mette _if quando la politica  con predicato non può essere differenziata da quella senza predicato.

Funzione count:

``` c++
template <typename iter, typename Value>
unsigned long
count(Iter first, Iter last, const Value& value){
  unsigned long c = 0;
  for( ; first != last; ++first) {
    if(+first == value)
	  ++c;
	}
	return c;
}

//oppure con predicato

template <typename iter, typename UnsignedPredicate>
unsigned long
count(Iter first, Iter last, UnsignedPredicate pred){
  unsigned long c = 0;
  for( ; first != last; ++first) {
    if(pred(*first))
	  ++c;
	}
	return c;
}
```
E si possono applicare a tutti.

Creare un algoritmo che è a sua volta un predicato:

``` c++
//se tutti gli elementi di una sequenza soddisfano un predicato
template <typename Iter, typename UnaryPredicate>
bool
all_of(Iter first, Iter last, UnaryPredicate pred) {
  for( ; first != last; ++first) {
    if(!pred(*first))
	  return false;
	}
  return true;
}

//esistono anche

any_of e none_of
```
Equal: confronto tra due sequenze. Bisogna mettersi d'accordo per la specifica. La seconda sequenza deve essere lunga almeno quanto la prima, può essere più grande.

``` c++
template <typename Iter>
bool equal(Iter first1, Iter last1, Iter first2);

//fa schifo la dichiarazione, perchè gli iteratori 
//possono essere di tipo diverso tra di loro quindi non è generico

//in questo modo possono essere anche di tipo diverso
template <typename Iter1, typaname Iter2>
bool equal(Iter1 first1, Iter1last1, Iter2 first2) {
  for ( ; first1 != last1; ++first1, ++first2) {
    if(!(*first1 == *first2)) //più generico caso mai qualcuno non ha dichiarato !=
	  return false;
	}
  return true;
}

//variante con controlli, non nella STL
template <typename Iter1, typaname Iter2>
bool equal(Iter1 first1, Iter1last1, Iter2 first2, Iter2 last2) {
  for ( ; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if(!(*first1 == *first2)) //più generico caso mai qualcuno non ha dichiarato !=
	  return false;
	}
  return true;
}

//versione con predicati
template <typename Iter1, typaname Iter2, typename BinaryPredicate>
bool equal(Iter1 first1, Iter1last1, Iter first2, BinaryPredicate pred) {
  for ( ; first1 != last1; ++first1, ++first2) {
    if(!(pred(*first1,  *first2)))
	  return false;
	}
  return true;
}
```
**for_each**

``` c++
template <typanem Iter, typename Fun>
void for_each(Iter first, Iter last, Fun fun) {
  for ( ; first != last; ++first)
    fun(*first);
}

//e quindi se definisco una funzione ausiliaria

void stampa(const std::string& s); //che stampa un solo gusto

//poi invece di fare stampa_tutti mi basta fare

for_each(ag, ag + 2, stampa);
```

Perchè si sono inventati le lambda expression? Perchè il programmatore scriveva dei cicli espliciti nel quale fare quella funzione, quindi mi devo spostare per andare a vedere come l'ho definita la funzione e poi è molto difficile da leggere. Con le lambda expression posso definire delle funzioni lì dove mi servono, senza scrivere codice per iterare sugli oggetti, con possibili errori nel ciclo.

Algoritmi che scrivono sequenze:

``` c++
template <typename In, typename Out>
<ritorno>
copy(In first, In last, Out out1) {
  
  for (, firs != last ; ++first, ++out) {
    *out1 = *first;	
  }
}

int main() {
  /* vs */
  /* ls */
  out = ls.begin();
  out = copy(vs.begin(), vs.end(), out);
  copy(vs2.begin(), vs2.end(), out);
}
```
Si suppone che la sequenza di output abbia una dimensione tale per poter scrivere tutto il contenuto di input, quindi niente controlli.
Il tipo di ritorno può essere void, ma a volte è utile sapere quando ho sovrascritto gli elementi, non posizionarmi in fondo ma dove mi ero fermato durante la sovrascrittura.
Tutte le volte che nella STL ho un algoritmo che lavora su seq di uscita, questo si fa ritornare un iteratore che indica il punto nel quale è arrivato a fare le scritture.

