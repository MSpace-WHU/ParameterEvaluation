/********************************************************************************
** Form generated from reading UI file 'ParameterEvaluation.ui'
**
** Created by: Qt User Interface Compiler version 6.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAMETEREVALUATION_H
#define UI_PARAMETEREVALUATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ParameterEvaluationClass
{
public:
    QAction *action_MatchingFeature;
    QAction *action_StemCurve;
    QAction *action_Chinese;
    QAction *action_English;
    QAction *action_ShowDBTree;
    QAction *action_ShowProperty;
    QAction *action_ShowOutput;
    QWidget *visualWidget;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *visualArea;
    QMenuBar *menuBar;
    QMenu *ParameterEvaluation;
    QMenu *Language;
    QMenu *display;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *nodeTree;
    QWidget *nodeMangment;
    QHBoxLayout *horizontalLayout;
    QTreeView *fileTree;
    QDockWidget *property;
    QWidget *propertyMangment;
    QHBoxLayout *horizontalLayout_2;
    QTableView *propertyInformation;
    QDockWidget *output;
    QWidget *outputInformation;
    QVBoxLayout *verticalLayout;
    QTextEdit *logger;

    void setupUi(QMainWindow *ParameterEvaluationClass)
    {
        if (ParameterEvaluationClass->objectName().isEmpty())
            ParameterEvaluationClass->setObjectName(QString::fromUtf8("ParameterEvaluationClass"));
        ParameterEvaluationClass->resize(847, 607);
        QFont font;
        font.setPointSize(11);
        ParameterEvaluationClass->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ParameterEvaluation/1.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ParameterEvaluationClass->setWindowIcon(icon);
        action_MatchingFeature = new QAction(ParameterEvaluationClass);
        action_MatchingFeature->setObjectName(QString::fromUtf8("action_MatchingFeature"));
        action_MatchingFeature->setFont(font);
        action_StemCurve = new QAction(ParameterEvaluationClass);
        action_StemCurve->setObjectName(QString::fromUtf8("action_StemCurve"));
        action_StemCurve->setFont(font);
        action_Chinese = new QAction(ParameterEvaluationClass);
        action_Chinese->setObjectName(QString::fromUtf8("action_Chinese"));
        action_Chinese->setFont(font);
        action_English = new QAction(ParameterEvaluationClass);
        action_English->setObjectName(QString::fromUtf8("action_English"));
        action_English->setFont(font);
        action_ShowDBTree = new QAction(ParameterEvaluationClass);
        action_ShowDBTree->setObjectName(QString::fromUtf8("action_ShowDBTree"));
        action_ShowDBTree->setCheckable(true);
        action_ShowDBTree->setChecked(true);
        action_ShowDBTree->setFont(font);
        action_ShowProperty = new QAction(ParameterEvaluationClass);
        action_ShowProperty->setObjectName(QString::fromUtf8("action_ShowProperty"));
        action_ShowProperty->setCheckable(true);
        action_ShowProperty->setChecked(true);
        action_ShowProperty->setFont(font);
        action_ShowOutput = new QAction(ParameterEvaluationClass);
        action_ShowOutput->setObjectName(QString::fromUtf8("action_ShowOutput"));
        action_ShowOutput->setCheckable(true);
        action_ShowOutput->setChecked(true);
        action_ShowOutput->setFont(font);
        visualWidget = new QWidget(ParameterEvaluationClass);
        visualWidget->setObjectName(QString::fromUtf8("visualWidget"));
        verticalLayout_3 = new QVBoxLayout(visualWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        visualArea = new QVBoxLayout();
        visualArea->setSpacing(6);
        visualArea->setObjectName(QString::fromUtf8("visualArea"));

        verticalLayout_3->addLayout(visualArea);

        ParameterEvaluationClass->setCentralWidget(visualWidget);
        menuBar = new QMenuBar(ParameterEvaluationClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 847, 26));
        ParameterEvaluation = new QMenu(menuBar);
        ParameterEvaluation->setObjectName(QString::fromUtf8("ParameterEvaluation"));
        ParameterEvaluation->setFont(font);
        Language = new QMenu(menuBar);
        Language->setObjectName(QString::fromUtf8("Language"));
        Language->setFont(font);
        display = new QMenu(menuBar);
        display->setObjectName(QString::fromUtf8("display"));
        ParameterEvaluationClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ParameterEvaluationClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ParameterEvaluationClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ParameterEvaluationClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ParameterEvaluationClass->setStatusBar(statusBar);
        nodeTree = new QDockWidget(ParameterEvaluationClass);
        nodeTree->setObjectName(QString::fromUtf8("nodeTree"));
        nodeTree->setFont(font);
        nodeMangment = new QWidget();
        nodeMangment->setObjectName(QString::fromUtf8("nodeMangment"));
        horizontalLayout = new QHBoxLayout(nodeMangment);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        fileTree = new QTreeView(nodeMangment);
        fileTree->setObjectName(QString::fromUtf8("fileTree"));

        horizontalLayout->addWidget(fileTree);

        nodeTree->setWidget(nodeMangment);
        ParameterEvaluationClass->addDockWidget(Qt::LeftDockWidgetArea, nodeTree);
        property = new QDockWidget(ParameterEvaluationClass);
        property->setObjectName(QString::fromUtf8("property"));
        property->setFont(font);
        property->setFloating(false);
        propertyMangment = new QWidget();
        propertyMangment->setObjectName(QString::fromUtf8("propertyMangment"));
        horizontalLayout_2 = new QHBoxLayout(propertyMangment);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        propertyInformation = new QTableView(propertyMangment);
        propertyInformation->setObjectName(QString::fromUtf8("propertyInformation"));

        horizontalLayout_2->addWidget(propertyInformation);

        property->setWidget(propertyMangment);
        ParameterEvaluationClass->addDockWidget(Qt::LeftDockWidgetArea, property);
        output = new QDockWidget(ParameterEvaluationClass);
        output->setObjectName(QString::fromUtf8("output"));
        output->setFont(font);
        outputInformation = new QWidget();
        outputInformation->setObjectName(QString::fromUtf8("outputInformation"));
        verticalLayout = new QVBoxLayout(outputInformation);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        logger = new QTextEdit(outputInformation);
        logger->setObjectName(QString::fromUtf8("logger"));

        verticalLayout->addWidget(logger);

        output->setWidget(outputInformation);
        ParameterEvaluationClass->addDockWidget(Qt::BottomDockWidgetArea, output);

        menuBar->addAction(ParameterEvaluation->menuAction());
        menuBar->addAction(display->menuAction());
        menuBar->addAction(Language->menuAction());
        ParameterEvaluation->addAction(action_MatchingFeature);
        ParameterEvaluation->addAction(action_StemCurve);
        Language->addAction(action_Chinese);
        Language->addAction(action_English);
        display->addAction(action_ShowDBTree);
        display->addAction(action_ShowProperty);
        display->addAction(action_ShowOutput);

        retranslateUi(ParameterEvaluationClass);

        QMetaObject::connectSlotsByName(ParameterEvaluationClass);
    } // setupUi

    void retranslateUi(QMainWindow *ParameterEvaluationClass)
    {
        ParameterEvaluationClass->setWindowTitle(QCoreApplication::translate("ParameterEvaluationClass", "ParameterEvaluation", nullptr));
        action_MatchingFeature->setText(QCoreApplication::translate("ParameterEvaluationClass", "\346\240\221\346\234\250\345\217\202\346\225\260\351\242\204\346\265\213\347\273\223\346\236\234\350\257\204\344\273\267", nullptr));
        action_StemCurve->setText(QCoreApplication::translate("ParameterEvaluationClass", "\345\271\262\346\233\262\347\272\277\351\242\204\346\265\213\347\273\223\346\236\234\350\257\204\344\273\267", nullptr));
        action_Chinese->setText(QCoreApplication::translate("ParameterEvaluationClass", "\344\270\255\346\226\207", nullptr));
        action_English->setText(QCoreApplication::translate("ParameterEvaluationClass", "English", nullptr));
        action_ShowDBTree->setText(QCoreApplication::translate("ParameterEvaluationClass", "\346\226\207\344\273\266\347\256\241\347\220\206", nullptr));
        action_ShowProperty->setText(QCoreApplication::translate("ParameterEvaluationClass", "\346\226\207\344\273\266\345\261\236\346\200\247", nullptr));
        action_ShowOutput->setText(QCoreApplication::translate("ParameterEvaluationClass", "\347\250\213\345\272\217\350\276\223\345\207\272", nullptr));
        ParameterEvaluation->setTitle(QCoreApplication::translate("ParameterEvaluationClass", "\345\217\202\346\225\260\350\257\204\344\273\267", nullptr));
        Language->setTitle(QCoreApplication::translate("ParameterEvaluationClass", "\350\257\255\350\250\200", nullptr));
        display->setTitle(QCoreApplication::translate("ParameterEvaluationClass", "\346\230\276\347\244\272", nullptr));
        nodeTree->setWindowTitle(QCoreApplication::translate("ParameterEvaluationClass", "\346\226\207\344\273\266\347\256\241\347\220\206", nullptr));
        property->setWindowTitle(QCoreApplication::translate("ParameterEvaluationClass", "\346\226\207\344\273\266\345\261\236\346\200\247(\346\234\252\345\274\200\345\217\221)", nullptr));
        output->setWindowTitle(QCoreApplication::translate("ParameterEvaluationClass", "\347\250\213\345\272\217\350\276\223\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ParameterEvaluationClass: public Ui_ParameterEvaluationClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMETEREVALUATION_H
