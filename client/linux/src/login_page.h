#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

#include <QWidget>
#include <QDebug>
#include <QDesktopWidget>
#include <QMouseEvent>
#include "ltalk.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlDatabase>

namespace Ui {
class LoginPage;

}

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();
    void init();
    void dealWithFailed(int code);
    void dealWithSuccess();
    void dealWithRecv(QJsonObject &json_obj);
signals:
    void login(QString account, QString password);
    void logined(QString account, QString uid, QString token);
private:
    Ui::LoginPage *ui;
    QPoint mouse_pos_;
    bool pressed_;
    QString account_;

private slots:
    void on_pushButton_login_clicked();
    void on_toolButton_close_clicked();
    void on_toolButton_min_clicked();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
};

#endif // LOGIN_PAGE_H
