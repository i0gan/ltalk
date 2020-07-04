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
    if(reply->rawHeader("Content-Type") == "application/json") {
        qDebug() << reply->readAll();
    }
}
