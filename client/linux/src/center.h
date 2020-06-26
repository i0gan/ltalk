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
#include <QDir>

#include "ltalk.h"
#include "login_page.h"
#include "main_page.h"
#include "util.h"
#include "change_theme_page.h"

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
    void handleGetUserInfoReply(const QJsonObject &json_obj);
    void generateUserPath();

private slots:
    void dealWithLocalCmd(LocalCmd cmd);
    void changeTheme(QString theme);

private:
    QString theme_;
    LoginPage *login_page_;
    MainPage *main_page_;
    ChangeThemePage *change_theme_page_;
    QNetworkAccessManager *network_access_mannager;
    UserInfo user_;
    QVector<UserInfo> friend_list_;
    QVector<GroupInfo> group_list_;
    QByteArray recved_data_;
};

#endif // CENTER_H
