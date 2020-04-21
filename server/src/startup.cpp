#include "startup.h"
StartUp::StartUp() {

}
StartUp::~StartUp() {

}

bool StartUp::init(int argv, char **argc) {

    if(argv < 2) {
        std::cout << "-h get more info" << std::endl;
        return false;
    }

    std::string arg = argc[1];
    if(arg == "-h" || arg == "--help") {
        std::cout << "Usage: ./ltalks [OPTION...] [SECTION] PAGE...\n"
                     "-r, --run    run ltalk server\n"
                     "-s, --stop   stop ltalk server\n"
                     "-h, --help   help of ltalk server\n"
                  ;
    }
    if(arg == "-r" || arg == "--run") {
        this->run();
    }else if(arg == "-s" || arg == "--stop") {

    }else if(arg == "-p" || arg == "--print") {

    }else {
        std::cout << "-h get more info" << std::endl;
    }

    return true;
}

bool StartUp::run() {
    return true;
}


bool StartUp::loadConfig() {
    return true;
}
bool StartUp::initNetwork() {
    return true;
}
bool StartUp::initDatabase() {
    return true;
}
bool StartUp::initLog() {
    return true;
}


//int socket_bind_listen(int port) {
//    if(port < 1024 || port > 65535)
//        return -1;
//    int listen_fd = 0;
//    //if(listen_fd = socket(A))
//}
