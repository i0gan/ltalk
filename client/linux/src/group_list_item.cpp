#include "group_list_item.h"
#include "ui_group_list_item.h"

GroupListItem::GroupListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupListItem)
{
    ui->setupUi(this);
}

GroupListItem::~GroupListItem()
{
    delete ui;
}
