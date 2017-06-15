## Esercitazione 200502 exe 2 ##

``` c++
class Base {
public:
  Base() {
    std::cout << "Constructor Base::Base()" << std::endl;
  }
  virtual void f(int) {
    std::cout << "Base::f(int)" << std::endl;
  }
  virtual void f(double) {
    std::cout << "Base::f(double)" << std::endl;
  }
  virtual void g(int) {
    std::cout << "Base::g(int)" << std::endl;
  }
  virtual ~Base() {
    std::cout << "Destructor Base::~Base()" << std::endl;
  }
};
class Derived : public Base {
public:
  Derived() {
    std::cout << "Constructor Derived::Derived()" << std::endl;
  }
  void f(char c) {
    std::cout << "Derived::f(char)" << std::endl;
  }
  void g(int) {
    std::cout << "Derived::g(int)" << std::endl;
  }
  ~Derived() {
    std::cout << "Destructor Derived::~Derived()" << std::endl;
  }
};


int main() {
  Base b;
  Derived d; // fino a qui abbiamo stampato tre righe, base, base e derived
  Base& rb = b; // questo non stampa nulla, (riferimento statico)
  Base* pb = &d; // upcast che si può fare implicitamente
  // viene creato un oggetto ma è un puntatore (dinamico)
  std::cout << "=== 1 ===" << std::endl;
  b.f(1);
  rb.f(’a’);
  rb.g(1);
  std::cout << "=== 2 ===" << std::endl;
  d.f(1);
  d.f(1.0);
  d.g(3.3);
  std::cout << "=== 3 ===" << std::endl;
  pb->f(1.0);
  pb->f(’a’);
  pb->g(3.3); // unico caso in cui c'è overriding
  return 0;
}
```

Prima di costruire il Derived si costruisce il tipo base, nella lista di inizializzazione è come se ci fosse scritto `Derived() : Base()`. I membri della classe base vengono costruiti prima dei membri della classe derivata. Questo è scritto in modo implicito.
Questo è un esercizio per risoluzione dell'overriding ma prima di fare questo bisogna fare risoluzione dell'overloding.

`b.f(1)`, va a vedere nella classe base, c'è f(int) e f(double). vorremmo chaimare f(int) ma il metodo è virtual. Lo chiamiamo davvero quello? Bisogna farsi le domande:
* è marcato virtual?
* stiamo lavorando su un puntatore a tipo oggetto? si.
* vogliamo obbligatoriamente il tipo base? no
* abbiamo effettivamente un tipo diferso dal tipo statico? no quindi viene stampato Base::f(int). Quindi non abbiamo effetivamente overriding.

`rb.f('a')`, va nella classe base, cerca f, ho promozione chiamo f(int) e ci facciamo le stesse domande di prima. Viene stampato f(int) poichè tipo statico e tipo dinamico coincidono.

`rb.g(1)`, riferimento alla base, cerco funzione g, c'è solo una utilizzabile quindi stampa "Base::g(int)".

`d.f(1)`, usiamo un oggetto di tipo statico derived, il compilatore sa che è derived. Quindi partiamo con la ris dell'overloading dalla classe derivata. Quante f sono definite? 1, visibili? 1. Perchè le altre della classe base sono nascoste, hiding. Quindi stampa "Derived::f(char)". Tipo statico e dinamico coincidono.

`d.f(1.0)`, uguale a chiamata di prima

`d.g(3.3)`, si chiama g(int) nella Derived, ed è l'unica candidata ed è utilizzabile. Stampiamo "Derived::g(int)"

`pb->f(1.0)`, questo è tipo statico puntatore a base, tipo dinamico puntatore a derivata. Dove cerchiamo f? Il compilatore conosce solo il tipo statico quindi vado a cercare nella classe base. Ne trovo due tra le due preferisce f(double). Questa volta abbiamo che la chiamate veiene fatta su metodo virtuale, il tipo statikco è diverso dal dinamico e la chiamata non è qualificata, quindi si dice a run time di scegliere la chiamata migliore. C'è una catena di derivazioni dalla classe base alla derivata e in questa catena da classe statico a dinamico e vedere se qualcuno ha fatto sovrascrittura del metodo f(double). Ci possono essere anche 18 overriding ma io prendo l'ultimo tipo concreto che lo implementa.
Quindi bisogna a vedere se c'è l'overriding nella classe derivata. Abbiamo una f? si. Prende un double? No, quindi c'è hiding e nella tabella dei metodi virtuali l'indirizzo per f(double) è sempre quello della classe base, stampando "Base::f(double)".

