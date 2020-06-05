#pragma once
#include <cstdlib>
#include <string>
#include <string.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "../ltalk.hh"
#include "vessel.hpp"
//std::basic_string
/*
 * This file is static function set
 *
*/

namespace Ltalk {
namespace Util {

ssize_t ReadData(int fd, void *buffer, size_t length);
ssize_t ReadData(int fd, std::string &in_buffer);
ssize_t ReadData(int fd, std::string &in_buffer, int length);

ssize_t WriteData(int fd, void *buffer, size_t length);
ssize_t WriteData(int fd, Ltalk::Vessel &out_buffer);
ssize_t WriteData(int fd, const std::string &header,const std::string &content);

void IgnoreSigpipe(); //avoid server terminate with SIGPIPE signal
bool SetFdNonBlocking(int listen_fd); //set fd as non bloking
void SetFdNoDelay(int fd);
void SetFdNoLinger(int fd);
void ShutDownWriteFd(int fd);

}
}

