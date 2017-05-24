## Templates ##

Strutture dati, come il vector, spesso sono indipendeti dal tipo dei dati che manipolano. Quindi è necessario dare al tipo una rappresentazione indipendente.

> Un template è una classe o funzione che parametrizziamo con un insieme di tipi o valori.
> Usiamo i template per rappresentare concetti che vengono compresi meglio in una forma più generale.

### Parametrizzare i tipi ###

Un vector di double può essere parametrizzato a un vector di qualsiasi tipo nel seguente modo:

``` c++
template<typename T>
class Vector{
public:
  explicit Vector(int s);
  ~Vector() { delete[] elem; }
  
  //...operazioni di copia e spostamento...
  
  T& operator[](int i);
  const T& operator[](int i)const;
  int size() const {return sz;}
private:
  T* elem;
  int sz;
};
```

Il prefisso `template<typename T>` significa "per tutti i tipi T".

Le funzioni membro possono essere definite nel seguente modo:

``` c++
template<typename T>
Vector<T>::Vector(int s) { ... }

template<typename T>
const T& Vector<T>::operator[](int i) const { ... }
```

E` quindi possibile definire oggetti Vector nel seguente modo:

``` c++
Vector<char> vc{200};
Vector<list<int>> vli(45);
```

ed usarli nel seguente modo:

``` c++
void write(const Vector<string>& vs)
{
  for(int i = 0; i != vs.size(); ++i)
    cout << vs[i] << '\n';
}
```
per avere supporto dei range-for loop per il Vector, e' necessario implementare i metodi `begin()` e `end()`:

``` c++
template<typename T>
T* begin(Vector<T>& x)
{
  return x.size() ? &x[0] : nullptr;
}

template<typename T>
T* end(Vector<T>& x)
{
  return begin(x) + x.size();
}
```

date queste funzioni e' possibile scrivere:

``` c++
void f2(Vector<string>& vs)
{
  for(auto& s : vs)
    cout << s << '\n';
}
```

I templates sono **meccanismi a tempo di compilazione**, questo significa che non si va ad incorrere in overhead a tempo di esecuzione a differenza di codi non templatico.

I template possono proendere anche argomenti per valore, per esempio:

``` c++
template<typename T, int N>
struct Buffer{
  using type_name = T;
  constexpr int size() { return N; }
  T[N];
  // ...
};
```
questo permette di creare buffer di qualsiasi tipo di dimensione arbitraria, ad esempio `Buffer<char, 1024> bc;` crea un buffer di caratteri allocato a tempo di compilazione sullo stack.
Il tipo passato per valore deve essere una constant expression.

### Funzioni template ###

I template possono essere usati non solo per parametrizzare i container con dei tipi, ma anche per parametrizzare gli algoritmi, come ad esempio una funzione che calcola la somma degli elementi di un container:

``` c++
template<typename Container, typename Value>
Value sum(const Container& c, Value v)
{
  for(auto x : c)
    v += c;
  return v;	
}
```
I tipi `Container` e `Value` sono dedotti dagli argomenti della chiamata di funzione.
sum() e' una semplificazione della funzione della `accumulate()` presente nella STL.

### Concetti e Programmazione Generica ###

> I templates offrono meccanismi per computazione a tempo di compile time e manipolazione di tipi che porta a codice più efficiente e compatto

L'uso più comune dei templates è nella **programmazione generica**, ovvero nella progettazione e implementazione di algoritmi per uso **generale**. Con generale si intende che un algoritmo può essere eseguito su tutti i tipi che rispettano dei determinati requisiti, come l'esempio precedente, la funzione sum() lavora con qualsiasi tipo Container che abbia implementati i metodi begin() e end().
La funzione sum() presenta dei requisiti, ovvero vuole come parametri un container e un numero. Questi tipi di requisiti vengono chiamati **concetti** e servono per identificare i concetti fondamentali di un particolare campo di applicazione. Questo permette di **formalizzare a un certo grado** tale da rendere efficace la programmazione generica.
Si prenda come esempio il concetto **regolare**. Un tipo è regolare quando si comporta come un int o vector, cioè:
* può essere costruito di default
* può essere copiato mediante costruttore o assegnamento
* può essere comparato usando == e !=
* non presenta problemi tecnici dati trucchi di programmazione

Un string è un esempio di tipo regolare e come gli int è anche **ordinato**. due stringhe possono essere comparate con gli operatori <, <=, > e >=.

> I concetti non sono solo una nozione sintattica ma anche semantica

### Function Objects ###

Sono oggetti che possono essere chiamati come funzioni.

``` c++
template<typename T>
class Less_than {
  //valore col quale fare il confronto
  const T val_;
public:
  Less_than(const T& v) : val_{v} { }
  
