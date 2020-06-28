#include "message_list_item.h"
#include "ui_message_list_item.h"

MessageListItem::MessageListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageListItem)
{
    ui->setupUi(this);
}

MessageListItem::~MessageListItem() {
    delete ui;
}
