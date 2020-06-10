#pragma once

#include <mysql/mysql.h>
#include <iostream>
#include <unistd.h>
#include "../../ltalk.hh"

//#define DB_WAIT_TIMES 1000 // wait times
//#define DB_WAIT_TIME  10   // will usleep(WAIT_TIME)

namespace  Database{

extern MYSQL  global_mysql;
#define MYSQL_DEFAULT_CONNECT_PORT 3306
class Mysql {
	public:
        Mysql() {}
        ~Mysql() {}
        static bool Connect(std::string host,
                std::string user,
                std::string password,
                std::string name,
                int port = MYSQL_DEFAULT_CONNECT_PORT,
                const char *unix_socket = nullptr,
                std::size_t client_flag = 0);
        static void Disconnect();
};

class MysqlQuery {
public:
    MysqlQuery();
    MysqlQuery(MYSQL *mysql);
    ~MysqlQuery();
    bool Next();
    char* Value(int index);
    bool Exec(std::string sql);
    bool Select(const std::string &table_name, const std::string &key_sql, const std::string &condition = "none");
    bool Insert(const std::string &table_name, const std::string &key_sql, const std::string &value_sql);
    bool Update(const std::string &table_name, const std::string &key_sql, const std::string &value_sql, const std::string &condition);
    bool Delete(const std::string &table_name, const std::string &condition);
    void operator=(MysqlQuery &mysql_res);
    void   set_mysql(MYSQL *mysql);
    MYSQL *get_mysql();
    static void Escape(std::string &str);

private:
    MYSQL_RES *res_;
    MYSQL_ROW row_;
    int number_of_fields_;
    MYSQL *mysql_;
    int get_fields();
    void Clean();

};
}
