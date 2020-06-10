#include "startup.hh"

extern MYSQL Database::global_mysql;
extern std::unordered_map<std::string, Data::User> Data::map_user;
extern std::unordered_map<std::string, Data::Group> Data::map_group;
extern std::string Data::web_root;
extern std::string Data::web_page;
extern std::string Data::web_404_page;

Ltalk::StartUp::StartUp() {

}

Ltalk::StartUp::~StartUp() {

}

bool Ltalk::StartUp::Init(int argv, char **argc) {
    setbuf(stdout, nullptr);

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

    if(LoadConfig() == false) {
        std::cout << "Load config file failed!\n" << std::endl;
        abort();
    }

    if(RunDatabaseModule() == false) {
        std::cout << "Run database module error\n";
        abort();
    }
    //std::cout << "global_mysql_ptr" << global_mysql_ptr << '\n';
    if(RunLoggerModule() == false) {
        std::cout << "Run logger module failed" << std::endl;
        abort();
    }

    std::cout << "tcp port: " << tcp_port_ << "  number of thread: " << number_of_thread_ << '\n';
    if(false == this->RunNetworkModule()) {
        std::cout << "Run network module failed!\n";
        abort();
    }

    return true;
}

// Load config file
bool Ltalk::StartUp::LoadConfig() {
    using json = nlohmann::json;
    std::string file_json;
    FILE* config_file_ptr = fopen(DEFAULT_CONFIG_FILE, "r");
    while(!feof(config_file_ptr)) {
        char buffer[MAX_BUF_SIZE];
        fread(buffer, 1, MAX_BUF_SIZE, config_file_ptr);
        file_json += buffer;
    }
    fclose(config_file_ptr);

    json obj;
    try{
        obj = json::parse(file_json);
    } catch(json::parse_error &e) {
        d_cout << e.what() << '\n';
        return false;
    }
    // Parse json
    try {
        tcp_port_ = obj["server"]["tcp_port"];
        udp_port_ = obj["server"]["udp_port"];
        number_of_thread_ = obj["server"]["number_of_thread"];
        log_path_ = obj["server"]["log_path"];
        db_host_ = obj["database"]["host"];
        db_port_ = obj["database"]["port"];
        db_user_ = obj["database"]["user"];
        db_password_ = obj["database"]["password"];
        db_name_ = obj["database"]["name"];
        Data::web_root = obj["server"]["web_root"];
        Data::web_page = obj["server"]["web_page"];
        Data::web_404_page = obj["server"]["web_404_page"];
    }  catch (json::exception &e) {
        d_cout << e.what() << '\n';
        abort();
    }
    return true;
}
bool Ltalk::StartUp::RunNetworkModule() {
    Net::Net net(tcp_port_, number_of_thread_);
    net.Start();

    return true;
}
bool Ltalk::StartUp::RunDatabaseModule() {
    return Database::Mysql::Connect(db_host_, db_user_, db_password_, db_name_, db_port_);;
}
bool Ltalk::StartUp::RunLoggerModule() {
    return true;
}
