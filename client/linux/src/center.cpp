#include "center.h"

Center::Center(QObject *parent) : QObject(parent) {

}

Center::~Center() {

}

void Center::init() {
    login_page_ = new LoginPage();
    main_page_ = new MainPage();
    login_page_->init();
    connect(login_page_, &LoginPage::login, this, &Center::requestLogin);
    connect(login_page_, &LoginPage::logined, this, &Center::dealWithLogined);
    network_access_mannager = new QNetworkAccessManager(this);
    connect(network_access_mannager, &QNetworkAccessManager::finished, this, &Center::requestReply);
    connect(main_page_, &MainPage::localCmd, this, &Center::dealWithLocalCmd);
}

void Center::start() {
    login_page_->show();
}


void Center::requestLogin(QString account, QString password) {
    //qDebug() << "requstlogin : " << account << " " << password;
    QNetworkRequest request;
    request.setRawHeader("Origin", "http://lyxf.xyz");
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Date", Util::getTime().toUtf8().data());

    //mannager->post()
    QJsonDocument json_document;
    QJsonObject json_object;
    json_object.insert("request", 1);
    json_object.insert("token", "none");
    json_object.insert("uid", "none");
    json_object.insert("client_version", "linux 0.1");
    json_object.insert("datetime", QDateTime::currentDateTime().toString("yy-MM-dd dd:mm:ss"));
    json_object.insert("content_type", "login_info");
    QJsonObject json_object_content;
    json_object_content.insert("account", account.toUtf8().data());
    json_object_content.insert("password", password.toUtf8().toBase64().data());
    json_object.insert("content", json_object_content);
    json_document.setObject(json_object);
    QByteArray byte_array = json_document.toJson(QJsonDocument::Compact);
    //qDebug() << byte_array;
    QUrl url;
    url = SERVER_REQUEST_URL + QString("/?request=login&platform=linux");
    request.setUrl(url);
    network_access_mannager->post(request, byte_array);
}

void Center::requestReply(QNetworkReply *reply) {
    QJsonDocument json_document;
    QJsonObject json_object;
    QJsonParseError json_error;
    do {
        if(reply->rawHeader(QString("Content-Type").toUtf8()) == QString("application/json").toUtf8()) {
            recved_data_ = reply->readAll();
            //qDebug() << recv_data;
            json_document = QJsonDocument::fromJson(recved_data_, &json_error);
            if(json_error.error != QJsonParseError::NoError) {
                qDebug() << "json 文件解析失败";
                break;
            }
        }else {
            qDebug() << "接收类型不对" << reply->rawHeader(QString("content-type").toUtf8());
            //qDebug() << reply->readAll();
            break;
        }
        json_object = json_document.object();
        QJsonValue json_value_request = json_object.value("request");
        if(!json_value_request.isString()) {
            qDebug() << "Json类型不对";
            break;
        }
        QString request = json_value_request.toString();
        if(request == "login") {
            login_page_->dealWithRecv(json_object);
            break;
        }else if(request == "get_user_info") {
            qDebug() << "get_user_info: " << recved_data_;
            handleGetUserInfoReply(json_object);
            break;
        }else {
             qDebug() << "收到数据出错";
            break;
        }
    } while(false);
}

void Center::dealWithLogined(QString account, QString uid, QString token) {
    user_.account = account;
    user_.uid = uid;
    user_.token = token;
    qDebug() << account << " uid: " << uid << "token: " << token;
    login_page_->close();
    main_page_->init();
    main_page_->show();

    requestGetUserInfo();
}


void Center::dealWithLocalCmd(size_t cmd) {
    qDebug() << "sss";
    switch (cmd) {
    case LOCAL_CMD_EXIT: {
        this->exit();
    } break;
    case LOCAL_CMD_LOG_OUT: {

    } break;
    default: {

    } break;
    }
}

void Center::exit() {
    login_page_->close();
    main_page_->close();
    delete  main_page_;
    delete  login_page_;
}

void Center::keepConnect() {

}

void Center::requestGetUserInfo() {
    QNetworkRequest request;
    request.setRawHeader("Origin", "http://ltalk.co");
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Date", Util::getTime().toUtf8().data());
    QUrl url;
    url = SERVER_REQUEST_URL + QString("/?request=get_user_info&platform=linux&account=") + user_.account + "&uid=" + user_.uid + "&token=" + user_.token;
    request.setUrl(url);
    network_access_mannager->get(request);
}

void Center::requestGetFriendList() {

}

void Center::requestGetGroupList() {

}

void Center::handleGetUserInfoReply(const QJsonObject &json_obj) {
    int code = json_obj.value("code").toInt();
    QJsonObject content;
    if(code != 0) {
        return;
    }
    if(json_obj.value("content").isObject())
        content = json_obj.value("content").toObject();
    user_.account = content.value("account").toString();
    user_.email = content.value("email").toString();
    user_.nickname = content.value("nickname").toString();
    user_.head_image_url = content.value("head_image_url").toString();
    user_.name = content.value("name").toString();
    user_.phone_number = content.value("phone_number").toString();
    user_.address = content.value("address").toString();
    user_.creation_time = content.value("creation_time").toString();
    user_.network_state = content.value("network_state").toString();
    user_.ocupation = content.value("occupation").toString();
    main_page_->setUserInfo(user_);
    //qDebug() << "email" << user_.creation_time ;//json_obj;
}
