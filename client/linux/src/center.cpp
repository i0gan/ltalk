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
    QNetworkAccessManager *mannager = new QNetworkAccessManager(this);
    connect(mannager, &QNetworkAccessManager::finished, this, &Center::loginReply);

    request.setUrl(QUrl(SERVER_REQUEST_URL));
    mannager->get(request);
    QJsonDocument json_document;
    //json_document = QString("sss");
}

void Center::loginReply(QNetworkReply *reply) {
    qDebug() << "recv: " << reply->readAll();
}
