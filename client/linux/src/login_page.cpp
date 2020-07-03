#include "login_page.h"
#include "ui_login_page.h"

LoginPage::LoginPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginPage), pressed_(false)
{
    ui->setupUi(this);
}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::init() {
    setWindowTitle("Ltalk login");
    setWindowIcon(QIcon(":/ui/logo.ico"));
    ui->pushButton_login->setShortcut(Qt::Key_Return);
    QPoint pos;
    pos.setX((QApplication::desktop()->width() - width()) / 2);
    pos.setY((QApplication::desktop()->height() - height()) / 2);
    move(pos);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    // set open link
    QString html;
    html = "<html><head/><body><p><a href=\"";
    html += SERVER_REQUEST_URL;
    html += "/?request=register&platform=web\">";
    html += "<span style=\" text-decoration: underline; color:#00ffff;\">";
    html += "register";
    html += "</span></a></p></body></html>";
    ui->label_register->setText(html);
    html = "<html><head/><body><p><a href=\"";
    html += SERVER_REQUEST_URL;
    html += "\">";
    html += "<span style=\" text-decoration: underline; color:#00ffff;\">";
    html += "forgot";
    html += "</span></a></p></body></html>";
    ui->label_forgot->setText(html);

    ui->label_register->setOpenExternalLinks(true);
    ui->label_forgot->setOpenExternalLinks(true);
    ui->label_additonal_image->setPixmap(QPixmap(":/image/login_additional_image.png"));
    ui->label_additonal_image->setScaledContents(true);
}
void LoginPage::on_pushButton_login_clicked()
{
    QString account = ui->lineEdit_account->text();
    QString password = ui->lineEdit_password->text();
    if(account.size() < 4) {
        qDebug() << "account formate error";
        return;
    }
    if(password.size() < 6) {
        qDebug() << "password formate error";
        return;
    }
    account_ = account;
    emit login(account, password);
}

void LoginPage::on_toolButton_close_clicked() {
    emit localCmd(LocalCmd::exit);
}

void LoginPage::on_toolButton_min_clicked() {
    showMinimized();
}

void LoginPage::mousePressEvent(QMouseEvent *event) {
    pressed_ = true;
    mouse_pos_ = event->pos();
}

void LoginPage::mouseReleaseEvent(QMouseEvent *) {
    pressed_ = false;
}

void LoginPage::mouseMoveEvent(QMouseEvent *event) {
    if(pressed_) {
        move(event->globalPos() - mouse_pos_);
    }
}


void LoginPage::dealWithSuccess() {
    ui->label_process_text->setText("<html><head/><body><p align=\"center\"><span style=\" color:#00ff26;\">登录成功</span></p></body></html>");
}

void LoginPage::dealWithFailed(int code) {
    QString show_text = "<html><head/><body><p align=\"center\"><span style=\" color:#ff0000;\">";
    if(code == 1) {
        show_text += "帐号或密码错误";
    }else if(code == 12){
        show_text += "帐号不存在";
    }else if(code == 13){
        show_text += "你已经在其他地方登录";
    }else {
        show_text += "登录失败";
    }
    show_text += "</span></p></body></html>";
    ui->label_process_text->setText(show_text);
}

void LoginPage::dealWithRecv(QJsonObject &json_obj) {
    int code = json_obj.value("code").toInt();
    if(code == 0) {
        QString token = json_obj.value("token").toString();
        QString uid = json_obj.value("uid").toString();
        dealWithSuccess();
        emit logined(account_, uid, token);
    }else {
        dealWithFailed(code);
    }
}

void LoginPage::on_toolButton_changeTheme_clicked() {
    localCmd(LocalCmd::show_chnage_theme_page);
}

void LoginPage::setTheme(QString theme) {
    if(theme == "default") {
        ui->label_frame->setStyleSheet("QLabel{ border-image : url(':/ui/themes/default/dialog_page.png')}");
    }else if(theme == "love") {
        ui->label_frame->setStyleSheet("QLabel{ border-image : url(':/ui/themes/love/dialog_page.png')}");
    }
}
