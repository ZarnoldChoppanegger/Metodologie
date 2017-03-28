Come faccio a fare in modo da non avere resource leak?

associamo un blocco try catch a ogni uso della risorsa:

	```C++

	void job(){
    Res* r1 = acquisisci_risorsa("res1");
    Res* r2 = acquisisci_risorsa("res2");

	try{

	do_task(r1,r1);
	//non vogliamo gestire l'errore, noi siamo i man in the middle, spesso no abbiamo l'informazione per saperee come gestire l'eccezione, che tipo di risorse vengono gestite
	}

	//...allora catturo ogni cosa brutto può essere successa
	catch(...){
	//Qui cosa faccio? Rilascio risorse
	rilascia_risorsa(r2);
	rilascia_risorsa(r1);
	//rilasciate in modo inverso, poichè così è bello
	//e le rilascio anche alla fine della funzione, che non è bello...
	/*... cosa faccio? LANCIO UN' ALTRA ECCEZIONE
	in modo che quelli di sopra sappiano che c'è stato un problema */

	throw;
	/*questa la si può invocare solo dentro un'eccezione e significa:
	rilancia l'eccezione catturata (neutralità rispetto all'eccezione)
	le eccezioni ci attraversano e le lasciamo andare verso l adestinazione vera
	verso chi le deve gestire. Questa è una caratteristica fondamentale per
	avere codice exception safe.
	Il problema è che è scomodissima da fare! Uso 50 volta una risorsa,
	50 try catch.
	Quindi blocco try catch alla risorsa non a chi la usa, quest'ultimo deve solo
	rilanciare l'eccezione*/
	}

	rilascia_risorsa(r2);
	Res* r3 = acquisisci_risorsa("res3");
	do_task(r1,r3);
	rilascia_risorsa(r3);
	rilascia_risorsa(r1);
	}

	void job(){
    /*try{ errore nel momento r1 va fuori scope nel catch
    Res* r1 = acquisisci_risorsa("res1");
    ...
    }*/
    
    Res* r1 = ...;
    try{ //Protettore di r1
    
    Res* r2 = acquisisci_risorsa("res2");
    try{ //Protettore di r2
	do_task(r1,r2);  //qualunque cosa sia successa salto a catch
	rilascia_risorsa(r2);
	}
	catch(...){
	rilascia_risorsa(r2);
	throw;
	/* me ne frego se c'è anche r1
	perchè quando esco rilancio l'eccezione che rilascia r1
	(neutralità)*/
    ...
    }

    Res* r3 = ...;
    try{
	do_task(r1,r3);
	rilascia_risorsa(r3);
	rilascia_risorsa(r1);
    }

	catch(...){
	rilascia_risorsa(r3);
	throw;
	}
    }

	catch(...){
	//fai qualcosa
	rilascia_risorsa(r1);
	throw;
	}
	}

	```

	Facendo così siamo resource leak free ma leggere e scrivere è un casino, ripetitivo, verboso. Diventa un generatore infinito di errori. Questo è quello che devono fare i poveracci che scrivono in java :( ...
	In java però hanno **finally**, blocco da mettere dopo il try e si occupa di sostituire quello che abbiamo scritto in c++ (rilascio risorse e throw).

	**Perchè non c'è in c++? Perchè c'è un'alternativa 100 volte meglio!**

	In c++ avevamo un costrutto che faceva più o meno questa cosa (rilascio risorse e rilancio eccezione)?

	## RAII - RRID ##

	* **RAII**: resource acquisition is initialization
	* **RRID**: resource release is Destruction, è quellaa funzione che quando l'oggetto va fuori scope non importa come, la funzione distruttore viene eseguita. Se l'uscita è eccezionale la lascia propagare come prima quindi è anche resource safe

	Il distruttore ha una terza proprietà, viene chiamato in maniera implicita fintanto che c'è l'oggetto di una classe da distruggere. Quindi se la gestione delle risorse la mettiamo dentro costruttore e distruttore.

	```c++
	class RAII_RRID_Res;

	void job(){
    RAII_RRID_Res r1("res1"); //dentro farà magia nera per acquisire risorsa
    //per rilasciare risorsa lo mettiamo nel ditruttore della classe chiamato implicitamente quando va fuori scope
    {
    RAII_RRID_Res r2("res2"); //c'è un problema...
    do_task(r1, r2);
    /* vogliamo che la risorsa 2 venga eliminata prima della fine della funzione */
    }
    //così anche per r3
    }
    ```

	Codice più breve, non ci si dimentica di deallocare risorse, l'importante è ricordarsi di chiamare la classe RAII.

	Come distruggere:

	```c++
	class RAII_RRID_Res{
	public:
	RAII_RRID_Res(const char* name){
	r = acquisisci_risorsa(name);
	}

	~RAII_RRID_Res(){
	rilascia_risorsa(r);
	}

	private:
	Res* r;
	};
	
	```

	e questo si scrive una volta sola!

	Idioma RAII, perchè? Gli idiomi dovrebbero essere quei costrutti linguistici che si usano in un linguaggio e non possibile tradurli facilmente in un altro linguaggio, in java infatti non c'è. In java passa il camion dell'immondizia che vede se hai messo finally e toglie risorse.
	(SU QUESTI ARGOMENTI SI SCHIANTANO GLI STUDENTI)

	Problemi conla classe di prima:

	* Col codice di prima non ho modo di usare le risorse perchè non ho un puntatore alla classe, non fa conversione implicita.
	* Opzione 1: uso get
	* Opzione 2: faccio conversione implicita

	```c++
	operator Res*() const {return r;}
	```
	* Ha costruttore di default? NO
	* Ha costruttore di copia, assegnamento (...e altra roba) e questi sono fatti male, perchè copiano i membri, quindi il puntatore alla risorsa e con due oggetti RAII mi ritrovo che entrambi puntano alla stessa risorsa e se viene eliminato uno dei due, mi ritrovo con dangling pointer. Se scriviamo uno dei costruttori bisogna scrivere anche gli altri (c++99) ora nel c++11 dovrebbero disattivarsi tutti se almeno uno è stato definito.
	```c++11

	RAII_RRID_Res(const RAII_RRID_Res&) = delete;
	RAII_RRID_Res& operator=(const RAII_RRID_Res&) = delete;

	```

