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
    request.setRawHeader("Referer", "http://ltalk.lyxf.xyz");
    request.setRawHeader("Content-Type", "text/json");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Linux x64)");
    //request.setRawHeader("")

    //mannager->post()
    QJsonDocument json_document;
    QJsonObject json_object;
    json_object.insert("Account", 11343);
    json_object.insert("Password", password.toUtf8().toBase64().data());
    json_object.insert("Request", "login");
    json_object.insert("Platform", QString("Ltalk for linux x64"));
    json_object.insert("Client-Version", 0.1);
    json_object.insert("Token", QString("none"));
    json_object.insert("Date", QDateTime::currentDateTime().toString("yy-MM-dd dd:mm:ss"));

    json_document.setObject(json_object);
    QByteArray byte_array = json_document.toJson(QJsonDocument::Compact);
    qDebug() << byte_array.data();
    request.setUrl(QUrl(SERVER_REQUEST_URL));
    network_access_mannager->post(request, byte_array);
}

void Center::requestReply(QNetworkReply *reply) {
    qDebug() << "recv: " << reply->readAll();


}