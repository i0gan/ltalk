#include <iostream>
#include "startup.hh"

int main(int argv, char **argc) {
    Ltalk::StartUp startup;
    startup.Init(argv, argc);
    return 0;
}
