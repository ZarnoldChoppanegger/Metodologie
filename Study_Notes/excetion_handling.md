## Exception handling ##

### Exceptions ###

Nel caso venga violata un'invariante e si generi un errore il creatore della classe/libreria non sa come l'utente voglia gestire l'errore. D'altra parte l'utente della classe non può costantemente controllare che non avvengano errori, altrimenti l'invariante non verrebbe mai violata.
La soluzione è quella di rilevare la violazione dell'invariante e avvisare l'utente di ciò. Quest'ultimo poi deciderà quali siano le operazioni più appropriate per gestire l'errore.
Per lanciare una segnalazione dell'avvenuta di un errore si usa il costrutto **throw**. Il codice lanciante l'eccezione andrà poi messo dall'utente in un blocco **try** il quale poi passerà il controllo dell'esecuzione del programma, nel caso di avvenuta eccezione, ad uno specifico blocco **catch** nel quale l'utente definisce il comportamento che il programma deve avere nel caso di errori.

### Invarianti ###

L'uso delle eccezioni per segnalare un errore sono un esempio di come una funzione controlla i suoi argomenti, e si "rifiuta" di proseguire l'esecuzione poichè una supposizione di base, cioè una **precondizione** non è stata rispettata.

>Ogni volta che definiamo una funzione, dovremmo considerare quali sono le sue precondizioni e se possibile testarle

>Il compito del costruttore è stabilire l'invariante di classe (creare l'ambiente nel quale le funzioni membro devono operare) e in oltre spesso richiedono l'acquisizione delle risorse

Le funzioni membro devono controllare che l'invariante si corretta anche dopo la loro esecuzione (**postcondizioni**).

### Asserzioni ###

Ci sono alcuni programmi che non possono lanciare eccezione, poichè non è possibile gestirle o verificare le precondizioni. I motivi di ciò possono essere legati alle performance o alla continuità del sistema.
Un modo per evitare il controllo delle asserzioni durante il normale funzionamento è quello di spostarle alla sola fase di debuging. Ciò è possibile mediante la macro **assert(A)** , contenuta nella libreria standard **<cassert>**, la quale esegue il controllo sulla precondizione **A** se e solo se non è definita la macro **#NDEBUG**. Se l'asserzione fallisce il compilatore manda un messaggio d'errore indicando la posizione nella quale l'asserzione è stata valutata.

``` c++
#include <iostream>
//uncomment to disable assert()
//#define NDEBUG
#include <cassert>

int main()
{
  assert(2 + 2 == 4);
  std::cout << "nessun errore lanciato da compilatore\n";
  assert(2+ 2 == 5);
  sdt::cout << "non stamperà mai questo messaggio perchè lancerà arrore\n";
  
  return 0;
}
```
