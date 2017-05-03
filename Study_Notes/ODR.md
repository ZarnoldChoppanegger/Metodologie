## ODR ##

* L'intento della ODR è quello di permettere l'inclusione di definizioni (classi, template, enum...)  in differenti unità di traduzione da un unico comune file sorgente.
* Quando si viola la ODR di potrebbe non ricevere alcuna notifica di questo e quindi le cose iniziano a funzionare in modo bizzarro.
* Abbiamo un insieme di unità di traduzione. Le unità di traduzione diventano object file, che poi vengono linkati. [unità di traduzione] --> .cpp + .hpp
* Data una funzione foo() dichiarata in un header, sarebbe strano avere più definizioni della funzione. L'implementazione dovrebbe gestire più definizioni.

> ISO 6.2.1- "No translation unit **shall** contain more than one definitions of any variable, function, calss type, enumaration type, or template"
Quel "shall" significa che si è certi che il compilatore ritornerà un errore se vi è più di una dichiarazione nella **stessa unità di traduzione**

>ISO 6.2.4- "Every **program** shall contain exactly one definition of every **non-inline function** or **variable** template that is odr-used in that program; no diagnostic required"
Se hai due unità di traduzione e in entrambe c'è la stessa definizione di una funzione, la regola è violata. No diagnostic required significa che è responsabilità del programmatore rispettare questa regola, lo standard dichiara che il compilatore o linker non hanno l'obbligo di lanciare alcun errore; può succedere di tutto, *undefine behaviour*. In pratica il linker se ne rende conto e lancia errore (unresolved external).
* Definizioni di funzioni non possono andare in header file, poichè se includessi l'header in più file mi ritroverei con molteplici implementazioni della funzione, violando quindi la ODR.

**Quindi cosa posso definire negli header file?**
* **funzioni inline** delle quali è necessario a tempo di compilazione avere subito a disposizione la definizione e quindi si è obbligati ad inserirla nell'header file.
* **funzioni template** quando conosco il tipo T, solo allora potrò definire la funzione. Quindi l'implementazione della funzione dipende dal tipo T. Il codice oggetto oggetto prodotto dal compilatore  dipenderà dal corpo della funzione template. Quando si chiama una funzione template, la definizione della funzione deve essere disponibile. Questo è simile alle funzioni inline, dove il corpo è necessario, in questo caso è necessario per poter instanziare il template. Quindi se definiamo un template in un file sorgente e poi proviamo ad usarlo in un altro, non sarà possibile perchè il compilatore non avrà a disposizione il corpo del template.

Queste fanno eccezione alla ODR perchè se non definite nell' header file non sarebbe possibile usarle. Quindi la ODR permette di avere **molteplici definizioni della stessa funzione inline o template** fintanto che queste definizioni:
1. appaiono in **diverse unità di traduzione**
2. sono **identiche** token a token
3. il **significato** di questi **token è lo stesso** in ogni unità di traduzione

``` c++
/* TUTTO VALIDO */

//s.h
struct S { int a; char b; };
void f(S∗);

// file1.cpp:
#include "s.h"
// use f() here

// file2.cpp:
#include "s.h"
void f(S∗ p) { /* ... */ }
``` 

Se si viola la ODR per queste funzioni, no diagnostic required e in pratica il linker non darà alcun errore o avviso, quindi tutto compilerà sembrerà andare bene ma demoni usciranno dal naso... (è importante rispettare questa regola, è la più inportante della ODR).

Quindi, definire una funzione inline o template in un header va bene, perchè la definizione che comparirà in ciascuna unità di traduzione sarà identica. Il linker quando vedrà i file oggetto con molteplici definizioni di questo tipo di funzioni, ne prenderà solo una e le altre le scarterà.
