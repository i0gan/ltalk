#ifndef ABOUT_PAGE_H
#define ABOUT_PAGE_H

#include <QWidget>
#include <QDesktopWidget>
#include <QIcon>
#include <QMouseEvent>

namespace Ui {
class AboutPage;

}

class AboutPage : public QWidget
{
    Q_OBJECT

public:
    explicit AboutPage(QWidget *parent = nullptr);
    ~AboutPage();
    void init();
    void setTheme(QString theme);

private slots:
    void on_toolButton_close_clicked();

private:
    Ui::AboutPage *ui;
    bool pressed_;
    QPoint mouse_pos_;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
};

#endif // ABOUT_PAGE_H
