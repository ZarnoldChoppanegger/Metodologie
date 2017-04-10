## Classi ##

### Default copy ###

Di default gli oggetti possono essere copiati, ovvero è possibile inizializzare un oggetto assegnandone un altro dello stesso tipo. Questo è possibile poichè di default le classi possiedono un **costruttore di copia**, il quale esegue una copia membro a membro degli oggetti coinvolti nell'assegnamento.

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
* *struct* è un modo abbreviato per definire una classe con membri di default pubblici. È meglio usare *struct* per classi che sono pensate come semplici strutture dati, mentre *class* per "tipi con invarianti".
* costruttori e funzioni di accesso a membri dato possono essere utili anche per le struct ma con lo scopo di abbreviare la scrittura e non come garanti per le invarianti.
* in una definizione di funzione è più logico dichiarare i membri pubblici prima di quelli privati, al fine di enfatizzare l'interfaccia della classe.

### Costruttori ###
* usare una funzione per inizializzare un oggetto può portare a errori, l'utente della classe infatti può dimenticarsi di inizializzare l'oggetto o inizializzarlo due volte, entrambe le situazioni possono portare a risultati disastrosi; quindi è meglio fornire una funzione con lo specifico obiettivo di inizializzare gli oggetti, questa funzione è chiamata **costruttore**.
* un costruttore ha lo stesso nome della classe e se richiede argomenti, questi vanno forniti (vedi [Costruttori](https://github.com/wabbaJack/Metodologie/blob/master/Codice_Prova/costruttori.cc))
* da* è consigliabile usare l'inizializzazione con la notazione **{  }** poichè è più esplicita rispetto a ciò che viene fatto e quindi permette di evitare potenziali errori.
* le regole di overloading per le funzioni vengono applicate anche ai costruttori.
* in un costruttore possono essere definiti anche dei valori di default per i parametri e questi dovrebbero essere all'infuori dell'insieme dei possibili valori assegnabili all'argomento, es.

```c++
class Date {
	int d, m, y;
public:
	Date(int dd =0, int mm =0, int yy =0);
// ...
};
```
* dando un'inizializzazione adeguata, i costruttori semplificano tanto l'implementazione delle funzioni membro.

### explicit Constructors ###

Quando un costruttore può essere invocato con un solo parametro, questo può essere usato per effettuare conversione implicita dal tipo dell'argomento al tipo della classe.
In certi casi questo è utile, come per la classe rappresentante i numeri complessi:

``` c++
complex<double> d {1};
// d=={1,0}
```
dove vi è presenza della parte reale, con valore 1, e della parte immaginaria con valore 0.
Nel caso però della classe razionale, questo può portare a errori:

``` c++
void foo(const Razionale& r) {...}
foo(123);  //conversione da int a Razionale, legittimo
foo('a');  //conversione da char a Razionale, può causare errore
foo(true);  //da bool a Razionale, può causare errore
```

Per evitare che venga fatta implicitamente conversione, è possibile esprimere in modo esplicito questa richiesta, ponendo la parola chiave **explicit** prima della dichiarzione del costruttore che può essere invocato con un solo parametro. Quindi per invocare questi costruttori è necessario usare la sintassi di conversione esplicita, impedendo quindi che l'utente inizializzi innavertitamente l'oggetto con tipi di dato non previsti dalla classe. 
Di default è quindi sempre meglio dichiarare un costruttore explicit a meno che non si abbiano delle buone motivazioni per non farlo, come per la classe dei numeri complessi, in quel caso documentare bene l'interfaccia della propria classe.

Un 'altro modo per evitare conversione esplicita con alcuni tipi, è quello di dichiarare i rispettivi costruttori nel campo private, così avrò in fase di linking errori che segnaleranno l'uso inappropriato del costruttore.

``` c++
class Razionale {
public:
  Razionale(int num, int den);
private:
  Razionale(char);
  Razionale(signed char);
  Razionale(unsigned char);
  ....
}
```
Oppure dopo la dichiarazione dei parametri del costruttore, usando *=delete*, rende non disponibile il costruttore:

``` c++
Razionale(char) = delete;
razionale(signed char) = delete;
...
```

### Invariante ###
>Un'affermazione di ciò che si assume essere vero per una classe è chiamata *invariante di classe*

* A stabilire un'invariante di classe sono spesso i costruttori, poichè inizializzano l'oggetto.
* L'invariante deve essere rispettata per ogni chiamata di funzione membro.
* L'invariante è spesso dichiarata come commento che deve essere reso vero dal costruttore.

``` c++
class Vector {
public:
Vector(int s);
// ...
private:
double∗ elem; // elem points to an array of sz doubles
int sz;
// sz is non-negative
};

Vector::Vector(int s)
{
if (s<0) throw Bad_siz e{s};
sz = s;
elem = new double[s];
}
```
Perchè bisogna definire un'invariante?
* per concentrare gli sforzi di progettazione della classe.
* per chiarire il comportamento della classe.
* per semplificare la definizione delle funzioni membro.
* per chiarire la gestione delle risorse della classe.
* per semplificare la documentazione della classe.k
