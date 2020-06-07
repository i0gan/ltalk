#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

#include <QWidget>
#include <QDebug>
#include <QDesktopWidget>
#include <QMouseEvent>
#include "ltalk.h"

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
signals:
    void login(QString account, QString password);
private:
    Ui::LoginPage *ui;
    QPoint mouse_pos_;
    bool pressed_;

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
