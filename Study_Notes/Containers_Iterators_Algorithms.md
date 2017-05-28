## Container(s) ##
**Contenitori sequenziali:** gli elementi al loro interno sono definiti in un ordinamento e si trovano sempre nella stessa posizione.
**Contenitori associativi:** gli elementi al loro interno sono associati ad una chiave e la loro ricerca avviente per mezzo di questa.

Lo standard non definisce quale deve essere la struttura interna di un contenitore. Ne definisce però l'interfaccia e complessità. L'implementazione del contenitore è a carico dell'implementatore che deciderà di fare le cose più appropriate per ottimizzare.

### Vector ###
``` c++
template<typename T, 
         typename Allocator = std::allocator<T>>
class Vector;
```
* Struttura dati sequenziale, simile a un array. Gli elementi sono adiacenti l'uno all'altro, quindi è possibile accederci non solo mediante iteratori ma anche mediante offset su regolari puntatori agli elementi del vector. 
* Il vector occupa più memoria di un normale vettore statico, questo perchè viene allocato più spazio per futuri inserimenti e quindi evitare riallocazioni frequenti, ma solo quando necessario. È possibile vedere lo spazio allocato mediante la funzione membro `capacity()` e ritornare al sistema la memoria non utilizzata mediante la funzione `shrink_to_fit()`, implementata con `copy & swap`.
* Allocazione di memoria è in generale un'operazione costosa, quindi è possibile allocare una quantità di memoria predefinita mediante la funzione membro `reserve()`, eliminando quindi riallocazione.
* [insert()](http://en.cppreference.com/w/cpp/container/vector/insert) ha diverse interfacce, lo scopo principale è inserire uno o n elementi prima di una data posizione. Durante gli inseriementi gli iteratori possono invalidarsi, quindi bisogna stare attenti. L'**inserimento di un solo elemento** è implementato con **exception safe forte**, quello di più elementi no, quindi dopo tanti inseriemnti, se questi non vanno a buon fine, non conosciamo nulla della struttura del vector.

``` c++
iterator insert( iterator pos, const T& value );
iterator insert( const_iterator pos, size_type count, const T& value );
```
* [remove()](http://en.cppreference.com/w/cpp/container/vector/erase) rimuove gli elementi in una data posizione o in un range. 

### Map ###
La map è un container associativo ordinato, che contiene coppie di chiavi-valori con chiavi uniche.
Le chiavi vengono ordinate usando la funzione `Compare`, la quale determina che due chiavi sono equivalenti (non uniche) se nessuno dei due risulta essere minore dell'altro. Ovvero il criterio di ordinameto è [Strickt Weak Ordering](https://en.wikipedia.org/wiki/Weak_ordering).

## Algoritmi ##

* Gli algoritmi della STL sono scritti in modo da poter essere utilizzati su tutti i contenitori della STL. Questo è possibile poichè gli algoritmi non lavorano sui contenitori ma con gli iteratori dei contenitori. Cioè lavorano su **sequenze** definite da coppie di iteratori (per input) o singoli iteratori (per output).
* Quando si lavora su due sequenze gli algoritmi prendono come parametri la coppia di iteratori identificanti la prima sequenza (inizio e fine), e un iteratore identificante la seconda sequenza (inizio). Non è richiesta anche la fine per la seconda sequenza poichè si ha la precondizione che la seconda sequenza sia almeno lunga quanto la prima.
* Questo può portare l'utente a fare errori, come scrivere su locazioni di memoria non valide, poichè ha sbagliato a passare gli iteratori o perchè questi sono stati invalidati. Sarebbe più semplice per l'utente che gli algoritmi lavorassero sui contenitori direttamente, in questo modo si eviterebbero tutti i possibili errori, ma ciò renderebbe gli algoritmi poco generici, questi infatti lavorerebbero solo su specifici contenitori e richiederebbero specifiche implementazioni. Essendo i contenitori N e gli algoritmi M, il numero di algoritmi da implementare sarebbe N * M. Oltretutto  non è possibile lavorare su sottosequenze dei contenitori se non mediante gli iteratori.
* La maggiorparte degli algoritmi al termine dell'esecuzione ritornano un iteratore, tranne in rare eccezioni quando bisogna ritornare un oggetto di tipo `pair`, costituito da una coppia di iteratori. Il motivo fondamentale sta nel fatto che quando la STL è stata creata non c'era ancora la semantica di spostamento, quindi l'unico modo efficiente per ottenere informazioni dagli algoritmi era mediante gli iteratori. Se l'algoritmo ritorna l'iteratore di fine per indicare "non trovato".
* Ci sono algoritmi con predicati e algortimi senza predicati. Questo incrementa la flessibilità della STL e i suoi usi. Le funzioni che prendono come parametro anche un predicato hanno come suffisso `_if`; questa distinzione viene fatta per disambiguare, un predicato infatti a volte può essere interpretato come un elemento da confrontare con gli elementi della sequenza. 
* È bene che i predicati non modifichino mai gli elementi su cui vengono applicati nell'algoritmo, questo perchè rende oscure le operazioni fatte sull'oggetto. Per evitare incidenti, passare oggetti ai predicati per `const reference`.
* Allo stesso modo i predicati non dovrebbero avere uno stato che cambia, alterando il significato dell'operazione. Gli algoritmi copiano un predicato e quindi il cambiamento di stato può portare a comportamenti indefiniti. 
* La complessità degli algoritmi è specificata dallo standard

**Scrivere alcuni algoritmi fatti a lezione**
* **search()** algoritmo che permette di ricercare una sequenza come sottosequenza di un'altra. Utile per le stringe. Per trovare un solo elemento usare `binary_search()` oppure `find()`.
* **copy(b, e, out)** copia gli elementi del range `[b, e)` nella sequenza descritta da `out`, il quale è un iteratore di output. Quindi questo significa che si possono scrivere elementi su qualsiasi contenitore basta che questo possa essere descritto con un output iterator.

``` c++
void f(list<int>& li, ostream& out)
{
  copy(li.begin(), li.end(), ostream_iterator<int>(out));
}
```

Si può però incorrere nel problema di scrivere in locazioni nelle quali non bisogna scrivere. Allora è consigliabile usare gli **inseritori (inserter)**, cioè classi che inseriscono elementi nei contenitori chiamando gli opportuni iteratori. Ci sono `inserter`, `back_inserter` e `front_inserter`.

* **unique()** algoritmo che "elimina" dal contenitore tutti gli elementi uguali adiacenti, lasciandone uno solo. Quello che in realtà accade è che gli oggetti unici vengono spostati all'inizio della sequenza. L'algoritmo quindi restituirà l'iteratore indicante la fine della sequenza con elementi unici. Bisogna quindi aggiornare l'iteratore di fine del contenitore, poichè si rischia di fare operazioni su elementi spazzatura.

* **remove()** rimuove tutti gli elementi di una sequenza che sono equivalenti a una determinato valore o rispettano una condizione. In realtà, come per la unique(), questo sposta gli elementi da eliminare alla fine della sequenza. Quello che ritornerà sarà un iteratore che punta alla fine della sequenza senza i valori eliminati.
* **replace()** rimpiazza i valori della sequenza con altri.

``` c++
std::string s {"*questa*e`*roba*bella**"};
 auto p2 = std::remove(s.begin(), s.end(), '*');
    
std::copy(s.begin(), p2, std::ostream_iterator<char>(std::cout));
    
std::cout << '\n';
// la sequenza è stata modificata
// alla fine si trovano degli elementi spazzatura
// bisogna aggiornare l'iteratore di fine
std::cout << s << '\n';
// così è meglio
s.erase(p2, s.end());
std::cout << s << '\n';
```

* **swap()** scambia i valori tra due elementi in maniera molto efficiente. **swap_ranges(b, e, b2)** scambia gli elementi tra i due range, `[b, e)` e `[b2, b2+(e-b))`. Dal c++11 vi è l'overload della **swap()** che prende come parametri due array e invoca la **swap_ranges()**.

* **sort()** ordina una sequenza di elementi. Il confronto è fatto di default con l'operatore `<`, per chiedere ai tipi confrontati solo l'overload di questo operatore, e l'ugualianza avviene come `!(a < b) && (!b < a)`. L'algoritmo richiede che gli iteratori usati siano `random-access iterators`, questo significa che su strutture dati come le liste non può essere usata la sort(), a meno che non si copi il contenuto della lista in un vector, riordinare gli elementi e copiarli nuovamente nella lista. 

* Quando si scorre una sequenza si testa la fine di questa usando l'operatore `!=`. La ragione è data in parte perchè è un'espressione più precisa, in parte perchè se si usasse l'operatore `<`, questo potrebbe essere usato solo sui `random-access iterators`.

## Iteratori ##
> Gli iteratori esistono principalmente per isolare gli algoritmi dalle strutture dati (contenitori) su cui operano e vice versa

Esistono cinque categorie di iteratori:

* **input iterator:**
  * è possibile avanzare usando `++`
  * si possono leggere ripetutamente gli elementi mediante `*`
  * possono essere confrontati con `!=` e `==`
  * sono gli iteratori forniti da `istream`
* **output iterator:**
  * si può avanzare con `++`
  * non possono essere confrontati
  * si possono scrivere gli elementi nella sequenza una sola volta con `*`
  * sono gli iteratori forniti da `ostream`
* **forward iterator:**
  * si avanza con `++`
  * possono essere confrontati con `!=` o `==`
  * si possono leggere e scrivere (se non costanti) gli elementi con `*`
  * se punta a un oggetto di una classe, è possibile usare `->` per dereferenziare i membri
  * sono gli iteratori offerti da `forward_list`
* **bidirectional iterator:**
  * si può iterare in avanti con `++`, ma anche indietro con `--`
  * possono essere confrontati con `!=` o `==`
  * si può leggere e scrivere con `*`
  * se punta a un oggetto di una classe, è possibile usare `->` per dereferenziare i membri
  * sono gli iteratori offerti da `list`, `set` e `map`
* **random-access iterator:**
  * si può iterare in avanti con `++`, ma anche indietro con `--`
  * si può accedere alla n-esima posizione con `+=`, `+`, `-=`, `-`
  * si può leggere o scrivere un elemento con `*` oppure con `[]`
  * se punta a un oggetto di una classe è possibile usare `->` per dereferenziare i membri
  * è possibile trovare la distanza tra due random-access iterator
  * è possibile confrontare due random-access iterator con `!=`, `==`, `<=`,`<`,`>=`,`>`
  * è il tipo di iteratore offerto da `vector`

Gli iteratori sono concetti non classi, quindi la gerachia che sussiste tra questi non è implementata mediante ereditarietà.

### Iterator traits ###
> Think of a trait as a small object whose main purpose is to carry information used by another object or algorithm to determine "policy" or "implementation details".

**iterator_traits:** è una classe fornita dalla STL contenente informazioni riguardanti un iteratore. Questo serve quando si vuole implementare un algoritmo in modo generico solo in termini di iteratori.
Se `Iterator` è un iteratore, `iterator_traits` deve fornire le seguenti informazioni riguardanti un iteratore:

``` c++
std::iterator_traits<Iterator>::difference_type
std::iterator_traits<Iterator>::value_type
std::iterator_traits<Iterator>::iterator_category
std::iterator_traits<Iterator>::reference
std::iterator_traits<Iterator>::pointer
```

Gli iterator_traits sono quindi come dei "passacarte". Perchè quindi usare gli iterator traits, se gli iterator comunque contengono queste informazioni al loro interno?
Perchè gli iteratori non sono gli unici costrutti con cui è possibile iterare su sequenze, ma è possiible farlo anche con i puntatori, i quali sono tipi built-in che non permettono di fare introspezione su di essi. Quindi si fa una **specializzazione parziale** degli iterator-traits, per puntatori non costanti `T*` e puntatori costanti `const T*`:

``` c++
template<typename T>
struct iterator_traits<T*>
{
  public:
    using difference_type = ptrdiff_t;
	using value_type = T;
	using iterator_category = random_access_iterator_tag;
	using pointer = T*;
	usign reference = T&;
}

template<typename T>
struct iterator_traits<const T*>
{
  public:
    using difference_type = ptrdiff_t;
	usign value_type = T;
	using iterator_category = random_access_iterator_tag;
	using pointer = const T*;
	usign reference = const T&;
}
```
### Iterator tags ###
È spesso necessario sapere il tipo di iteratore per avere un'implementazione più efficiente di un algoritmo. Quindi sapendo il tipo di iteratore a compile-time verrà scelata l'implementazione più adatta. Per facilitare questo sono stati introdotti gli **iterator-tags**, tipi che fungono da tag per la scelta dell'algoritmo migliore a compile time. Dato un iteratore `Iter`, `std::iterator_traits<Iter>::iterator_category` dovrebbe definire il tipo di tag che più descrive l'iteratore passato.

``` c++
namespace std {
  struct input_iterator_tag { };
  struct output_iterator_tag { };
  struct forward_iterator_tag: public input_iterator_tag { };
  struct bidirectional_iterator_tag: public forward_iterator_tag { };
  struct random_access_iterator_tag: public bidirectional_iterator_tag { };
}
```
Questo meccanismo per la scelta dell'implementazione migliore, costituito da tag e risoluzione dell'overload, è chiamato `tag-dispatching`.

### Iterator Adaptors ###
La STL fornisce "adattatori" che danno la possibilità di generare operazioni utili legate agli iteratori a partire da un dato tipo di iteratore:

**reverse_iterator:** permette di iterare in senso inverso come se si iterasse in modo normale (es. ++ scorrerà la sequenza all'indietro invece che in avanti)
**front_insert_iterator:** inserisce elementi all'inizio della sequenza
**back_insert_iterator:** inserisce elementi alla fine della sequenza
**insert_iterator:** inserisce elementi in qualsiasi posizione della sequenza
**raw_storage_iterator:** permette di scrivere su spazio non inizializzato

Gli adattatori `front_inser_iterator`, `back_insert_iterator` e `insert_iterator`, inseriranno elementi nelle sequenze, chiamando rispettivamente i metodi `push_front()`, `push_back()` e `insert()`. Questi adattatori sono accompagnati da funzioni dette inserter (`front_inserter()`, `back_inserter()`, `inserter()`), che permettono l'uso di questi adattatori con deduzione automatica a tempo di compilazione, questo per rendere il codice più conciso, semplice da scrivere e leggere.
Usare gli inserter è fondamentale quando si vogliono inserire elementi in un container in tutta sicurezza, poichè questo potrebbe avere dimensioni non adatte per ricevere elementi e quindi necessiterebbe di riallocazione. L'unico modo per riallocare solo nel momento necessario è usare la specifica funzione membro (`push_front()`, `push_back()` e `insert()`).

``` c++
std::istream_iterator<char> cin_begin(std::cin);
std::istream_iterator<char> cin_end();

std::vector<int> vi;

std::copy(cin_begin, cin_end, std::back_inserter(vi));
```

