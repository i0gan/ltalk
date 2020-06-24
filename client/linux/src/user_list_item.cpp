#include "user_list_item.h"
#include "ui_user_list_item.h"

UserListItem::UserListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserListItem)
{
    ui->setupUi(this);
}

UserListItem::~UserListItem()
{
    delete ui;
}
