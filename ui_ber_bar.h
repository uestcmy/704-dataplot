/********************************************************************************
** Form generated from reading UI file 'ber_bar.ui'
**
** Created: Sat Mar 22 15:35:50 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BER_BAR_H
#define UI_BER_BAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BER_BAR
{
public:

    void setupUi(QWidget *BER_BAR)
    {
        if (BER_BAR->objectName().isEmpty())
            BER_BAR->setObjectName(QString::fromUtf8("BER_BAR"));
        BER_BAR->resize(400, 300);

        retranslateUi(BER_BAR);

        QMetaObject::connectSlotsByName(BER_BAR);
    } // setupUi

    void retranslateUi(QWidget *BER_BAR)
    {
        BER_BAR->setWindowTitle(QApplication::translate("BER_BAR", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BER_BAR: public Ui_BER_BAR {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BER_BAR_H
