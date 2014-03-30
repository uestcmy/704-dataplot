/********************************************************************************
** Form generated from reading UI file 'qpsk.ui'
**
** Created: Wed Mar 19 21:39:43 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QPSK_H
#define UI_QPSK_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QPSK
{
public:

    void setupUi(QWidget *QPSK)
    {
        if (QPSK->objectName().isEmpty())
            QPSK->setObjectName(QString::fromUtf8("QPSK"));
        QPSK->resize(400, 300);

        retranslateUi(QPSK);

        QMetaObject::connectSlotsByName(QPSK);
    } // setupUi

    void retranslateUi(QWidget *QPSK)
    {
        QPSK->setWindowTitle(QApplication::translate("QPSK", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QPSK: public Ui_QPSK {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QPSK_H
