#include "login_page.h"
#include "ui_login_page.h"

LoginPage::LoginPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginPage)
{
    ui->setupUi(this);
}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::init() {
    pressed_ = false;
    setWindowTitle("Ltalk login");
    setWindowIcon(QIcon(":/ui/logo.ico"));
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
    html += "/register/index.html\">";
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
}
void LoginPage::on_pushButton_login_clicked()
{
    QString account = ui->lineEdit_account->text();
    QString password = ui->lineEdit_password->text();
    if(account.size() <= 2) {
        qDebug() << "account formate error";
        return;
    }
    if(password.size() < 2) {
        qDebug() << "password formate error";
        return;
    }

    emit login(account, password);
}

void LoginPage::on_toolButton_close_clicked()
{
    close();
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

void LoginPage::on_toolButton_min_clicked()
{
    showMinimized();
}
