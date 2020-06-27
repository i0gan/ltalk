#include "cut_image.h"
#include "ui_cut_image.h"

CutImage::CutImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CutImage)
{
    ui->setupUi(this);
}

CutImage::~CutImage()
{
    delete ui;
}

void CutImage::on_toolButton_close_clicked() {

}

void CutImage::init() {
    setWindowTitle("Cut image");
    setWindowIcon(QIcon(":/ui/logo.ico"));
    QPoint pos;
    pos.setX((QApplication::desktop()->width() - width()) / 2);
    pos.setY((QApplication::desktop()->height() - height()) / 2);
    move(pos);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

void CutImage::mousePressEvent(QMouseEvent *event) {
    pressed_ = true;
    mouse_pos_ = event->pos();
}

void CutImage::mouseReleaseEvent(QMouseEvent *) {
    pressed_ = false;
}

void CutImage::mouseMoveEvent(QMouseEvent *event) {
    if(pressed_) {
        move(event->globalPos() - mouse_pos_);
    }
}
