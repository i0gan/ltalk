#include "change_theme_page.h"
#include "ui_change_theme_page.h"

ChangeThemePage::ChangeThemePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeThemePage)
{
    ui->setupUi(this);
}

ChangeThemePage::~ChangeThemePage()
{
    delete ui;
}

void ChangeThemePage::setTheme(QString theme) {
    if(theme == "default") {
        ui->label_frame->setStyleSheet("QLabel{ border-image : url(':/ui/themes/default/dialog_page.png')}");
    }else if(theme == "love") {
        ui->label_frame->setStyleSheet("QLabel{ border-image : url(':/ui/themes/love/dialog_page.png')}");
    }
}

void ChangeThemePage::init() {
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->label_theme_default->installEventFilter(this);
    ui->label_theme_love->installEventFilter(this);
    QPoint pos;
    pos.setX((QApplication::desktop()->width() - width()) / 2);
    pos.setY((QApplication::desktop()->height() - height()) / 2);
    move(pos);

}

void ChangeThemePage::mousePressEvent(QMouseEvent *event) {
    pressed_ = true;
    mouse_pos_ = event->pos();
}

void ChangeThemePage::mouseReleaseEvent(QMouseEvent *) {
    pressed_ = false;
}

void ChangeThemePage::mouseMoveEvent(QMouseEvent *event) {
    if(pressed_) {
        move(event->globalPos() - mouse_pos_);
    }
}

bool ChangeThemePage::eventFilter(QObject *object, QEvent *e) {
    bool ret = true;
    if(e->type() == QEvent::MouseButtonPress && object == ui->label_theme_default) {
        emit changed("default");
    }else if(e->type() == QEvent::MouseButtonPress && object == ui->label_theme_love) {
        emit changed("love");
    }else
        ret = false;
    return false;
}

void ChangeThemePage::on_toolButton_close_clicked() {
    hide();
}

