#include "center.h"

Center::Center(QObject *parent) : QObject(parent)
{

}

Center::~Center() {
    if(opened_login_page_)
        delete login_page_;
}

void Center::init() {
    opened_login_page_ = false;
    login_page_ = new LoginPage();
    opened_login_page_ = true;
    login_page_->init();
    connect(login_page_, &LoginPage::login, this, &Center::requestLogin);
    network_access_mannager = new QNetworkAccessManager(this);
    connect(network_access_mannager, &QNetworkAccessManager::finished, this, &Center::requestReply);
}

void Center::start() {
    login_page_->show();
}


void Center::requestLogin(QString account, QString password) {
    qDebug() << "requstlogin : " << account << " " << password;
    QNetworkRequest request;
    request.setRawHeader("Origin", "http://lyxf.xyz");
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Date", getTime().toUtf8().data());

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
            QByteArray recv_data = reply->readAll();
            //qDebug() << recv_data;
            json_document = QJsonDocument::fromJson(recv_data, &json_error);
            if(json_error.error != QJsonParseError::NoError) {
                qDebug() << "json 文件解析失败";
                break;
            }
        }else {
            qDebug() << "接收类型不对";
            reply->readAll();
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
            break;
        }else {
             qDebug() << "收到数据出错";
            break;
        }
    } while(false);
}

QString Center::getTime() {
    return QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
}
