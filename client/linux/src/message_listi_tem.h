#ifndef MESSAGE_LISTI_TEM_H
#define MESSAGE_LISTI_TEM_H

#include <QWidget>

namespace Ui {
class MessageListItem;
}

class MessageListItem : public QWidget
{
    Q_OBJECT

public:
    explicit MessageListItem(QWidget *parent = nullptr);
    ~MessageListItem();

private:
    Ui::MessageListItem *ui;
};

#endif // MESSAGE_LISTI_TEM_H
