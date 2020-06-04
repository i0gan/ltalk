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

```sql
# 用户表, 储存用户的信息
create table user_(
id int primary key auto_increment,
uid varchar(256),
account varchar(128),
nickname varchar(256),
head_image_url varchar(256),
phone varchar(128),
email varchar(128),
creation_time  varchar(256),
network_state varchar(256), 
platform varchar(256), 
password varchar(256),
last_login varchar(256)
);

# 群组表, 储存群组的信息
create table group_(
id int primary key auto_increment,
gid varchar(256),
account varchar(128),
name varchar(256),
head_image_url varchar(256),
announcement varchar(1024),
host_uid int,
total int,
creation_time varchar(256)
);

# 好友关系表, 储存用户之间的关系
create table user_friend_(
id int primary key auto_increment,
uid varchar(256),
tid varchar(256),
);

# 群组关系表, 储存群组与用户的关系
create table group_user_(
id int primary key auto_increment,
gid varchar(256),
tid varchar(256),
);


/# 事件表, 储存某些事件没有处理完成, 就将其储存起来
create table event_(
id int primary key auto_increment,
eid varchar(256),
request varchar(256),
uid varchar(256),
tid varchar(256),
gid varchar(256),
message varchar(1024),
creation_time  varchar(255)
);

```



## 编译

```sh
make
```

二进制文会编译在./bin目录下



## 运行

在运行之前，确保配置文件正确， 配置文件为当前目录下的config.json文件中。默认配置如下，注意，不能有注释

```json
 {
    "server" : {
        "tcp port" : "8080",
        "udp port" : "8000",
        "number of thread" : "8",
        "log path" : "./"
    },
    "database" : {
        "host" : "127.0.0.1",
        "port" : "3306",
        "user" : "ltalk",
        "password" : "123456",
        "name" : "ltalk"
    }
 }
```

确保配置文件正确后即可编译

```
./dev # 或则 cd ./bin && ./ltalks
```

