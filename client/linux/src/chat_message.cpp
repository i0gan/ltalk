#include "chat_message.h"

ChatMessage::ChatMessage(QWidget *parent) : QWidget(parent),
    sending_(false)
{
    QFont te_font = this->font();
    te_font.setPointSize(12);
    this->setFont(te_font);

    send_movie_= new QMovie(this);
    send_movie_->setFileName(":/loading.gif");
    send_stat_ = new QLabel(this);
    send_stat_->setMovie(send_movie_);
    send_stat_->resize(16, 16);
    send_stat_->setAttribute(Qt::WA_TranslucentBackground);
    send_stat_->setAutoFillBackground(false);
}

void ChatMessage::setText(const QString &text, ChatMessage::MessageType message_type) {
    message_type_ = message_type;
    text_ = text;
    if(message_type_ == MessageType::myself) {
        if(!sending_) {
            //send_stat_->move()
        }
    }
}

QSize ChatMessage::fontSize() {
    int min_height = 40;
    int head_image_wh = 50;          // 头像的宽高
    int width_of_head_image_to_edge = 30; // 头像与边缘的间隔
    int head_image_width = 10;        // 头像与消息框的间隔
    int head_image_tmp_height = 10;
    int shape_width = 10;  // 尖角的宽度
    int text_space_width = 10; // 内容离边框的宽度

    frame_width_ = this->width() - 2*(head_image_wh+width_of_head_image_to_edge+head_image_width);
    text_width_ = frame_width_ - 2 * text_space_width;
    space_width_ = this->width() - text_width_;
    head_image_left_ = QRect(width_of_head_image_to_edge, head_image_tmp_height, head_image_wh, head_image_wh);
    head_image_right_ = QRect(this->width() - width_of_head_image_to_edge - head_image_wh, head_image_tmp_height, head_image_wh, head_image_wh);

    QSize size = getSize(text_); // 整个的size
    qDebug() << "fontRect Size:" << size;
    int font_hight = size.height() < min_height ? min_height : size.height();

    sharp_left_ = QRect(head_image_wh+width_of_head_image_to_edge+head_image_width, line_height_/2, shape_width, font_hight - line_height_);
    sharp_right_ = QRect(this->width() - head_image_width - head_image_wh - width_of_head_image_to_edge - shape_width, line_height_/2, shape_width, font_hight - line_height_);

    if(size.width() < (text_width_+ space_width_)) {
        frame_left_.setRect(sharp_left_.x()+sharp_left_.width(), line_height_/4*3, size.width()-space_width_+2*text_space_width, font_hight-line_height_);
        frame_right_.setRect(this->width() - size.width() + space_width_ - 2*text_space_width - head_image_wh - width_of_head_image_to_edge - head_image_width - shape_width,
                                 line_height_/4*3, size.width()-space_width_+2*text_space_width, font_hight - line_height_);
    } else {
        frame_left_.setRect(sharp_left_.x()+sharp_left_.width(), line_height_/4*3, frame_width_, font_hight - line_height_);
        frame_right_.setRect(head_image_wh + width_of_head_image_to_edge + head_image_width - shape_width, line_height_/4*3, frame_width_, font_hight - line_height_);
    }

    text_left_.setRect(frame_left_.x()+text_space_width,frame_left_.y()+head_image_tmp_height,
                           frame_left_.width()-2*text_space_width,frame_left_.height()-2*head_image_tmp_height);
    text_right_.setRect(frame_right_.x()+text_space_width,frame_right_.y()+head_image_tmp_height,
                            frame_right_.width()-2*text_space_width,frame_right_.height()-2*head_image_tmp_height);
    return QSize(size.width(), font_hight);
}