`pb->f('a')`, a compilazione si guarda solo tipo statico e quindi si guarda la classe base e il compilatore sceglie la Base::f(int), poi a tempo di esecuzione si fanno le valutazioni giuste. Mi hanno fatto overriding di f(int)? No, viene di nuovo stampato "Base::f(int)".

`pb->g(3.3);` statico, classe base c'è g(double) si va conversione. Supporto a tempo di esecuzione vede che è virtual, non è chiamata specificata, si è fatto overriding? Si, quindi stampa "Derived::g(int)". `f(char c)` non fa overriding è una funzione diversa.

Finito? No, si esce dallo scope e gli oggetti creati vengono distrutti gli oggetti in ordine inverso alla costruzione: 
* viene distrutto pb, puntatore stupido, non fa niente.
* d viene distrutto. Il distruttore è virtuale, e la virtualità si eredita comunque. Tipo statico e dinamico coincidono e quindi è una distruzione "normale", spampando "Destructor Derived::~Derived()" e poi distruttore classe base "Destructor Base::~Base()".
* rimane da distruggere b, tipo statico e dinamico coincidono e invoca distruttore classe base.

Quando nel c++ si attiva meccanismo poliformismo dinamico bisogna stare attenti a quando si fa overriding perchè si potrebbe invece fare hiding. La parola chiave `override` mostra in modo esplicito che si vuole fare l'override di un metodo e nel caso non venga fatto il compilatoer da errore.

## DOMANDE ##
* esistono le funzioni membro statiche virtuali?
No, perchè il supporto a run-time ha bisogno che gli venga passato un oggetto e quindi le funzioni memebro statiche virtuali non hanno senso di esistere.

* esistono le funzioni membro templatiche virtuali?
No, perchè diventerebbe troppo complicata. É un problema tecnico, quando io ho una funzione membro quella in verità non è una funzione ma un "generatore di funzioni" e se sono virtuali mi genera funzioni virtuali. Ma allora creerebbe una tabella dei metodi virtuali molto grande e quindi impratiabile.

* esistono distruttori virtuali?
Si. È opportuno siano virtuali? Si, quando definisco un'interfaccia astratta ci vuole un costruttore virtuale definito (vuoto). Quando vado a lavorare con puntatori ad oggetti, con contenitori di tipo uniforme (contenitori con tipi base), quando libero risorse faccio delete su puntatori a classe base, ma con distruttore virtuale posso deallocare anche risorse classe derivata perchè viene invocato anche il costruttore del tipo dinamico. 

* esistono costruttori virtuali?
No perchè l'oggetto deve ancora essere costruito, i costruttori virtuali non esistono. Ci sono cose simili, ma non sono costruttori virtuali. Si possono definire dei metodi che si comportano come dei costruttori ma non lo sono, ad esempio `virtual Animale* clone() const = 0;`. Perchè ritorna un puntatore? Perchè se ritornasse per copia mi ritornerebbe solo una fettina, solo classe base, il puntatore funziona con tipi dinamici. 
esempio implementazione:

``` c++
virtual Pecora* clone() const override {
  return new Pecora(*this);
}
```
Il tipo di ritorno è diverso però tra classe base e derivata e diremmo che in realtà c'è hiding. C'è eccezione però che dice che il tipo di ritorno deve essere un puntatore a classe derivata.
Non creo problemi perchè ci sono due utenti del metodo clone(), quello che lo chiama su un puntatore a tipo animale. Ma anche quello che va a lavorare su classe concreta, cioè su quella derivata.

``` c++
void pastore(Pecora* pp) {
  Pecora* pq = pp.clone();
  // se il tipo di ritorno non fosse
  // Pecora* allora avrei Downcast 
  // che non é permesso
}

void noe(Animale* pa) {
  Animale* pb = pa.clone();
}
```
Se non ritornassi il tipo puntatore alla classe derivata, mi ritroverei nel caso nel quale vado a fare down-cast che non è possibile. E allora hanno messo questa eccezione.

* si possono definire membri privati/protetti virtuali?

