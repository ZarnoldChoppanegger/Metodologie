# Exception safety #

## Requisiti: ##

### Liello base ###

> 1. il codice non "ruba" risorse

> 2. il codice è neutrale rispetto alle eccezioni, ovvero se qualcuno che ho invocato lancia un'eccezione, l'unica cosa che voglio fare è catturarla temporaneamente per pulire e rilanciare fuori in modo che tutti siano avvisati e possano pulire 

Queste  danno il livello base, le garanzie base.

Il livello superiore potrebbe essere: io lavoro su dei servizi che invoco e alcuni di questi lanciano eccezioni perchè vanno male. Io ho provato a fare una op sulla struttura dati, e quindi al momento dell'errore la mia struttura dati in che stato è?
Se venite interrotti a metà del lavoro in che stato ritorna la struttura dati, rispetta l'invariante? Con una **semantica di tipo transazionale**, se l'operazione non va a buon fine ritorna eccezone e lo stato della struttura rimane uguale a quello di prima dell'invocazione. A differrenza del requisito base permette di essere in uno stato predicibile e quindi è possibile continuare l'esecuzione senza avere undefine behaviour. es. uso un vector quello che mi ritorna è **sempre** un vector **ben formato**, lo stato del mio oggetto è fatto bene e soddisfa l'invariante, posso almeno distruggerlo per riprendere le risorse.

> 3. gli oggetto sui quali si stava lavorando soddisfano le loro invarianti (e quindi sono distruggibili senza causare UB)

### Livello forte ###
> **Semantica atomica** o tutto o niente. Le operazioni su oggetti oltre a garantire il livello base, possono solo avere successo o nessun effetto. Questo è fondamentale per operazioni base come la push_back(), insert(), etc... 

### Livello no-throw ### 
> Le mie operazioni andrano sempre a buon fine, **non lanceranno mai eccezioni**. Le funzioni che godono di queste livello sono funzioni ausiliari di supporto, e devono avere questo livello (es. op per rilasciare risorsa, non è che non possono lanciare eccezione ma se lo fanno se la gestiscono in maniera neutrale e devono **USCIRE** in modo normale **NON** eccezionale). **I distruttori non devono mai lasciare uscire un'eccezione, altrimenti demoni usciranno dal naso**

Come facio a essere sicuro che la mia funzione non lasci uscire eccezione?

* segno eccezione in log (brutto)

* rifletto sulle operazioni che faccio: ci sono operazioni che sono garantite che non propagano eccezioni? (es. costruttori distruttori, devono rispettare contratto, eventuali funzioni che sappiamo non lanciano eccezioni, assegnamento a tipi built-in e tutte le operazioni su questi, funzioni swap su built-in e tutte quelle funzioni che per documentazione lo promettono che non fanno uscire eccezioni)


Contenitori a dimensione variabile, se esauriamo capacità dobbiamo chiedere risorse che sono sotto il nostro controllo.

## Costruzione classe Stack ##
Progettare pila che contenga oggetti di qualunque tipo, dinamica. Si usa c++98.
Programmare usando direttamente funzioni e classi templati a volte è più difficile, perchè compilatore dà messaggi meno significativi di quelli non templatici.
Si svilupperanno tre versioni, una non templatica con un tipo fissato utilizzando quante meno operazioni di modo da usare questa classe in quella templatica (seconda versione); questa è una tecnica comune di fare "clone" di una classe. 
La terza versione assomiglierà  più ai contenitori stl.

``` c++

/* Perchè inizia con la maiuscola? Può essere utile usare convenizioni (tipi maiuscola, variabili minuscola) Quindi quando si lavora su un progetto bisogna adeguarsi alle convenzioni */
	
#include "Stack.hh"

/* test per scrivere l'interfaccia della nostra classe */

void prova() {
	
	Stack s1;
	/* si fanno queste perchè non sappiamo come è implementato lo stack, può essere in maniera efficiente e quindi si sanno subito le dimensioni, oppure non efficiente e quindi per trovare le dimensioni è costoso */
	assert(s1.size() == 0);
	assert(s1.is_empty());
	
	/* semantica per valore, si intende sempre come copia a meno che non specifichiamo il riferimento, quindi fa copia profonda di default */
	stack s2(s1);
	
	/* FRAME CONDITION*/
	assert(s1.size() == 0 && s2.size() == 0);
	
	s2.push("aaa");
	assert(s1.size() == 0 && s2.size == 1); //vogliamo assicurarci che s2 sia cresciuto
	
	//questo non rompe invarianti
	T& t = s2.top();
	assert(t == "aaa");
	
	/* la diff da quella in cima è che quella è una costruzione diretta, questa  potrebbe
		essere interpretata come costruzione indiretta. Con la costruzione diretta si può fare
		conversione implicita, con quella indiretta il compilatore darebbe errore*/
		
	Stack s3 = s2;
	
	s3 = s2 = s1;
	
	//[...]
}
```

