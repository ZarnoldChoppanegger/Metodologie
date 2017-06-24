## Scope ##

* **Scope locale:** lo scope di un nome si estende dal punto della sua dichiarazione fino alla fine del blocco. Un blocco è identificato da { }
* **Scope di classe:** un nome viene chiamato nome membro di classe, se viene dichiarato all'interno di una classe e all'infuori di qualsiasi altra classe, funzione, enum di classe, o qualsiasi altro namespace. Lo scope del nome è visibile in tutta la classe, cioè dall'inizio della sua definizione " { " fino alla fine " } "
* **Scope di globale/namespace:** un nome è chiamato membro di namespace se è definito in un namespace all'infuori di qualsiasi altra classe, namespace, funzione. Lo spazio globale è tecnicamente visto come namespace
* **Scope di istruzione:** un nome fa parte di uno scope di istruzione se è definito all'interno di () delle istruzioni while-, for-, if-, switch-. Il nome è visibile fino alla fine del blocco dell'istruzione.
* **Scope di funzione:**

## Ciclo di vita ##

* **Ciclo di vita statico:** oggetti creati in namespace, classi o funzioni che vivono fino alla termine del programma. Possono procurare problemi in programmazione thread, perchè sono risorse condivise
* **Ciclo di vita dinamico:** gli oggetti non seguono la strategia LIFO ma vengono creati dall'utente a run-time e quindi vengono allocati nell' HEAP. L'utente decide quando creare un oggetto con instruzione new e ha l'obbligo di terminare il suo ciclo di vita al momento giusto tramite struzione delete, per evitare di avere problemi come memory leak.
* **Ciclo di vita automatico:** il ciclo di vita è controllato dal compilatore. Un oggetto viene creato quando viene data la sua dichiarazione e muore quando va fuori scope. In un'implementazione tipica, gli oggetti automatici vengon allocati in uno stack. A ogni chiamata di funzione viene data una porzione di stack, chiamata stack frame, nella quale può memorizzare tutti i suoi oggetti automatici.
* **Oggetti temporanei:** (es. valori intermedi di computazione o oggetti usati per mantenere un valore per riferimento a argomento const)
* **Oggetti thread local:** oggetti locali ad un thread che vengono creati/distrutti quando il loro thread viene creato/distrutto.
