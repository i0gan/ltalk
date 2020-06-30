#include "profile_page.h"
#include "ui_profile_page.h"

ProfilePage::ProfilePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfilePage),
    pressed_(false),
    request_step_(ImageType::head_image),
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
    ui->lineEdit_QQ->installEventFilter(this);
    ui->lineEdit_address->installEventFilter(this);
    ui->lineEdit_nickname->installEventFilter(this);
    ui->lineEdit_occupation->installEventFilter(this);

    ui->progressBar_upload->hide();
    ui->pushButton_saveModifiedInfo->hide();
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
        modifyImage(ImageType::head_image);
    }else if(object == ui->label_profile_image_1 && e->type() == QEvent::MouseButtonDblClick) {
        modifyImage(ImageType::profile_image_1);
    }else if(object == ui->label_profile_image_2 && e->type() == QEvent::MouseButtonDblClick) {
        modifyImage(ImageType::profile_image_2);
    }else if(object == ui->label_profile_image_3 && e->type() == QEvent::MouseButtonDblClick) {
        modifyImage(ImageType::profile_image_3);
    }else if(object == ui->label_profile_image_4 && e->type() == QEvent::MouseButtonDblClick) {
        modifyImage(ImageType::profile_image_4);
    }else if(object == ui->lineEdit_QQ && e->type() == QEvent::MouseButtonDblClick) {
        ui->lineEdit_QQ->setReadOnly(false);
        ui->pushButton_saveModifiedInfo->show();
    }else if(object == ui->lineEdit_address && e->type() == QEvent::MouseButtonDblClick) {
        ui->lineEdit_address->setReadOnly(false);
        ui->pushButton_saveModifiedInfo->show();
    }else if(object == ui->lineEdit_nickname && e->type() == QEvent::MouseButtonDblClick) {
        ui->lineEdit_nickname->setReadOnly(false);
        ui->pushButton_saveModifiedInfo->show();
    }else if(object == ui->lineEdit_occupation && e->type() == QEvent::MouseButtonDblClick) {
        ui->lineEdit_occupation->setReadOnly(false);
        ui->pushButton_saveModifiedInfo->show();
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
    QPixmap image = QPixmap(saved_file_name);
    switch (crop_image_type_) {
    case ImageType::head_image: {
        ui->label_headImage->setPixmap(image);
        ui->label_headImage->setScaledContents(true);
    } break;
    case ImageType::profile_image_1: {
        ui->label_profile_image_1->setPixmap(image);
        ui->label_profile_image_1->setScaledContents(true);
    } break;
    case ImageType::profile_image_2: {
        ui->label_profile_image_2->setPixmap(image);
        ui->label_profile_image_2->setScaledContents(true);
    } break;
    case ImageType::profile_image_3: {
        ui->label_profile_image_3->setPixmap(image);
        ui->label_profile_image_3->setScaledContents(true);
    } break;
    case ImageType::profile_image_4: {
        ui->label_profile_image_4->setPixmap(image);
        ui->label_profile_image_4->setScaledContents(true);
    } break;
    default: break;
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
    request_url += "/?request=upload_profile_image&platform=linux&account=";
    request_url += user_info_.account + "&uid=" + user_info_.uid + "&token=";
    request_url += user_info_.token;
    switch (image) {
    case ImageType::head_image: {
        request_url += "&type=head_image&name=head_image.jpg";
    } break;
    case ImageType::profile_image_1: {
        request_url += "&type=profile_image_1&name=profile_image_1.jpg";
    } break;
    case ImageType::profile_image_2: {
        request_url += "&type=profile_image_2&name=profile_image_2.jpg";
    } break;
    case ImageType::profile_image_3: {
        request_url += "&type=profile_image_3&name=profile_image_3.jpg";
    } break;
    case ImageType::profile_image_4: {
        request_url += "&type=profile_image_4&name=profile_image_4.jpg";
    } break;
    default:
        break;
    }
    qDebug() << "upload " << request_url;
    request_.setUrl(request_url);
    reply_ = network_mannager_->post(request_, image_data);
    connect(reply_, &QNetworkReply::uploadProgress, this, &ProfilePage::uploadProgress);
    is_modifying_image_ = false;
}

void ProfilePage::uploadProgress(qint64 bytesSent, qint64 bytesTotal) {
    if(bytesSent == bytesTotal) {
        ui->progressBar_upload->hide();
    }else {
        ui->progressBar_upload->show();
        ui->progressBar_upload->setRange(0, bytesTotal);
        ui->progressBar_upload->setValue(bytesSent);
    }
    qDebug() << "uploaded: " << bytesSent << " / " << bytesTotal;
}

void ProfilePage::setTheme(QString theme) {
    if(theme == "default") {
        ui->label_frame->setStyleSheet("QLabel{ border-image : url(':/ui/themes/default/dialog_page.png')}");
    }else if(theme == "love") {
        ui->label_frame->setStyleSheet("QLabel{ border-image : url(':/ui/themes/love/dialog_page.png')}");
    }
    image_cropper_page_->setTheme(theme);
}

void ProfilePage::requestReply(QNetworkReply *reply) {
    do {
        if(!(reply->error() == QNetworkReply::NetworkError::NoError))
            break;
        QByteArray data = reply->readAll();


        if(reply->rawHeader(QString("Content-Type").toUtf8()) == QString("application/json").toUtf8()) {
            //qDebug() << recv_data;
            QJsonDocument json_document;
            QJsonParseError json_error;
            QJsonObject json_obj;
            json_document = QJsonDocument::fromJson(data, &json_error);
            if(json_error.error != QJsonParseError::NoError) {
                qDebug() << "json 文件解析失败";
                break;
            }
            json_obj = json_document.object();
            dealWithServerResponse(json_obj);
            return;
        }

        QDir dir;
        QString user_images_dir = dir.homePath() + '/' +  DATA_PATH + ('/' + user_info_.account) + "/images";
        QString store_path = user_images_dir;
        if(!dir.exists(user_images_dir))
            dir.mkpath(user_images_dir);
        qDebug() << "recv: " << user_images_dir << "  " << dir.absolutePath();
        QPixmap image;
        image.loadFromData(data);
        switch (request_step_) {
        case ImageType::head_image: {
            ui->label_headImage->setPixmap(image);
            ui->label_headImage->setScaledContents(true);
            requestGetImage(ImageType::profile_image_1);
        } break;
        case ImageType::profile_image_1: {
            ui->label_profile_image_1->setPixmap(image);
            ui->label_profile_image_1->setScaledContents(true);
            requestGetImage(ImageType::profile_image_2);
        } break;
        case ImageType::profile_image_2: {
            ui->label_profile_image_2->setPixmap(image);
            ui->label_profile_image_2->setScaledContents(true);
            requestGetImage(ImageType::profile_image_3);
        } break;
        case ImageType::profile_image_3: {
            ui->label_profile_image_3->setPixmap(image);
            ui->label_profile_image_3->setScaledContents(true);
            requestGetImage(ImageType::profile_image_4);
        } break;
        case ImageType::profile_image_4: {
            ui->label_profile_image_4->setPixmap(image);
            ui->label_profile_image_4->setScaledContents(true);
            request_step_ = ImageType::none;
        } break;
        default:
            break;
        }

    } while(false);
}
void ProfilePage::dealWithServerResponse(const QJsonObject &json_obj) {
    int code = json_obj.value("code").toInt();
    if(code == 0 && json_obj.value("request").toString() == "upload_profile_image") {
        qDebug() << "上传成功";


    }else {
        qDebug() << "上传失败!";
    }
}


void ProfilePage::requestGetImage(ImageType request_step) {
    QString url;
    request_step_ = request_step;
    switch (request_step) {
    case ImageType::head_image: {
        url = user_info_.head_image;
    } break;
    case ImageType::profile_image_1: {
        url = user_info_.profile_image_1;
    } break;
    case ImageType::profile_image_2: {
        url = user_info_.profile_image_2;
    } break;
    case ImageType::profile_image_3: {
        url = user_info_.profile_image_3;
    } break;
    case ImageType::profile_image_4: {
        url = user_info_.profile_image_4;
    } break;
    default:
        break;
    }
    if(url == "none")
        return;
    request_.setRawHeader("Origin", "http://ltalk.co");
    request_.setRawHeader("Accept", "*/*");
    request_.setRawHeader("Content-Type", "application/json");
    request_.setRawHeader("Accept", "application/json");
    request_.setRawHeader("Date", Util::getTime().toUtf8().data());
    url += "&platform=linux";
    request_.setUrl(url);
    network_mannager_->get(request_);
}

void ProfilePage::setNextRequestStep() {
    if(request_step_ == ImageType::head_image)
        request_step_ = ImageType::profile_image_1;
    else if(request_step_ == ImageType::profile_image_1)
        request_step_ = ImageType::profile_image_2;
    else if(request_step_ == ImageType::profile_image_2)
        request_step_ = ImageType::profile_image_3;
    else if(request_step_ == ImageType::profile_image_3)
        request_step_ = ImageType::profile_image_4;
    else
        request_step_ = ImageType::head_image;
}

void ProfilePage::setUserInfo(const UserInfo &user_info) {
    user_info_ = user_info;
    requestGetImage(ImageType::head_image);
}

void ProfilePage::on_pushButton_saveModifiedInfo_clicked()
{
    ui->lineEdit_QQ->setReadOnly(true);
    ui->lineEdit_address->setReadOnly(true);
    ui->lineEdit_nickname->setReadOnly(true);
    ui->lineEdit_occupation->setReadOnly(true);
    ui->pushButton_saveModifiedInfo->hide();

    QJsonDocument json_document;
    QJsonObject json_main_obj;
    QJsonObject json_content_obj;

}
