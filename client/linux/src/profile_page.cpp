#include "profile_page.h"
#include "ui_profile_page.h"

ProfilePage::ProfilePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfilePage),
    pressed_(false),
    request_step_(RequestStep::getHeadImage),
    is_modifying_image_(false),
    crop_image_type_(ImageType::none)
{
    ui->setupUi(this);
    image_cropper_page_ = new ImageCropperPage();
    network_mannager_ = new QNetworkAccessManager();
}

ProfilePage::~ProfilePage()
{
    delete image_cropper_page_;
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
    ui->label_headImage->installEventFilter(this);
    ui->label_profile_image_1->installEventFilter(this);
    ui->label_profile_image_2->installEventFilter(this);
    ui->label_profile_image_3->installEventFilter(this);
    ui->label_profile_image_4->installEventFilter(this);

    image_cropper_page_->init();
    connect(network_mannager_, &QNetworkAccessManager::finished, this, &ProfilePage::requestReply);
    connect(image_cropper_page_, &ImageCropperPage::finshed, this, &ProfilePage::dealWithCroped);
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

bool ProfilePage::eventFilter(QObject *object, QEvent *e) {
    bool ret = true;
    if(object == ui->label_headImage && e->type() == QEvent::MouseButtonDblClick) {
        modifyImage(ImageType::headImage);
    }else if(object == ui->label_profile_image_1 && e->type() == QEvent::MouseButtonDblClick) {
        modifyImage(ImageType::profileImage_1);
    }else if(object == ui->label_profile_image_2 && e->type() == QEvent::MouseButtonDblClick) {
        modifyImage(ImageType::profileImage_2);
    }else if(object == ui->label_profile_image_3 && e->type() == QEvent::MouseButtonDblClick) {
        modifyImage(ImageType::profileImage_3);
    }else if(object == ui->label_profile_image_4 && e->type() == QEvent::MouseButtonDblClick) {
        modifyImage(ImageType::profileImage_4);
    }else
        ret = false;
    return ret;
}
void ProfilePage::modifyImage(ImageType image) {
    if(is_modifying_image_ == true)
        return;
    crop_image_type_ = image;
    is_modifying_image_ = true;
    QString file_name = QFileDialog::getOpenFileName(nullptr, "选择图片", "../", "image (*.png *.jpg)");
    if(file_name.isEmpty()) {
        is_modifying_image_ = false;
    }
    if(image_cropper_page_->crop(QPixmap(file_name), CropperShape::RECT, QSize(10, 10)))
        image_cropper_page_->show();
    else
        is_modifying_image_ = false;

}

void ProfilePage::on_toolButton_min_clicked() {
    showMinimized();
}

void ProfilePage::on_toolButton_close_clicked() {
    hide();
}

void ProfilePage::dealWithCroped(QString saved_file_name) {
    QString style = "QLabel{ border-image:";
    style += QString("url(%1)} QLabel:hover{ border:4px;}").arg(saved_file_name);
    if(crop_image_type_ == ImageType::headImage) {
        ui->label_headImage->setStyleSheet(style);
    }else if(crop_image_type_ == ImageType::profileImage_1) {
        ui->label_profile_image_1->setStyleSheet(style);
    }else if(crop_image_type_ == ImageType::profileImage_2) {
        ui->label_profile_image_2->setStyleSheet(style);
    }else if(crop_image_type_ == ImageType::profileImage_3) {
        ui->label_profile_image_3->setStyleSheet(style);
    }else if(crop_image_type_ == ImageType::profileImage_4) {
        ui->label_profile_image_4->setStyleSheet(style);
    }
    uploadImage(crop_image_type_, saved_file_name);
}

void ProfilePage::uploadImage(ImageType image, QString saved_file_name) {
    QByteArray image_data;
    QFile image_file;
    image_file.setFileName(saved_file_name);
    image_file.open(QIODevice::ReadOnly);
    image_data = image_file.readAll();
    qDebug() << "post image data";
    request_.setRawHeader("Origin", "http://ltalk.co");
    request_.setRawHeader("Accept", "*/*");
    request_.setRawHeader("Content-Type", "obj");
    request_.setRawHeader("Accept", "application/json");
    request_.setRawHeader("Date", Util::getTime().toUtf8().data());
    QString request_url = SERVER_REQUEST_URL;
    request_url += "/?request=upload_profile_image&platform=linux&name=head_image&account=";
    request_url += user_info_.account + "&uid=" + user_info_.uid + "&token=";
    request_url += user_info_.token;
    request_.setUrl(request_url);
    network_mannager_->post(request_, image_data);
    is_modifying_image_ = false;
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
    if(url == "none") {
        setNextRequestStep();
        return;
    }
}


void ProfilePage::setNextRequestStep() {
    if(request_step_ == RequestStep::getHeadImage)
        request_step_ = RequestStep::getProfileImage_1;
    else if(request_step_ == RequestStep::getProfileImage_1)
        request_step_ = RequestStep::getProfileImage_2;
    else if(request_step_ == RequestStep::getProfileImage_2)
        request_step_ = RequestStep::getProfileImage_3;
    else if(request_step_ == RequestStep::getProfileImage_3)
        request_step_ = RequestStep::getProfileImage_4;
    else
        request_step_ = RequestStep::getHeadImage;
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
