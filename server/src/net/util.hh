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
/*
 * This file is static function set
 *
*/
#define MAX_BUF_SIZE 4096
namespace Ltalk {
namespace Util {

ssize_t ReadData(int fd, void *buf, size_t len);
ssize_t ReadData(int fd, std::string &in_buffer);
ssize_t ReadData(int fd, std::string &in_buffer, bool &is_zero);

ssize_t WriteData(int fd, void *buf, size_t len);
ssize_t WriteData(int fd, std::string &out_buffer);
void IgnoreSigpipe(); //avoid server terminate with SIGPIPE signal
bool SetFdNonBlocking(int listen_fd); //set fd as non bloking
void SetFdNoDelay(int fd);
void SetFdNoLinger(int fd);
void ShutDownWriteFd(int fd);

}
}

