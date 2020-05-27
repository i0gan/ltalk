#include "startup.hh"
StartUp::StartUp() {

}

StartUp::~StartUp() {

}

bool StartUp::Init(int argv, char **argc) {

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
    }else if(arg == "-r" || arg == "--run") {
        this->Run();
    }else if(arg == "-s" || arg == "--stop") {

    }else if(arg == "-p" || arg == "--print") {

    }else {
        std::cout << "-h get more info" << std::endl;
    }

    return true;
}

bool StartUp::Run() {

    std::cout << "Load config file" << std::endl;
    this->LoadConfig();
    std::cout << "init network" << std::endl;
    this->InitNetwork();
    std::cout << "Init database" << std::endl;
    this->InitDatabase();
    std::cout << "Init log" << std::endl;
    this->InitLog();

    return true;
}


bool StartUp::LoadConfig() {
    return true;
}
bool StartUp::InitNetwork() {
    return true;
}
bool StartUp::InitDatabase() {
    return true;
}
bool StartUp::InitLog() {
    return true;
}


//int socket_bind_listen(int port) {
//    if(port < 1024 || port > 65535)
//        return -1;
//    int listen_fd = 0;
//    //if(listen_fd = socket(A))
//}
