#include <iostream>
using namespace std;

struct A {
  virtual void f(int)
  { cout << "A::f(int)" << endl; }
  virtual void f(double) { cout << "A::f(double)" << endl; }
  virtual void g() { cout << "A::g(double)" << endl; }
  virtual ~A() { cout << "Destructor A::~A()" << endl; }
};

struct B : public A {
  void f(int) { cout << "B::f(int)" << endl; }
  virtual void f(double) const { cout << "B::f(double)" << endl; }
  virtual void g(int) { cout << "B::g(int)" << endl; }
  ~B() { cout << "Destructor B::~B()" << endl; }
};

struct C : public B {
  void f(int) const { cout << "C::f(int)" << endl; }
  void g(int) { cout << "C::g(int)" << endl; }
  ~C() { cout << "Destructor C::~C()" << endl; }
};

int main() {
  A* a = new A;
  B b;
  C c;
  A& ra_b = b;
  B& rb_b = b;
  A& ra_c = c;
  B& rb_c = c;
  cout << "=== 1 ===" << endl;
  ra_b.f(1); // vado dentro A e trovo la funzione desiderata ma vedo che tipo statico e dinamico non coincidono , quindi scendo alla classe base e vede se è stato fatto everriding e quindi la scelgo
  rb_b.g(1); // qui tipi statico e dinamico concidono quindi prendo sicuro quello della classe B
  ra_c.f(1); //tipo statico e dinamico non coincidono, quindi vado prima nella classe A e trovo la funzione, questa è virtual e quindi vado a cercare nelle classi derivate. nella classe B fa overriding e la scelgo, non scelgo quella della classe C prchè è etichettata const qundi è diversa dalla funzione desiderata
  rb_c.g(1); // tipo statico e dinamico non coincidono quindi vado nella classe B e vedo se la trovo. nella classe B e presente ed è anche virtual, allora vado a vedere se è stato fatto overriding nella classe C, e in effetti è stato fatto. quindi scelgo quello della classe C
  cout << "=== 2 ===" << endl;
  static_cast<A*>(&b)->f(1.2);// prende l'oggetto che sta dentro le parentesi tonde() e lo trasforma nel tipo delle che sta dentro le parentesi < >. quindi è come se diventasse A b; e vado a cercare la funzione f dentro la classe A
  static_cast<A*>(&c)->f(1); // traforma l'oggett c nel tipo A, quindi come se avessi A c; vado a cercare la funzione f dentro A la trovo ed è virtuale e quindi faccio overriding con la classe B e scelgo quella
  static_cast<B*>(&c)->g(1.2); // stessa cosa fatta per le due precedenti
  cout << "=== 3 ===" << endl;
  b.f(2); // entro nella classe B e prendo quello
  c.g(3); // entro nella classe C e prendo quello
  cout << "=== 4 ===" << endl;
}

////svolgimento
/*
stamperà:
=== 1 ===
B::f(int);
B::g(int);
B::f(int);
C::g(int);
=== 2 ===
A::f(double)
B::f(int)
C::g(int)
=== 3 ===
B::f(int);
C::g(int);
=== 4 ===
Destructor C::~C()
Destructor B::~B()
Destructor A::~A()
Destructor B::~B()
Destructor A::~A()

*/
