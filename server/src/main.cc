#include <iostream>
#include "startup.hh"

int main(int argv, char **argc) {

    if(argv < 2) {
        std::cout << "-h get more info" << std::endl;
        return 0;
    }

    Ltalk::StartUp startup;
    std::string arg = argc[1];
    if(arg == "-h" || arg == "--help") {
        std::cout << "Usage: ./ltalks [OPTION...] [SECTION] PAGE...\n"
                     "-r, --run    run ltalk server\n"
                     "-s, --stop   stop ltalk server\n"
                     "-h, --help   help of ltalk server\n"
                     ;
    }else if(arg == "-r" || arg == "--run") {
        startup.Run(); // 启动服务
    }else if(arg == "-s" || arg == "--stop") {

    }else if(arg == "-p" || arg == "--print") {

    }else {
        std::cout << "-h get more info" << std::endl;
    }

    return 0;
}
