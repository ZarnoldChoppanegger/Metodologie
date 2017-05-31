## Ereditarietà multipla ##

`struct A : public B` ma nessuno mi vieta di continuare l'ereditarietà e aggiungere `, public C, private D, protected E`. Quindi la mia struttura sarà costituita da sotto oggetti di tipo `B, C, D, E`.
Posso fare implicitamente conversione da A a B e C, perchè ereditarietà public. Se eredito A posso accedere anche a campi protetti di A, quindi posso convertire anche ad E. 
I sotto oggetti vengono costruiti nell'ordine in cui sono dichiarati. 

``` c++
struct A : public B, public C {

}
```
Quando si ha ereditarietà multipla, si ereditano anche i metodi. Quindi nel caso un metodo non sia dichiarato nella classe derivata, cosa succede?
* non si trova il metodo
* si trovano insiemi di funzioni lungo un solo cammino, cioè solo in una delle classi base
* si trovano insiemi di funzioni lungo cammini diversi; allora come ci si può comportare?
  * opzione 1, si mettono insieme. NO, non funziona così, quello che si ha è ambiguita. Il fatto che siano raggiungibili lungo cammini diversi significa che possono avere semantiche diverse. Questa potrebbe essere un'**ambiguita latente** poichè il compilatore non la segnala come ambiguita.
  * si suppone che ci sia ambiguita. L'utente può risolverla qualificando la chiamata di funzione es. `a.B::foo()`. Non è comodo! Chi ha progettato la classe A, deve lui trovare il modo di fare la qualificazione, allora:
	* dentro la classe derivata tira dentro tutte le funzioni `using B::foo()` e `using C::foo()`, oppure...
	* faccio `hiding`, `void foo() { B::foo(); }` quindi l'utente quando chiamerà foo chiamerà quella di B.

Ereditare molteplici volte la stessa classe è vietato perchè avrei ambiguita. In modo indiretto però riuscirei a farlo e non sarebbe ambiguo.

A volte voglio che la classe base delle mie classi base `B` e `C` sia una sola `D`. Perchè? Perchè è l'unica risorsa, voglio cioè che le due classi base condividano la stessa risorsa (fatto nella STL da `iostream`). Il meccanismo che consente questo, cioè la condivisione delle risorse, può essere usato con la parola chiave `virtual` messa dopo lo specificatore di accesso (public, private, protected). Non ha nulla a che vedere con i metodi virtuali e significa mettere l'oggetto come riferimento o puntatore all'oggetto. 
Quando si ha ereditarietà di questo genere, significa che si parla di `schema a diamante`.  (**DDD - Deadly Diamond of Death**).
Se una delle classi base non ha il virtual allora questo ha un sotto oggetto D tutto suo.
Nel DDD abbiamo un grafo e quindi la **costruzione della classe derivata** è diversa, poichè  se la classe base è stata già inizializzata da una delle classi derivate, anche le altre dovrebbero inizializzarla, quindi si continuerà a creare e distruggere la classe base. 
> Se nella gerachia di classi ci sono **classi virtuali condivise**, queste vengono costruite dalla classe più derivata, e le altre classi non lo devono fare, perchè sanno che qualcun altro lo ha fatto

In che ordine vengono inizializzate più classi virtuali condivise? Faccio DFS del grafo e inizializzo nell'ordine che trovo.

Si supponga che le classi base siano totalmente astratte, hanno importanza tutte le cose dette per l'inizializzazione? No.
Quindi quando ho a che fare con classi astratte, il DDD non è un problema e questo è il motivo per il quale in java è impedita l'ereditarietà multipla ma è permessa con interfacce perchè sono astratte e non si creano problemi. 

Per disambiguare metodi di interfacce diverse si creano degli adattatori es. `connetti_modem` e `connetti_fax`.
Il distruttore distrugge nel modo inverso al costruttore, non ci si fanno problemi.

### exe 3 - 27/02/2006 ###


### exe 2 - 22/05/2005 ###
* creo gerarchia classi

