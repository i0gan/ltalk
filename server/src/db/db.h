#ifndef Db_H
#define Db_H

#include <mysql/mysql.h>
#include <iostream>
#include <unistd.h>
#include "core.h"

#define DB_WAIT_TIMES 1000 // wait times
#define DB_WAIT_TIME  10   // will usleep(WAIT_TIME)

////对mysql的一些常用操作进行了封装
class Query {
public:
	Query();
	Query(MYSQL_RES *res);
	~Query();
    MYSQL_ROW next(); //从资源res中获取下一行信息
	char* getOne(int idx);
    void operator=(Query &query);
    void operator=(MYSQL_RES *res);
private:
    int getFields();  //get num of fields
    bool isInUse;
    MYSQL_RES *res;    //从数据库中获得的资源指针1
	MYSQL_ROW row;
    int numOfFields;   //每一行的元素个数
};

class Db {
	public:
        Db() : mysql(new MYSQL), isConnected(false) {} //构造时初始化类成员
		~Db() { delete mysql; }
        bool connect(std::string host, //连接数据库
				std::string user,
				std::string passwd,
				std::string name,
				int port = DB_PORT,
				const char *unix_socket = nullptr,
				std::size_t clientflag = 0);

        void disconnect();             //断开连接
        bool exec(std::string sqlstr); //执行sql语句
        MYSQL_RES *select(std::string sql);  // get res 
		bool wait();
	
	private:	
		MYSQL *mysql;
        std::string host; //ip地址
        std::string user; //用户
        std::string passwd; //密码
        std::string name;     //所连接的数据库
        int			port;  //mysql 的端口号
        bool isConnected;  //是否连接
        bool lock;      //储存是否还在使用数据库, 以免多线程访问数据库时,造成数据库处理不过来.
};

#endif
