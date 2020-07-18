#include "http.h"

//Http::Http(QObject *parent) : QObject(parent)
//{

//}

Http::Http(QObject *parent) : QObject(parent), tcp_socket_(new QTcpSocket) , recv_step_(Step::get_header),
    recv_length_(0)
{
    //reply_ = new QNetworkReply();
    ::QObject::connect(tcp_socket_, &QTcpSocket::readyRead, this, &Http::recvData);
    ::QObject::connect(tcp_socket_, &QTcpSocket::connected, this, &Http::connected);
}

void Http::connect(QString host, quint16 port) {
    QString host_t;
    if(strstr(host.toUtf8().data(), "http://")) {
        for(int i = 0; i < host.size() && host.size() > 7; ++i) {
            if(i <= 6)
                continue;
            host_t.append(host[i]);
        }
    }
    tcp_socket_->connectToHost(host_t, port);
    host_ = host_t;
    port_ = port;
}

void Http::get(QString url) {
    if(is_connected_ == false)
        connect(host_, port_);

    recv_step_ = Step::get_header;
    QString http_header =
            "GET " + url + " HTTP/1.1\r\n";
    for(auto  iter = map_header_send_info_.begin(); iter != map_header_send_info_.end(); ++iter) {
        http_header += (iter->first + ": " + iter->second + "\r\n").c_str();
    }
    http_header += "Date: " + ::Util::getTime() + "\r\n";
    http_header += "\r\n";
    qDebug() << "http_header : " << http_header;
    tcp_socket_->write(http_header.toUtf8());
}

void Http::post(QString url, const QByteArray &data) {
    if(is_connected_ == false)
        connect(host_, port_);
    recv_step_ = Step::get_header;
    QString http_header =
            "POST " + url + " HTTP/1.1\r\n";
    for(auto  iter = map_header_send_info_.begin(); iter != map_header_send_info_.end(); ++iter) {
        http_header += (iter->first + ": " + iter->second + "\r\n").c_str();
    }
    http_header += "Date: " + ::Util::getTime() + "\r\n";
    http_header += "Content-Length: " + QString::number(data.size()) + "\r\n";
    http_header += "\r\n";
    //qDebug() << "http_header : " << http_header;
    tcp_socket_->write(http_header.toUtf8());
    tcp_socket_->write(data);
}

void Http::connected() {
    is_connected_ = true;
}

void Http::recvData() {

    //QByteArray recv = tcp_socket_->readAll();
    //qDebug() << "recv: " << recv;
    if(recv_step_ == Step::get_header) {
        tcp_socket_->readLine(); // read a line
        while(true) {
            QByteArray one_line_data = tcp_socket_->readLine();
            //qDebug() << "recv_one_line: " + one_line_data;
            if(one_line_data.size() < 1) break;
            std::string one_line = std::string(one_line_data.data(), one_line_data.size());
            one_line.pop_back(); //pop \n
            one_line.pop_back(); //pop \r
            int value_start_pos = -1;
            value_start_pos = one_line.find(':');
            // Last line have not  key and value
            if(value_start_pos < 0) {
                break;
            }
            std::string key = one_line.substr(0, value_start_pos);
            std::string value = one_line.substr(value_start_pos + 2);
            map_header_recv_info_[key] = value;
            recv_length_ = atoi(map_header_recv_info_["Content-Length"].c_str());
        }
        recv_step_ = Step::get_content;
        recv_data_.clear();
    }

    if(recv_step_ == Step::get_content) {
        recv_data_ += tcp_socket_->read(recv_length_ - recv_data_.size());
        if(recv_length_ == recv_data_.size()) {
            recv_step_ = Step::finish;
            emit finished();
        }
    }
}

const QByteArray &Http::data() {
    return recv_data_;
}

void Http::setRawHeader(QString key, QString value) {
    map_header_send_info_[key.toUtf8().data()] = value.toUtf8().data();
}

QString Http::rawHeader(QString key) {
    return map_header_recv_info_[key.toUtf8().data()].c_str();
}

void Http::reset() {
    map_header_send_info_.clear();
    map_header_recv_info_.clear();
    recv_step_ = Step::get_content;
    host_ = "";
    port_ = 0;
}
