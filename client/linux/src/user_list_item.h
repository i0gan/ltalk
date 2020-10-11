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
    void leftClick(QString uid);
    void doubleClick(QString uid);
public:
    explicit UserListItem(QString uid, QWidget *parent = nullptr);
    ~UserListItem();
    void init();
    void setImage(const QPixmap &pixmap);
    void setImageUrl(QString url);
    void setAccount(const QString &account);
    void setName(const QString &name);
    void setNetwork(const QString &network);
    QString getImageUrl() { return image_url_; }


private:
    Ui::UserListItem *ui;
    QString image_url_;
    QString uid_;
protected:
    bool eventFilter(QObject *obj, QEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override {
        if(event->button() == Qt::LeftButton)
            emit leftClick(uid_);
    }
};

#endif // USER_LIST_ITEM_H
