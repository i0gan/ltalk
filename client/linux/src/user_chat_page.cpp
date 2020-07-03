#include "user_chat_page.h"
#include "ui_user_chat_page.h"

UserChatPage::UserChatPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserChatPage)
{
    ui->setupUi(this);
}

UserChatPage::~UserChatPage()
{
    delete ui;
}
