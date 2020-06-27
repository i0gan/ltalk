#ifndef CUT_IMAGE_H
#define CUT_IMAGE_H

#include <QWidget>
#include <QDesktopWidget>
#include <QMouseEvent>

namespace Ui {
class CutImage;
}

class CutImage : public QWidget
{
    Q_OBJECT

public:
    explicit CutImage(QWidget *parent = nullptr);
    ~CutImage();
    void init();

private slots:
    void on_toolButton_close_clicked();

private:
    Ui::CutImage *ui;
    bool pressed_;
    QPoint mouse_pos_;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
};

#endif // CUT_IMAGE_H
