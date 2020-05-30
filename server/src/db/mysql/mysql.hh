#pragma once

#include <mysql/mysql.h>
#include <iostream>
#include <unistd.h>

#define DB_WAIT_TIMES 1000 // wait times
#define DB_WAIT_TIME  10   // will usleep(WAIT_TIME)
namespace Ltalk {
class Mysql_Res {
public:
    Mysql_Res();
    Mysql_Res(MYSQL_RES *res);
    ~Mysql_Res();
    MYSQL_ROW Next();
    char* GetOne(int idx);
    void operator=(Mysql_Res &mysql_res);
    void operator=(MYSQL_RES *res);
private:
    int get_fields();
    bool isInUse;
    MYSQL_RES *res_;
    MYSQL_ROW row_;
    int num_of_fields_;
};

class Mysql {
	public:
        Mysql() : mysql_(new MYSQL), is_connected_(false) {}
        ~Mysql() { delete mysql_; }
        bool Connect(std::string host,
                std::string user,
                std::string password,
                std::string name,
                int port,
                const char *unix_socket = nullptr,
                std::size_t client_flag = 0);

        void Disconnect();
        bool Exec(std::string sqlstr);
        MYSQL_RES *select(std::string sql);
        bool Wait();
	
	private:	
        MYSQL *mysql_;
        std::string host_;
        std::string user_;
        std::string password_;
        std::string name_;
        int			port_;
        bool is_connected_;
        bool db_lock_;
};
};
