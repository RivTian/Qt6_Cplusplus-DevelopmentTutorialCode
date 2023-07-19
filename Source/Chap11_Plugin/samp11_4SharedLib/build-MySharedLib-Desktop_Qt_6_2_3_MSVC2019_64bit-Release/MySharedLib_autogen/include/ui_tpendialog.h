/********************************************************************************
** Form generated from reading UI file 'tpendialog.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TPENDIALOG_H
#define UI_TPENDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TPenDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QSpinBox *spinWidth;
    QComboBox *comboPenStyle;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnColor;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_5;
    QPushButton *btnOK;
    QSpacerItem *verticalSpacer_4;
    QPushButton *btnCancel;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QDialog *TPenDialog)
    {
        if (TPenDialog->objectName().isEmpty())
            TPenDialog->setObjectName(QString::fromUtf8("TPenDialog"));
        TPenDialog->resize(328, 158);
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font.setPointSize(10);
        TPenDialog->setFont(font);
        horizontalLayout = new QHBoxLayout(TPenDialog);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(TPenDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(98, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 2, 1, 1);

        spinWidth = new QSpinBox(groupBox);
        spinWidth->setObjectName(QString::fromUtf8("spinWidth"));
        spinWidth->setMinimumSize(QSize(100, 0));
        spinWidth->setMinimum(1);
        spinWidth->setMaximum(100);

        gridLayout->addWidget(spinWidth, 1, 1, 1, 1);

        comboPenStyle = new QComboBox(groupBox);
        comboPenStyle->setObjectName(QString::fromUtf8("comboPenStyle"));

        gridLayout->addWidget(comboPenStyle, 0, 1, 1, 2);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(98, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 2, 1, 1);

        btnColor = new QPushButton(groupBox);
        btnColor->setObjectName(QString::fromUtf8("btnColor"));
        btnColor->setAutoFillBackground(true);
        btnColor->setFlat(false);

        gridLayout->addWidget(btnColor, 2, 1, 1, 1);


        horizontalLayout->addWidget(groupBox);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_5);

        btnOK = new QPushButton(TPenDialog);
        btnOK->setObjectName(QString::fromUtf8("btnOK"));

        verticalLayout->addWidget(btnOK);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        btnCancel = new QPushButton(TPenDialog);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        verticalLayout->addWidget(btnCancel);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(TPenDialog);
        QObject::connect(btnOK, &QPushButton::clicked, TPenDialog, qOverload<>(&QDialog::accept));
        QObject::connect(btnCancel, &QPushButton::clicked, TPenDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(TPenDialog);
    } // setupUi

    void retranslateUi(QDialog *TPenDialog)
    {
        TPenDialog->setWindowTitle(QCoreApplication::translate("TPenDialog", "QPen\345\261\236\346\200\247\350\256\276\347\275\256\345\257\271\350\257\235\346\241\206", nullptr));
        groupBox->setTitle(QCoreApplication::translate("TPenDialog", "QPen\345\261\236\346\200\247\350\256\276\347\275\256", nullptr));
        label_2->setText(QCoreApplication::translate("TPenDialog", "\347\272\277 \345\256\275", nullptr));
        label->setText(QCoreApplication::translate("TPenDialog", "\347\272\277 \345\236\213", nullptr));
        label_3->setText(QCoreApplication::translate("TPenDialog", "\351\242\234 \350\211\262", nullptr));
        btnColor->setText(QString());
        btnOK->setText(QCoreApplication::translate("TPenDialog", "\347\241\256 \345\256\232", nullptr));
        btnCancel->setText(QCoreApplication::translate("TPenDialog", "\345\217\226 \346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TPenDialog: public Ui_TPenDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TPENDIALOG_H
