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
    QString head_image_url;
    QString name;
    QString phone_number;
    QString address;
    QString ocupation;
    QString creation_time;
    QString network_state;
    QString grade;
};

struct GroupInfo {
    QString account;
    QString gid;
    QString name;
    QString head_image_url;
};

// Local cmd
enum class LocalCmd {
    EXIT,
    SHOW_MAIN_PAGE,
    SHOW_CHANGE_THEME_PAGE,
    HIDE_MAIN_PAGE,
    LOG_OUT
};



#endif // LTALK_H
