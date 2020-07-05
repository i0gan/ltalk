# Ltalk server

## 如何搭建服务端

### Debian, Ubuntu, Kali 如下

```
sudo apt install g++
sudo apt install mysql-server
sudo apt install libmysql++
sudo apt install make

```
### 获取代码
```sh
git clone https://github.com/I0gan/ltalk.git
cd ltalk
cd server

```
### 编译

编译的二进制文件在build/ltalks

```
make

```

### 运行

```
./start (普通运行)
./start -b (后台运行)

```

