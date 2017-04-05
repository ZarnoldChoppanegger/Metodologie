## void* ##

* un puntatore (void*) è interpretato come puntatore a un "oggetto di tipo sconosciuto"

* un puntatore a un qualsiasi tipo può essere associato a un puntatore void\*,  tranne i puntatori a funzioni e a membri. In oltre void\* può essere assegnato a un altro void\*, può essere comparato per equità o inequità e può essere convertito esplicitamente in un altro tipo.
Altri tipi di operazioni sarebbero insicure in quanto il compilatorenon  conosce il tipo di dato puntato

es.
```c++
void foo(int* pi)
{
	void* pv = pi;
	*pv;   //errore non si può dereferenziare
	++pv;     //errore non si può incrementare il puntatore senza conoscere il tipo
	
	int* pi2 = static_cast<int*>(pv);
	double* pd1 = pv;
	double* pd2 = pi;
	double* pd3 = static_cast<double*>(pv);   //possibile ma non sicuro poichè è implementation defined
}
```
## nullptr ##

* la parola chiave **nullptr** rappresenta un puntatore che non punta a nessun oggetto

* **nullptr** può essere associato a qualsiasi tipo di puntatore

* era uso comune usare 0 per indicare il puntatore nullo poichè non essite alcun oggetto allocato con l'indirizzo 0

* è stato uso comune usare la macro NULL per denotare un puntatore nullo, ma NULL differisce nelle varie implementazioni e può rappresentare 0 o 0L, in C (void*)0 che non è legale in C++

* nullptr rende il **codice più leggibile** e **evita confusioni** quando una funzione sovvraccaricata accetta come parametri sia un puntatore che un intero

## array ##

* lo standard C++ 99 non accetta array creati dinamicamente

## pointers and const ##

* Usando i puntatori due oggetti vengono  coinvolti: il puntatore e l'oggetto puntato.
  * **prefissando** *const* alla dichiarazione del puntatore fa si che l'oggetto puntato, e non il puntatore, sia costante. (es. 
  ```C++
  char s[] = "Gorm";
  const char* pc = s;
  ```
  )
  *  per creare un puntatore costante a un oggetto è necessario usare **\*const** 
  (es. 
  
  ```C++
  char *const pc;
  ```
  )

* Una costante indica solo una promessa di non voler modificare il valore, ma se questa viene acceduta in altri modi è possibile modificare il valore della costante

## lvalue references ##

* un riferimento è visto come un'alias di un' oggetto
* un riferimento deve essere sempre inizializzato
* un riferimento non è un oggetto, a differenza dei puntatori
* non esistono operatori che operano su riferimenti ma solo sull'oggetto riferito
* è possibile accedere al puntatore dell'oggetto riferito tramite il riferimento
* è possibile avere un riferimento costante mediante la parola chiave const prefissa al riferimento, quindi è possibile:

``` C++
double& dr = 1; //errore è necessario usare un lvalue
const double& cdr {1};

//l'implementazione è questa:

double temp = double{1};
const double& cdr {temp};
```
## rvalue reference ##

* Un riferimento a rvalue è un riferimento a un oggetto temporaneo, il quale l'utente del riferimento può modificare, **assumendo che questo valore non venga mai più utilizzato**.
* Utilizzando il riferimento a rvalue è possibile trasformare delle possibili copie costose in movimenti "economici", quindi ottimizzando il passaggio di informazioni.

## reference to reference ##

* è possibile avere un riferimento a un riferimento di un tipo, ciò che si ottiene è un riferimento a quel tipo, piuttosto che un tipo speciale di riferimento a un riferimento di tipo.

``` C++
//riferimento a rvalue di int
  using rr_i = int&&;
  using rr_rr_i = rr_i&&;

  rr_rr_i z {10};
```

