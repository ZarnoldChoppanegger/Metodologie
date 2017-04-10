## Classi ##

### Default copy ###

Di default gli oggetti possono essere copiati, ovvero è possibile inizializzare un oggetto assegnandone un altro dello stesso tipo. Questo è possibile poichè di default le classi possiedono un **costruttore di copia**, il quale esegue l acopia

### class and struct ###

Il costrutto 
``` c++
class X { ... };
```
è chiamato *definizione di classe* e definisce un tipo di dato, definito da utente, chiamato X.

* una definizione di classe può essere replicata in differenti file sorgente, usando #include, senza violare la ODR.

``` c++
struct S {...};

class S {public: ...};
```
* *struct* è un modo abbreviato per definire una classe con membri di default pubblici. È meglio usare struct per classi che sono pensate come semplici strutture dati, mentre class per "tipi con invarianti".
* costruttori e funzioni di accesso a membri dato possono essere utili anche per le struct ma con lo scopo di abbreviare la scrittura e non come granti per le invarianti.
* in una definizione di funzione è più logico dichiarare i membri pubblici prima di quelli privati, al fine di enfatizzare l'interfaccia della classe.

### Costruttori ###
* usare una funzione per inizializzare un oggetto può portare a errori, l'utente della classe infatti può dimenticarsi di inizializzare l'oggetto o inizializzarlo due volte, entrambe le situazioni possono portare a risultati disastrosi; quindi è meglio fornire una funzione con lo specifico obiettivo di inizializzare gli oggetti, questa funzione è chiamata **costruttore**.
* un costruttore ha lo stesso nome della classe e se richiede argomenti, questi vanno forniti (vedi Codice_Prova/costruttori.cc)
* è consigliabile usare l'inizializzazione con la notazione **{  }** poichè è più esplicita rispetto a ciò che viene fatto e quindi permette di evitare potenziali errori.
* le regole di overloading per le funzioni vengono applicate anche ai costruttori.
* in un costruttore possono essere definiti anche dei valori di default per i parametri e questi dovrebbero essere all'infuori dell'insieme dei possibili valori assegnabili all'argomento, es.

``` c++
class Date {
	int d, m, y;
public:
	Date(int dd =0, int mm =0, int yy =0);
// ...
};
```

