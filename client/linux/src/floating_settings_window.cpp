#include "floating_settings_window.h"
#include "ui_floating_settings_window.h"

FloatingSettingsWindow::FloatingSettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FloatingSettingsWindow)
{
    ui->setupUi(this);
}

FloatingSettingsWindow::~FloatingSettingsWindow()
{
    delete ui;
}

void FloatingSettingsWindow::init() {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    setAttribute(Qt::WA_TranslucentBackground);
}

void FloatingSettingsWindow::setParentPos(QPoint pos) {
    QPoint Pos = (pos + QPoint(50, 460));
    move(Pos);
}

bool FloatingSettingsWindow::event(QEvent *event) {
    if (event->type() == QEvent::ActivationChange) {
        if(QApplication::activeWindow() != this) {
            this->hide();
        }
    }
    return QWidget::event(event);
}

void FloatingSettingsWindow::on_pushButton_exit_clicked() {
    emit localCmd(LocalCmd::EXIT);
}

void FloatingSettingsWindow::on_pushButton_changeTheme_clicked() {
    emit localCmd(LocalCmd::SHOW_CHANGE_THEME_PAGE);
}
