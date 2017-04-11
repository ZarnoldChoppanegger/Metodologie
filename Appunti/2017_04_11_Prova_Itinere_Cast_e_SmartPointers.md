###20141125###

#### exe1 #####
* conversione da tipo classe facendo operator nome_tipo_a_cui_convertire
* guarda namespace, candidate, sono utilizzabili? (guardi conversioni)

``` c++
void g(int i , double d);
void g(int i, int j);

N::g(2.3, 5);
/* su primo argomento pari, sul secondo la prima conversione std la seconda match perfetto, chiama la seconda */

N::g(5, 2.3);
/* primo argomento match perfetto enrambe le chiamate, sul secondo il match perfetto si ha solo per la prima funzione*/
```
* chiamata H, l'oggetto passato è modificabile, la 9 non è utilizzabile, rimangono 7 e 10, nella 10 qualificazione, nella 7 match perfetto quindi si chiama la 7. 
* conversioni implicite tra ppuntatori, 0 in qualsiasi puntatore, nullpr, prendere qualunque puntatore in void, puntatore a oggetto classe derivata può essere convertito implicitamente a puntatore della clsse base l'inverso non si può fare perchè non si vule rompere il contratto, non si hanno garanzie. Classe derivata eredito metodi classe base più altri definiti, ma nessuno garantisce che classe base è anche derivata. 
* implicitamente viene invocato il distruttore, il distruttore non è qualificato a const, però una regola del linguaggio dice che un distruttore anche se non const può essere invocato da un oggetto costante

#### exe2 #####
* deve essere un codice che da errore e questo sparisce quando inserisco guardie
(guarda codice exe2.cc)

#### exe3 #####
* const_cast, fare un cast significa "tramettere" da un tipo di dato a un altro, in linguaggi di programmazione con cast si intende conversione esplicita(forzata)
``` c++
int foo(int);

int Louis_Cypher(const int& ri){
	int& r = ri; // non funziona
	int& r = const_cast<int&>(ri); // sentirsi in colpa (ma non sempre)
	r = 2 * r +1;
	//tramite r posso fare modifiche e imbroglio l'utente
	
	/* se l'oggetto sul quale voglio fare const_cast è nato oggetto modificabile OK
		sorprese per l'utente, se però l'oggetto nasce come costante allora il compilatore
		sarebbe autorizzato a fare cose strane, il compilatore può allocare in zone di memoria di sola lettura,
		perciò se togliamo const andiamo in undefine behaviour (demoni usciranno dal naso).
		Può avere senso const_cast quando, per esempio nella razionale andava fatto la normalizzazione del razionale,
		la classe è eagher(?), è possibile però pensare a implementazioni LAZY, es la classe razionale potrebbe non fare semplificazione subito,
		lo semplificherà solo quando sarà necessario, e quando diventa necessario? Non lo sappiamo. Es stampa razionale, da documentazione protrebbe
		dire che dovrebbe stampare numero normalizzato, la routine è const, io vorrei modificare l'oggetto però, e nella routine const non posso farlo,
		si ha differenza tra rappresentazione fisica e logica, fisica cambio oggetto ma all'utente dal punto di vista logico rimane lo stesso oggetto.
	*/
}

int main(){
  int i = (int) 5.7; //cast stile C; da evitare, perchè è del C roba vecchia...
  //...tranne quando si annotano le funzioni per dire al compilatore che se qualcuno ignora il risultato della funzione lancia un warning, in quel caso l'utente potrebbe voler deliberatamente voler evitare i segnali di warninig e quinidi buttare il risultato
  
  (void) foo(5); //uso idiomatico; consentito
  
 int j = static_cast<int>(5.7); //cast statico.
 /* statico perchè tutto noto a tempo di compilazione, è "buono" perchè i cast espliciti segnalano forzature di tipo,
     quando leggi codice lo vedi subito che viene fatta la conversione, quello implicito è difficile da trovare
	 e quindi vuoi rendere evidente un'operazione pericolosa, come nel software critico.
	 Alcune volte si può dire al compilatore che ogni volta che vede conversione implicita, segnali errore
	 in modo che l'utente pensi a quello che sta facendo.
 */
 
 void* q = ....
 
 int* p = reinterpret_cast<int*>(q); // cast di reinterpretazione (wild casts)
 /* più brutti ma a volte necessari, come codice per rappresentazione binaria di file
	 prima nel file ci sarà del codice che dice la posizione dei dati nel file, quindi mi sposto
	 in quelle locazioni per recuperare dati nel rispettivo tipo, lo so io utente non il compilatore.
	 È possibile farlo anche in stile C.
 */

int* p = reinterpret_cast<int*>(q);

int k = int(5.7); 
/* cast funzionale. 
	perchè? corrisponderebbe a infocazione del costruttore e funziona anche per tipi built-in. 
	Si usa con tipi definiti dall'utente oppure con parametri template 
*/

Base* pb = new Derived;
Derived* pb = dynamic_cast<Derived*>(pb); // cast dinamico
/* "chiedo" di vedere l'oggetto al tipo richiesto se possibile, 
	a tempo di esecuzione controllo se è possibile vederlo a quel tipo.
	Questo è l'unico cast che non ha equivalente nel C, perchè è un cast che sfrutta l'O.O.
*/

}
```

