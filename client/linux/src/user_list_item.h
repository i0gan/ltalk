#ifndef USER_LIST_ITEM_H
#define USER_LIST_ITEM_H

#include <QWidget>
#include <QEvent>
#include <QDebug>
#include <QMouseEvent>

namespace Ui {
class UserListItem;
}

class UserListItem : public QWidget
{
    Q_OBJECT

public:
    explicit UserListItem(QWidget *parent = nullptr);
    ~UserListItem();
    void init();

private:
    Ui::UserListItem *ui;
protected:
    bool eventFilter(QObject *obj, QEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
};

#endif // USER_LIST_ITEM_H
