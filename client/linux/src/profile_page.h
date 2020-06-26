#ifndef PROFILE_PAGE_H
#define PROFILE_PAGE_H

#include <QWidget>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace Ui {
class ProfilePage;
}

class ProfilePage : public QWidget
{
    Q_OBJECT

public:
    explicit ProfilePage(QWidget *parent = nullptr);
    ~ProfilePage();
    void init();
    void setTheme(QString theme);

private:
    Ui::ProfilePage *ui;
    bool pressed_;
    QPoint mouse_pos_;
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
private slots:
    void on_toolButton_min_clicked();
    void on_toolButton_close_clicked();
};

#endif // PROFILE_PAGE_H
