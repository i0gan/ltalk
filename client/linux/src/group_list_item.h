#ifndef GROUP_LIST_ITEM_H
#define GROUP_LIST_ITEM_H

#include <QWidget>

namespace Ui {
class GroupListItem;
}

class GroupListItem : public QWidget
{
    Q_OBJECT

public:
    explicit GroupListItem(QWidget *parent = nullptr);
    ~GroupListItem();

private:
    Ui::GroupListItem *ui;
};

#endif // GROUP_LIST_ITEM_H
