#ifndef CHANGE_THEME_PAGE_H
#define CHANGE_THEME_PAGE_H

#include <QWidget>
#include <QMouseEvent>
#include <QEvent>
#include <QDesktopWidget>
#include <QDebug>

namespace Ui {
class ChangeThemePage;
}

class ChangeThemePage : public QWidget
{
    Q_OBJECT

public:
    explicit ChangeThemePage(QWidget *parent = nullptr);
    ~ChangeThemePage();
    void init();
    void setTheme(QString theme);
signals:
    void changed(QString theme);

private:
    Ui::ChangeThemePage *ui;
    QPoint mouse_pos_;
    bool pressed_;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    bool eventFilter(QObject *object, QEvent *e) override;
private slots:

    void on_toolButton_close_clicked();
};

#endif // CHANGE_THEME_PAGE_H
