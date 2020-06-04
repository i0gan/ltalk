#pragma once

#include <mysql/mysql.h>
#include <iostream>
#include <unistd.h>
#include "../../ltalk.hh"

//#define DB_WAIT_TIMES 1000 // wait times
//#define DB_WAIT_TIME  10   // will usleep(WAIT_TIME)

namespace Ltalk {
extern MYSQL* global_mysql_ptr;
#define MYSQL_DEFAULT_CONNECT_PORT 3306
class Mysql {
	public:
        Mysql() : is_connected_(false) {}
        ~Mysql() {}
        bool Connect(std::string host,
                std::string user,
                std::string password,
                std::string name,
                int port = MYSQL_DEFAULT_CONNECT_PORT,
                const char *unix_socket = nullptr,
                std::size_t client_flag = 0);
        void Disconnect();
        MYSQL *get_mysql();
        void   set_mysql_as_global();

	private:	
        std::string host_;
        std::string user_;
        std::string password_;
        std::string name_;
        int			port_;
        bool is_connected_;
        MYSQL mysql_;
};

class Query {
public:
    Query();
    Query(MYSQL *mysql);
    Query(MYSQL_RES *res);
    ~Query();
    MYSQL_ROW Next();
    char* GetOne(int idx);
    bool Exec(std::string sql);
    bool Select(std::string sql);
    void operator=(Query &mysql_res);
    void   set_mysql(MYSQL *mysql);
    MYSQL *get_mysql();
    void Clean();


private:
    MYSQL_RES *res_;
    MYSQL_ROW row_;
    int number_of_fields_;
    MYSQL *mysql_;
    int get_fields();
};
}
