#include <iostream>
#include "startup.hh"

int main(int argv, char **argc) {
    StartUp startup;
    startup.Init(argv, argc);
    return 0;
}
