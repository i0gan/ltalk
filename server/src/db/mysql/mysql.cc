#include "mysql.hh"

MYSQL Ltalk::Db::global_mysql;
bool Ltalk::Db::Mysql::Connect(std::string host,
                           std::string user,
                           std::string password,
                           std::string name,
                           int port,
                           const char *unix_socket,
                           std::size_t clientflag) {
    bool return_value = false;
    do {
        if(nullptr == mysql_init(&global_mysql)) {
            std::cout << "mysql init fail!"	 << mysql_error(&global_mysql) <<std::endl;
            break;
        }
        //connect mysql
        bool is_connect = mysql_real_connect(
                    &global_mysql,
                    host.c_str(),
                    user.c_str(),
                    password.c_str(),
                    name.c_str(),
                    port, unix_socket, clientflag);

        if(false == is_connect) {
            std::cout << "mysql_real_connect:" << mysql_error(&global_mysql) << std::endl;
            break;
        }
        return_value = true;
    } while(false);
    return return_value;
}

void Ltalk::Db::Mysql::Disconnect() {
    mysql_close(&global_mysql);
}

Ltalk::Db::MysqlQuery::MysqlQuery() :
    res_(nullptr),
    mysql_(&global_mysql) {
}

Ltalk::Db::MysqlQuery::MysqlQuery(MYSQL *mysql) :
    res_(nullptr),
    mysql_(mysql) {
}

void Ltalk::Db::MysqlQuery::operator=(MysqlQuery &query) {
    row_ = query.row_;
    res_ = query.res_;
    query.Clean(); // avoid double free
}

Ltalk::Db::MysqlQuery::~MysqlQuery() {
    if(!res_) {
        mysql_free_result(res_); //release res
    }
}

void Ltalk::Db::MysqlQuery::Clean() {
    row_ = 0;
    res_ = nullptr;
    number_of_fields_ = 0;
    mysql_ = nullptr;
}

bool Ltalk::Db::MysqlQuery::Exec(std::string sql) {
    int ret = true;
    if(mysql_real_query(mysql_, sql.data(), sql.size())) {
        std::cout << "mysql_real_query : " << mysql_error(mysql_) << std::endl;
        ret = false;
    }
    return ret;
}

bool Ltalk::Db::MysqlQuery::Select(const std::string &table_name, const std::string &key_sql, const std::string &condition) {
    std::string sql;
    sql = "SELECT " + key_sql;
    sql += " FROM " + table_name;
    if(condition != "none")
        sql += " WHERE " + condition;
    sql += " ;";
    if(mysql_real_query(mysql_, sql.data(), sql.size())) {
        std::cout << "mysql_real_query : " << mysql_error(mysql_) << std::endl;
        return false;
    }
    if(res_) mysql_free_result(res_);
    if(!(res_ = mysql_store_result(mysql_))) {
        std::cout << "mysql_store_result : " << mysql_error(mysql_) << std::endl;
    }
    number_of_fields_ = static_cast<int>(mysql_num_fields(res_));
    return true;
}

bool Ltalk::Db::MysqlQuery::Insert(const std::string &table_name, const std::string &key_sql, const std::string &value_sql) {
    std::string sql;
    sql = "INSERT INTO " + table_name;
    sql += " (" + key_sql + ") ";
    sql += "VALUE(" + value_sql + ");";
    return Exec(sql);
}

bool Ltalk::Db::MysqlQuery::Update(const std::string &table_name, const std::string &key_sql, const std::string &value_sql, const std::string &condition) {
    std::string sql;
    std::string keys = key_sql + ',';
    std::string values = value_sql + ',';
    sql = "UPDATE " + table_name + " SET ";
    while(true) {
        int key_pos = keys.find(',');
        int value_pos = values.find(',');
        if(key_pos < 0 || value_pos < 0) {
            sql.pop_back();
            break;
        }
        std::string key = keys.substr(0, key_pos);
        std::string value = values.substr(0, value_pos);
        sql += key + "='" + value + "',";
        keys = keys.substr(key_pos + 1);
        values = values.substr(value_pos + 1);
    }
    sql += " WHERE " + condition + ';';
    return Exec(sql);
}

bool Ltalk::Db::MysqlQuery::Delete(const std::string &table_name, const std::string &condition) {
    std::string sql;
    sql = "DELETE FROM " + table_name;
    sql += " WHERE " + condition + ';';
    return Exec(sql);
}

bool Ltalk::Db::MysqlQuery::Next() {
    bool result = false;
    do {
        if(!res_) {
            break;
        }
        row_ = mysql_fetch_row(res_);
        if(row_) {
            result = true;
            break;
        }
    } while (false);
    return result;
}

char* Ltalk::Db::MysqlQuery::Value(int index) {
    if((number_of_fields_ < index + 1 ) && !row_) {
        //std::cout << "out_of_range: " << index << " / " << number_of_fields_ << std::endl;
        return nullptr;
    }
    return row_[index];
}

void Ltalk::Db::MysqlQuery::set_mysql(MYSQL *mysql) {
    mysql_ = mysql;
}
MYSQL *Ltalk::Db::MysqlQuery::get_mysql() {
    return mysql_;
}

void Ltalk::Db::MysqlQuery::Escape(std::string &str) {
   std::string str_ret;
   for(auto c : str) {
       if(c == '\'')
           str_ret += "\\'";
       else if(c == '"')
           str_ret += "\\\"";
       else if(c == '\b')
           str_ret += "\\b";
       else if(c == '\n')
           str_ret += "\\n";
       else if(c == '\t')
           str_ret += "\\t";
       else
           str_ret += c;
   }
   str = str_ret;
}
