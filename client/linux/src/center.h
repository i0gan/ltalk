#ifndef CENTER_H
#define CENTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonArray>

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
    void loginReply(QNetworkReply *reply);

private:
    LoginPage *login_page_;
    bool opened_login_page_;
signals:

};

#endif // CENTER_H
