#include "add_user_page.h"
#include "ui_add_user_page.h"

AddUserPage::AddUserPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddUserPage),
    network_access_mannager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
}

AddUserPage::~AddUserPage()
{
    delete ui;
}

void AddUserPage::init() {
    setWindowTitle("Ltalk login");
    setWindowIcon(QIcon(":/ui/logo.ico"));
    ui->pushButton_search->setShortcut(Qt::Key_Return);
    QPoint pos;
    pos.setX((QApplication::desktop()->width() - width()) / 2);
    pos.setY((QApplication::desktop()->height() - height()) / 2);
    move(pos);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->label_headImage->hide();
    ui->pushButton_add->hide();
    connect(network_access_mannager, &QNetworkAccessManager::finished, this, &AddUserPage::dealWithRecv);
}

void AddUserPage::mousePressEvent(QMouseEvent *event) {
    pressed_ = true;
    mouse_pos_ = event->pos();
}

void AddUserPage::mouseReleaseEvent(QMouseEvent *) {
    pressed_ = false;
}

void AddUserPage::mouseMoveEvent(QMouseEvent *event) {
    if(pressed_) {
        move(event->globalPos() - mouse_pos_);
    }
}

void AddUserPage::on_toolButton_close_clicked() {
    hide();
}

void AddUserPage::on_pushButton_search_clicked() {
    QString account = ui->lineEdit_input->text();
    if(account.isEmpty()) {
        return;
    }

    request_.setRawHeader("Origin", "http://ltalk.co");
    request_.setRawHeader("Accept", "application/json");
    request_.setRawHeader("Date", Util::getTime().toUtf8().data());
    QUrl url;
    url = SERVER_REQUEST_URL + QString("/?request=search_user&platform=linux&search=") + account + "&type=account";
    request_.setUrl(url);
    network_access_mannager->get(request_);
}

void AddUserPage::dealWithRecv(QNetworkReply *reply) {
    QByteArray recv_data;
    recv_data = reply->readAll();
    if(reply->rawHeader("Content-Type") == "application/json") {
        QJsonDocument json_document;
        QJsonObject json_obj;
        QJsonParseError json_parse_error;

        json_document = QJsonDocument::fromJson(recv_data, &json_parse_error);
        if(json_parse_error.error != QJsonParseError::NoError) {
            qDebug() << "json 解析失败";
            return;
        }

        json_obj = json_document.object();
        if(json_obj.value("content-type").toString() == "user_info" && json_obj.value("code").toInt() == int(ResponseCode::SUCCESS)) {
            dealWithSearchReply(json_obj.value("content").toObject());
        }else if(json_obj.value("code").toInt() == int(ResponseCode::NOT_EXIST)){
            dealWithNotExist();
        }

    }
}

void AddUserPage::dealWithSearchReply(const QJsonObject &json_obj) {
    searched_user_info_.account = json_obj.value("account").toString();
    searched_user_info_.uid = json_obj.value("uid").toString();
    searched_user_info_.head_image = json_obj.value("head_image").toString();
    searched_user_info_.address = json_obj.value("address").toString();
    searched_user_info_.nickname = json_obj.value("nickname").toString();
    searched_user_info_.ocupation = json_obj.value("occupation").toString();
    searched_user_info_.network_state = json_obj.value("network_state").toString();
    qDebug() << "okkkk " << searched_user_info_.head_image;

}

void AddUserPage::dealWithNotExist() {
    qDebug() << "找不到";
}
