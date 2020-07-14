#include "keep_http.h"

KeepHttp::KeepHttp() : tcp_socket_(new QTcpSocket)
{
    //reply_ = new QNetworkReply();
    ::QObject::connect(tcp_socket_, &QTcpSocket::readyRead, this, &KeepHttp::recvData);
}

void KeepHttp::connect(QString host, quint16 port) {
    tcp_socket_->connectToHost(host, port);

}

void KeepHttp::recvData() {
    QByteArray recv = tcp_socket_->readAll();
    if(step_ == Step::get_header) {
        tcp_socket_->readLine(); // read a line
        while(true) {
            QByteArray one_line_data = tcp_socket_->readLine();
            if(one_line_data.size() < 1) break;
            std::string one_line = std::string(one_line_data.data(), one_line_data.size());
            one_line.pop_back(); //pop \r

            int value_start_pos = -1;

            value_start_pos = one_line.find(':');
            // Last line have not  key and value
            if(value_start_pos < 0) {
                qDebug() << "ok!";
                break;
            }
            std::string key = one_line.substr(0, value_start_pos);
            std::string value = one_line.substr(value_start_pos + 2);
            map_header_info_[QString(key.data())] = QString(value.data());
        }
        step_ = Step::get_content;
    }

    if(step_ == Step::get_content) {

    }

}
