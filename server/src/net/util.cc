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
