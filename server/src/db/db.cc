#include "db.hh"

bool Db::Connect(std::string host,
				std::string user,
                std::string password,
				std::string name,
				int port,
				const char *unix_socket,
				std::size_t clientflag) {

    this->host_ = host;
    this->user_ = user;
    this->password_ = password;
    this->name_ = name;
    this->port_ = port;

    //初始化mysql
    if(nullptr == mysql_init(this->mysql_)) {
        std::cout << "mysql init fail!"	 << mysql_error(this->mysql_) <<std::endl;
		return false;
	}

	//connect mysql
	bool isConnect = mysql_real_connect(
            this->mysql_,
			host.c_str(),
			user.c_str(),
            password.c_str(),
			name.c_str(),
			port, unix_socket, clientflag);

	if(false ==isConnect) {
        std::cout << "mysql_real_connect:" << mysql_error(this->mysql_) << std::endl;

		return false;
	}
    db_lock_ = false; //初始化数据库没有在使用
	return true;
}

void Db::Disconnect() {
    mysql_close(this->mysql_);
}

bool Db::Exec(std::string sqlstr) {
    if(!Wait()) { // If wait out of time will give up to select
		return false; 
	}

    //设置为处理中
    db_lock_ = true;
    int flag = mysql_real_query(this->mysql_, sqlstr.c_str(), sqlstr.length());
		if(0 != flag) { //if successful return 0
            std::cout << "mysql_real_query : " << mysql_error(this->mysql_) << std::endl;
            db_lock_ = false; //处理完成
            return false;
    }
    db_lock_ = false; //处理完成
	return true;
}

bool Db::Wait() {
    //等待处理
    int i = 0;
    while(true) {
        if(i > DB_WAIT_TIMES) {
            return false;
        }
        if(false == db_lock_) {
            break;
        }
        usleep(DB_WAIT_TIME);
        ++i;
    }
	return true;
}

MYSQL_RES *Db::select(std::string sqlstr) {
    if(!Wait()) { // If wait out of time will give up to select
		return nullptr; 
	}

    //设置为处理中
    db_lock_ = true;

    //HIDE std::cout << sqlstr << std::endl;
    int flag = mysql_real_query(this->mysql_, sqlstr.c_str(), sqlstr.length());
	if(flag != 0) { //if seccessful return 0
        std::cout << "mysql_real_query : " << mysql_error(this->mysql_) << std::endl;
        db_lock_ = false; //处理完成
        return nullptr;
	}
    //判断执行选择的结果
    MYSQL_RES *res = mysql_store_result(this->mysql_);

	if(nullptr == res) {
        std::cout << "mysql_store_result : " << mysql_error(this->mysql_) << std::endl;
    }
    db_lock_ = false; //处理完成
    return res;
}


//******************** Query **********************
/*
	This Class to deal with database res
*/

Query::Query() : isInUse(false), res_(nullptr) {

}

Query::Query(MYSQL_RES *res) {
    this->isInUse = true;
    this->res_ = res;
    this->row_ = nullptr;
    this->num_of_fields_ = 0;
    if(nullptr != res) {
        num_of_fields_ = static_cast<int>(mysql_num_fields(res));
    }
}

void Query::operator=(Query &query) {
    this->isInUse = query.isInUse;
    this->row_ = query.row_;
    this->res_ = query.res_;
}

Query::~Query() {
    if(this->res_ != nullptr && this->isInUse == true) {
        mysql_free_result(res_); //release res
	}
}

void Query::operator=(MYSQL_RES *res){
    if(this->res_ != nullptr && this->isInUse == true) {
        mysql_free_result(res); //release res
        num_of_fields_ = static_cast<int>(mysql_num_fields(res));
    }
    this->res_ = res;
}

MYSQL_ROW Query::Next() {

    if(nullptr == res_) {
		return nullptr;
	}
    row_ = mysql_fetch_row(res_);
    return row_;
}


char* Query::GetOne(int idx) {
    if((num_of_fields_ - 1) < idx) {
        std::cout << "资源越界: " << idx << " / " << num_of_fields_ << std::endl;
        return nullptr;
    }
    return row_[idx];
}
