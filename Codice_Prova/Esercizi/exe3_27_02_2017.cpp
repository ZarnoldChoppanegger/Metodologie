// File FaxModem.hh
class FaxModem_AllStars {
private:
  // ...
  public:
  void fax_function_1();
  void fax_function_2(const char*);
  void fax_function_3(int);
  // ...
  void modem_function_1(const char*);
  void modem_function_2();
  void modem_function_3(unsigned int);
  // ...
};

/////////////////////////////////////////////////////////////////////

//Fax_Interface.hh

class Fax_Interface {
public:
  virtual void fax_function_1() = 0;
  virtual void fax_function_2(const char*) = 0;
  virtual void fax_function_3(int) = 0;
  //...
};

/////////////////////////////////////////////////////////////////////

//Fax_AllStarts_Adapter.hh
#include "Fax_Interface.hh"
#include "FaxModem_AllStars.hh"

class Fax_AllStars_Adapter : public Fax_Interface {
public:
  void fax_function_1() {
    fax.fax_function_1();
  }
  void fax_function_2(const char* c) {
    fax.fax_function_2(c);
  }
  void fax_function_3(int i) {
    fax.fax_function_3(i);
  }
private:
  FaxModem_AllStars fax;
};

/////////////////////////////////////////////////////////////////////

//Modem_Interface.hh

class Modem_Interface {
public:
  virtual void modem_function_1(const char*) = 0;
  virtual void modem_function_2() = 0;
  virtual void modem_function_3(unsigned int) = 0;
};

/////////////////////////////////////////////////////////////////////

#include "Modem_Interface.hh"
#include "FaxModem_AllStarts.hh"

class Modem_AllStars_Adapter : public Modem_Interface {
public:
  void modem_function_1(const char* c) {
    modem.modem_function_1(c);
  }
  void modem_function_2() {
    modem.modem_function_2();
  }
  void modem_function_3(unsigned int ui) {
    modem.modem_function_3(ui);
  }

private:
  FaxModem_AllStars modem;
};

/////////////////////////////////////////////////////////////////////

// File User.cc
#include "Fax_Interface.hh"
#include "Modem_Interface..hh"

void user_function_1(Fax_Interface& f) {
  f.fax_function_1();
  // ...
  f.fax_function_3(12);
  // ...
}
void user_function_2(Modem_Interface& m, const char* command) {
  m.modem_function_1(command);
  // ...
  m.modem_function_3(1024);
  // ...
}
void user_function_3(Fax_Interface& f, Modem_Interface& m) {
  f.fax_function_2("+390521906950");
  // ...
  m.modem_function_2();
  //...
}
