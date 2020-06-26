#ifndef FLOATING_SETTINGS_WINDOW_H
#define FLOATING_SETTINGS_WINDOW_H

#include <QWidget>
#include "ltalk.h"

namespace Ui {
class FloatingSettingsWindow;
}

class FloatingSettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FloatingSettingsWindow(QWidget *parent = nullptr);
    ~FloatingSettingsWindow();
    void init();
    void setParentPos(QPoint pos);
signals:
    void localCmd(LocalCmd cmd);
private:
    Ui::FloatingSettingsWindow *ui;

protected:
    bool event(QEvent *event);
private slots:
    void on_pushButton_exit_clicked();
    void on_pushButton_changeTheme_clicked();
};

#endif // FLOATING_SETTINGS_WINDOW_H
