Se ci passano uno stack vuoto ma di capacità 3 milioni devo obbligatoriamente allocare tutto quello spazio non usato? NO
Deciadiamo di creare la copia allocando solo spazio necessario. Se contiene 12 elementi la capacità sarà di 12 elementi. Vogliamo sia una copia del vector passato (make_copy).
 
 ```c++
 static value_type* make_copy(const value_type* src, size_type src_size, size_type dst_capacity);
 ```

allocazione potrebbe lanciare eccezione (dst_capacity) e copia di ciascun elemento potrebbe lanciare eccezione.
Qua ci sono due operazioni che sono sotto il nostro controllo, allocazione e costruzione. La sfortuna è quando la prima va a buon fine e succede qualcosa di brutto in quelle successive.

Implementazione make_copy

``` c++
Stakc::make_copy(...){
	assert(dst_capacity >= src_size);
	//allocazione risorsa
	value_type* dst = new value_type[dst_capacity];
	//se operazione va a buon fine restituisce puntatore a primo elemento allocato
	try{
	//L'obiettivo del try è proteggere la risorsa da eccezioni
		//Proviamo a copiare gli elementi
		for(size_type i = 0; i < src_size; i++){
		//è assegnamento per copia! Dovrà distruggere cosa c'è dentro dst[i] e copiare src[i] dentro dst[i]
		dst[i] = src[i];
		//la distruzione non lancerà mai eccezione!
		}
	}
	
	catch(...){
		//qualcosa è andato storto
		//dellochiamo la risorsa (distruggiamo e deallochiamo, dall'ultima fino alla prima (incluse casette di default))
		delete[] dst; //non può lanciare eccezione
		
		// e rilanciamo l'eccezione per essere neutrali
		throw;
		
	}
		
		return dst;
	}
```
le due top, implementate in modo identico ma ritornano tipo diverso

``` c++

inline stack::value_type&
Stack::top(){
	//NOTA: è responsabilità del chiamante assicurarsi che lo stack contenga almeno un elemento
	assert(size_ > 0);
	return vec_[size_ -1]; //si può avere wrap around perchè intero senza segno
}

//non rilascio risorse qua, lo farò in un altro momento
inline void
Stack::pop(){
	assert(size_ > 0); //colpa dell'utente
	--size_;
	assert(OK());
}

inline void
Stack::push(const value_type& elem) {
	if(size_ == capacity_) {
	//se stack pieno
	size_type new_capacity = 2*size_ + 1;
	value_type* temp = make_copy(...);
	}
	
	//altrimenti sovrascriviamo elemento per copia
	vec_[size_] = elem;
	//registro che abbimo un elemento in più
	++size;
	assert(OK());
	//siamo exception safe?
	//lo siamo se gli operatori di copia sono anche loro exception safe forte!
	//perchè non dobbiamo preoccuparci di tornare indietro nessuna operazione lancerà eccezione
}
```

Nel costruttore di copia siamo exception safe forte? Si, perchè la make_copy è exception safe in maniera forte.

``` c++
inline void
Stack::push(const value_type& elem) {
	if(size_ == capacity_){
	//devo espandere la struttura dati, calcolando la nuova dimensione, con fattore moltiplicativo > di 1(?)
	size_type new_capacity = 2*size_ +1 ;
	//questa è la nostra risorsa che è esposta a rischi.
	//perchè non ci metto try? Perchè so che la mia risorsa che andrà in un posto che è protetta...
	value_type* temp = make_copy(vec_, size_, new_capacity);
	//da qui (e fino alla fine del blocco) ho solo operazioni che non possono lanciare eccezioni
	delete[] vec_;
	//se succede qualcosa viene invocato il ditruttore e quindi ciò che sta in vec_ viene deallocato
	vec_ = temp;
	//ora abbiamo spazio per fare la push
	capacity_ = new_capacity;	
	}
	vec_[size_] = elem;
	//se lancia eccezione, usciamo con questa
	//è cambiata la struttura? si, ma per l'utente logicamente non ha importanza, lastruttura contiene sempre gli stessi elementi
	//fisicamente diverso ma uguale per l'utente
	++size;
	asser(OK());
}
```
Quindi per rendere tutto exception safe non abbiamo applicato idioma RAII, ma solo un blocco try catch.

(nm --demangle stack.o)

Vogliamo vedere nel test se lo stack ha deallocato le risorse. Allora ci inventiamo un tipo di dato T, che tiene traccia del numero di istanze create/distrutte.

Quando spazio conpush finisce bisogna riallocare, quindi ditrugge oggetti vecchi.

Nonostante il lancia dell'eccezione vogliamo che tutte le risorse vengano rilasciate.
Test di proprietà exception safety.

