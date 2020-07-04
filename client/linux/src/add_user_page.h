#ifndef ADD_USER_PAGE_H
#define ADD_USER_PAGE_H

#include <QWidget>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>


#include "ltalk.h"
#include "util.h"
namespace Ui {
class AddUserPage;
}

class AddUserPage : public QWidget
{
    Q_OBJECT

public:
    explicit AddUserPage(QWidget *parent = nullptr);
    ~AddUserPage();
    void init();

private:
    Ui::AddUserPage *ui;
    bool pressed_;
    QPoint mouse_pos_;
    QNetworkRequest request_;
    QNetworkAccessManager *network_access_mannager;
    UserInfo searched_user_info_;
    void dealWithSearchReply(const QJsonObject &json_obj);
    void dealWithNotExist();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
private slots:
    void on_toolButton_close_clicked();
    void on_pushButton_search_clicked();
    void dealWithRecv(QNetworkReply *reply);

};

#endif // ADD_USER_PAGE_H
