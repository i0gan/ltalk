#include "user_list_item.h"
#include "ui_user_list_item.h"

UserListItem::UserListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserListItem)
{
    ui->setupUi(this);
    this->installEventFilter(this);
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
