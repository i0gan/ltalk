#ifndef IMAGE_CROPPER_PAGE_H
#define IMAGE_CROPPER_PAGE_H

#include <QWidget>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QIcon>
#include <QMessageBox>

#include "third/image_cropper_label.h"

namespace Ui {
class ImageCropperPage;
}

class ImageCropperPage : public QWidget
{
    Q_OBJECT

public:
    explicit ImageCropperPage(QWidget *parent = nullptr);
    ~ImageCropperPage();
    void init();
    bool crop(const QPixmap &pixmap, CropperShape shape, QSize size);
    void setSize(QPoint size);
    void setScale(int width, int hight);
    void setPixmap(const QPixmap &pixmap);
    void setPixmap(const QString &file_name);
    bool isOpen();
signals:
    void finshed(QString saved_file_name);
    void failed();
private slots:
    void on_pushButton_OK_clicked();
    void on_toolButton_close_clicked();

private:
    Ui::ImageCropperPage *ui;
    bool pressed_;
    QPoint mouse_pos_;
    QPixmap pixmap_;
    ImageCropperLabel *image_label_;
    bool opened_;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;

};

#endif // IMAGE_CROPPER_PAGE_H
