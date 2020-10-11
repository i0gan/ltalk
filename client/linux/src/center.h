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
#include "http.h"
#include "user_chat_page.h"

class Center : public QObject
{
    enum class RequestStep {
        keep_connect,
        getUserInfo,
        getFriendList,
        getGroupList,
        getChatMessage,
    };
    Q_OBJECT
public:
    explicit Center(QObject *parent = nullptr);
    ~Center();
    void init();
    void test();
    void start();
    void requestLogin(QString account, QString password);
    void requestAddUser(QString target_account, QString target_uid, QString verify_message);
    void requestReply();
    void dealWithLogined(QString account, QString uid, QString token);
    void exit();
    void keepConnect();
    void dealWithKeepConnectReply();
    void requestGetUserInfo();
    void handleGetUserInfoReply(const QJsonObject &json_obj);
    void requestGetFriendList();
    void handleGetFriendListReply(const QJsonObject &json_obj);
    void requestGetGroupList();

    void handleAddUser(const QJsonObject &json_obj);
    void generateUserPath();
    void changeTheme(QString theme);
    void requestStep();

    void createUserChatPage(const UserInfo &info);
    void requestSendMessage(QString message);
    void dealWithOpenUserChatPage(QString uid);

private slots:
    void dealWithLocalCmd(LocalCmd cmd);
    void testRecv();


private:
    Http http_;
    QString theme_;
    LoginPage *login_page_;
    MainPage *main_page_;
    ChangeThemePage *change_theme_page_;
    AboutPage *about_page_;
    ProfilePage *profile_page_;
    ImageCropperPage *image_cropper_page_;
    AddUserPage *add_user_page_;
    QTimer *request_step_timer_;
    RequestStep request_step_ = RequestStep::getUserInfo;
    bool is_request_info_ = true;
    UserInfo user_;
    QMap<QString, UserInfo> friend_list_;
    QMap<QString, UserChatPage *> friend_window_list_;
    QByteArray recved_data_;
};

#endif // CENTER_H
