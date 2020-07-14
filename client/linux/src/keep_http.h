#ifndef KEEPHTTP_H
#define KEEPHTTP_H

#include <QTcpSocket>
#include <QNetworkReply>
#include <QArrayData>
#include <QMap>

class KeepHttp
{
    enum class Step {
        get_header,
        get_content,
        finish
    };

public:
    KeepHttp();
    void init();
    void connect(QString host, quint16 port);
signals:
    void finished(QArrayData data);
private:
    QTcpSocket *tcp_socket_;
    Step step_;
    size_t recv_length;
    QByteArray recv_data_;
    QByteArray send_data_;
    QMap<QString, QString> map_header_info_;
private slots:
    void recvData();
};

#endif // KEEPHTTP_H
