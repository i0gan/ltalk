#ifndef Db_H
#define Db_H

#include <mysql/mysql.h>
#include <iostream>
#include <unistd.h>

#define DB_WAIT_TIMES 1000 // wait times
#define DB_WAIT_TIME  10   // will usleep(WAIT_TIME)

//对mysql的一些常用操作进行了封装
class Query {
public:
	Query();
	Query(MYSQL_RES *res);
	~Query();
    MYSQL_ROW Next(); //从资源res中获取下一行信息
    char* GetOne(int idx);
    void operator=(Query &query);
    void operator=(MYSQL_RES *res);
private:
    int get_fields();  //get num of fields
    bool isInUse;
    MYSQL_RES *res_;    //从数据库中获得的资源指针1
    MYSQL_ROW row_;
    int num_of_fields_;   //每一行的元素个数
};

class Db {
	public:
        Db() : mysql_(new MYSQL), is_connected_(false) {} //构造时初始化类成员
        ~Db() { delete mysql_; }
        bool Connect(std::string host, //连接数据库
                std::string user,
                std::string password,
                std::string name,
                int port,
                const char *unix_socket = nullptr,
                std::size_t client_flag = 0);

        void Disconnect();             //断开连接
        bool Exec(std::string sqlstr); //执行sql语句
        MYSQL_RES *select(std::string sql);  // get res 
        bool Wait();
	
	private:	
        MYSQL *mysql_;
        std::string host_;     //ip地址
        std::string user_;     //用户
        std::string password_; //密码
        std::string name_;   //所连接的数据库
        int			port_;   //mysql 的端口号
        bool is_connected_;  //是否连接
        bool db_lock_;          //储存是否还在使用数据库, 以免多线程访问数据库时,造成数据库处理不过来.
};

#endif