Lo stack dovrebbe essere ridimensionabile, non posso permetteermi di avere uno stack grande quanto il numero di elementi. Allora ha due concetti: quanti elementi sono memorizzati (size) e capacità, quanti elementi posso memorizzare, questo serve per chiedere risorse al sistema, quando voglio aggiungere capacità, copio tutti gli elementi in nuova locazione con capacità più grande (costo lineare al numero di elementi che si possono memorizzare).
Uso tipo senza segno per variabile che determina dimensioni. Il tipo dei valori contenuti nello stack (typedef T value_type;) e gli si da un alis in modo da poterlo chiamare più tardi. Quindi (value_type* vec_;) è un puntatore a un tipo value_type, quando sforo faccio riallocazione. 

``` c++
/*possiamo fare un costruttore di default, del quale possiamo specificare la dimensione e con explicit evitiamo che si possa fare conversione implicita*/

explicit Stack(size_tyoe capacity = 16);

void swap(Stack& y); //scambia il contenuto con quello di y

void push(const value_type& elem); //si vuole copiare non fare modifiche

void pop();

bool is_empty();

value_type& top();
const value_type& top() const; //è essenziale restituire riferimento costante

/* sono due top diverse, una è const l'altra no. Per decidere se due funzioni sono diverse bisogna guardare nome e tipo e parametri funzione. Le due funzioni hanno argomenti diversi e quindi vanno in overloading.
es. T& t = s2.top() quale funzione chiamata?  la prima match perfetto, nella seconda conversione di qualificazione e quindi vince la prima e abbiamo un riferimento modificabile, questo però non ha nulla a che vedere con la risoluzione dell'overloading, se scrivevo const T& t = s2.top() non cambiava nulla. */

bool OK() const; //è l'invariante di classe

```
Implementazione:

``` c++
bool Stack::OK() const{
	if(capacity == 0) {
	#indef NDEBUG
		std::cerr << "Errore"
	#endif //NDEBUG
		return false;
	}
	
	if (vec_ == 0){
	#ifndef NDEBUG
		//errore
	#endif //NDEBUG
		return false;
	}
	
	//Tutto ok
	return true;
	
	/* in realtà però non c'è modo di controllare che sia stata allocata la diensione chiesta, ci sono proprietà non verificabili perchè non codificabili come per esempio la posizione di un puntatore*/
	
}

```
Le inline

``` c++
/* contratto wide, fino a size 16 imposto io la dimensione*/
inline
Stack::Stack(const size_type capacity) 
	: vec_ (new value_type[capacity == 0 ? 16 : capacity]), //non si può :vec_(new value_type[capacity_]) capacity_ non ancora inizializzato
	capacity_(capacity == 0 ? 16 : capacity),
	size_(0) {
		assert(OK());
		}

/* funziona? c'è da chiedersi se siamo exception safe. Cosa può andare male? 
	La new, questa fa: 1. alloca memoria grezza, quanta? quanto è il sizeof(value_type)? 
	Il sistema restituisce spazio necessario. 2. si chiama il costruttore di default n volte per inizializzare gli oggetti dal primo verso l'ultimo. 
	OSS: pensavamo stack fosse vuoto, invece contiene oggetti.
	Prima cosa che può andare storto è che il sistema non dia memoria, eccezione bad_alloc.
	Oppure l'allocazione ha successo e durante l'inizializzazione si hanno altro allocazioni, bad_alloc, oppure eccezioni per  altri possibili errori. Possiamo conoscerli tutti gli errori? NO
	Di fronte a queste eccezioni siamo exception safe?
	La new da garanzia che se da bad_alloc rilascia risorse. Quindi noi cosa dobbiamo fare?
	Niente, lasciamo che l'eccezione si propaghi verso l'uscita.
	L'allocazione va a buon fine, e verso la fine c'è errore, la new ancora è exception safe e disfa tutto nell'ordine inverso,  e il costruttore ha la garanzia di no-throw quindi nessuno lancia un'altra eccezione.
	Quindi anche se si lancia eccezione noi non siamo con nessuna risorsa e quindi non dobbiamo preoccuparcene.
	Il mio costruttore quindi è execption safe, requisito forte e fornisce queste garanzie sotto le condizioni che la new si comporta bene e che il distruttore dell'oggetto value_type non lanci eccezione*/

inline
Stack::~Stack(){
	delete[] vec_;
	
	/* la delete comincia dall'ultimo degli elementi e li ditrugge, sapendo che non verranno generate eccezioni perchè il costruttore di value_type non lascia uscire eccezioni.*/
}

/* quando abbiamo regola dei tre dobbiamo ridefinire sia operatore di copia che di assegnamento e assicurarsi siano exception safe. Si definisce per bene una di queste funzioni e l'altra la si implementa con un template*/

inline Stack&
Stack::operator=(const Stack& y) {
	/* prima facciamo la copia e poi distruggiamo risorse perchè se qualcosa va male devo tornare in dietro */
	Stack temp(y);
	swap(temp);
	return *this;
	
	/* così implementiamo un operatore di assegnamento senza ridefinirlo e in modo exception safe*/
}

/* funzione di servizio, no-throw*/

Stack::swap(Stack& y){ std::swap(vec_m, y.vec_); std::swap(size_,
	y.size_); std::swap(capacity_, y.capacity); } 
	
```
