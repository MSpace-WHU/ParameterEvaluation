/********************************************************************************
** Form generated from reading UI file 'StemCurveEvaluation.ui'
**
** Created by: Qt User Interface Compiler version 6.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STEMCURVEEVALUATION_H
#define UI_STEMCURVEEVALUATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_StemCurveEvaluation
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *_inputFileOfPredictedTree;
    QPushButton *_selectInputFileOfPredictedTree;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *_inputFileOfReferenceTree;
    QPushButton *_selectedInputFileOfReferenceTree;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QLineEdit *_inputFileOfIndex;
    QPushButton *_selectedInputFileOfIndex;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_5;
    QLineEdit *_radius;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_8;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *_outputFile;
    QPushButton *_selectedOututFile;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer;
    QPushButton *_btn_OK;

    void setupUi(QDialog *StemCurveEvaluation)
    {
        if (StemCurveEvaluation->objectName().isEmpty())
            StemCurveEvaluation->setObjectName(QString::fromUtf8("StemCurveEvaluation"));
        StemCurveEvaluation->resize(352, 343);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(StemCurveEvaluation->sizePolicy().hasHeightForWidth());
        StemCurveEvaluation->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(11);
        StemCurveEvaluation->setFont(font);
        verticalLayout = new QVBoxLayout(StemCurveEvaluation);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_2 = new QGroupBox(StemCurveEvaluation);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        _inputFileOfPredictedTree = new QLineEdit(groupBox_2);
        _inputFileOfPredictedTree->setObjectName(QString::fromUtf8("_inputFileOfPredictedTree"));

        horizontalLayout->addWidget(_inputFileOfPredictedTree);

        _selectInputFileOfPredictedTree = new QPushButton(groupBox_2);
        _selectInputFileOfPredictedTree->setObjectName(QString::fromUtf8("_selectInputFileOfPredictedTree"));

        horizontalLayout->addWidget(_selectInputFileOfPredictedTree);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        _inputFileOfReferenceTree = new QLineEdit(groupBox_2);
        _inputFileOfReferenceTree->setObjectName(QString::fromUtf8("_inputFileOfReferenceTree"));

        horizontalLayout_3->addWidget(_inputFileOfReferenceTree);

        _selectedInputFileOfReferenceTree = new QPushButton(groupBox_2);
        _selectedInputFileOfReferenceTree->setObjectName(QString::fromUtf8("_selectedInputFileOfReferenceTree"));

        horizontalLayout_3->addWidget(_selectedInputFileOfReferenceTree);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_6->addWidget(label_6);

        _inputFileOfIndex = new QLineEdit(groupBox_2);
        _inputFileOfIndex->setObjectName(QString::fromUtf8("_inputFileOfIndex"));

        horizontalLayout_6->addWidget(_inputFileOfIndex);

        _selectedInputFileOfIndex = new QPushButton(groupBox_2);
        _selectedInputFileOfIndex->setObjectName(QString::fromUtf8("_selectedInputFileOfIndex"));

        horizontalLayout_6->addWidget(_selectedInputFileOfIndex);


        verticalLayout_2->addLayout(horizontalLayout_6);


        verticalLayout->addWidget(groupBox_2);

        groupBox = new QGroupBox(StemCurveEvaluation);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_9->addWidget(label_5);

        _radius = new QLineEdit(groupBox);
        _radius->setObjectName(QString::fromUtf8("_radius"));

        horizontalLayout_9->addWidget(_radius);


        horizontalLayout_2->addLayout(horizontalLayout_9);

        horizontalSpacer_3 = new QSpacerItem(800, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout->addWidget(groupBox);

        groupBox_3 = new QGroupBox(StemCurveEvaluation);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        horizontalLayout_8 = new QHBoxLayout(groupBox_3);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        _outputFile = new QLineEdit(groupBox_3);
        _outputFile->setObjectName(QString::fromUtf8("_outputFile"));

        horizontalLayout_4->addWidget(_outputFile);

        _selectedOututFile = new QPushButton(groupBox_3);
        _selectedOututFile->setObjectName(QString::fromUtf8("_selectedOututFile"));

        horizontalLayout_4->addWidget(_selectedOututFile);


        horizontalLayout_8->addLayout(horizontalLayout_4);


        verticalLayout->addWidget(groupBox_3);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalSpacer = new QSpacerItem(300, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        _btn_OK = new QPushButton(StemCurveEvaluation);
        _btn_OK->setObjectName(QString::fromUtf8("_btn_OK"));

        horizontalLayout_7->addWidget(_btn_OK);


        verticalLayout->addLayout(horizontalLayout_7);


        retranslateUi(StemCurveEvaluation);

        QMetaObject::connectSlotsByName(StemCurveEvaluation);
    } // setupUi

    void retranslateUi(QDialog *StemCurveEvaluation)
    {
        StemCurveEvaluation->setWindowTitle(QCoreApplication::translate("StemCurveEvaluation", "\346\240\221\346\234\250\345\271\262\346\233\262\347\272\277\351\242\204\346\265\213\347\273\223\346\236\234\350\257\204\344\273\267", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("StemCurveEvaluation", "\346\226\207\344\273\266\350\276\223\345\205\245", nullptr));
        label_2->setText(QCoreApplication::translate("StemCurveEvaluation", "\351\242\204  \346\265\213  \346\226\207  \344\273\266\357\274\232", nullptr));
        _selectInputFileOfPredictedTree->setText(QCoreApplication::translate("StemCurveEvaluation", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        label_3->setText(QCoreApplication::translate("StemCurveEvaluation", "\345\217\202  \350\200\203  \346\226\207  \344\273\266\357\274\232", nullptr));
        _selectedInputFileOfReferenceTree->setText(QCoreApplication::translate("StemCurveEvaluation", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        label_6->setText(QCoreApplication::translate("StemCurveEvaluation", "\345\214\271\351\205\215\345\205\263\347\263\273\346\226\207\344\273\266\357\274\232", nullptr));
        _selectedInputFileOfIndex->setText(QCoreApplication::translate("StemCurveEvaluation", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        groupBox->setTitle(QCoreApplication::translate("StemCurveEvaluation", "\345\214\271\351\205\215\345\205\263\347\263\273\345\217\202\346\225\260\350\256\276\347\275\256", nullptr));
        label_5->setText(QCoreApplication::translate("StemCurveEvaluation", "\345\214\271\351\205\215\346\220\234\347\264\242\345\215\212\345\276\204(m):", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("StemCurveEvaluation", "\346\226\207\344\273\266\350\276\223\345\207\272", nullptr));
        label_4->setText(QCoreApplication::translate("StemCurveEvaluation", "\350\257\204\344\273\267\347\273\223\346\236\234\346\226\207\344\273\266\357\274\232", nullptr));
        _selectedOututFile->setText(QCoreApplication::translate("StemCurveEvaluation", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        _btn_OK->setText(QCoreApplication::translate("StemCurveEvaluation", "\345\256\214\346\210\220", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StemCurveEvaluation: public Ui_StemCurveEvaluation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STEMCURVEEVALUATION_H
