#include "user_chat_page.h"
#include "ui_user_chat_page.h"

UserChatPage::UserChatPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserChatPage)
{
    ui->setupUi(this);
}

UserChatPage::~UserChatPage()
{
    delete ui;
}

void UserChatPage::init() {
    setWindowTitle("Ltalk login");
    setWindowIcon(QIcon(":/ui/logo.ico"));
    ui->pushButton_send->setShortcut(Qt::Key_Return);
    QPoint pos;
    pos.setX((QApplication::desktop()->width() - width()) / 2);
    pos.setY((QApplication::desktop()->height() - height()) / 2);
    move(pos);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QItemSelectionModel model;
    ui->listWidget_chat->setSelectionMode(QAbstractItemView::NoSelection);
    //ui->listWidget_chat->setAttribute(Qt::WA_TranslucentBackground);
}

void UserChatPage::mousePressEvent(QMouseEvent *event) {
    pressed_ = true;
    mouse_pos_ = event->pos();
}

void UserChatPage::mouseReleaseEvent(QMouseEvent *) {
    pressed_ = false;
}

void UserChatPage::mouseMoveEvent(QMouseEvent *event) {
    if(pressed_) {
        move(event->globalPos() - mouse_pos_);
    }
}

void UserChatPage::on_toolButton_close_clicked() {
    close();
}

void UserChatPage::on_toolButton_min_clicked() {
    showMinimized();
}

void UserChatPage::on_pushButton_send_clicked()
{

    QString msg = ui->plainTextEdit_input->toPlainText();
    if(msg.isEmpty()) {
        return;
    }

    ChatMessage *m = new ChatMessage(ui->listWidget_chat->parentWidget());
    m->setText(msg, ChatMessage::myself);
    m->setHeadImage(QPixmap(":/ui/friends.png"));
    m->setFixedSize(ui->listWidget_chat->size());
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget_chat);
    QSize size = m->fontSize();
    item->setSizeHint(size);
    ui->listWidget_chat->setItemWidget(item, m);
    newMessage("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    ui->plainTextEdit_input->clear();
}

void UserChatPage::newMessage(QString message) {
    ChatMessage *m = new ChatMessage(ui->listWidget_chat->parentWidget());
    m->setText(message, ChatMessage::other_side);
    m->setHeadImage(QPixmap(":/ui/friends.png"));
    m->setFixedSize(ui->listWidget_chat->size());
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget_chat);
    QSize size = m->fontSize();
    item->setSizeHint(size);
    ui->listWidget_chat->setItemWidget(item, m);
}
