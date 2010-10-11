/********************************************************************************
** Form generated from reading UI file 'SMFTESTMUSICNACTIVITY.ui'
**
** Created: Mon Oct 11 12:12:49 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SMFTESTMUSICNACTIVITY_H
#define UI_SMFTESTMUSICNACTIVITY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SmfTestApp
{
public:

    void setupUi(QWidget *SmfTestApp)
    {
        if (SmfTestApp->objectName().isEmpty())
            SmfTestApp->setObjectName(QString::fromUtf8("SmfTestApp"));
        SmfTestApp->resize(400, 300);

        retranslateUi(SmfTestApp);

        QMetaObject::connectSlotsByName(SmfTestApp);
    } // setupUi

    void retranslateUi(QWidget *SmfTestApp)
    {
        SmfTestApp->setWindowTitle(QApplication::translate("SmfTestApp", "SmfTestApp", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SmfTestApp: public Ui_SmfTestApp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SMFTESTMUSICNACTIVITY_H
