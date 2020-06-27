#include "profile_page.h"
#include "ui_profile_page.h"

ProfilePage::ProfilePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfilePage),
    pressed_(false),
    request_step_(RequestStep::getHeadImage)
{
    ui->setupUi(this);
}

ProfilePage::~ProfilePage()
{
    delete ui;
}
void ProfilePage::init() {
    setWindowTitle("Profile");
    setWindowIcon(QIcon(":/ui/logo.ico"));
    QPoint pos;
    pos.setX((QApplication::desktop()->width() - width()) / 2);
    pos.setY((QApplication::desktop()->height() - height()) / 2);
    move(pos);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    network_mannager_ = new QNetworkAccessManager();
    connect(network_mannager_, &QNetworkAccessManager::finished, this, &ProfilePage::requestReply);
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

void ProfilePage::requestReply(QNetworkReply *reply) {
    do {
        if(!(reply->error() == QNetworkReply::NetworkError::NoError))
            break;
        if(request_step_ == RequestStep::getHeadImage) {
            request_step_ = RequestStep::getProfileImage_1;
        }
    } while(false);
}

void ProfilePage::requestGetImage(RequestStep request_step, QString url) {

}
void ProfilePage::setUserInfo(const UserInfo &user_info) {
    user_info_ = user_info;
    QDir dir;
    QString user_images_dir = dir.homePath() + '/' +  DATA_PATH + ('/' + user_info_.account) + "/images";
    QString head_image_path = user_images_dir + "/head_image";
    if(!dir.exists(user_images_dir))
        dir.mkpath(user_images_dir);
    // 设置头像
    QString style = "QLabel{ border-image:";
    style += QString("url(%1)} QLabel:hover{ border:4px;}").arg(head_image_path);

    ui->label_headImage->setStyleSheet(style);
    ui->label_profile_image_1->setStyleSheet(style);
    ui->label_profile_image_2->setStyleSheet(style);
    ui->label_profile_image_3->setStyleSheet(style);
    ui->label_profile_image_4->setStyleSheet(style);
}
