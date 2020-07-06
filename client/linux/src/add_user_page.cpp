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
    setWindowTitle("Ltalk add user");
    setWindowIcon(QIcon(":/ui/logo.ico"));
    ui->pushButton_search->setShortcut(Qt::Key_Return);
    QPoint pos;
    pos.setX((QApplication::desktop()->width() - width()) / 2);
    pos.setY((QApplication::desktop()->height() - height()) / 2);
    move(pos);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    connect(network_access_mannager, &QNetworkAccessManager::finished, this, &AddUserPage::dealWithRecv);
    ui->label_notice->clear();
    ui->stackedWidget->setCurrentWidget(ui->search_page);

    ui->label_headImage->hide();
    ui->label_account->hide();
    ui->label_signature->hide();
    ui->label_address->hide();
    ui->label_network->hide();
    ui->label_nickname->hide();
    ui->pushButton_add->hide();
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
    url = SERVER_DOMAIN + QString("/?request=search_user&platform=linux&search=") + account + "&type=account";
    request_.setUrl(url);
    qDebug() << "url: " << url;
    network_access_mannager->get(request_);

    ui->label_headImage->hide();
    ui->label_account->hide();
    ui->label_signature->hide();
    ui->label_address->hide();
    ui->label_network->hide();
    ui->label_nickname->hide();
    ui->pushButton_add->hide();
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

    }else {
        QPixmap head_image;
        head_image.loadFromData(recv_data);
        ui->label_headImage->setPixmap(head_image);
        ui->label_headImage->setScaledContents(true);
    }
}

void AddUserPage::dealWithSearchReply(const QJsonObject &json_obj) {
    searched_user_info_.account = json_obj.value("account").toString();
    searched_user_info_.uid = json_obj.value("uid").toString();
    searched_user_info_.head_image = json_obj.value("head_image").toString();
    searched_user_info_.signature = json_obj.value("signature").toString();
    searched_user_info_.address = json_obj.value("address").toString();
    searched_user_info_.nickname = json_obj.value("nickname").toString();
    searched_user_info_.network_state = json_obj.value("network_state").toString();


    ui->label_account->setText("帐号: " + searched_user_info_.account);
    ui->label_nickname->setText("昵称: " + searched_user_info_.nickname);
    ui->label_signature->setText("签名: " + searched_user_info_.signature);
    if(searched_user_info_.network_state == "offline") {
        ui->label_network->setText("网络: 在线");
    }else {
        ui->label_network->setText("网络: " + searched_user_info_.network_state + "在线");
    }
    ui->label_address->setText("位置: " + searched_user_info_.address);

    ui->label_headImage->show();
    ui->label_account->show();
    ui->label_signature->show();
    ui->label_address->show();
    ui->label_network->show();
    ui->label_nickname->show();
    ui->pushButton_add->show();

    request_.setRawHeader("Origin", "http://ltalk.co");
    request_.setRawHeader("Accept", "application/json");
    request_.setRawHeader("Date", Util::getTime().toUtf8().data());

    QUrl url;
    url = SERVER_DOMAIN + searched_user_info_.head_image + "&platform=linux";
    request_.setUrl(url);
    network_access_mannager->get(request_);

    ui->label_notice->clear();
}

void AddUserPage::dealWithNotExist() {
    QString show_text = "<html><head/><body><p align=\"center\"><span style=\" color:#ff0000;\">";
    show_text += "帐号不存在";
    show_text += "</span></p></body></html>";
    ui->label_notice->setText(show_text);
}

void AddUserPage::setTheme(QString theme) {
    if(theme == "default") {
        ui->label_frame->setStyleSheet("QLabel{ border-image : url(':/ui/themes/default/dialog_page.png')}");
    }else if(theme == "love") {
        ui->label_frame->setStyleSheet("QLabel{ border-image : url(':/ui/themes/love/dialog_page.png')}");
    }
}

void AddUserPage::on_pushButton_add_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->send_page);
}

void AddUserPage::on_pushButton_send_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->search_page);
    QString show_text = "<html><head/><body><p align=\"center\"><span style=\" color:#FFFFFF;\">";
    show_text += "验证消息发送成功";
    show_text += "</span></p></body></html>";
    ui->label_notice->setText(show_text);
    QString send_msg = ui->plainTextEdit->toPlainText();
    emit addUser(searched_user_info_.account, searched_user_info_.uid, send_msg);
}
