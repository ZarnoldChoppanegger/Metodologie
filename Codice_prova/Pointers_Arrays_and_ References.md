## void* ##

* un puntatore (void*) è interpretato come puntatore a un oggetto di tipo sconosciuto

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