**Valgrind:**strumento per fare test. Esegue il programma in ambiente separato e ogni volta che chiede risorsa la da con più informazioni, per sapere chi l'ha chiesto, quando, che dimensione, etc...

Se mettiamo un tipo non costruibile per default non riusciamo a fare uno stack ocn quel tipo
Gli oggetti del mio tipo devono avere costruttore di default, devono avere assegnamento per copia, costruttore di copia (?), e distruttore. Tutte le altre proprietà di T non ci interessano, quindi possiamo templatizzare la classe.

STACK TEMPLATE

``` c++
template <typename T>
class Stack {
public:
	typedef T value_type;
	
	explicit Stack(size_type capacity = 16);
	
	Stack(const Stack& y); 
	/* ma se è di tipo T, quello che tipo è? 
		posso scrivere anche Stack<T>& y, 
		quando siamo dentro lo scope della classe templatica 
		se scrivo solo nome del template il nome è sottinteso
	*/
private:
}
```

Metodi inline:

``` c++

template <typename T>
inline
//dopo :: sono nello scope della classe e vale l'abbreviazione Stack<T>
Stack<T>::Stack(const size_type capacity) : vec_(new value_type[capacity == 0 ? 16 : capacity])...
{
...
	assert(OK());
	
	/* la funzione membro non è templatica! Una volta fissato il tipo vi sarà una sola funzione.
		Il template è per il tipo della classe.
		Si possono avere classi non templatiche ma che contengono metodi templatici (es. costruttore).
		Oppure si possono avere classi templatiche con membri templatici all'interno, quindi è doppiamente templatica
	*/
}..

template <typename T>
//non si è ancora nello scope bisogna specificare il tipo!
inline Stack<T>&
stack<T>::operator=(const Stack& y) {...}

//quello dovrebbe essere un nome di un tipo.
/* quando si ha a che fare con i template, il compilatore non sa come potrebbe essere instanziato il template
	Abbiamo syze_type che dipende dal tipo del template. (specializzazione/istanziazione)
	La specializzazione corrisponde al caso per il quale esiste un particolare valore del parametro per il quale non
	volgliamo usare il codice generico ma del codice speciale.
	Che relazione esiste tra una classe istanziate e una specializzata? Nessuna
	size_type è un nome di un tipo o di un valore? Il compilatore non lo sa, di default sarebbe un valore,
	quindi in questo caso ci si trova con errore e quindi bisogna specificare che è il nome di un tipo
*/
template <typename T>
inline Stack<T>::size_type // GIUSTO: inline typename Stack<T>::size_type
Stack<T>::size() const {
	return size_;
}
/* regola, se prendi una classe templatica e tiri fuori il nome di qualcosa il compilatore 
	non sa cosa sia e devi specificare se è tipo o valore(default) */
```
Con le classi templatiche, generò solo i metodi usati. Sarebbe inutile generarli tutti. Istanziazione ondemand, benefici:
* si evita code bloating, istanzio template enorme, vagonate di codice, eseguibile enorme
* si è visto che per esempio tutte le volte che scriviamo una funzione nella classe templatica, usiamo alcune funzionalità della classe templatica, creando una dipendenza. Se mettiamo insieme tutte le richieste l'oggetto T diventa abbastanza complicato, quindi se istanziamo un oggetto che non usa tutte le funzionalità, l'importante è che non le forniamo.

Cosa fa il compilatore quando guarda il codice template?
Il compilatore si trova a che fare con due tipologie di codice diverse l'una dall'altra. Uno ha a che fare con uno o più parametri templatici (es costruisci T), questo è chiamato codice dipendente. Per analizzarlo il compilatore deve sapere quale è il valore vero dei parametri, quindi spesso non ci fa nulla.
Invece ci sono porzioni che non dipendo no da parametri template, il compilatore sa tutto e quindi può fare tutti i controlli che servono.
Il compilatore lavora in due modalità distinte.
Solo quando si va ad istanziare l'oggetto T lavora sul codice dipendente. Ma quando siamo in un'unità di traduzione diversa il compilatore vede solo il risultato del preprocessing, quindi gli header e quindi deve lavorare in tutte le unità di traduzione, ma non può perchè l'implementazione non è inclusa, quindi devo includere le implementazioni nell'header.
L'implementazione deve stare in un header file con i template, sennò compilatore non può generare codice.
La classe stack diventa header only.

Perchè la pop non restituisce il valore rimosso?
value_type pop(); // restituisce per valore non riferimento
Non viene restituito valore perchè il metodo non può essere implementato con exception safety forte!
Il requisito forte vuole che se qualcosa va male si torni indietro senza modifiche. Con la pop non c'è modo di farlo.
