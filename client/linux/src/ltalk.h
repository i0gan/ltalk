#ifndef LTALK_H
#define LTALK_H
#include <QString>
#include <QVector>
#include <QDateTime>

#define SERVER_REQUEST_URL "http://192.168.100.8"
#define SERVER_BASE_URL_VALUE "/?platform=linux"


// Local cmd
#define LOCAL_CMD_EXIT           0x00
#define LOCAL_CMD_SHOW_MAIN_PAGE 0x01
#define LOCAL_CMD_HIDE_MAIN_PAGE 0x02
#define LOCAL_CMD_LOG_OUT        0x100
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

#endif // LTALK_H
