#include "util.hh"

ssize_t Ltalk::Util::ReadData(int fd, void *buf, size_t len) {
    ssize_t read_left = len;
    ssize_t read_num = 0;
    ssize_t read_sum = 0;
    char *read_ptr = static_cast<char *>(buf);
    while(read_left > 0) {
        if((read_num = read(fd, buf, MAX_BUF_SIZE)) < 0) {
            if(errno == EINTR)
                continue;
            else if(errno == EAGAIN) {
                return read_sum;
            }else {
                return -1;
            }
        } else if (read_num == 0) {
            break;
        }
        read_sum += read_num;
        read_left -= read_num;
        read_ptr += read_num;
    }
    return read_sum;
}
//ssize_t Ltalk::Util::ReadData(int fd, std::string &buf) {
//    return 1;
//}
ssize_t Ltalk::Util::WriteData(int fd, void *buf, size_t n) {
    return 1;
}
ssize_t Ltalk::Util::WriteData(int fd, std::string &buf) {
    return 1;
}

void Ltalk::Util::IgnoreSigpipe() {
    struct sigaction sa;
    bzero(&sa, sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigaction(SIGPIPE, &sa, NULL);
}

bool Ltalk::Util::SetFdNonBlocking(int listen_fd) {
    do {
    int flag = fcntl(listen_fd, F_GETFL, 0);
    if(flag == - 1)
        break;
    if(fcntl(listen_fd, F_SETFL, flag | O_NONBLOCK) == -1) {
        break;
    }
    return true;
    } while(false);
    return false;
}

void Ltalk::Util::SetFdNoDelay(int fd) {
    int enable = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&enable, sizeof (enable));
}
