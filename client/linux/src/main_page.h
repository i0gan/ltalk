#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H

#include <QWidget>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QDebug>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPicture>
#include <QFile>
#include <QDir>

#include "floating_settings_window.h"
#include "tray_icon.h"
#include "ltalk.h"
#include "util.h"

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();
    void init();
    void setUserInfo(const UserInfo &user_info);
    void setTheme(QString theme);
signals:
    void localCmd(LocalCmd cmd);

private:
    Ui::MainPage *ui;
    QString account_;
    bool pressed_;
    QPoint mouse_pos_;
    FloatingSettingsWindow *floating_settings_window_;
    bool opened_floating_settings_window_;
    TrayIcon *tray_icon_;
    bool opened_tray_icon_;
    void requestGetHeadImage(QString uid, QString token, QString head_image_url);
    void requestGetHeadImageReply(QNetworkReply *reply);
    QNetworkAccessManager *network_get_head_image;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
private slots:
    void on_toolButton_min_clicked();
    void on_toolButton_close_clicked();
    void on_pushButton_groups_clicked();
    void on_pushButton_friends_clicked();
    void on_pushButton_messages_clicked();
    void on_pushButton_settings_clicked();
    void dealWithLocalCmd(LocalCmd cmd);
};

#endif // MAIN_PAGE_H
