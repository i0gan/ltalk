#ifndef CENTER_H
#define CENTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkConfiguration>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QDateTime>
#include "ltalk.h"
#include "login_page.h"
class Center : public QObject
{
    Q_OBJECT
public:
    explicit Center(QObject *parent = nullptr);
    ~Center();
    void init();
    void start();

    void requestLogin(QString account, QString password);
    void requestReply(QNetworkReply *reply);

private:
    LoginPage *login_page_;
    bool opened_login_page_;
    QNetworkAccessManager *network_access_mannager;
    QString getTime();
signals:

};

#endif // CENTER_H
