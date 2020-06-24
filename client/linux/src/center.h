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
#include <QFile>
#include <QDateTime>

#include "ltalk.h"
#include "login_page.h"
#include "main_page.h"

struct UserInfo {
    QString account;
    QString uid;
    QString name;
    QString head_image_url;
    QString email;
    QString ocupation;
    QString grade;
};

struct GroupInfo {
    QString account;
    QString gid;
    QString name;
    QString head_image_url;
};


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
    void dealWithLogined(QString account, QString uid, QString token);
    void exit();
    void keepConnect();
    void requestGetUserInfo();
    void requestGetFriendList();
    void requestGetGroupList();
private slots:
    void dealWithLocalCmd(size_t cmd);


private:
    LoginPage *login_page_;
    MainPage *main_page_;
    QNetworkAccessManager *network_access_mannager;
    QString token_;
    UserInfo user_;
    QVector<UserInfo> friend_list_;
    QVector<GroupInfo> group_list_;
    QString getTime();
};

#endif // CENTER_H
