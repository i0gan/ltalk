#include <iostream>
#include "startup.h"

int main(int argv, char **argc) {
    StartUp startup;
    startup.init(argv, argc);
    return 0;
}
