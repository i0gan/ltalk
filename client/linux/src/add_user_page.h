#ifndef ADD_USER_PAGE_H
#define ADD_USER_PAGE_H

#include <QWidget>

namespace Ui {
class AddUserPage;
}

class AddUserPage : public QWidget
{
    Q_OBJECT

public:
    explicit AddUserPage(QWidget *parent = nullptr);
    ~AddUserPage();

private:
    Ui::AddUserPage *ui;
};

#endif // ADD_USER_PAGE_H
