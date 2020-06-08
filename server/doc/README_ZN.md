# Ltalk 服务端开发文档



## Mysql 数据库 

服务端模块: db/mysql/mysql.hh，主要包含两大类Mysql 和MysqlRes,

### Mysql class

```c++
Mysql() : is_connected_(false) {}
        ~Mysql() {}
// 连接数据库函数
bool Connect(std::string host,  //ip
       std::string user,		//用户名称
       std::string password,   //用户密码
       std::string name,		//数据库名称
      int port = MYSQL_DEFAULT_CONNECT_PORT, //默认端口为3306
      const char *unix_socket = nullptr,	   //unix_socket默认为null
      std::size_t client_flag = 0);

// 断开数据库
void Disconnect();
MYSQL *get_mysql();			  //获取数据库指针
void   set_mysql_as_global(); //将全局数据库指针变量设为该变量


```



### MysqlRes class

该类若没有设置数据库指针, 默认为从全局数据库指针,该变量在db/mysql/mysql.hh中声明, 在db/mysql/mysql.cc中实现

```c++
Query();
    Query(MYSQL *mysql);
    Query(MYSQL_RES *res);
    ~Query();
MYSQL_ROW Next();
    char* GetOne(int idx);       //根据索引获取一个数据中资源的值
    bool Exec(std::string sql);  //执行sql语句
    bool Select(std::string sql);//执行sql语句
    void operator=(Query &mysql_res); //重载=
    void   set_mysql(MYSQL *mysql);   //设置数据库指针
    MYSQL *get_mysql();
    void Clean();
```