* dobbiamo controllare tramite asserzioni, quando voglio controllare invariante assert di check_inv() ma non quando codice in esecuzione.
* in quali punti vogliamo controllare l'invariante? tutti i metodi const non modificano l'invariante (se non fanno const_cast)

``` c++
#include <cassert>

C(int a, int b){
	//check_inv(); //non ha senso
	//Implementazione costruttore
	assert(check_inv());
}

void foo(C& y) {
 assert(check_inv() && y.check_inv());
 //codice che implementa il metodo foo
 //alla fine devo controllare la stessa cosa, perchè il metodo non è const e non è segnato const nemmeno y quindi va controllato anche questo
 assert(check_inv() && y.check_inv());
}

void bar(const C& y) {
  assert(check_inv() && y.check_inv());
  //codice che
  assert(check_inv()); 
  /* non è obbligatorio fare check su y, non è un male ma inopportuno */
}

void ying(const C& y) //uguale a bar

void yang(const C& y) const{
  assert(check_inv() && y.check_inv());
  // codice che implementa il metodo
  //è inutile controllare alla fine perchè il metodo assicura di non modificare oggetti
}

~C() {
  assert(check_inv());
  // codice che implementa il distruttore.
  // (ricordati come funzione costruzione e distruzione), prima costruisco sotto-oggetto della classe base e poi inizializzo quello che rimane, distruzione avviene in modo inverso
}

static void zen(int i, double d) {
	/* non dobbiamo controllare invariante perchè il metodo è statico (non ciclo vita)
		non si applica alla classe e quindi non può accedere al puntatore this
	*/
}
```
### 20160422 ###

#### exe2 ####
type decay (qualificazine di lvalue) corrispondenza esatta, ai -> int*
ai[5]->const int* non è possibile, perchè voglio trasformare un intero in un puntatore a costante, se ai[5] = 0 non possibile perchè dovrebbe essere una costante per convertire
se ai + 5 -> const int* allora possibile perchè non dereferenzio

#### exe6 ####

``` c++
void foo() {
  A* a1 = new A(1);
  A* a2 = new A(2);
  try{
    job1(a1, a2);
	job2(a1, new A(3));
  }
  catch (...) {
    delete a2;
	delete a1;
  }
	
}

//devo fare in modo che si comporti bene

void foo() {
  A* a1 = new A(1);
  A* a2 = new A(2);
  try{
    job1(a1, a2);
	A* a3 = new A(3);
	job2(a1, a3);
	delete a3;
	delete a2;
	delete a1;
  }
  catch (...) {
    delete a2;
	delete a1;
  }
	
}
```
1. "Quando non si hanno eccezioni le risorse di a1,a2,a3 non vengono rilasciate", e proporre soluzione.
2. Cosa può andare storto nel caso vengano lanciate eccezioni? Deve esserci un blocco try catch per ogni risorsa acquisita
3. (bonus) try catch non neutrale

