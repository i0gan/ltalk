#ifndef USER_CHAT_PAGE_H
#define USER_CHAT_PAGE_H

#include <QWidget>

namespace Ui {
class UserChatPage;
}

class UserChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit UserChatPage(QWidget *parent = nullptr);
    ~UserChatPage();

private:
    Ui::UserChatPage *ui;
};

#endif // USER_CHAT_PAGE_H
