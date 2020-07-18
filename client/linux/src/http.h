#ifndef HTTP_H
#define HTTP_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QByteArray>
#include <QTcpSocket>
#include <string>
#include <map>
#include "util.h"
#include <cstdlib>
class Http : public QObject
{
    enum class Step {
        get_header,
        get_content,
        finish
    };

    Q_OBJECT

signals:
    void finished();
public:
    explicit Http(QObject *parent = nullptr);
    void init();
    void connect(QString host, quint16 port = 80);
    void get(QString url);
    void post(QString url, const QByteArray &data);
    const QByteArray &data();
    void setRawHeader(QString key, QString value);
    QString rawHeader(QString key);
    void reset();

private:
    bool is_connected_ = false;
    QString host_;
    quint16 port_;
    QTcpSocket *tcp_socket_;
    Step recv_step_;
    int recv_length_;
    QByteArray recv_data_;
    QByteArray send_data_;
    std::map<std::string, std::string> map_header_send_info_;
    std::map<std::string, std::string> map_header_recv_info_;

private slots:
    void recvData();
    void connected();
};

#endif // HTTP_H