``` c++
class Base {
public:
  void foo() { bar(); }
private:
  virtual void bar() const = 0;
}
```
Ha senso fare una cosa del genere? Tecnicamente si. Ma è bene farlo? Ci sono casi in cui ha senso e in cui non ha senso? I metodi virtuali pubblici danno un meccanismo per parametrizzare l'interfaccia della classe a tempo di esecuzione. Nell'esempio di prima diamo la possibilità di parametrizzare un tipo privato che comunque non è possibile chiamarlo nelle classi derivate, quindi fornisco implementazione alternativa di un metodo che non posso chaimarlo. Ha senso? Si. C'è un **design pattern** chiamato **strategy** (Cerco su Google e mi sembra si sia riferito di piú al [NVI](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-Virtual_Interface)). Prevede che noi dobbiamo implementare un algoritmo che va eseguito in un certo modo che l'utente non può farci niente, come foo(). Ma alcune operazioni di foo() implementate da bar() danno la possibilità di essere customizzate e quindi l'utente deve e può specificare come devono essere eseguite queste operazioni.

* Si può impedire l'overriding?
Mi piacerebbe arrivare a un punto nel quale nessun'altro può ulteriormente specializzare la classe, cioè impedire di derivare ulteriormente la classe. Nello standard nuovo dovrebbe essere parola chiave **final** 

* Si può impedire derivazione di una classe? (non importa)

* Un costruttore (che non può essere virtuale) può invocare al suo interno funzioni virtuali?
Posso chiamare funzioni membro, ma quelle virtuali? 
Regola: non invocare metodi virtuali nei costruttori. Funziona, ma non lo fare.
Nella parte nascosta del mio oggetto, che cosa c'è scritto?
Il comportamento delle funzioni virtuali nel costruttore cambia in base a dove vengono chiamati.

* Un distruttore (che può essere virtuale) può invocare al suo interno funzioni virtuali?
Si. Ma è un bene o un male?
Regola: non invocare metodi virtuali nei distruttori.
Stessi ragionamenti dei costruttori ma in modo duale.
Il tipo dinamico viene depotenziato man mano che si procede con la distruzione, quindi se chiamo metodi virtuali, arrivo troppo tardi.

> Se dovete chiamare dei metodi all'interno di una classe chiama solo metodi statici non virtuali

* Come mai i distruttori virtuali NON vanno in loop?
Prima viene chiamato ogg classe derivata e poi base. Ma il distruttore della classe base è virtuale? Si. Quindi, quando ci ritroviamo a chiamare il ditruttore della classe base andrebbe per meccanismo di ovverriding a chiamare quello della classe derivata, andando in loop.

``` c++
class Base {
  virtual void pippo() { std::cout << "pluto"; }
};

class Derivata {
  virtual void pippo() 
  { 
    std::cout << "paperino";
    Base* pb = this;
	// va in loop infinito
	pb->pippo();
	// non va in loop infinito
	pb->Base::pippo();
  }
};
```
Il motivo è che quando fa la chiamata del distruttore della classe base, fa qualificazione e specifica che vuole obbligatoriamente il tipo base.

**EXE per casa: scrivere l'esempio più piccolo possibile nel quale si ha classe base e derivata dinamiche, e il fatto di non aver dichiarato il distruttore crea memory leak.**

## Dynamic cast ##
Cast per poliformismo dinamico.
Supponendo che clone() ritorni Animale* invece di Pecora*

``` c++
void pastore(Pecora* pp) {
  Animale* pq = pp.clone();
  
  // demoni fluiranno dal naso se non ci azzechi col tipo
  Pecora* pp = (Pecora*) pa;
  // cast più educato. Interrogazione a tempo di esecuzione
  // chiede se quello passato è un tipo pecora. 
  // Risponde più o meno come quando si fa risoluzione overriding
  Pecora* pp = dynamic_cast<Pecora*>(pa);
  //a robe di questo tipo lo stomaco dovrebbe sobbalzare
  
  pp.tosatura();
}

void noe(Animale* pa) {
  Animale* pb = pa.clone();
}
```
**RTTI:** identificazione di un tipo a run-time. Si applica solo sulle classi dinamiche, quindi non posso fare dynamic cast su un puntatore di una classe che non è dinamica, ovvero quella classe che non ha nemmeno un metodo virtuale.
