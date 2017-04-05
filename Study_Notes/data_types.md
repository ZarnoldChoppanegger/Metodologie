## Data types ##

* **Integral types:**  bool, char, int
* **Arithmetic types:** Integral types e floating point
* **Definiti da utente:** classi e enumerazioni, poichè hanno bisogno di essere definiti dall'utente invece di essere disponibili all'uso senza una precedente dichiarazione
* **Built-in:** tipi primitivi, puntatori e riferimenti

## Booleans ##
* I booleani hanno solo due valori *true* e *false*.
* Un booleano può essere convertito implicitamente in un intero, true varrà 1, false varrà 0.
* Un intero può essere convertito implicitamente in un booleano, in questo caso lo 0 rappresenterà false e qualsiasi numero diverso da 0 rappresenterà true.
* Per evitare conversione implicita si può usare la lista di inizializzazione, impedendo narrowing conversion, ad esempio:

``` c++
bool b1 = 7;  // 7!=0 quindi b1 diventa true
bool b2 {7};  //errore: narrowing conversion
bool b3 {true};  //ok

//se proprio voglio convertire un intero

int i = 7;
bool b4 { i != 0 }; //ok
```
* Un puntatore può essere convertito in booleano. Se il puntatore è non nullo sarà convertito a valore true, se il puntatore è nullo sarà convertito a false. È preferibile non comparare un puntatore a nullptr poichè l'espressioneèpiù lunga quindi error prone.



