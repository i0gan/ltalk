#include "user_list_item.h"
#include "ui_user_list_item.h"

UserListItem::UserListItem(int num, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserListItem)
{
    ui->setupUi(this);
    this->installEventFilter(this);
    num_ = num;
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
        qDebug() << "okkk";
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
