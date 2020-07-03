#include "add_user_page.h"
#include "ui_add_user_page.h"

AddUserPage::AddUserPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddUserPage)
{
    ui->setupUi(this);
}

AddUserPage::~AddUserPage()
{
    delete ui;
}
