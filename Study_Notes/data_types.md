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
* Un puntatore può essere convertito in booleano. Se il puntatore è non nullo sarà convertito a valore true, se il puntatore è nullo sarà convertito a false. È preferibile non comparare un puntatore a nullptr poichè l'espressione è più lunga quindi error prone.

## Characters ##

* Ci sono diversi insiemi di caratteri e di encoding dei caratteri. Il c++ quindi offre diversi tipi di dato per rappresentare i caratteri:
  * **char:** tipo di default per implementare insiemi di caratteri ed è di solito a 8 bit.
  * **unsigned char:** assicura che non ci sia segno
  * **signed char:** come char ma è capace di contenere sia numeri positivi che negativi
  * **wchar_t:** fornito per rappresentare set di caratteri più grandi come l' UNICODE. 
  * **char16_t:** a type for holding 16bit character sets, such as UTF-16
  * **char32_t:** a type for holding 32bit character sets, such as UTF-32

In quasi ogni implementazione il tipo *char* sarà implementato in maniera identica a *unsigned char* e *signed char*, ma sono comunque considerati tipi diversi.

>Ogni qualvolta sia possibile, è meglio non fare supposizioni  sulla rappresentazione di oggetti

La scelta di rappresentare semplici char come interi con o senza segno è implementation-defined. Sono definiti solo i signed char (-127, 127) e gli unsigned char (0, 255). C'è differenza tra i due tipi solo per i caratteri che si trovano all'infuori dell'intervallo 0-127. Usare caratteri che si trovano fuori dall'intervallo possono portare a problemi di portabilità.




