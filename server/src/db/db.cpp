#include "db.h"

bool Db::connect(std::string host,
				std::string user,
				std::string passwd,
				std::string name,
				int port,
				const char *unix_socket,
				std::size_t clientflag) {

	this->host = host;
	this->user = user;
	this->passwd = passwd;
	this->name = name;
	this->port = port;

    //初始化mysql
	if(nullptr == mysql_init(this->mysql)) {
		std::cout << "mysql init fail!"	 << mysql_error(this->mysql) <<std::endl;
		return false;
	}

	//connect mysql
	bool isConnect = mysql_real_connect(
			this->mysql,
			host.c_str(),
			user.c_str(),
			passwd.c_str(),
			name.c_str(),
			port, unix_socket, clientflag);

	if(false ==isConnect) {
		std::cout << "mysql_real_connect:" << mysql_error(this->mysql) << std::endl;

		return false;
	}
    lock = false; //初始化数据库没有在使用
	return true;
}

void Db::disconnect() {
	mysql_close(this->mysql);
}

bool Db::exec(std::string sqlstr) {
	if(!wait()) { // If wait out of time will give up to select
		return false; 
	}

    //设置为处理中
    lock = true;
	int flag = mysql_real_query(this->mysql, sqlstr.c_str(), sqlstr.length());
		if(0 != flag) { //if successful return 0
            std::cout << "mysql_real_query : " << mysql_error(this->mysql) << std::endl;
            lock = false; //处理完成
            return false;
    }
    lock = false; //处理完成
	return true;
}

bool Db::wait() {
    //等待处理
    int i = 0;
    while(true) {
        if(i > DB_WAIT_TIMES) {
            return false;
        }
        if(false == lock) {
            break;
        }
        usleep(DB_WAIT_TIME);
        ++i;
    }
	return true;
}

MYSQL_RES *Db::select(std::string sqlstr) {
	if(!wait()) { // If wait out of time will give up to select
		return nullptr; 
	}

    //设置为处理中
    lock = true;

    //HIDE std::cout << sqlstr << std::endl;
	int flag = mysql_real_query(this->mysql, sqlstr.c_str(), sqlstr.length());
	if(flag != 0) { //if seccessful return 0
		std::cout << "mysql_real_query : " << mysql_error(this->mysql) << std::endl;
        lock = false; //处理完成
        return nullptr;
	}
    //判断执行选择的结果
    MYSQL_RES *res = mysql_store_result(this->mysql);

	if(nullptr == res) {
        std::cout << "mysql_store_result : " << mysql_error(this->mysql) << std::endl;
    }
    lock = false; //处理完成
    return res;
}


//******************** Query **********************
/*
	This Class to deal with database res
*/

Query::Query() : isInUse(false), res(nullptr) {

}

Query::Query(MYSQL_RES *res) {
    this->isInUse = true;
	this->res = res;
    this->row = nullptr;
    this->numOfFields = 0;
    if(nullptr != res) {
        numOfFields = static_cast<int>(mysql_num_fields(res));
    }
}

void Query::operator=(Query &query) {
    this->isInUse = query.isInUse;
    this->row = query.row;
    this->res = query.res;
}

Query::~Query() {
    if(this->res != nullptr && this->isInUse == true) {
        mysql_free_result(res); //release res
	}
}

void Query::operator=(MYSQL_RES *res){
    if(this->res != nullptr && this->isInUse == true) {
        mysql_free_result(res); //release res
        numOfFields = static_cast<int>(mysql_num_fields(res));
    }
    this->res = res;
}

MYSQL_ROW Query::next() {

	if(nullptr == res) {
		return nullptr;
	}
	row = mysql_fetch_row(res);
    return row;
}


char* Query::getOne(int idx) {

    if((numOfFields - 1) < idx) {
        std::cout << "资源越界: " << idx << " / " << numOfFields << std::endl;
        return nullptr;
    }
	return row[idx];
}
