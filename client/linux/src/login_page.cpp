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
    setWindowTitle("Ltalk login");
    QRect rect = QApplication::desktop()->availableGeometry();
    pos.setX((rect.width() - width()) / 2);
    pos.setY((rect.height() - height()) / 2);
    move(pos);

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
