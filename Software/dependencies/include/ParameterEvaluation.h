#pragma once

#include <QtWidgets/QMainWindow>
#include <QtGui/QStandardItemModel>
#include <QTranslator>
#include "ui_ParameterEvaluation.h"
#include "FormSettings.h"
#include "MatchingFeatureEvaluation.h"
#include "StemCurveEvaluation.h"
#include "MatchingFeature.h"
#include "StemCurve.h"

class ParameterEvaluation : public QMainWindow
{
    Q_OBJECT

public:
    ParameterEvaluation(QWidget* parent = nullptr);
    ~ParameterEvaluation();


private:
    void InitForm();
    void InitTree();
    void InitLanguage();
    void InitDataMangementMenu();
    void InitDockWidget();
    //隐藏数据
    void HiddenData(QStandardItem* item);
    //在左侧数据管理窗口中选中数据并右键可以出来删除数据的选项
    void handleContextMenuRequested(const QPoint& pos);
    //右键出来删除数据选项后删除数据
    void DeleteData();
    //转化到中文
    void changeLanguage_Chinese();
    //转化到英文
    void changeLanguage_English();
    //显示文件管理窗口
    void showDBTree();
    //显示文件属性窗口
    void showProperties();
    //显示文件输出窗口
    void showOutput();
    //是否显示文件管理
    void visualDBTree();
    //是否显示文件属性
    void visualProperties();
    //是否显示文件输出
    void visualOutput();

    //树木提取参数评价
    void matchingFeatureEvaluation();
    //干曲线参数评价
    void stemCurveEvaluation();
    //显示树木评价的输出参数的表格（在右上方显示）
    void showTreeInputInformation(std::vector<std::pair<size_t, size_t>> matchingIndex, std::vector<size_t> unMatchingIndexOfExt, std::vector<size_t> unMatchingIndexOfRef);

private:
    Ui::ParameterEvaluationClass ui;
    std::shared_ptr<FormSettings> _formSettings;
    std::shared_ptr<QStandardItemModel> _rootNode;
    //osg点云和窗口左侧数据管理树之间的对应关系
    std::map<QStandardItem*, osg::ref_ptr<osg::Group>> _itemToPointCloud;

    //在左侧数据管理窗口中选中数据并右键出来的菜单
    std::shared_ptr<QMenu> _dataMangementMenu;

    //树木评价函数
    std::shared_ptr<MatchingFeature> _matchingFeature;
    std::shared_ptr<StemCurve> _stemCurve;

    //语言控制
    QTranslator chinese;
    QTranslator english;

    bool output_English = true;

    //DockWidget表格
    QDockWidget* _dataShowOfExt;
    QWidget* tmpOfExt;
    QVBoxLayout* verticalLayoutOfExt;
    QTableWidget* _resultOfExt;

    QDockWidget* _dataShowOfRef;
    QWidget* tmpOfRef;
    QVBoxLayout* verticalLayoutOfRef;
    QTableWidget* _resultOfRef;

    QDockWidget* _dataShowOfUExt;
    QWidget* tmpOfUExt;
    QVBoxLayout* verticalLayoutOfUExt;
    QTableWidget* _resultOfUExt;

    QDockWidget* _dataShowOfURef;
    QWidget* tmpOfURef;
    QVBoxLayout* verticalLayoutOfURef;
    QTableWidget* _resultOfURef;
};