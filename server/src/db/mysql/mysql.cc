#include "mysql.hh"

bool Ltalk::Mysql::Connect(std::string host,
                           std::string user,
                           std::string password,
                           std::string name,
                           int port,
                           const char *unix_socket,
                           std::size_t clientflag) {
    bool return_value = false;

    this->host_ = host;
    this->user_ = user;
    this->password_ = password;
    this->name_ = name;
    this->port_ = port;

    do {
        if(nullptr == mysql_init(&mysql_)) {
            std::cout << "mysql init fail!"	 << mysql_error(&mysql_) <<std::endl;
            break;
        }
        //connect mysql
        bool is_connect = mysql_real_connect(
                    &mysql_,
                    host.c_str(),
                    user.c_str(),
                    password.c_str(),
                    name.c_str(),
                    port, unix_socket, clientflag);

        if(false == is_connect) {
            std::cout << "mysql_real_connect:" << mysql_error(&mysql_) << std::endl;
            break;
        }

        return_value = true;
    } while(false);

    return return_value;
}

void Ltalk::Mysql::Disconnect() {
    mysql_close(&mysql_);
}

MYSQL *Ltalk::Mysql::get_mysql() {
    return &mysql_;
}

void Ltalk::Mysql::set_mysql_as_global() {
    global_mysql_ptr = &mysql_;
}


Ltalk::Query::Query() :
    res_(nullptr),
    mysql_(global_mysql_ptr) {

}

Ltalk::Query::Query(MYSQL *mysql) :
    res_(nullptr),
    mysql_(mysql) {

}

Ltalk::Query::Query(MYSQL_RES *res) :
    res_(res),
    row_(nullptr),
    number_of_fields_(0) {

    if(nullptr != res) {
        number_of_fields_ = static_cast<int>(mysql_num_fields(res));
    }
}


void Ltalk::Query::operator=(Query &query) {
    row_ = query.row_;
    res_ = query.res_;
    query.Clean();
}

Ltalk::Query::~Query() {
    if(res_ != nullptr) {
        mysql_free_result(res_); //release res
    }
}

void Ltalk::Query::Clean() {
    row_ = 0;
    res_ = nullptr;
    number_of_fields_ = 0;
    mysql_ = nullptr;
}

bool Ltalk::Query::Exec(std::string sql) {
    int ret = false;

    int flag = mysql_real_query(this->mysql_, sql.c_str(), sql.length());
    if(0 == flag) {
        std::cout << "mysql_real_query : " << mysql_error(this->mysql_) << std::endl;
        ret = true;
    }
    return ret;
}

bool Ltalk::Query::Select(std::string sql) {

    int flag = mysql_real_query(this->mysql_, sql.c_str(), sql.length());
    if(flag != 0) {
        std::cout << "mysql_real_query : " << mysql_error(this->mysql_) << std::endl;
        return false;
    }

    res_ = mysql_store_result(this->mysql_);

    if(nullptr == res_) {
        std::cout << "mysql_store_result : " << mysql_error(this->mysql_) << std::endl;
    }
    return true;
}



MYSQL_ROW Ltalk::Query::Next() {

    if(nullptr == res_) {
        return nullptr;
    }
    row_ = mysql_fetch_row(res_);
    return row_;
}

char* Ltalk::Query::GetOne(int idx) {
    if((number_of_fields_ - 1) < idx) {
        std::cout << "out_of_range: " << idx << " / " << number_of_fields_ << std::endl;
        return nullptr;
    }
    return row_[idx];
}
