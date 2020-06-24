#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H

#include <QWidget>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QDebug>
#include <QMenu>

#include "floating_settings_window.h"
#include "tray_icon.h"
namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();
    void init();
signals:
    void localCmd(size_t cmd);

private:
    Ui::MainPage *ui;
    bool pressed_;
    QPoint mouse_pos_;
    FloatingSettingsWindow *floating_settings_window_;
    bool opened_floating_settings_window_;
    TrayIcon *tray_icon_;
    bool opened_tray_icon_;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
private slots:
    void on_toolButton_min_clicked();
    void on_toolButton_close_clicked();
    void on_pushButton_groups_clicked();
    void on_pushButton_friends_clicked();
    void on_pushButton_messages_clicked();
    void on_pushButton_settings_clicked();
    void dealWithLocalCmd(size_t cmd);
};

#endif // MAIN_PAGE_H
