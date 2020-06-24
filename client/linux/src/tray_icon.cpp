#include "tray_icon.h"

TrayIcon::TrayIcon(QObject *parent) : QObject(parent) {

}

void TrayIcon::init() {
    system_tray_icon_.show();
    system_tray_icon_.setIcon(QIcon(":/ui/logo.ico"));
    connect(&system_tray_icon_, &QSystemTrayIcon::activated, this, &TrayIcon::dealWithTrayIconActivated);
    action_1_.setIcon(QIcon(":/ui/logo.ico"));
    action_1_.setText("打开ltalk");
    action_2_.setIcon(QIcon(":/ui/logo.ico"));
    action_2_.setText("退出登录");
    action_3_.setIcon(QIcon(":/ui/logo.ico"));
    action_3_.setText("退出");
    connect(&action_1_, &QAction::triggered, this, &TrayIcon::clickAction_1);
    menu_.addAction(&action_1_);
    menu_.addAction(&action_2_);
    menu_.addAction(&action_3_);
    //menu
    system_tray_icon_.setContextMenu(&menu_);

}

void TrayIcon::clickAction_1() {
    emit localCmd(LOCAL_CMD_SHOW_MAIN_PAGE);
}
void TrayIcon::clickAction_2() {
    emit localCmd(LOCAL_CMD_LOG_OUT);
}
void TrayIcon::clickAction_3() {
    emit localCmd(LOCAL_CMD_EXIT);
}


void TrayIcon::dealWithTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::Trigger: {
        emit localCmd(LOCAL_CMD_SHOW_MAIN_PAGE);
    } break;
    case QSystemTrayIcon::MiddleClick: {

    }break;
    default:
        break;
    }
}
