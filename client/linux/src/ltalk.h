#ifndef LTALK_H
#define LTALK_H
#include <QString>
#include <QVector>
#include <QDateTime>

#define SERVER_REQUEST_URL "http://192.168.100.8"
#define SERVER_BASE_URL_VALUE "/?platform=linux"

#define DATA_PATH "Documents/Ltalk"




struct UserInfo {
    QString uid;
    QString token;
    QString account;
    QString email;
    QString nickname;
    QString name;
    QString phone_number;
    QString address;
    QString ocupation;
    QString creation_time;
    QString network_state;
    QString last_login;
    QString grade;
    QString head_image;
    QString profile_image_1;
    QString profile_image_2;
    QString profile_image_3;
    QString profile_image_4;
};

struct GroupInfo {
    QString account;
    QString gid;
    QString name;
    QString head_image;
};

// Local cmd
enum class LocalCmd {
    exit,
    showMainPage,
    showChnageThemePage,
    showAboutPage,
    ShowProfilePage,
    logout
};



#endif // LTALK_H
