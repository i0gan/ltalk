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
#include <QEvent>
#include <QUrl>

#include "floating_settings_window.h"
#include "tray_icon.h"
#include "ltalk.h"
#include "util.h"
#include "user_list_item.h"

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT
    enum class GetWhat{
        none,
        myHeadImage,
        friendHeadImage,
        groupHeadImage,
    };
public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();
    void init();
    void setUserInfo(const UserInfo &user_info);
    void setTheme(QString theme);
    void addUserListItem(const UserInfo &user_info);
    void addGroupListItem();
    void addMessageListItem();
signals:
    void localCmd(LocalCmd cmd);

private:
    Ui::MainPage *ui;
    QString account_;
    bool pressed_;
    QPoint mouse_pos_;

    FloatingSettingsWindow *floating_settings_window_;
    TrayIcon *tray_icon_;
    QNetworkAccessManager *net_mannager_;
    QNetworkRequest request_;
    GetWhat get_what_;
    QString save_file_path_;
    void requestGetFile(QString url, GetWhat what, QString save_file_path,  bool private_file = false);
    void requestGetFileReply(QNetworkReply *reply);
    UserInfo user_info_;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    bool eventFilter(QObject *object, QEvent *e) override;
private slots:
    void on_toolButton_min_clicked();
    void on_toolButton_close_clicked();
    void on_pushButton_groups_clicked();
    void on_pushButton_friends_clicked();
    void on_pushButton_messages_clicked();
    void on_pushButton_settings_clicked();
    void dealWithLocalCmd(LocalCmd cmd);
    void on_pushButton_changeTheme_clicked();
    void on_pushButton_addUser_clicked();
};

#endif // MAIN_PAGE_H
