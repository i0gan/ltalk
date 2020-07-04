#ifndef LTALK_H
#define LTALK_H
#include <QString>
#include <QVector>
#include <QDateTime>
#define DEBUG

#ifdef DEBUG
#define SERVER_REQUEST_URL "http://192.168.100.8"
#else
#define SERVER_REQUEST_URL "http://ltalk.co"
#endif

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
    QString qq;
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
    show_main_page,
    show_chnage_theme_page,
    show_about_page,
    show_profile_page,
    show_add_user_page,
    logout,
    main_page_get_head_image,
    main_page_get_friend_list,
    main_page_get_group_list,
    get_user_info,
};


enum class ResponseCode {
    SUCCESS = 0,
    FAILURE,
    NOT_FOUND,
    INTERNAL_ERROR,
    NO_ACCESS,
    NO_PERMISSSION = 5,
    ERROR_PARSING_URL,
    ERROR_PARSING_CONTENT,
    ERROR_HTTP_CONTENT,
    ERROR_JSON_CONTENT_TYPE,
    ERROR_JSON_CONTENT = 10,
    EXIST,
    NOT_EXIST,
    LOGINED,
};

#endif // LTALK_H
