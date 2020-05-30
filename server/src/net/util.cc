#include "util.hh"

ssize_t Ltalk::ReadData(int fd, void *buf, size_t n) {
    return 1;
}
ssize_t Ltalk::ReadData(int fd, std::string &buf) {
    return 1;
}
ssize_t Ltalk::WriteData(int fd, void *buf, size_t n) {
    return 1;
}
ssize_t Ltalk::WriteData(int fd, std::string &buf) {
    return 1;
}

void Ltalk::IgnoreSigpipe() {
    struct sigaction sa;
    bzero(&sa, sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigaction(SIGPIPE, &sa, NULL);
}

bool Ltalk::SetFdNonBlocking(int listen_fd) {
    do {
    int flag = fcntl(listen_fd, F_GETFL, 0);
    if(flag == - 1)
        break;
    if(fcntl(listen_fd, F_SETFL, flag | O_NONBLOCK) == -1) {
        return true;
    }
    } while(false);
    return false;
}
