#ifndef USER_CHAT_PAGE_H
#define USER_CHAT_PAGE_H

#include <QWidget>
#include <QDesktopWidget>
#include <QDebug>
#include <QMouseEvent>

#include "chat_message.h"

namespace Ui {
class UserChatPage;
}

class UserChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit UserChatPage(QWidget *parent = nullptr);
    ~UserChatPage();
    void init();
    void newMessage(QString message);
private:
    Ui::UserChatPage *ui;
    bool pressed_;
    QPoint mouse_pos_;
    QPixmap my_head_image_;
    QPixmap otherside_head_image_;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
private slots:
    void on_toolButton_close_clicked();
    void on_toolButton_min_clicked();
    void on_pushButton_send_clicked();
};

#endif // USER_CHAT_PAGE_H
