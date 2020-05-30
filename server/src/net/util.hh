#pragma once
#include <cstdlib>
#include <string>
#include <string.h>
#include <sys/signal.h>
/*
 * This file is static function set
 *
*/

namespace Ltalk {
ssize_t ReadData(int fd, void *buf, size_t n);
ssize_t ReadData(int fd, std::string &buf);
ssize_t WriteData(int fd, void *buf, size_t n);
ssize_t WriteData(int fd, std::string &buf);
void IgnoreSigpipe(); //avoid server terminate with SIGPIPE signal
}

