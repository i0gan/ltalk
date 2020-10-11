#include <iostream>
#include <signal.h>

#include "startup.hh"
Ltalk::StartUp startup;

void quit(int s) {
    startup.Stop();
}
int main(int argv, char **argc) {

    if(argv < 2) {
        std::cout << "--help to get help" << std::endl;
        return 0;
    }
    // 绑定退出
    signal(SIGINT, quit);
    startup.ShowText();
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
