#ifndef USER_LIST_ITEM_H
#define USER_LIST_ITEM_H

#include <QWidget>
#include <QEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QEvent>

namespace Ui {
class UserListItem;
}

class UserListItem : public QWidget
{
    Q_OBJECT
signals:
    void click(int num);
    void doubleClick(int num);
public:
    explicit UserListItem(int num, QWidget *parent = nullptr);
    ~UserListItem();
    void init();
    void setImage(const QPixmap &pixmap);
    void setImageUrl(QString url);
    QString getImageUrl() { return image_url_; }
private:
    Ui::UserListItem *ui;
    QString image_url_;
    int num_;
protected:
    bool eventFilter(QObject *obj, QEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override {
        if(event->button() == Qt::LeftButton)
            emit doubleClick(num_);
    }
};

#endif // USER_LIST_ITEM_H
