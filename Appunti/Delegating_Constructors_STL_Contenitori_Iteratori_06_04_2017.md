	``` c++
	class C{
    template <typename T>
    void foo(T t);
	};
	
	template <typename T>
	void
	C::foo(T t);){
    //Fai qualcosa
	}
	//La classe ha potenzialmente infiniti metodi, dipendono da T
	
	template <typename U>
	class C{
    template <typename T>
    void foo(T t);
	};
	
	template <typename U>
	template <typename T>
	void
	C<U>::foo(T t)){
    //Fai qualcosa
	}
	
	/* Templatizzazione rispetto alla classe e rispetto al metodo della classe */
	
	
	template <typename U>
	template <typename U, typename T>
	voidC<U>::foo(T t, V v){
    //Fai qualcosa
	}
	```
	Si può avere un numero infinito di templatizzazione basta annidare le templatizzazioni
	
	``` c++
	template <typename U>
	class C{
    template <typename W>
    calss D {
    
    template <tupename T, typename 
    void foo(T t, V v);
    }
	};
	```
	
	``` c++
	Stack<double sd(si);
	```
	
	Darà errore! Non sa che funzione chiamare.Se si guarda il costruttore si vede che, in modo implicito, si fa la copia solo di stack che hanno lo stesso tipo di dato.
	Esiste una maniera per convertire in maniera implicita? NO. Il fatto che siano stati instanziati tutti e due come Stack non vuol dire niente. Se ho una funzione che vuole Stack di interi devo dare Stack di interi. Come posso fare per convertire? Tramite costruttore ma non di copia.
	
	``` c++
	Stack(const Stack<U>& y); 
	```
	Questa è una funzione membro templatica in una classe templatica. NON è costruttore di copia, e ha senso marcarlo explicit.
	
	``` c++
	template <typename T>
	template <typename U>
	Stack<T>::Stack(const Stack<U>& y)
	: //simile lista di inizializzazione di costruttore copia e poi push back come se piovesse { 
	}
	
	/* In c++ 98 non c'erano molte scelte, si rinunciava a lista di inizializzazione, diminuendo manutenzione
	*  ma con effetto collaterale di inizializzare tutto, distruggere e reiniziallizare tutto
	*/
	```
	**Delegating constructors (c++11)**
	delegare il lavoro di costruzione di un costruttore a un'altro chiamandolo nella lista di inizializzazione.
	
	``` c++
	template <typename T>
	template <typename U>
	Stack<T>::Stack(const Stack<U>& y)
	: Stack/y.size() == 0 ? 1 : y.size()){
    //Inseriamo gli elementi
    for(size_type i = 0; i < y.size(); ++i)
    {
    push(y.vec_[i]);
    }
	}
	```
	Quando posso usare questo blocco?
	Quando uso un tipo U che può essere convertito implicitamente a T.
	**Il programma però non compila**: nell'istanziazione del costruttore, lo Stack<int>:vec_ è privato! perchè siamo nella classe Stack di double e vogliamo toccare i metodi privati di Stack di int, quindi questo non si può fare.
	Come si può fare?
	1. creiamo una funzione ausiliaria che dia accesso in sola lettura agli elementi dello stack, ma allora non sarebbe più tecnicamente uno stack, accesso solo in cima
	2. dare accesso agli stack di double accesso alle parti private di double, dichiarando una funzione friend o classe friend.
	
	``` c++
	friend class stack;
	//così dico che io stesso sono amico di me...non è utile
	
	template<typename U> //il nome del parametro è solo per documentazione non è obbligatorio
	friend class Stack;
	
	//es. friend class Vector<T>
	//sono amici tutti i Vector del mio stesso tipo
	
	/* non importa quale sia il tipo U, l'altra classe Stack è mia amica */
	
	//quindi posso fare in linea di principio un unico costruttore perchè copre anche il caso per il tipo T (mio)
	
	template <typename U>
	explicit Stack(conts Stack<U>& y);
	
	/* ma si va in un'eccezione del linguaggio, perchè non vale per i costruttori,
    il costruttore di copia è speciale*/
	```
	
	## STL: Contenitori e Algoritmi ##
	
	### Contenitori ###
	
	* **vector<T>**
	* **deque<T>**
	* **list<T>** sono liste bidirezionali, doppiamente linkate, oltre all'elemento  successivo c'è anche il precedente
	* **array<T>** (c++11) i vector sono a dim dinamica, gli array a dimensione fissa, come array del c, ma essendo classi finite, hanno informazioni come dimensione, non decadono a puntatori, sono più comodi da usare
	* **slist<T>** (c++11) single listed list, solo in avanti
	
	Questi sono chiamati contenitori sequenziali, gli elementi sono definiti in un ordinamento, come li inserisco così li tiro fuori, si trovano alla stessa posizione. Con le liste non è così, sono più inefficienti, perchè si va a deferenziare un puntatore per ogni passo, perdendo **località**, quindi la cache funziona male, deve andare a prendere punti a caso nella RAM e costa.
	
	Altri contenitori sono chiamati **associativi**, creano associazione logica chiave valore:
	* **map** funzioni, associazioni, tabelle, dati valori ne restituisco altri
	* **multimap** dato un valore ne restituisco tanti
	* **set** mappe in cui tutta l'informazione sta nella chiave (es. insieme numeri matricola seguono metodologie)
	* **multiset** contenitori associativi ordinati, non sono sequenze, sulle chiavi utilizzate per fare l'accesso è definito un ordinamento, implementato tramite albero binario di ricerca bilanciato
	
	Da c++11 ci sono anche quelli non ordinati:
	* **unordered_map**
	* **unordered_multimap**
	* **unordered_set**
	* **unordered_multiset**
	Danno una complessità migliore rispetto a quelli di prima perchè usano matrici hash.
	
	Ci sono anche dei sotto contenitori, come le stringhe che possono essere trattare come dei contenitori. Cercare un carattere simile a cercare un valore in un contenitore
	
	### Algoritmi ###
	
	Gli algoritmi devono funzionare per tutti i contenitori ma questi sono implementati in maniera diversa.
	N contenitori, M algoritmi, quanti ne scrivo? N * M ... :(
	Quelli dello standard hanno pensato: 
	* usare codice templatico, ma abbastanza spesso, dato un algoritmo alle volte capita che questo abbia implementazioni più efficienti per alcuni contenitori
	* alle volte non interessava sapere come è fatto il contenitore sequenziale, ma interessava sapere come fare a scorrere il contenitore sequenziale e ci sono modalità diverse. Quindi di fronte al alto numero di contenitori ci sono poco modalità e quindi si individuavano dei concetti che astraevano il modo di scorrere i contenitori.
	
	Per far comunicare gli algoritmi coi contenitori, gli algoritmi non devono mai sapere nulla dei contenitori, gli alg devono lavorare con un concetto astratto di **sequenza**, che magari vengono da un contenitore, ma non è detto.
	Per lavorare sulle sequenze per i quali non si sa come sono disposti nella memoria, si è introdotto il concetto di **iteratori**
	
	### Iteratori ##
	SS pag.103
	
	>Iterare sugli elementi di una sequenza.
	
	Quello che fanno i contenitori è dare un'interfaccia complessa, però mi danno anche la possibilità di guardarli come delle **sequenze**.
	
	
	``` c++
	int main(){
    int a[10];
    
    for (int i = 0; i < 10; ++i){
    std::cout << a[i];
    }
    
    /* in questo modo ho fatto un'assunzione forte sul mio contenitore,
    dico che è indicizzabile, questo è un costrutto di iterazione che non usa iteratori
    */
    
    //con iteratori
    
    for (int* p = a; p != a + 10; ++p){
    std::cout << *p;
    }
    
    /* questo non usa indicizzazione 
    questo puntatore p è un iteratore su uno pseudo
    contenitore
    */
	
	}
	```
	Tutti gli iteratori sono pensati per essere il più possibile simili all'iteratore sopra.
	* mettermi all'inizio / fine contenitore
	* spostarmi avanti / indietro
	* dereferenziare il valore puntato
	
	Noi usiamo solo "l'interfaccia" offerta dagli iteratori, che non sono delle classi, l'interfaccia è solo un concetto.
	
	Gli iteratori devono essere appropriati all'uso che se ne vuole fare. Esiste un'entità più astratta che sono i concetti. Gli iterator
	i sono concetti.
	
	Bisogna tenere distinti i concetti di classe tipo, template per fare tipi e l'entità concetto che è qualcosa di più astratto.
	>qualsiasi cosa nel mio programma che soddisfi le proprietà di quel concetto allora è l'entità di quel concetto
	
	**Duck type system**
	
	``` c++
	for(int* p = vi.begin(); p != vi.end(); ++p)
    std::cout << *p;
	//così non mi interessa quale è il tipo puntato
	
	for(std::vector<int>::iterator p = vi.begin(); p != vi.end(); ++p)
    std::cout << *p;
	
	/* non mi interessa cosa è l'iteratore, se è un puntatore, classe... */
	
	template <typename Iter >
	void stampa_tutti(Iter first, Iter last) {
    for(Iter p = first; p != last; ++p)
    std::cout << *p;
    }
	//USO
	//vector  interi
	stampa_tutti(vi.begin(), vi.end());
	
	//anche per array C
	stampa_tutti(a, a + 10);
	/* funziona perchè il concetto è lo stesso! */
	```
