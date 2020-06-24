#ifndef USER_LIST_ITEM_H
#define USER_LIST_ITEM_H

#include <QWidget>

namespace Ui {
class UserListItem;
}

class UserListItem : public QWidget
{
    Q_OBJECT

public:
    explicit UserListItem(QWidget *parent = nullptr);
    ~UserListItem();

private:
    Ui::UserListItem *ui;
};

#endif // USER_LIST_ITEM_H
