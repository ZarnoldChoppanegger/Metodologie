# STL (Roba a caso) #

## Predicati (unari, binari) ##

**Cos'è un predicato?**
Un predicato è un **puntatore a funzione** o un **oggetto funzione** (cioè un oggetto che fornisce la chiamata all'operatore (), operator()()), che ritorna si/no a una domanda relativa ad un oggetto.
Molti algoritmi usano predicati per sapere come operare sugli elementi.
**Esempi:**

* Implementazione standard dell'algoritmo `find_if()`:

``` c++
template<typename Iter, typename Pred> inline
Iter find_if(Iter first, Iter last, Pred pred)
{
  while(first != last && pred(*first))
  {
    ++first;
  }
  
  return first;
}
```
Il parametro di tipo `Pred` può essere sia un puntatore a funzione che un oggetto funzione.

* Uso della funzione `find_if()` con predicato puntatore a funzione:

``` c++
bool GreaterThanFive(int i)
{
  return i > 5;
}

bool IsAnyElementGreaterThanFive( vector<int>& v)
{
  return find_if(v.begin(), v.end(), GreaterThanFive) 
         != v.end();
}
```

* Uso della `find_if()` con oggetto funzione:

``` c++
class GreaterThanFive
{
public:
  GreatherThanFive() { }
  bool operator()(int i) const { return i > 5; }
};

bool IsAnyElementGreaterThanFive( vector<int>& v)
{
  return find_if(v.begin(), v.end(), GreaterThanFive()) 
         != v.end();
}
```

**Quali sono i benefici nell' usare gli oggetti funzione invece dei puntatori a funzione? Quindi, quand'è che è utile usare un oggetto funzione?**

``` c++
class GreatherThan
{
public:
  GreatherThan(int value) : value_{value} { }
  bool operator()(int i) const { i > value_; }
  
private:
  const int value_;
};

bool IsAnyElementGreatherThanFive( vector<int>& v)
{
  return find_if(v.begin(), v.end(), GreatherThan(5))
         != v.end();
}
```
In questo caso l'oggetto funzione ci permette di esprimere un predicato che effettua confronti con qualsiasi valore e non soltanto con il 5, come nel caso precedente con il punatore a funzione. Questo permette di scrivere codice generico ed infatti è possibile portarsi oltre e generalizzare anche sul tipo di dato.

``` c++
template<typename T>
class GreatherThan
{
public:
  // non passo per riferimento ne costruttore 
  // perchè magari l'oggetto passato può cambiare 
  // prima di fare il confronto?
  GreatherThan(T value) : value_{value} { }
  bool operator()(const T& t) const { return t > value; }
  
private:
  const T value_;
};


bool IsAnyElementGreatherThanFive( vector<int>& v)
{
  return find_if(v.begin(), v.end(), GreatherThan<int>(5))
         != v.end();
}
```

In questi esempi gli oggetti funzione hanno una proprietà molto importante: possono essere copiati in modo equivalente. Cioè, se si fa una copia di `GreatherThan<int>(5)`, questa avrà lo stesso comportamento dell'oggetto originale. Se invece avessimo avuto oggetti funzione che cambiavano stato al loro interno, ogni volta che venivano chiamati, gli algoritmi avrebbero dovuto soddisfare delle condizioni:
* l'algoritmo non può prendere il predicato per copia
* l'algoritmo deve applicare il predicato sugli oggetti in un ordine conosciuto

Lo standard però non impone queste due proprità agli algoritmi, perciò usare predicati che cambiano stato possono portare a risultati inattessi, quindi:

> Gli oggetti funzione non devono mai cambire stato interno e quindi poter usare le loro copie allo stesso modo come l'originale ritornando sempre il risultato previsto


## Traits ##
> Un classe trait racchiude un insieme di tipi, dei quali fornisce informazioni utili per altre classi o funzioni template per manipolare gli oggetti del tipo per le quali sono state istanziate

## Typename ##

**Che cos'è typename, e cosa fa?**
>Un nome usato in una dichiarazione o definizione di template, e dipendente da un parametro template, non è nome di tipo se non viene qualificato in modo esplicito con la parola chiave `typename`.

**Cosa c'è di sbagliato in questo codice?**
``` c++
template<typename T>
class X_base
{
public:
  typedef T instantiated_type;
};

template<typename A, typename B>
class X : public X_base<B>
{
public:
  bool operator()(const instantiated_type& i) const
  {
    return i != instantiated_type();
  }
};
```
Quando il compilatore fa il parse della funzione operator(), i nomi dipendenti dai parametri template non sono visibili, quindi `instantiated_type` dichiarato nella classe X_base non è visibile, quindi il compilatore non sa che cos'è. Nomi dipendenti diventano visibili solo dopo che il template è stato istanziato. Per il risolvere il problema bisogna dire come minimo al compilatore che instantiated_type è il nome di un tipo di dato.

``` c++
template<typename A, typename B>
class X : public X_base<B>
{
public:
  typedef typename X_base<B>::instantiated_type instantiated_type;
  
  bool operator()(const instantiated_type& i) const
  {
    return i != instantiated_type();
  }
};
```
## Contenitori, Puntatori e Contenitori che non lo sono ##


