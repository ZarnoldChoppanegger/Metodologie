#include <string>
#include <iostream>

namespace N {
  class C {
  private:
    std::string& first(){                // funzione #1
      std::cout<< "1\n";}
  public:
    const std::string& first() const{    // funzione #2
      std::cout<< "2\n";}
    std::string& last(){                 // funzione #3
      std::cout<< "3\n";}
    const std::string& last() const{     // funzione #4
      std::cout<<"4\n"; }
    C(const char*){                      // funzione #5
      std::cout<<"5\n";}
  }; // class C
  
  void print(const C&){                  // funzione #6
    std::cout<<"6\n";}
  
  std::string& f(int){                    // funzione #7
    std::cout<<"7\n";} 

} // namespace N
class A {
public:
  explicit A(std::string&){              // funzione #8
    std::cout<<"8\n";}
};

void print(const A&){                    // funzione #9
  std::cout<<"9\n";}

void f(N::C& c)                         // funzione #10 
{
  std::cout<< "10\n";
  const std::string& f1 = c.first();                  //chiamata A
  std::string& f2 = c.first();                        //chiamata B
  const std::string& l1 = c.last();                   //chiamata C
  std::string& l2 = c.last();                         //chiamata D
}

void f(const N::C& c)                   // funzione #11 
{
  std::cout <<"11\n";
  
  const std::string& f1 = c.first();                     //chimata E;
  std::string& f2 = c.first();                       //chiamata F
  const std::string& l1 = c.last();                    // chiamata G
  std::string& l2 = c.last();                          // chiamata H
}

int main() {
  N::C c("begin");                                     // chimata I
  f(c);                                                //chiamata L
  f("middle");                                         // chiamata M
  //  print("end");                                        // chiamata N
}



/* svolgimento

A    C={1,2}  U={1,2}  M={1} // dato che nella funzione la "c" non ci viene passata costante vado a prendere a funzione 1 perchè e non costante
B    C={1,2}  U={1,2}  M={1} // ci viene dato errore perchè è un metodo privato
C    C={3,4}  U={3,4}  M={3}
D    C={3,4}  U={3,4}  M={3}
E    C={1,2}  U={1,2}  M={2}
F    C={1,2}  U={1,2}  M={2}
G    C={3,4}  U={3,4}  M={4}
H    C={3,4}  U={3,4}  M={4}
I    C={5}  U={5}  M={5}
L    C={10,11}  U={10,11}  M={10}
M    C={10,11}  U={10,11}  M={11}
N    C={9}  U={9}  M={9}

*/
