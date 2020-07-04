# 服务器编译环境

g++                    (版本: 6.0+)
mysql-server      (mysql 服务器)
libmysql++-dev  (mysql c语言接口开发包)
make

* 安装 (Debian, Ubuntu....)

```sh
  sudo apt install g++
  sudo apt install mysql-server
  sudo apt install libmysql++-dev
  sudo apt install make
```



# 服务器运行环境


## 数据库环境搭建:

### 创建数据库及用户

使用root登录mysql 输入一下sql语句.

#### 创建用户: ltalk 密码为: 123456　
```sql
	create user 'ltalk'@'localhost' identified by '123456';
```

#### 刷新权限: 
```sql
flush privileges;
```

#### 创建im数据库:
```sql
create database ltalk DEFAULT CHARSET utf8 COLLATE utf8_general_ci;
```

#### 将用户赋予操作权限
```sql
grant all privileges on ltalk.* to 'ltalk'@'localhost';
```

#### 完成之后, 使用所创建的用户登录

```sh
mysql -u ltalk -p
use ltalk;
```

#### 创建表

用户表

```sql
create table user_(
id int primary key auto_increment,
uid varchar(256),
account varchar(256),
email varchar(256),
nickname varchar(256),
name varchar(256),
signature varchar(512),
qq varchar(256),
phone_number varchar(256),
address varchar(512),
hometown varchar(512),
occupation varchar(256),
created_time  varchar(256),
network_state varchar(256),
last_login varchar(256),
head_image varchar(512),
profile_image_1 varchar(512),
profile_image_2 varchar(512),
profile_image_3 varchar(512),
profile_image_4 varchar(512),
password varchar(256)
);
```

群组表

```sql

create table group_(
id int primary key auto_increment,
gid varchar(256),
account varchar(256),
name varchar(256),
announcement varchar(1024),
host_uid varchar(256),
admin_uid_1 varchar(256),
admin_uid_2 varchar(256),
admin_uid_3 varchar(256),
total int,
online int,
created_time varchar(256)
head_image varchar(512),
profile_image_1 varchar(512),
profile_image_2 varchar(512),
profile_image_3 varchar(512),
profile_image_4 varchar(512),
);
```

好友关系表

```sql
create table user_friend_(
id int primary key auto_increment,
uid varchar(256),
tid varchar(256),
remark varchar(512)
);
```

好友关系表

```sql
create table group_user_(
id int primary key auto_increment,
gid varchar(256),
tid varchar(256),
remark varchar(512)
);
```

事件表

```sql
create table event_(
id int primary key auto_increment,
eid varchar(256),
request varchar(256),
uid varchar(256),
tid varchar(256),
gid varchar(256),
message varchar(1024),
created_time  varchar(256)
);

```



## 编译

```sh
make
```

二进制文会编译在./build目录下



## 运行

在运行之前，确保配置文件正确， 配置文件为当前目录下的etc/config.json文件中。默认配置如下，注意，不能有注释

```json
{
    "server" : {
        "tcp_port" : 80,
        "udp_port" : 8000,
        "number_of_thread": 8,
        "data_path" : "data",
        "log_path" : "data/log",
        "web_root" : "data/web",
        "web_page" : "main/index.html",
        "web_404_page" : "data/web/404.html"
    },
    "database" : {
        "host" : "127.0.0.1",
        "port" : 3306,
        "user" : "ltalk",
        "name" : "ltalk",
        "password" : "123456"
    }
}
```



运行

```
./start
```