``` c++
void foo() {
  A* a1 = new A(1);
  try{
    A* a2 = new A(2);
    try {
		job1(a1, a2);
		A* a3 = new A(3);
		try {
			job2(a1, a3);
			delete a3;
		} catch (...) {
			delete a3;
			throw;
		}
		delete a2;
		delete a1;
    } catch(...) {
	delete a2;
	throw;
  }
  }  catch (...) {
	delete a1;
	throw;
  }	
}
```
Spiegare perchè la soluzione si può dire exception safety?
* Ho messo blocco try catch dopo acquisizione risorsa e si chiude dopo eliminazione risorsa, a prescindere di quello che succede le risorse vengono deallocate
* Ci sono ipotesi particolari: il distruttore della classe A deve essere no-throw.

#### smart pointers ####
Gli altri vengono chiamati dumb pointers o naked pointers.
* gli smart pointers sono stati definiti nel c++11.
* unique_ptr puntatori che possiedono un oggetto in maniera unica, chiamano distruttore in automatico, non possono esse copiati ma possono essere spostati
* share_ptr pensati nel caso si vuole lavorare su una risorsa a partire da più puntatori, al loro intero oltre al puntatore dumb si portano un "contatore" che indica quanti sono gli share_ptr che puntano alla risorsa (reference counting). Tutti gli oggetti che condividono la risorsa possono andare fuori scope, ma non nello stesso ordine, se vanno fuori scope solo l'ultimo a puntare chiama il distruttore.
* hanno una cosa in più  rispetto a idioma RAII, conversione a puntatore grezzo.

``` c++
//header in cui vengono definiti smart pointers
#include <memory>

void foo() {
 std::unique_ptr<A> a1 = new A(1);
 std::unique_ptr<A> a2 = new A(2);
 job1(a1, a2);
 job(a1, std::unique_ptr<A>(new A(3)));	
}

/* funziona quasi tutto ma può esserci un problema */

job2(std::unique_ptr<A>(new A(3)), std::unique_ptr<A>(new A(4)));

/* il linguaggio non dichiara in modo esplicito come vengono valutati gli argomenti di una funzione 
	si può avere memory leak, quindi mai smart pointers in chiamata di funzione
*/
```
In c++98 c'era una cosa simile

``` c++
std::auto_ptr<A>
```
Sono deprecati perchè non funzionano bene, perchè nel c++ 98 non c'era semantica di spostamento. C'erano vagonate di codice nella stl nel quale si presumeva di fare la copia e auto_ptr non funzionava.

#### exe3 ####

``` c++
struct Matrix {
	size_type num_rown() const;
	
	/* è legittimo modificare e ci da accesso in lettura e scrittura */
	value_type& get(size_type row, size_type col);
	const value_type& get(size_type row, size_type col) const;
	
	/* il this deve essere modificabile,
		devi fare riferimento a costante,
		restituire per riferimento per concatenare
	*/
	Matrix& operator+=(const Matrix& y);
	
	/* non modifica neanche x, quindi this costante
		restituzione per copia perchè altrimenti perdo il reference,
		per questo operatore è il motivo per il quale sono stati fatti move constructor e move assignment
	*/
	Matrix operator+(const Matrix& y) const;
	
	void print(ostream& os) const;
	
	/* meno unario,
		calcola il negato ma non modifica,
		creo per copia un'altra matrice nella quale ci sono gli elementi negati
	*/
	Matrix operator-() const;
}
```
**Test driven design**

``` c++
void foo(const Matrix& m){
	auto n_rows = m.num_rows();
	/* non posso farlo perchè m è riferimento a costante,
		non esiste alcun costruttore che permette di trasformare riferimento non costante i costante
	*/
	
	auto v = m.get(1, 1);
	/* non posso farlo perchè la get funziona in lettura scrittura,
		se voglio accedere in sola lettura non posso, il metodo non è sbagliato,
		devo poter scrivere nella matrice, ma devo anche poter leggere senza const_cast,
		quindi scrivo due get...
	*/
}

//non è match perfetto...
void foo(Matrix& m){
	auto n_rows = m.num_rows();
}
```

