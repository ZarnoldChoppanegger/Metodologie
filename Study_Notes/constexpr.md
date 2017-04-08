## constexpr ##

* **constexpr** significa far valutare un'espressione a compile time. 
* a un tipo constexpr può essere assegnato solo un'espressione constexpr, a differenza del const, es:

``` c++
const int x = 7;
const string s = "abccd";
const int y = sqrt(x);

constexpr int xx = x; //OK la x è un'espressione costante
constexpr string ss = s; //errore; s non è un literal type
constexpr int yy = y; //errore; sqrt(x) non è una funzione constexpr
```

* una classe con costruttore *constexpr* è di tipo literal type se il corpo del costruttore è vuoto
* una funzione membro constexpr è implicitamente anche costante, quindi non bisogna scrivere:

``` c++
constexpr int foo(...) const {...}
```
