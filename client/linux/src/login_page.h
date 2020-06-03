#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

#include <QWidget>
#include <QDebug>
#include <QDesktopWidget>

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
    QPoint pos;
private slots:
    void on_pushButton_login_clicked();

};

#endif // LOGIN_PAGE_H
