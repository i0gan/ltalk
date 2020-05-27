#include <QCoreApplication>
#include <QApplication>
#include "widget/login/login.h"
#include <cstdlib>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login login;
    login.show();

    return a.exec();
}
