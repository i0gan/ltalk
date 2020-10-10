CREATE DATABASE IF NOT EXISTS `ltalk` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

USE `ltalk`;

DROP TABLE IF EXISTS `user_`;
DROP TABLE IF EXISTS `group_`;
DROP TABLE IF EXISTS `user_friend_`;
DROP TABLE IF EXISTS `group_user_`;
DROP TABLE IF EXISTS `event_`;

CREATE TABLE `user_` (
    `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
    `uid` varchar(256) NOT NULL,
    `account` varchar(256) NOT NULL,
    `email` varchar(256) NOT NULL,
    `nickname` varchar(256) NOT NULL,
    `name` varchar(256) NOT NULL,
    `signature` varchar(512) NOT NULL,
    `qq` varchar(256) NOT NULL,
    `phone_number` varchar(256) NOT NULL,
    `address` varchar(512) NOT NULL,
    `hometown` varchar(512) NOT NULL,
    `occupation` varchar(256) NOT NULL,
    `created_time`  varchar(256) NOT NULL,
    `network_state` varchar(256) NOT NULL,
    `last_login` varchar(256) NOT NULL,
    `head_image` varchar(512) NOT NULL,
    `profile_image_1` varchar(512) NOT NULL,
    `profile_image_2` varchar(512) NOT NULL,
    `profile_image_3` varchar(512) NOT NULL,
    `profile_image_4` varchar(512) NOT NULL,
    `password` varchar(256) NOT NULL,
    PRIMARY KEY (`id`),
    UNIQUE KEY `uid` (`uid`),
    UNIQUE KEY `account` (`account`),
    UNIQUE KEY `phone_number` (`phone_number`),
    UNIQUE KEY `email` (`email`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
