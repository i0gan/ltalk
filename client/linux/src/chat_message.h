#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H
#include <QWidget>
#include <QPaintEvent>
#include <QMovie>
#include <QLabel>
#include <QTextOption>
#include <QPen>
#include <QPainter>

class ChatMessage : public QWidget
{
    //Q_OBJECT
public:
    enum MessageType {
        system,
        time,
        myself,
        other_side,
    };

    explicit ChatMessage(QWidget *parent = nullptr);
    void setText(QString text, ChatMessage::MessageType message_type);
    void setHeadImage(QPixmap head_image);
    void playSending();
    void stopSending();
    QSize getSize(QString str);
    QSize fontSize();
protected:
    void paintEvent(QPaintEvent *event);

private:
    bool sending_;
    QString text_;
    QString time_;
    MessageType message_type_;
    QPixmap head_image_;

    QLabel *send_stat_;
    QMovie *send_movie_;

    int line_height_;
    int text_width_;
    int space_width_;
    int frame_width_;

    QRect head_image_left_;
    QRect head_image_right_;
    QRect sharp_left_;
    QRect sharp_right_;
    QRect frame_left_;
    QRect frame_right_;
    QRect text_left_;
    QRect text_right_;
};

#endif // CHATMESSAGE_H
