/********************************************************************************
** Form generated from reading UI file 'MatchingFeatureEvaluation.ui'
**
** Created by: Qt User Interface Compiler version 6.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATCHINGFEATUREEVALUATION_H
#define UI_MATCHINGFEATUREEVALUATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MatchingFeatureEvaluation
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *_inputFileOfPredictedTree;
    QPushButton *_selectInputFileOfPredictedTree;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QLineEdit *_inputFileOfReferenceTree;
    QPushButton *_selectedInputFileOfReferenceTree;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_9;
    QLineEdit *_inputFileOfIndex;
    QPushButton *_selectedInputFileOfIndex;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_8;
    QLineEdit *_columnOfMF;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QLineEdit *_dimension;
    QHBoxLayout *horizontalLayout_13;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_6;
    QLineEdit *_radius;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_8;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QLineEdit *_outputFile;
    QPushButton *_selectOutputFile;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *_isShowResult;
    QSpacerItem *horizontalSpacer;
    QPushButton *_btn_OK;

    void setupUi(QDialog *MatchingFeatureEvaluation)
    {
        if (MatchingFeatureEvaluation->objectName().isEmpty())
            MatchingFeatureEvaluation->setObjectName(QString::fromUtf8("MatchingFeatureEvaluation"));
        MatchingFeatureEvaluation->resize(578, 345);
        QFont font;
        font.setPointSize(11);
        MatchingFeatureEvaluation->setFont(font);
        verticalLayout_2 = new QVBoxLayout(MatchingFeatureEvaluation);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(MatchingFeatureEvaluation);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_3->addWidget(label);

        _inputFileOfPredictedTree = new QLineEdit(groupBox);
        _inputFileOfPredictedTree->setObjectName(QString::fromUtf8("_inputFileOfPredictedTree"));

        horizontalLayout_3->addWidget(_inputFileOfPredictedTree);

        _selectInputFileOfPredictedTree = new QPushButton(groupBox);
        _selectInputFileOfPredictedTree->setObjectName(QString::fromUtf8("_selectInputFileOfPredictedTree"));

        horizontalLayout_3->addWidget(_selectInputFileOfPredictedTree);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_4->addWidget(label_2);

        _inputFileOfReferenceTree = new QLineEdit(groupBox);
        _inputFileOfReferenceTree->setObjectName(QString::fromUtf8("_inputFileOfReferenceTree"));

        horizontalLayout_4->addWidget(_inputFileOfReferenceTree);

        _selectedInputFileOfReferenceTree = new QPushButton(groupBox);
        _selectedInputFileOfReferenceTree->setObjectName(QString::fromUtf8("_selectedInputFileOfReferenceTree"));

        horizontalLayout_4->addWidget(_selectedInputFileOfReferenceTree);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_9->addWidget(label_9);

        _inputFileOfIndex = new QLineEdit(groupBox);
        _inputFileOfIndex->setObjectName(QString::fromUtf8("_inputFileOfIndex"));

        horizontalLayout_9->addWidget(_inputFileOfIndex);

        _selectedInputFileOfIndex = new QPushButton(groupBox);
        _selectedInputFileOfIndex->setObjectName(QString::fromUtf8("_selectedInputFileOfIndex"));

        horizontalLayout_9->addWidget(_selectedInputFileOfIndex);


        verticalLayout->addLayout(horizontalLayout_9);


        verticalLayout_3->addLayout(verticalLayout);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(MatchingFeatureEvaluation);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_7 = new QHBoxLayout(groupBox_2);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_12->addWidget(label_8);

        _columnOfMF = new QLineEdit(groupBox_2);
        _columnOfMF->setObjectName(QString::fromUtf8("_columnOfMF"));

        horizontalLayout_12->addWidget(_columnOfMF);


        horizontalLayout_5->addLayout(horizontalLayout_12);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout->addWidget(label_4);

        _dimension = new QLineEdit(groupBox_2);
        _dimension->setObjectName(QString::fromUtf8("_dimension"));

        horizontalLayout->addWidget(_dimension);


        horizontalLayout_5->addLayout(horizontalLayout);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_3);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_13->addWidget(label_6);

        _radius = new QLineEdit(groupBox_2);
        _radius->setObjectName(QString::fromUtf8("_radius"));

        horizontalLayout_13->addWidget(_radius);


        horizontalLayout_5->addLayout(horizontalLayout_13);


        horizontalLayout_7->addLayout(horizontalLayout_5);


        verticalLayout_2->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(MatchingFeatureEvaluation);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        horizontalLayout_8 = new QHBoxLayout(groupBox_3);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_6->addWidget(label_3);

        _outputFile = new QLineEdit(groupBox_3);
        _outputFile->setObjectName(QString::fromUtf8("_outputFile"));

        horizontalLayout_6->addWidget(_outputFile);

        _selectOutputFile = new QPushButton(groupBox_3);
        _selectOutputFile->setObjectName(QString::fromUtf8("_selectOutputFile"));

        horizontalLayout_6->addWidget(_selectOutputFile);


        horizontalLayout_8->addLayout(horizontalLayout_6);


        verticalLayout_2->addWidget(groupBox_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        _isShowResult = new QCheckBox(MatchingFeatureEvaluation);
        _isShowResult->setObjectName(QString::fromUtf8("_isShowResult"));

        horizontalLayout_2->addWidget(_isShowResult);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        _btn_OK = new QPushButton(MatchingFeatureEvaluation);
        _btn_OK->setObjectName(QString::fromUtf8("_btn_OK"));

        horizontalLayout_2->addWidget(_btn_OK);


        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(MatchingFeatureEvaluation);

        QMetaObject::connectSlotsByName(MatchingFeatureEvaluation);
    } // setupUi

    void retranslateUi(QDialog *MatchingFeatureEvaluation)
    {
        MatchingFeatureEvaluation->setWindowTitle(QCoreApplication::translate("MatchingFeatureEvaluation", "\346\240\221\346\234\250\345\217\202\346\225\260\351\242\204\346\265\213\347\273\223\346\236\234\350\257\204\344\273\267", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MatchingFeatureEvaluation", "\346\226\207\344\273\266\350\276\223\345\205\245", nullptr));
        label->setText(QCoreApplication::translate("MatchingFeatureEvaluation", "\351\242\204  \346\265\213  \346\226\207  \344\273\266\357\274\232", nullptr));
        _selectInputFileOfPredictedTree->setText(QCoreApplication::translate("MatchingFeatureEvaluation", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        label_2->setText(QCoreApplication::translate("MatchingFeatureEvaluation", "\345\217\202  \350\200\203  \346\226\207  \344\273\266\357\274\232", nullptr));
        _selectedInputFileOfReferenceTree->setText(QCoreApplication::translate("MatchingFeatureEvaluation", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        label_9->setText(QCoreApplication::translate("MatchingFeatureEvaluation", "\345\214\271\351\205\215\345\205\263\347\263\273\346\226\207\344\273\266\357\274\232", nullptr));
        _selectedInputFileOfIndex->setText(QCoreApplication::translate("MatchingFeatureEvaluation", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MatchingFeatureEvaluation", "\345\214\271\351\205\215\345\205\263\347\263\273\345\217\202\346\225\260\350\256\276\347\275\256", nullptr));
        label_8->setText(QCoreApplication::translate("MatchingFeatureEvaluation", "\345\214\271\351\205\215\347\211\271\345\276\201\345\210\227\345\217\267\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("MatchingFeatureEvaluation", "\345\214\271\351\205\215\345\235\220\346\240\207\347\273\264\345\272\246\357\274\232", nullptr));
        label_6->setText(QCoreApplication::translate("MatchingFeatureEvaluation", "\345\214\271\351\205\215\346\220\234\347\264\242\345\215\212\345\276\204(m)\357\274\232", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MatchingFeatureEvaluation", "\346\226\207\344\273\266\350\276\223\345\207\272", nullptr));
        label_3->setText(QCoreApplication::translate("MatchingFeatureEvaluation", "\350\257\204\344\273\267\347\273\223\346\236\234\346\226\207\344\273\266 \357\274\232", nullptr));
        _selectOutputFile->setText(QCoreApplication::translate("MatchingFeatureEvaluation", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        _isShowResult->setText(QCoreApplication::translate("MatchingFeatureEvaluation", "\346\230\257\345\220\246\346\230\276\347\244\272\345\214\271\351\205\215\347\273\223\346\236\234", nullptr));
        _btn_OK->setText(QCoreApplication::translate("MatchingFeatureEvaluation", "\345\256\214\346\210\220", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MatchingFeatureEvaluation: public Ui_MatchingFeatureEvaluation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATCHINGFEATUREEVALUATION_H
