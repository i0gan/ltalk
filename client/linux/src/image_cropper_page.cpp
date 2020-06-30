#include "image_cropper_page.h"
#include "ui_image_cropper_page.h"

ImageCropperPage::ImageCropperPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageCropperPage),
    opened_(false) {
    ui->setupUi(this);
    image_label_ = new ImageCropperLabel(400, 250, this);
}

ImageCropperPage::~ImageCropperPage()
{
    delete ui;
}

void ImageCropperPage::init() {
    setWindowTitle("Image cropper");
    setWindowIcon(QIcon(":/ui/logo.ico"));
    QPoint pos;
    pos.setX((QApplication::desktop()->width() - width()) / 2);
    pos.setY((QApplication::desktop()->height() - height()) / 2);
    move(pos);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    image_label_->move(80, 90);
}

bool ImageCropperPage::crop(const QPixmap &pixmap, CropperShape shape, QSize size) {
    if(pixmap.isNull()) {
        emit failed();
        return false;
    }
    image_label_->setCropper(shape, size);
    image_label_->setOriginalImage(pixmap);
    image_label_->enableOpacity(true);
    image_label_->setCropperFixedWidth(100);
    image_label_->setCropperFixedHeight(100);

    return true;
}

void ImageCropperPage::mousePressEvent(QMouseEvent *event) {
    pressed_ = true;
    mouse_pos_ = event->pos();
}

void ImageCropperPage::mouseReleaseEvent(QMouseEvent *) {
    pressed_ = false;
}

void ImageCropperPage::mouseMoveEvent(QMouseEvent *event) {
    if(pressed_) {
        move(event->globalPos() - mouse_pos_);
    }
}

void ImageCropperPage::setTheme(QString theme) {
    if(theme == "default") {
        ui->label_frame->setStyleSheet("QLabel{ border-image : url(':/ui/themes/default/dialog_page.png')}");
    }else if(theme == "love") {
        ui->label_frame->setStyleSheet("QLabel{ border-image : url(':/ui/themes/love/dialog_page.png')}");
    }
}

void ImageCropperPage::setPixmap(const QPixmap &pixmap) {
    pixmap_ = pixmap;
}

void ImageCropperPage::setPixmap(const QString &file_name) {
    pixmap_ = QPixmap(file_name);
}

void ImageCropperPage::on_pushButton_OK_clicked() {
    QPixmap pixmap = image_label_->getCroppedImage();
    QString saved_file_name = "tmp_crop_image.jpg";
    pixmap.save(saved_file_name);
    hide();
    emit finshed(saved_file_name);
}

void ImageCropperPage::on_toolButton_close_clicked()
{
    hide();
}
