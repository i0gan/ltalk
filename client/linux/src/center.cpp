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
    request.setRawHeader("Referer", "http://google.com");
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Linux x64)");
    //request.setRawHeader("")

    request.setUrl(QUrl(SERVER_REQUEST_URL));
    network_access_mannager->post(request, "I love you AAAAAA");
    //mannager->post()
    QJsonDocument json_document;
    QJsonObject json_object;
//    json_object.
//    json_document.setArray()
    //json_document = QString("sss");
}

void Center::requestReply(QNetworkReply *reply) {
    qDebug() << "recv: " << reply->readAll();


}
