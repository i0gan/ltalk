#include "main_page.h"
#include "ui_main_page.h"


MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage) {
    ui->setupUi(this);
}

MainPage::~MainPage() {
    if(opened_floating_settings_window_)
        delete floating_settings_window_;
    if(opened_tray_icon_)
        delete tray_icon_;
    delete ui;
}

void MainPage::init() {
    opened_floating_settings_window_ = false;
    opened_tray_icon_ = false;
    pressed_ = false;
    setWindowTitle("Ltalk");
    setWindowIcon(QIcon(":/ui/logo.ico"));
    QPoint pos;
    pos.setX((QApplication::desktop()->width() - width()) / 2);
    pos.setY((QApplication::desktop()->height() - height()) / 2);
    move(pos);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    floating_settings_window_ = new FloatingSettingsWindow();
    connect(floating_settings_window_, &FloatingSettingsWindow::localCmd, this, &MainPage::dealWithLocalCmd);
    floating_settings_window_->init();
    opened_floating_settings_window_ = true;
    opened_tray_icon_ = true;
    tray_icon_ = new TrayIcon();
    connect(tray_icon_, &TrayIcon::localCmd, this, &MainPage::dealWithLocalCmd);
    tray_icon_->init();
    network_get_head_image = new QNetworkAccessManager(this);
    connect(network_get_head_image, &QNetworkAccessManager::finished, this, &MainPage::requestGetHeadImageReply);
}

void MainPage::mousePressEvent(QMouseEvent *event) {
    pressed_ = true;
    mouse_pos_ = event->pos();
}

void MainPage::mouseReleaseEvent(QMouseEvent *) {
    pressed_ = false;
}

void MainPage::mouseMoveEvent(QMouseEvent *event) {
    if(pressed_) {
        move(event->globalPos() - mouse_pos_);
    }
}

void MainPage::on_toolButton_min_clicked()
{
    showMinimized();
}

void MainPage::on_toolButton_close_clicked() {
    hide();
}

void MainPage::on_pushButton_groups_clicked()
{
    ui->stackedWidget_list->setCurrentWidget(ui->page_groupList);
}

void MainPage::on_pushButton_friends_clicked()
{
    ui->stackedWidget_list->setCurrentWidget(ui->page_friendList);
}


void MainPage::on_pushButton_messages_clicked()
{
    ui->stackedWidget_list->setCurrentWidget(ui->page_messageList);
}

void MainPage::on_pushButton_settings_clicked()
{
    floating_settings_window_->setParentPos(this->pos());
    floating_settings_window_->show();
}

void MainPage::dealWithLocalCmd(LocalCmd cmd) {
    emit localCmd(cmd);
}

void MainPage::setUserInfo(const UserInfo &user_info) {
    ui->label_email->setText(user_info.email);
    ui->label_nickname->setText(user_info.nickname);
    account_ = user_info.account;
    requestGetHeadImage(user_info.uid, user_info.token, user_info.head_image_url);
}

void MainPage::requestGetHeadImage(QString uid, QString token, QString head_image_url) {
    QNetworkRequest request;
    request.setRawHeader("Origin", "http://ltalk.co");
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Date", Util::getTime().toUtf8().data());
    request.setRawHeader("Connection", "close");
    QUrl url;
    head_image_url = "http://192.168.100.8/user/418894113@qq.com/public/test.png";
    url = head_image_url + QString("/?request=get_public_file&platform=linux");
    request.setUrl(url);
    network_get_head_image->get(request);
}

void MainPage::requestGetHeadImageReply(QNetworkReply *reply) {
    QDir dir;
    QString user_images_dir = dir.homePath() + '/' +  DATA_PATH + ('/' + account_) + "/images";
    QString head_image_path = user_images_dir + "/head_image";
    if(!dir.exists(user_images_dir))
        dir.mkpath(user_images_dir);

    qDebug() << "recv: " << user_images_dir << "  " << dir.absolutePath();
    QFile head_image_file;
    head_image_file.setFileName(head_image_path);
    head_image_file.open(QIODevice::WriteOnly);
    head_image_file.write(reply->readAll());
    head_image_file.close();

    // 设置头像
    QString style = "QLabel{ border-image:";
    style += QString("url(%1); border-radius:30px;} QLabel:hover{ border:4px;}").arg(head_image_path);
    ui->label_headImage->setStyleSheet(style);
}

void MainPage::setTheme(QString theme) {
    if(theme == "default") {
        ui->label_frame->setStyleSheet("QLabel{ border-image : url(':/ui/themes/default/main_page.png')}");
    }else if(theme == "love") {
        ui->label_frame->setStyleSheet("QLabel{ border-image : url(':/ui/themes/love/main_page.png')}");
    }
}
