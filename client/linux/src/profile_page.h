#ifndef PROFILE_PAGE_H
#define PROFILE_PAGE_H

#include <QWidget>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDir>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>


#include "ltalk.h"
#include "image_cropper_page.h"
#include "util.h"

namespace Ui {
class ProfilePage;
}

class ProfilePage : public QWidget
{
    Q_OBJECT
    enum class ImageType {
        none = 0,
        headImage,
        profileImage_1,
        profileImage_2,
        profileImage_3,
        profileImage_4,
    };

public:
    explicit ProfilePage(QWidget *parent = nullptr);
    ~ProfilePage();
    void init();
    void setTheme(QString theme);
    void setUserInfo(const UserInfo &user_info);
    void setNextRequestStep();
signals:
    void localCmd(LocalCmd cmd);
private:
    Ui::ProfilePage *ui;
    bool pressed_;
    QPoint mouse_pos_;
    QNetworkAccessManager *network_mannager_;
    QNetworkRequest request_;
    QNetworkReply *reply_;
    void requestGetImage(ImageType request_step);
    void requestReply(QNetworkReply *reply);
    ImageType request_step_;
    UserInfo user_info_;
    ImageCropperPage *image_cropper_page_;
    bool is_modifying_image_;
    QString temp_image_path_;
    void modifyImage(ImageType image);
    void uploadImage(ImageType image, QString saved_file_name);
    ImageType crop_image_type_;
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void dealWithServerResponse(const QJsonObject &json_obj);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    bool eventFilter(QObject *object, QEvent *e) override;
private slots:
    void on_toolButton_min_clicked();
    void on_toolButton_close_clicked();
    void dealWithCroped(QString saved_file_name);
    void on_pushButton_saveModifiedInfo_clicked();
};

#endif // PROFILE_PAGE_H
