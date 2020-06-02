#include "startup.hh"

Ltalk::StartUp::StartUp() {

}

Ltalk::StartUp::~StartUp() {

}

bool Ltalk::StartUp::Init(int argv, char **argc) {

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

bool Ltalk::StartUp::Run() {

    std::cout << "Load config file" << std::endl;
    this->LoadConfig();

    std::cout << "Run database module" << std::endl;
    this->RunDatabaseModule();
    std::cout << "Run logger module" << std::endl;
    this->RunLoggerModule();

    std::cout << "Run network module" << std::endl;
    if(false == this->RunNetworkModule()) {
        std::cout << "init network fail!\n";
        abort();
    }


    return true;
}

// 加载配置文件
bool Ltalk::StartUp::LoadConfig() {
    using json = nlohmann::json;
    struct stat sbuf;
    if(stat(config_file_.c_str(), &sbuf) < 0) {
        d_cout << "The config file [" << config_file_ << "] is not existed!\n";
        abort();
    }

    int fd = open(config_file_.c_str(), O_RDONLY);
    if(-1 == fd) {
        d_cout << "Open config file failed!\n";
        abort();
    }
    void *mmap_ptr = mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if(mmap_ptr == (void *)-1) {
        d_cout << "mmap config file failed!\n";
        abort();
    }
    std::string file_json = static_cast<char *>(mmap_ptr);
    munmap(mmap_ptr, sbuf.st_size);

    json obj;
    try{
        obj = json::parse(file_json);
    } catch(json::parse_error &e) {
        d_cout << e.what() << '\n';
        abort();
    }
    //解析json
    try {
       std::string tcp_port = obj["server"]["tcp port"];
       tcp_port_ = atoi(tcp_port.c_str());

       std::string udp_port = obj["server"]["udp port"];
       udp_port_ = atoi(udp_port.c_str());

       std::string number_of_thread = obj["server"]["number of thread"];
       number_of_thread_ = atoi(number_of_thread.c_str());

       log_path_ = obj["server"]["log path"];
       db_host_ = obj["database"]["host"];

       std::string db_port = obj["database"]["port"];
       db_port_ = atoi(db_port.c_str());
       db_user_ = obj["database"]["user"];
       db_password_ = obj["database"]["password"];
       db_name_ = obj["database"]["name"];

    }  catch (json::exception &e) {
        d_cout << e.what() << '\n';
        abort();
    }

    //std::cout << "http_port " << http_port_ << std::endl;
    return true;
}
bool Ltalk::StartUp::RunNetworkModule() {
    EventLoop base_eventloop;
    Ltalk::Net net(tcp_port_, number_of_thread_, &base_eventloop);
    d_cout << "tcp port: " << tcp_port_ << "  number of thread: " << number_of_thread_ << '\n';
    net.Start();

    return true;
}
bool Ltalk::StartUp::RunDatabaseModule() {
    return true;
}
bool Ltalk::StartUp::RunLoggerModule() {
    return true;
}

//int socket_bind_listen(int port) {
//    if(port < 1024 || port > 65535)
//        return -1;
//    int listen_fd = 0;
//    //if(listen_fd = socket(A))
//}