  //operatore di chiamata
  bool operator()(const T& x) const { return x < val_; }
}
```
Quindi è possibile definire oggetti di tipo `Less_than` per qualche tipo

``` c++
Less_than<int> lti {42};
Less_than<string> lst {"Backus"};
```
ed è poi possibile usarli come chiamate di funzioni nel seguente modo:

``` c++
void fct(int n, const string& s)
{
  bool b1 = lti(n); //true se n < 42
  bool b2 = lts(s); //true se s < "Backus"
}
```
Oggeti come questi vengono usati largamente come argomenti per algoritmi. Per esempio, si possono contare le occorrenze di valori per i quali il predicato ritorna true:

``` c++
template<typename C, typename P>
int count(const C& c, P pred)
{
  int cnt = 0;
  for(const auto& x : c)
    if(pred(x)) ++cnt;

  return cnt;
}
```
e quindi sarà possibile passare un oggetto Lass_than come parametro della funzione:

``` c++
void f(const Vector<int>& vec, int x)
{
  cout << "Numero di valori minori di " << x
       << ": " << count(vec, Less_than<int>{x})
	   << "\n";
}
```
Il bello di queste funzioni oggetto è che si portano il valore, quindi non c'è bisogno di chiamare una funzione per ogni valore diverso (e ogni tipo), e non è necessario introdurre variabili globali per mantenere i valori. Oltretutto, per funzioni oggetto come Less_than è semplice fare inlining, quindi le chiamate saranno molto più efficienti.
Funzioni oggetto che rappresentano operazioni chiave in un algoritmo sono spesso chiamate `policy objects`.

Quando dichiarare una funzione oggetto separatamente da dove viene usata sembra inconveniente, è possibile generare la funzione oggetto in modo implicito mediante le `lambda expression`.

``` c++
void f(const Vector<int>& vec, int x)
{
  count << "Numero di valori minori di " << x
        << ": " << count(vec, [&x](int a) { return a < x; })
		<< "\n";
}
```

La notazione `[&x]`, significa indica che viene catturata per riferiemnto la variabile locale x. Se si vuole catturare per valore si usa `[=x]`, mentre se si vuole catturare tutto per riferimento e valore rispettivamente `[&]` e `[=]`.

### Aliases ###

È spesso utile introdurre degli alias per i tipi o template. Ad esempio, nell'header standard <cstddef> vi è la definizione dell'alias `size_t`, che in base all'implementazione può riferirsi a `unsigned int` oppure anche a `unsigned long`. Questo quindi permette allo sviluppatore di scrivere codice portatile.
È comune anche che i tipi parametrizzati offrano alias per i tipi relativi ai loro argomenti template. Per esempio:

``` c++
template<typename T>
class Vector {
public:
  using value_type = T;
  
  ...
};
```
Infatti ogni container della libreria standard fornisce **value_type** come nome del suo tipo. **Questo permette di scrivere codice per ogni container che segue questa convenzione**. 

``` c++
template<typename C>
using Element_type = typename C::value_type;

template>typename Container>
void algo(Container& c)
{
  Vector<Element_type<Container>>vec;
  //...
}
```
### Template Compilation Model ###

Il type checking dei template avviene nella definizione del template invece che nella dichiarazione. Questo porta a compile-time a una variante di cosa è spesso chiamato *duck typing* ("Se cammina come una papera e starnazza come una papera, allora è una papera". Si opera solo sui valori e il significato delle operazioni dipende solo da questi e non dai loro tipi. Questo differisce dal punto di vista alternativo che dice che gli oggetti hanno un tipo, il quale determina il significato delle operazioni. 
> Cosa è fatto a compile-time usando templates non coinvolge oggetti ma solo i valori
Lo sforzo pratico per usare i template è che la loro definizione (non dichiarazione) deve essere in scope. 


#### Esempio codice per uso `typename` e `using` ####
```c++
//g++  5.4.0

#include <iostream>

template<typename T>
class A{
public:
    typedef T v1_type;
    
    using v2_type = T;
};

template<typename U>
class B : public A<U>
{
public:
    typedef typename A<U>::v1_type v1_type;
    using v2_type = typename A<U>::v2_type;
};
    

int main()
{
    B<int>::v1_type x {10};
    B<double>::v2_type y {20};
    
    std::cout << x << '\n' << y;
    
    return 0;
}
```
