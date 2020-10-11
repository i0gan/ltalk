# 服务器编译环境

g++                    (版本: 6.0+)

mysql-server      (mysql 服务器)

libmysql++-dev  (mysql c语言接口开发包)

redis (redis服务器)

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

#### 完成之后, 在server目录下输入

```sh
mysql -ultalk -p123456 < src/setup.sql
```



# Redis

### 安装redis

```bash
sudo apt install redis-cli
git clone https://github.com/redis/hiredis
cd riredis
make
sudo make install
```

这时会遇到下面问题：

./redis: error while loading shared libraries: libhiredis.so.0.13: cannot open shared object file: No such file or directory

这是因为系统找不到动态库的问题，需要检查/etc/ld.so.conf文件中是否包含需要的动态库目录（如/usr/local/lib），如果没有进行添加。之后执行sudo /sbin/ldconfig，更新系统动态库配置 


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
        "web_page" : "index.html",
        "web_404_page" : "404.html"
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

