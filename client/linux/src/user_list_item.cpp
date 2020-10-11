#include "user_list_item.h"
#include "ui_user_list_item.h"

UserListItem::UserListItem(QString uid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserListItem)
{
    ui->setupUi(this);
    this->installEventFilter(this);
    uid_ = uid;
}

UserListItem::~UserListItem()
{
    delete ui;
}

void UserListItem::init() {

}

bool UserListItem:: eventFilter(QObject *obj, QEvent *e) {
    bool ret = true;
    if(obj == this && e->type() == QEvent::MouseButtonDblClick) {
        emit doubleClick(uid_);
    }else
        ret = false;
    return ret;
}

void UserListItem::mouseMoveEvent(QMouseEvent *e) {
    //if(e->type() == QMouseEvent::
}

void UserListItem::setImage(const QPixmap &pixmap) {
    ui->label_headImage->setPixmap(pixmap);
    ui->label_headImage->setScaledContents(true);
}

void UserListItem::setImageUrl(QString url) {
    image_url_ = url;
}
void UserListItem::setAccount(const QString &account){
    ui->label_account->setText(account);
}
void UserListItem::setName(const QString &name){
    ui->label_name->setText(name);
}

void UserListItem::setNetwork(const QString &network) {

}
