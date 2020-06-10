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
#include "../util/vessel.hpp"
//std::basic_string
/*
 * This file is static function set
 *
*/

namespace Net {
namespace Util {
ssize_t Read(int fd, void *buffer, size_t length);
ssize_t Read(int fd, std::string &in_buffer);
ssize_t Read(int fd, std::string &in_buffer, int length);

ssize_t Write(int fd, void *buffer, size_t length);
ssize_t Write(int fd, ::Util::Vessel &out_buffer);

void IgnoreSigpipe();                 //avoid server terminate with SIGPIPE signal
bool SetFdNonBlocking(int listen_fd); //set fd as non bloking
void SetFdNoDelay(int fd);
void SetFdNoLinger(int fd);
void ShutDownWriteFd(int fd);
}
}

