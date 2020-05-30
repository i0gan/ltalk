#pragma once
#include <cstdlib>
#include <string>
/*
 * This file is static function set
 *
*/

namespace Ltalk {
static ssize_t ReadData(int fd, void *buf, size_t n);
static ssize_t ReadData(int fd, std::string &buf);
static ssize_t WriteData(int fd, void *buf, size_t n);
static ssize_t WriteData(int fd, std::string &buf);

}