QSize ChatMessage::getSize(QString str) {
    QFontMetricsF fm(this->font());
    line_height_ = fm.height();
    qDebug() << "line_height : " << line_height_;
    int line_count = str.count("\n");
    int max_width = 0;
    //只有以后一行
    if(line_count == 0) {
        max_width = fm.width(str);
        QString value = str;
        if(max_width > text_width_) {
            max_width = text_width_;
            int size = text_width_ / fm.width(" ");
            int num = fm.width(value) / text_width_;
            num = ( fm.width(value) ) / text_width_;
            line_count += num;
            QString temp = "";

            for(int i = 0; i < num; i++) {
                temp += value.mid(i*size, (i+1)*size) + "\n";
            }
            str.replace(value, temp);
        }

    } else {
        for(int i = 0; i < (line_count + 1); i++) {
            QString value = str.split("\n").at(i);
            max_width = fm.width(value) > max_width ? fm.width(value) : max_width;
            if(fm.width(value) > text_width_) {
                max_width = text_width_;
                int size = text_width_ / fm.width(" ");
                int num = fm.width(value) / text_width_;
                num = ((i+num)*fm.width(" ") + fm.width(value)) / text_width_;
                line_count += num;
                QString temp = "";
                for(int i = 0; i < num; i++) {
                    temp += value.mid(i*size, (i+1)*size) + "\n";
                }
                str.replace(value, temp);
            }
        }
    }

    QSize size = QSize(max_width + space_width_ + 5, (line_count+1)*line_height_ + 2*line_height_);
    //qDebug() << "size " << size ;
    return size;
}

void ChatMessage::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//消锯齿
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::gray));

    if(message_type_ == MessageType::other_side) { //对方
        //头像
        painter.drawPixmap(head_image_left_, head_image_);

        //框加边
        QColor col_KuangB(234, 234, 234);
        painter.setBrush(QBrush(col_KuangB));
        painter.drawRoundedRect(frame_left_.x()-1,frame_left_.y()-1,frame_left_.width()+2,frame_left_.height()+2,4,4);
        //框
        QColor frame_color(255,255,255);
        painter.setBrush(QBrush(frame_color));
        painter.drawRoundedRect(frame_left_,4,4);

        //三角
        QPointF points[3] = {
            QPointF(sharp_left_.x(), 30),
            QPointF(sharp_left_.x()+sharp_left_.width(), 25),
            QPointF(sharp_left_.x()+sharp_left_.width(), 35),
        };
        QPen pen;
        pen.setColor(frame_color);
        painter.setPen(pen);
        painter.drawPolygon(points, 3);

        //三角加边
        QPen penSanJiaoBian;
        penSanJiaoBian.setColor(col_KuangB);
        painter.setPen(penSanJiaoBian);
        painter.drawLine(QPointF(sharp_left_.x() - 1, 30), QPointF(sharp_left_.x()+sharp_left_.width(), 24));
        painter.drawLine(QPointF(sharp_left_.x() - 1, 30), QPointF(sharp_left_.x()+sharp_left_.width(), 36));

        //内容
        QPen penText;
        penText.setColor(QColor(51,51,51));
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        painter.setFont(this->font());
        painter.drawText(text_left_, text_, option);
    }  else if(message_type_ == MessageType::myself) { // 自己
        //头像
//        painter.drawRoundedRect(m_iconRightRect,m_iconRightRect.width(),m_iconRightRect.height());
        painter.drawPixmap(head_image_right_, head_image_);

        //框
        QColor frame_color(75,164,242);
        painter.setBrush(QBrush(frame_color));
        painter.drawRoundedRect(frame_right_,4,4);

        //三角
        QPointF points[3] = {
            QPointF(sharp_right_.x()+sharp_right_.width(), 30),
            QPointF(sharp_right_.x(), 25),
            QPointF(sharp_right_.x(), 35),
        };
        QPen pen;
        pen.setColor(frame_color);
        painter.setPen(pen);
        painter.drawPolygon(points, 3);

        //内容
        QPen penText;
        penText.setColor(Qt::white);
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        painter.setFont(this->font());
        painter.drawText(text_right_, text_, option);
    }  else if(message_type_ == MessageType::time) { // 时间
        QPen penText;
        penText.setColor(QColor(153,153,153));
        painter.setPen(penText);
        QTextOption option(Qt::AlignCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        QFont te_font = this->font();
        //te_font.setFamily("MicrosoftYaHei");
        te_font.setPointSize(10);
        painter.setFont(te_font);
        painter.drawText(this->rect(), time_, option);
    }
}

void ChatMessage::setHeadImage(const QPixmap &head_image) {
    head_image_ = head_image;
}

void ChatMessage::playSending() {

}
void ChatMessage::stopSending() {
    send_stat_->hide();
    send_movie_->stop();
}
