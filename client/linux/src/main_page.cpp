#include "main_page.h"
#include "ui_main_page.h"


MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage),
    pressed_(false),
    get_what_(GetWhat::none){
    ui->setupUi(this);

    floating_settings_window_ = new FloatingSettingsWindow();
    net_mannager_ = new QNetworkAccessManager(this);
    tray_icon_ = new TrayIcon();
}

MainPage::~MainPage() {
    delete floating_settings_window_;
    delete tray_icon_;
    delete ui;
}

void MainPage::init() {
    setWindowTitle("Ltalk");
    setWindowIcon(QIcon(":/ui/logo.ico"));
    ui->label_headImage->installEventFilter(this);
    QPoint pos;
    pos.setX((QApplication::desktop()->width() - width()) / 2);
    pos.setY((QApplication::desktop()->height() - height()) / 2);
    move(pos);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    connect(floating_settings_window_, &FloatingSettingsWindow::localCmd, this, &MainPage::dealWithLocalCmd);
    floating_settings_window_->init();
    connect(tray_icon_, &TrayIcon::localCmd, this, &MainPage::dealWithLocalCmd);
    tray_icon_->init();
    connect(net_mannager_, &QNetworkAccessManager::finished, this, &MainPage::requestGetFileReply);


    //test
    addUserListItem(user_info_);
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

bool MainPage::eventFilter(QObject *object, QEvent *e) {
    bool ret = true;
    if(e->type() == QEvent::MouseButtonPress && object == ui->label_headImage) {
        emit localCmd(LocalCmd::show_profile_page);
    }else
        ret = false;
    return ret;
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
    ui->stackedWidget_list->setCurrentWidget(ui->page_userList);
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
    user_info_ = user_info;
    ui->label_email->setText(user_info.email);
    ui->label_nickname->setText(user_info.nickname);
    account_ = user_info.account;
    get_what_ = GetWhat::myHeadImage;
    //获取头像
    requestGetFile(user_info_.head_image, GetWhat::myHeadImage, "./");
}

void MainPage::requestGetFile(QString url, GetWhat what, QString save_file_path,  bool private_file) {
    if(url == "none")
        return;
    get_what_ = what;
    save_file_path_ = save_file_path;
    request_.setRawHeader("Origin", "http://ltalk.co");
    request_.setRawHeader("Accept", "*/*");
    request_.setRawHeader("Content-Type", "application/json");
    request_.setRawHeader("Accept", "application/json");
    request_.setRawHeader("Date", Util::getTime().toUtf8().data());
    QString request_url;
    if(what == GetWhat::myHeadImage) {
        request_url = url + "&platform=linux";
    }
    if(private_file)
        request_url += "&token" + user_info_.token;
    request_.setUrl(request_url);
    net_mannager_->get(request_);
}

void MainPage::requestGetFileReply(QNetworkReply *reply) {
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
    } else if(theme == "love") {
        ui->label_frame->setStyleSheet("QLabel{ border-image : url(':/ui/themes/love/main_page.png')}");
    }
}

void MainPage::on_pushButton_changeTheme_clicked() {
    emit localCmd(LocalCmd::show_chnage_theme_page);
}

void MainPage::addUserListItem(const UserInfo &user_info) {
    UserListItem *item = new UserListItem();
    ui->Widget_userList->layout()->addWidget(item);
}

void MainPage::on_pushButton_addUser_clicked() {
    emit localCmd(LocalCmd::show_add_user_page);
}
