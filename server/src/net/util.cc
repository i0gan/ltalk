#include "util.hh"

ssize_t Ltalk::Util::ReadData(int fd, void *buffer, size_t length) {
    ssize_t read_left = length;
    ssize_t read_len = 0;
    ssize_t read_sum = 0;
    char *read_ptr = static_cast<char *>(buffer);
    while(read_left > 0) {
        if((read_len = read(fd, read_ptr, MAX_BUF_SIZE)) < 0) {
            if(errno == EINTR)
                continue;
            else if(errno == EAGAIN)
                return read_sum;
            else
                return -1;
        } else if (read_len == 0) {
            break;
        }
        read_sum += read_len;
        read_left -= read_len;
        read_ptr += read_len;
    }

    return read_sum;
}
ssize_t Ltalk::Util::ReadData(int fd, std::string &in_buffer) {
    return 1;
}

ssize_t Ltalk::Util::ReadData(int fd, std::string &in_buffer, bool &is_zero) {
    ssize_t read_len = 0;
    ssize_t read_sum = 0;
    in_buffer.clear();
    char buffer[MAX_BUF_SIZE];

    while(true) {
        if((read_len = read(fd, buffer, MAX_BUF_SIZE)) < 0) {
            if(errno == EINTR)
                continue;
            else if (errno == EAGAIN)
                return read_sum;
            else
                return -1;
        } else if (read_len == 0) {
            is_zero = true;
            break;
        }
        read_sum += read_len;
        in_buffer += std::string(buffer, buffer + read_len);
    }
    return read_sum;
}
ssize_t Ltalk::Util::WriteData(int fd, void *buffer, size_t length) {
    ssize_t write_left = length;
    ssize_t write_len = 0;
    ssize_t write_sum = 0;
    char *write_ptr = static_cast<char *>(buffer);
    while(write_left > 0) {
        if((write_len = write(fd, write_ptr, write_left)) < 0) {
            if(errno == EINTR)
                continue;
            else if(errno == EAGAIN) {
                return write_sum;
            }else {
                return -1;
            }
        }
        write_sum += write_len;
        write_left -= write_len;
        write_ptr += write_len;
    }

    return write_sum;
}
ssize_t Ltalk::Util::WriteData(int fd, std::string &out_buffer) {
    ssize_t write_left = out_buffer.size();
    ssize_t write_len = 0;
    ssize_t write_sum = 0;
    char *write_ptr = const_cast<char *>(out_buffer.c_str());
    while(write_left > 0) {
        if((write_len = write(fd, write_ptr, write_left)) < 0) {
            if(errno == EINTR)
                continue;
            else if(errno == EAGAIN) {
                return write_sum;
            }else {
                return -1;
            }
        }
        write_sum += write_len;
        write_left -= write_len;
        write_ptr += write_len;
    }
    if(write_sum == static_cast<int>(out_buffer.size()))
        out_buffer.clear();
    else
        out_buffer = out_buffer.substr(write_sum);

    return write_sum;
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
void Ltalk::Util::SetFdNoLinger(int fd) {
    struct linger linger_s;
    linger_s.l_onoff = 1;
    linger_s.l_linger = 30;
    setsockopt(fd, SOL_SOCKET, SO_LINGER, &linger_s, sizeof (linger_s));

}
void Ltalk::Util::ShutDownWriteFd(int fd) {
    shutdown(fd, SHUT_WR);
}
