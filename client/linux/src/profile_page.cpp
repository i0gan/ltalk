#include "profile_page.h"
#include "ui_profile_page.h"

ProfilePage::ProfilePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfilePage)
{
    ui->setupUi(this);
}

ProfilePage::~ProfilePage()
{
    delete ui;
}
void ProfilePage::init() {
    pressed_ =  false;
    setWindowTitle("Profile");
    setWindowIcon(QIcon(":/ui/logo.ico"));
    QPoint pos;
    pos.setX((QApplication::desktop()->width() - width()) / 2);
    pos.setY((QApplication::desktop()->height() - height()) / 2);
    move(pos);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}
void ProfilePage::mousePressEvent(QMouseEvent *event) {
    pressed_ = true;
    mouse_pos_ = event->pos();
}

void ProfilePage::mouseReleaseEvent(QMouseEvent *) {
    pressed_ = false;
}

void ProfilePage::mouseMoveEvent(QMouseEvent *event) {
    if(pressed_) {
        move(event->globalPos() - mouse_pos_);
    }
}

void ProfilePage::on_toolButton_min_clicked() {
    showMinimized();
}

void ProfilePage::on_toolButton_close_clicked() {
    hide();
}

void ProfilePage::setTheme(QString theme) {
    if(theme == "default") {
        ui->label_frame->setStyleSheet("QLabel{ border-image : url(':/ui/themes/default/dialog_page.png')}");
    }else if(theme == "love") {
        ui->label_frame->setStyleSheet("QLabel{ border-image : url(':/ui/themes/love/dialog_page.png')}");
    }
}
