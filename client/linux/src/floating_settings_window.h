#ifndef FLOATING_SETTINGS_WINDOW_H
#define FLOATING_SETTINGS_WINDOW_H

#include <QWidget>

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

private:
    Ui::FloatingSettingsWindow *ui;

protected:
    bool event(QEvent *event);
};

#endif // FLOATING_SETTINGS_WINDOW_H
