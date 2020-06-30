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
#include <QTimer>

#include "ltalk.h"
#include "login_page.h"
#include "main_page.h"
#include "util.h"
#include "change_theme_page.h"
#include "about_page.h"
#include "profile_page.h"
#include "image_cropper_page.h"
#include "add_user_page.h"

class Center : public QObject
{
    enum class RequestStep {
        unlogin,
        login,
        getUserInfo,
        getFriendList,
        getGroupList,
        getChatMessage,
        wait
    };
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
    void dealWithKeepConnectReply();
    void requestGetUserInfo();
    void requestGetFriendList();
    void requestGetGroupList();
    void handleGetUserInfoReply(const QJsonObject &json_obj);
    void generateUserPath();
    void changeTheme(QString theme);

private slots:
    void dealWithLocalCmd(LocalCmd cmd);

private:
    QString theme_;
    LoginPage *login_page_;
    MainPage *main_page_;
    ChangeThemePage *change_theme_page_;
    AboutPage *about_page_;
    ProfilePage *profile_page_;
    ImageCropperPage *image_cropper_page_;
    AddUserPage *add_user_page_;
    QNetworkAccessManager *network_access_mannager;
    QNetworkRequest keep_connect_request_;
    QNetworkRequest resource_request_;
    QTimer *keep_connect_timer_;

    UserInfo user_;
    QVector<UserInfo> friend_list_;
    QVector<GroupInfo> group_list_;
    QByteArray recved_data_;
};

#endif // CENTER_H
