// compilare con -fpermissive
#include <iostream>

int main() {
    const float* f = 1234;

    //cosa succederà? scoppia in bocca?
    std::cout << *f << '\n';

    return 0;
}
