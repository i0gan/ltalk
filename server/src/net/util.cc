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
