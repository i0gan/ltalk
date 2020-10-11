#include "startup.hh"

extern MYSQL Database::global_mysql;
extern std::unordered_map<std::string, Data::User> Data::map_user;
extern std::unordered_map<std::string, Data::Group> Data::map_group;
extern std::string Data::web_root;
extern std::string Data::web_page;
extern std::string Data::web_404_page;

Work::EventLoop *Data::work_eventloop = nullptr;

Ltalk::StartUp::StartUp():
    sp_work_eventloop_thread_(new ::Work::EventLoopThread()){
}

Ltalk::StartUp::~StartUp() {
}

void Ltalk::StartUp::ShowText() {
    std::string show_text= ""
            "\033[40;30m _   _____  _    _     _  __    ____  _____ ______     _______ ____  \n\033[0m"
            "\033[40;31m| | |_   _|/ \\  | |   | |/ /   / ___|| ____|  _ \\ \\   / / ____|  _ \\  \n\033[0m"
            "\033[40;32m| |   | | / _ \\ | |   | ' /    \\___ \\|  _| | |_) \\ \\ / /|  _| | |_) | \n\033[0m"
            "\033[40;33m| |___| |/ ___ \\| |___| . \\     |__) | |___|  _ < \\ V / | |___|  _ <  \n\033[0m"
            "\033[40;36m|_____|_/_/   \\_\\_____|_|\\_\\   |____/|_____|_| \\_\\ \\_/  |_____|_| \\_\\ \n\033[0m";

    std::cout << show_text;
}
bool Ltalk::StartUp::Run() {
    //setbuf(stdout, nullptr);
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

    if(false == RunWorkModule()) {
        std::cout << "Run work module failed!\n";
        abort();
    }

    StartWork();

    std::cout << "tcp port: " << tcp_port_ << "  number of net thread: " << number_of_net_thread_ << '\n';
    if(false == this->RunNetworkModule()) {
        std::cout << "Run network module failed!\n";
        abort();
    }

    return true;
}

bool Ltalk::StartUp::Stop() {
    net_.Stop();
    sp_work_eventloop_thread_->Stop();
    return true;
}

// Load config file
bool Ltalk::StartUp::LoadConfig() {
    std::string file_json;
    FILE* config_file_ptr = fopen(DEFAULT_CONFIG_FILE, "r");
    while(!feof(config_file_ptr)) {
        char buffer[MAX_BUF_SIZE];
        int len = fread(buffer, 1, MAX_BUF_SIZE, config_file_ptr);
        file_json += std::string(buffer, len);
    }

    fclose(config_file_ptr);
    //std::cout << "json [" << file_json << "]\n";
    Third::Json obj;
    try{
        obj = Third::Json::parse(file_json);
    } catch(Third::Json::parse_error &e) {
        d_cout << e.what() << '\n';
        return false;
    }
    // Parse json
    try {
        tcp_port_ = obj["server"]["tcp_port"];
        udp_port_ = obj["server"]["udp_port"];
        number_of_net_thread_ = obj["server"]["number_of_net_thread"];
        log_path_ = obj["server"]["log_path"];
        db_host_ = obj["database"]["host"];
        db_port_ = obj["database"]["port"];
        db_user_ = obj["database"]["user"];
        db_password_ = obj["database"]["password"];
        db_name_ = obj["database"]["name"];
        Data::web_root = obj["server"]["web_root"];
        Data::web_page = obj["server"]["web_page"];
        Data::web_404_page = obj["server"]["web_404_page"];
    }  catch (Third::Json::exception &e) {
        d_cout << e.what() << '\n';
        abort();
    }
    return true;
}
bool Ltalk::StartUp::RunNetworkModule() {
    net_.Start(tcp_port_, number_of_net_thread_);
    return true;
}
bool Ltalk::StartUp::RunDatabaseModule() {
    return Database::Mysql::Connect(db_host_, db_user_, db_password_, db_name_, db_port_);;
}
bool Ltalk::StartUp::RunLoggerModule() {
    return true;
}

bool Ltalk::StartUp::RunWorkModule() {
    Data::work_eventloop = sp_work_eventloop_thread_->StartLoop();
    return true;
}

void Ltalk::StartUp::StartWork() {
    // 保持数据库连接, 每120 s ping一次
    keep_connect_mysql_event_ = ::Work::SPEvent(new ::Work::Event(std::bind(&StartUp::KeepConnectDatabase, this), true, 1000 * 1));
    ::Data::work_eventloop->AddWork(keep_connect_mysql_event_);
    // 消息推送
    push_message_event_ = ::Work::SPEvent(new ::Work::Event(std::bind(&::Work::PushMessage::Send, &push_message_), true));
    ::Data::work_eventloop->AddWork(push_message_event_);
}

void Ltalk::StartUp::KeepConnectDatabase() {
    ::Database::Mysql::Ping();
}
