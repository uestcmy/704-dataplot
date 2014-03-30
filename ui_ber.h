/********************************************************************************
** Form generated from reading UI file 'ber.ui'
**
** Created: Wed Mar 19 21:39:43 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BER_H
#define UI_BER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BER
{
public:

    void setupUi(QWidget *BER)
    {
        if (BER->objectName().isEmpty())
            BER->setObjectName(QString::fromUtf8("BER"));
        BER->resize(400, 300);

        retranslateUi(BER);

        QMetaObject::connectSlotsByName(BER);
    } // setupUi

    void retranslateUi(QWidget *BER)
    {
        BER->setWindowTitle(QApplication::translate("BER", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BER: public Ui_BER {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BER_H
