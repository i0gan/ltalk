/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <phonon/seekslider.h>
#include <phonon/videoplayer.h>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    Phonon::SeekSlider *seekSlider;
    Phonon::VideoPlayer *videoPlayer;

    void setupUi(QWidget *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName(QString::fromUtf8("login"));
        login->resize(600, 480);
        seekSlider = new Phonon::SeekSlider(login);
        seekSlider->setObjectName(QString::fromUtf8("seekSlider"));
        seekSlider->setGeometry(QRect(170, 280, 241, 81));
        videoPlayer = new Phonon::VideoPlayer(login);
        videoPlayer->setObjectName(QString::fromUtf8("videoPlayer"));
        videoPlayer->setGeometry(QRect(110, 40, 381, 241));

        retranslateUi(login);

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QWidget *login)
    {
        login->setWindowTitle(QCoreApplication::translate("login", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
