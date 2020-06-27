#ifndef PROFILE_PAGE_H
#define PROFILE_PAGE_H

#include <QWidget>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDir>

#include "ltalk.h"

namespace Ui {
class ProfilePage;
}

class ProfilePage : public QWidget
{
    Q_OBJECT
    enum class RequestStep {
        getHeadImage,
        getProfileImage_1,
        getProfileImage_2,
        getProfileImage_3,
        getProfileImage_4,
    };

public:
    explicit ProfilePage(QWidget *parent = nullptr);
    ~ProfilePage();
    void init();
    void setTheme(QString theme);
    void setUserInfo(const UserInfo &user_info);
    void setNextRequestStep();
private:
    Ui::ProfilePage *ui;
    bool pressed_;
    QPoint mouse_pos_;
    QNetworkAccessManager *network_mannager_;
    void requestGetImage(RequestStep request_step, QString url);
    void requestReply(QNetworkReply *reply);
    RequestStep request_step_;
    UserInfo user_info_;
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
private slots:
    void on_toolButton_min_clicked();
    void on_toolButton_close_clicked();
};

#endif // PROFILE_PAGE_H
