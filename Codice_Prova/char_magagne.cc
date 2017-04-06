#include <iostream>

int main(){
  // i char sulla tua macchina sono implementati come unsigned o signed char?

  //255 => tutti uno
  unsigned char uc = 255;
  signed char sc = 255;
  char c = 255;

  std::cout << "unsigned char : " << int{uc}
                  << "\nsigned char : " << int{sc}
                  << "\nchar : " << int{c}
                  << "\n";

  return 0;
}
