create user 'ltalk'@'localhost' identified by '123456';
flush privileges;
create database ltalk DEFAULT CHARSET utf8 COLLATE utf8_general_ci;
grant all privileges on ltalk.* to 'ltalk'@'localhost';

