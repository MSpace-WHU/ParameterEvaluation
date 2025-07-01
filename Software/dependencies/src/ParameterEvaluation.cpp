#include "../include/ParameterEvaluation.h"
#include <QtWidgets/QProgressDialog>
#include <QtConcurrent/QtConcurrent>
#include <QProgressBar>
#include <QFileDialog>

ParameterEvaluation::ParameterEvaluation(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    InitForm();
    InitTree();
    InitLanguage();
    InitDockWidget();
    InitDataMangementMenu();
    connect(_rootNode.get(), &QStandardItemModel::itemChanged, this, &ParameterEvaluation::HiddenData);
    connect(ui.fileTree, &QTreeView::customContextMenuRequested, this, &ParameterEvaluation::handleContextMenuRequested);
    connect(ui.action_MatchingFeature, &QAction::triggered, this, &ParameterEvaluation::matchingFeatureEvaluation);
    connect(ui.action_StemCurve, &QAction::triggered, this, &ParameterEvaluation::stemCurveEvaluation);
    connect(ui.action_Chinese, &QAction::triggered, this, &ParameterEvaluation::changeLanguage_Chinese);
    connect(ui.action_English, &QAction::triggered, this, &ParameterEvaluation::changeLanguage_English);

    connect(ui.action_ShowDBTree, &QAction::triggered, this, &ParameterEvaluation::showDBTree);
    connect(ui.action_ShowProperty, &QAction::triggered, this, &ParameterEvaluation::showProperties);
    connect(ui.action_ShowOutput, &QAction::triggered, this, &ParameterEvaluation::showOutput);
    connect(ui.nodeTree, &QDockWidget::visibilityChanged, this, &ParameterEvaluation::visualDBTree);
    connect(ui.property, &QDockWidget::visibilityChanged, this, &ParameterEvaluation::visualProperties);
    connect(ui.output, &QDockWidget::visibilityChanged, this, &ParameterEvaluation::visualOutput);
}

ParameterEvaluation::~ParameterEvaluation()
{
}

void ParameterEvaluation::InitForm()
{
    _formSettings = std::make_shared<FormSettings>(ui.visualArea);
    ui.logger->setReadOnly(true);
    //ui.NodeInformation->setContentsMargins(0, 0, 0, 0);
    //ui.ProPertyInformation->setContentsMargins(0, 0, 0, 0);
    //ui.OutInformation->setContentsMargins(0, 0, 0, 0);
    ui.action_ShowDBTree->setChecked(false);
    ui.action_ShowProperty->setChecked(false);
    ui.action_ShowOutput->setChecked(false);
}

void ParameterEvaluation::InitTree()
{
    ui.fileTree->setEditTriggers(QTreeView::NoEditTriggers);
    ui.fileTree->setSelectionBehavior(QTreeView::SelectRows);
    ui.fileTree->setHeaderHidden(true);

    _rootNode = std::make_shared<QStandardItemModel>(this);
    ui.fileTree->setModel(_rootNode.get());
    ui.fileTree->setContextMenuPolicy(Qt::CustomContextMenu);
}

void ParameterEvaluation::InitLanguage()
{
    chinese.load("ParameterEvaluation_zh.qm", qApp->applicationDirPath().append(""));
    english.load("ParameterEvaluation_en.qm", qApp->applicationDirPath().append(""));
    qApp->installTranslator(&english);
    ui.retranslateUi(this);
}

void ParameterEvaluation::InitDataMangementMenu()
{
    _dataMangementMenu = std::make_shared<QMenu>();
    QAction* deleteAction = new QAction(tr("删除"), this);
    
    connect(deleteAction, &QAction::triggered, this, &ParameterEvaluation::DeleteData);
    _dataMangementMenu->addAction(deleteAction);
}

void ParameterEvaluation::InitDockWidget()
{
    QFont font;
    font.setPointSize(10);
    tmpOfExt = new QWidget();
    _dataShowOfExt = new QDockWidget(this);

    _dataShowOfExt->setFont(font);
    _dataShowOfExt->setAutoFillBackground(true);
    verticalLayoutOfExt = new QVBoxLayout(tmpOfExt);
    verticalLayoutOfExt->setSpacing(6);
    verticalLayoutOfExt->setContentsMargins(11, 11, 11, 11);

    _dataShowOfExt->setWidget(tmpOfExt);
    this->addDockWidget(Qt::RightDockWidgetArea, _dataShowOfExt);
    this->tabifyDockWidget(ui.property, _dataShowOfExt);
    _dataShowOfExt->setVisible(false);

    tmpOfRef = new QWidget();
    _dataShowOfRef = new QDockWidget(this);

    _dataShowOfRef->setFont(font);
    _dataShowOfRef->setAutoFillBackground(true);
    verticalLayoutOfRef = new QVBoxLayout(tmpOfRef);
    verticalLayoutOfRef->setSpacing(6);
    verticalLayoutOfRef->setContentsMargins(11, 11, 11, 11);

    _dataShowOfRef->setWidget(tmpOfRef);
    this->addDockWidget(Qt::RightDockWidgetArea, _dataShowOfRef);
    this->tabifyDockWidget(ui.property, _dataShowOfRef);
    _dataShowOfRef->setVisible(false);

    tmpOfUExt = new QWidget();
    _dataShowOfUExt = new QDockWidget(this);

    _dataShowOfUExt->setFont(font);
    _dataShowOfUExt->setAutoFillBackground(true);
    verticalLayoutOfUExt = new QVBoxLayout(tmpOfUExt);
    verticalLayoutOfUExt->setSpacing(6);
    verticalLayoutOfUExt->setContentsMargins(11, 11, 11, 11);

    _dataShowOfUExt->setWidget(tmpOfUExt);
    this->addDockWidget(Qt::RightDockWidgetArea, _dataShowOfUExt);
    this->tabifyDockWidget(ui.property, _dataShowOfUExt);
    _dataShowOfUExt->setVisible(false);

    tmpOfURef = new QWidget();
    _dataShowOfURef = new QDockWidget(this);

    _dataShowOfURef->setFont(font);
    _dataShowOfURef->setAutoFillBackground(true);
    verticalLayoutOfURef = new QVBoxLayout(tmpOfURef);
    verticalLayoutOfURef->setSpacing(6);
    verticalLayoutOfURef->setContentsMargins(11, 11, 11, 11);

    _dataShowOfURef->setWidget(tmpOfURef);
    this->addDockWidget(Qt::RightDockWidgetArea, _dataShowOfURef);
    this->tabifyDockWidget(ui.property, _dataShowOfURef);
    _dataShowOfURef->setVisible(false);
}


void ParameterEvaluation::HiddenData(QStandardItem* item)
{
    Qt::CheckState state = item->checkState();

    osg::ref_ptr<osg::Group> showOrHidden = _itemToPointCloud[item];
    if (state == Qt::Checked)
    {
        //show osg data
        showOrHidden->setNodeMask(1);
        ui.logger->insertPlainText(QStringLiteral("显示数据！\n"));
    }
    else
    {
        //hide osg data
        showOrHidden->setNodeMask(0);
        ui.logger->insertPlainText(QStringLiteral("隐藏数据！\n"));
    }


}

void ParameterEvaluation::handleContextMenuRequested(const QPoint& pos)
{
    QModelIndex index = ui.fileTree->indexAt(pos);

    // if the selected item is valid, the context menu is displayed
    if (index.isValid())
    {
        _dataMangementMenu->exec(ui.fileTree->viewport()->mapToGlobal(pos));
    }
}

void ParameterEvaluation::DeleteData()
{
    QModelIndex currentIndex = ui.fileTree->currentIndex();

    QStandardItem* item = _rootNode->itemFromIndex(currentIndex);
    if (item != nullptr)
    {

        osg::ref_ptr<osg::Group> deleteNode = _itemToPointCloud[item];

        _formSettings->deleteTreeNode(deleteNode);

        _itemToPointCloud.erase(item);
        _rootNode->removeRow(item->row());
        
        _dataShowOfExt->setVisible(false);
        _dataShowOfRef->setVisible(false);
        _dataShowOfUExt->setVisible(false);
        _dataShowOfURef->setVisible(false);
    }
    else
    {
        ui.logger->insertPlainText(tr("无法删除节点！") + "\n");
    }



}


void ParameterEvaluation::changeLanguage_Chinese()
{
    qApp->installTranslator(&chinese);
    ui.retranslateUi(this);
    output_English = false;
}

void ParameterEvaluation::changeLanguage_English()
{
    qApp->installTranslator(&english);
    ui.retranslateUi(this);
    output_English = true;
}

void ParameterEvaluation::showDBTree()
{
    if (ui.nodeTree->isVisible())
    {
        ui.nodeTree->setVisible(false);
        ui.action_ShowDBTree->setChecked(false);
    }
    else
    {
        ui.nodeTree->setVisible(true);
        ui.action_ShowDBTree->setChecked(true);
    }
}

void ParameterEvaluation::showProperties()
{
    if (ui.property->isVisible())
    {
        ui.property->setVisible(false);
        ui.action_ShowProperty->setChecked(false);
    }
    else
    {
        ui.property->setVisible(true);
        ui.action_ShowProperty->setChecked(true);
    }
}

void ParameterEvaluation::showOutput()
{
    if (ui.output->isVisible())
    {
        ui.output->setVisible(false);
        ui.action_ShowOutput->setChecked(false);
    }
    else
    {
        ui.output->setVisible(true);
        ui.action_ShowOutput->setChecked(true);
    }
}

void ParameterEvaluation::visualDBTree()
{
    bool visible = ui.action_ShowDBTree->isChecked();
    ui.action_ShowDBTree->setChecked(!visible);
}

void ParameterEvaluation::visualProperties()
{
    bool visible = ui.action_ShowProperty->isChecked();
    ui.action_ShowProperty->setChecked(!visible);
}

void ParameterEvaluation::visualOutput()
{
    bool visible = ui.action_ShowOutput->isChecked();
    ui.action_ShowOutput->setChecked(!visible);
}

void ParameterEvaluation::matchingFeatureEvaluation()
{
    try
    {
        std::shared_ptr<MatchingFeatureEvaluation> _matchingFeatureEvaluation = std::make_shared<MatchingFeatureEvaluation>(this);
        if (!_matchingFeatureEvaluation->exec())
            return;
        std::shared_ptr<QProgressDialog> pDlg = std::make_shared<QProgressDialog>(this);
        pDlg->setWindowTitle(tr("提示"));
        pDlg->setModal(true); // set as a modal dialog, blocking user actions
        QProgressBar* pBar = new QProgressBar(pDlg.get());
        pBar->setRange(0, 0);
        pBar->setAlignment(Qt::AlignCenter);
        pDlg->setBar(pBar);
        _matchingFeatureEvaluation->process(pDlg.get(), _matchingFeature, ui.logger,output_English);
        if (_matchingFeatureEvaluation->isShowTable() && (_matchingFeature != nullptr))
        {
            if (_matchingFeature->getRelationsip()->getRelationshipIDs().size() == 0)
                return;

            showTreeInputInformation(_matchingFeature->getRelationsip()->getRelationshipIndexs(), _matchingFeature->getRelationsip()->getUnmatchedPredictedResult(), _matchingFeature->getRelationsip()->getUnmatchedReferenceResult());


            //displaying results in a 3D window
            osg::ref_ptr<osg::Group> inputFileNode = new osg::Group();
            _formSettings->showMatchingNode(inputFileNode, _matchingFeature->getRelationsip()->getPredictedID(), _matchingFeature->getRelationsip()->getPredictedCoordinate(), _matchingFeature->getRelationsip()->getReferencedID(), _matchingFeature->getRelationsip()->getReferenceCoordinate(), _matchingFeature->getRelationsip()->getRelationshipIndexs(), _matchingFeature->getRelationsip()->getUnmatchedPredictedResult(), _matchingFeature->getRelationsip()->getUnmatchedReferenceResult());

            QStandardItem* fileNode = new QStandardItem(tr("树木匹配结果示意图"));
            fileNode->setCheckable(true);
            fileNode->setCheckState(Qt::Checked);
            _rootNode->appendRow(fileNode);
            _itemToPointCloud.insert(std::make_pair(fileNode, inputFileNode));
            ui.fileTree->show();
        }
    }
    catch (const std::exception&)
    {
        ui.logger->insertPlainText(tr("参数结果预测结果评价：请检查文件格式和输入参数是否正确！") + "\n");
    }



}

void ParameterEvaluation::stemCurveEvaluation()
{
    try
    {
        std::shared_ptr<StemCurveEvaluation> _stemCurveEvaluation = std::make_shared<StemCurveEvaluation>(this);
        if (!_stemCurveEvaluation->exec())
            return;
        std::shared_ptr<QProgressDialog> pDlg = std::make_shared<QProgressDialog>(this);
        pDlg->setWindowTitle(tr("提示"));
        pDlg->setModal(true); // 设置为模态对话框，阻塞用户操作
        QProgressBar* pBar = new QProgressBar(pDlg.get());
        pBar->setRange(0, 0);
        pBar->setAlignment(Qt::AlignCenter);
        pDlg->setBar(pBar);
        _stemCurveEvaluation->process(pDlg.get(), _stemCurve, ui.logger,output_English);
    }
    catch (const std::exception&)
    {
        ui.logger->insertPlainText(tr("干曲线预测结果评价：请检查文件格式和输入参数是否正确！") + "\n");
    }

}

void ParameterEvaluation::showTreeInputInformation(std::vector<std::pair<size_t, size_t>> matchingIndex, std::vector<size_t> unMatchingIndexOfExt, std::vector<size_t> unMatchingIndexOfRef)
{
    QFont font;
    font.setPointSize(10);
    if (verticalLayoutOfExt->count() !=0)
    {
        verticalLayoutOfExt->removeWidget(_resultOfExt);
    }
    _dataShowOfExt->setWindowTitle(tr("预测结果数据表(模型用绿色表示,树木ID为：E_ID)"));
    _resultOfExt = new QTableWidget(_matchingFeature->getRelationsip()->getRelationshipIDs().size(), 5 + _matchingFeature->getRelationsip()->getPredictedFeature()[0].size(), tmpOfExt);
    _resultOfExt->setStyleSheet("selection-background-color: lightBlue");
    _resultOfExt->verticalHeader()->setHidden(true);
    _resultOfExt->setFont(font);
    _resultOfExt->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightGray;}"); //设置表头背景色
    _resultOfExt->setSelectionBehavior(QAbstractItemView::SelectRows);
    verticalLayoutOfExt->addWidget(_resultOfExt);
    _resultOfExt->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _resultOfExt->horizontalHeader()->setHighlightSections(false);
    _resultOfExt->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _resultOfExt->setSelectionMode(QAbstractItemView::ContiguousSelection);
    _dataShowOfExt->setVisible(true);

    QStringList header_ext;
    header_ext << tr("ID");
    header_ext << tr("X") << tr("Y") << tr("Z");
    if (_matchingFeature->getRelationsip()->getPredictedFeature().size() > 0)
    {
        for (size_t i = 0; i < _matchingFeature->getRelationsip()->getPredictedFeature()[0].size(); i++)
        {
            std::string tmpName;
            if (output_English == true)
            {
                tmpName = "Feature" + std::to_string(i + 1);
            }
            else
            {
                tmpName = "第" + std::to_string(i + 1) + "组特征";
            }

            header_ext << tmpName.c_str();
        }
    }

    header_ext << tr("对应的参考树ID");
    _resultOfExt->setHorizontalHeaderLabels(header_ext);

    for (size_t i = 0; i < matchingIndex.size(); i++)
    {
        _resultOfExt->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(_matchingFeature->getRelationsip()->getPredictedID()[matchingIndex[i].first])));
        _resultOfExt->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        for (size_t j = 0; j < 3; j++)
        {
            if (_matchingFeature->getPredictedCoordinate()[matchingIndex[i].first][j] == DBL_MAX)
            {
                _resultOfExt->setItem(i, j + 1, new QTableWidgetItem(QString("nan")));
            }
            else
            {
                _resultOfExt->setItem(i, j + 1, new QTableWidgetItem(QString::number(_matchingFeature->getPredictedCoordinate()[matchingIndex[i].first][j])));
            }

            _resultOfExt->item(i, j + 1)->setTextAlignment(Qt::AlignCenter);
        }
        for (size_t j = 0; j < _matchingFeature->getRelationsip()->getPredictedFeature()[matchingIndex[i].first].size(); j++)
        {
            if (_matchingFeature->getRelationsip()->getPredictedFeature()[matchingIndex[i].first][j] == DBL_MAX)
            {
                _resultOfExt->setItem(i, j + 1 + 3, new QTableWidgetItem(QString("nan")));
            }
            else
            {
                _resultOfExt->setItem(i, j + 1 + 3, new QTableWidgetItem(QString::number(_matchingFeature->getRelationsip()->getPredictedFeature()[matchingIndex[i].first][j])));
            }
            _resultOfExt->item(i, j + 1 + 3)->setTextAlignment(Qt::AlignCenter);
        }
        _resultOfExt->setItem(i, _matchingFeature->getRelationsip()->getPredictedFeature()[matchingIndex[i].first].size() + 4, new QTableWidgetItem(QString::fromStdString(_matchingFeature->getRelationsip()->getReferencedID()[matchingIndex[i].second])));
        _resultOfExt->item(i, _matchingFeature->getRelationsip()->getPredictedFeature()[matchingIndex[i].first].size() + 4)->setTextAlignment(Qt::AlignCenter);
    }
    _resultOfExt->horizontalHeader()->setSectionResizeMode(5 + _matchingFeature->getRelationsip()->getPredictedFeature()[0].size() - 1, QHeaderView::ResizeToContents);

    if (verticalLayoutOfRef->count() != 0)
    {
        verticalLayoutOfRef->removeWidget(_resultOfRef);
    }

    _dataShowOfRef->setWindowTitle(tr("参考结果数据表(模型用黄色表示,树木ID为：R_ID)"));
    _resultOfRef = new QTableWidget(_matchingFeature->getRelationsip()->getRelationshipIDs().size(), 5 + _matchingFeature->getRelationsip()->getReferenceFeature()[0].size(), tmpOfRef);
    _resultOfRef->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    _resultOfRef->setStyleSheet("selection-background-color: lightBlue");
    _resultOfRef->verticalHeader()->setHidden(true);
    _resultOfRef->setFont(font);
    _resultOfRef->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightGray;}"); //设置表头背景色
    _resultOfRef->setSelectionBehavior(QAbstractItemView::SelectRows);
    verticalLayoutOfRef->addWidget(_resultOfRef);
    _resultOfRef->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _resultOfRef->horizontalHeader()->setHighlightSections(false);
    _resultOfRef->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _resultOfRef->setSelectionMode(QAbstractItemView::ContiguousSelection);
    _dataShowOfRef->setVisible(true);

    QStringList header_ref;
    header_ref << tr("ID");
    header_ref << tr("X") << tr("Y") << tr("Z");
    if (_matchingFeature->getRelationsip()->getReferenceFeature().size() > 0)
    {
        for (size_t i = 0; i < _matchingFeature->getRelationsip()->getReferenceFeature()[0].size(); i++)
        {
            std::string tmpName;
            if (output_English == true)
            {
                tmpName = "Feature" + std::to_string(i + 1);
            }
            else
            {
                tmpName = "第" + std::to_string(i + 1) + "组特征";
            }

            header_ref << tmpName.c_str();
        }
    }
   
    header_ref << tr("对应的预测树ID");
    _resultOfRef->setHorizontalHeaderLabels(header_ref);

    for (size_t i = 0; i < matchingIndex.size(); i++)
    {
        _resultOfRef->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(_matchingFeature->getRelationsip()->getReferencedID()[matchingIndex[i].second])));
        _resultOfRef->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        for (size_t j = 0; j < 3; j++)
        {
            if (_matchingFeature->getReferenceCoordinate()[matchingIndex[i].second][j] == DBL_MAX)
            {
                _resultOfRef->setItem(i, j + 1, new QTableWidgetItem(QString("nan")));
            }
            else
            {
                _resultOfRef->setItem(i, j + 1, new QTableWidgetItem(QString::number(_matchingFeature->getReferenceCoordinate()[matchingIndex[i].second][j])));
            }

            _resultOfRef->item(i, j + 1)->setTextAlignment(Qt::AlignCenter);
        }
        for (size_t j = 0; j < _matchingFeature->getRelationsip()->getReferenceFeature()[matchingIndex[i].second].size(); j++)
        {
            if (_matchingFeature->getRelationsip()->getReferenceFeature()[matchingIndex[i].second][j] == DBL_MAX)
            {
                _resultOfRef->setItem(i, j + 1 + 3, new QTableWidgetItem(QString("nan")));
            }
            else
            {
                _resultOfRef->setItem(i, j + 1 + 3, new QTableWidgetItem(QString::number(_matchingFeature->getRelationsip()->getReferenceFeature()[matchingIndex[i].second][j])));
            }

            _resultOfRef->item(i, j + 1 + 3)->setTextAlignment(Qt::AlignCenter);
        }
        _resultOfRef->setItem(i, _matchingFeature->getRelationsip()->getReferenceFeature()[0].size() + 4, new QTableWidgetItem(QString::fromStdString(_matchingFeature->getRelationsip()->getPredictedID()[matchingIndex[i].first])));
        _resultOfRef->item(i, _matchingFeature->getRelationsip()->getReferenceFeature()[0].size() + 4)->setTextAlignment(Qt::AlignCenter);
    }
    _resultOfRef->horizontalHeader()->setSectionResizeMode(5 + _matchingFeature->getRelationsip()->getPredictedFeature()[0].size() - 1, QHeaderView::ResizeToContents);

    connect(_resultOfExt, &QTableWidget::itemClicked, [=](QTableWidgetItem* item) {
        int row = item->row(); // 获取行号
        _resultOfRef->setCurrentCell(row, QItemSelectionModel::Select);
        });
    connect(_resultOfRef, &QTableWidget::itemClicked, [=](QTableWidgetItem* item) {
        int row = item->row(); // 获取行号
        _resultOfExt->setCurrentCell(row, QItemSelectionModel::Select);
        });

    if (verticalLayoutOfUExt->count() != 0)
    {
        verticalLayoutOfUExt->removeWidget(_resultOfUExt);
    }
    _dataShowOfUExt->setWindowTitle(tr("未匹配的预测结果数据表(模型用红色表示,树木ID为：UE_ID)"));
    _resultOfUExt = new QTableWidget(_matchingFeature->getRelationsip()->getUnmatchedPredictedResult().size(), 4 + _matchingFeature->getRelationsip()->getPredictedFeature()[0].size(), tmpOfUExt);
    _resultOfUExt->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    _resultOfUExt->verticalHeader()->setHidden(true);
    _resultOfUExt->setFont(font);
    _resultOfUExt->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightGray;}"); //设置表头背景色
    _resultOfUExt->setSelectionBehavior(QAbstractItemView::SelectRows);
    verticalLayoutOfUExt->addWidget(_resultOfUExt);
    _resultOfUExt->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _resultOfUExt->horizontalHeader()->setHighlightSections(false);
    _resultOfUExt->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _resultOfUExt->setSelectionMode(QAbstractItemView::ContiguousSelection);
    _dataShowOfUExt->setVisible(true);


    QStringList header_uext;
    header_uext << tr("ID");
    header_uext << tr("X") << tr("Y") << tr("Z");
    if (_matchingFeature->getRelationsip()->getReferenceFeature().size() > 0)
    {
        for (size_t i = 0; i < _matchingFeature->getRelationsip()->getReferenceFeature()[0].size(); i++)
        {
            std::string tmpName;
            if (output_English == true)
            {
                tmpName = "Feature" + std::to_string(i + 1);
            }
            else
            {
                tmpName = "第" + std::to_string(i + 1) + "组特征";
            }

            header_uext << tmpName.c_str();
        }
    }
   

    _resultOfUExt->setHorizontalHeaderLabels(header_uext);

    for (size_t i = 0; i < unMatchingIndexOfExt.size(); i++)
    {
        _resultOfUExt->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(_matchingFeature->getRelationsip()->getPredictedID()[unMatchingIndexOfExt[i]])));
        _resultOfUExt->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        for (size_t j = 0; j < 3; j++)
        {
            if (_matchingFeature->getPredictedCoordinate()[unMatchingIndexOfExt[i]][j] == DBL_MAX)
            {
                _resultOfUExt->setItem(i, j + 1, new QTableWidgetItem(QString("nan")));
            }
            else
            {
                _resultOfUExt->setItem(i, j + 1, new QTableWidgetItem(QString::number(_matchingFeature->getPredictedCoordinate()[unMatchingIndexOfExt[i]][j])));
            }

            _resultOfUExt->item(i, j + 1)->setTextAlignment(Qt::AlignCenter);
        }
        for (size_t j = 0; j < _matchingFeature->getRelationsip()->getPredictedFeature()[0].size(); j++)
        {
            if (_matchingFeature->getRelationsip()->getPredictedFeature()[unMatchingIndexOfExt[i]][j] == DBL_MAX)
            {
                _resultOfUExt->setItem(i, j + 1, new QTableWidgetItem(QString("nan")));
            }
            else
            {
                _resultOfUExt->setItem(i, j + 4, new QTableWidgetItem(QString::number(_matchingFeature->getRelationsip()->getPredictedFeature()[unMatchingIndexOfExt[i]][j])));
            }

            _resultOfUExt->item(i, j + 4)->setTextAlignment(Qt::AlignCenter);
        }
    }

    if (verticalLayoutOfURef->count() != 0)
    {
        verticalLayoutOfURef->removeWidget(_resultOfURef);
    }
    _dataShowOfURef->setWindowTitle(tr("未匹配的参考结果数据表(模型用粉色表示,树木ID为：UR_ID)"));
    _resultOfURef = new QTableWidget(_matchingFeature->getRelationsip()->getUnmatchedReferenceResult().size(), 4 + _matchingFeature->getRelationsip()->getReferenceFeature()[0].size(), tmpOfURef);
    _resultOfURef->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    _resultOfURef->setStyleSheet("selection-background-color: lightBlue");
    _resultOfURef->verticalHeader()->setHidden(true);
    _resultOfURef->setFont(font);
    _resultOfURef->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightGray;}"); //设置表头背景色
    _resultOfURef->setSelectionBehavior(QAbstractItemView::SelectRows);
    verticalLayoutOfURef->addWidget(_resultOfURef);
    _resultOfURef->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _resultOfURef->horizontalHeader()->setHighlightSections(false);
    _resultOfURef->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _resultOfURef->setSelectionMode(QAbstractItemView::ContiguousSelection);
    _dataShowOfURef->setVisible(true);

    QStringList header_uref;
    header_uref << tr("ID");
    header_uref << tr("X") << tr("Y") << tr("Z");
 
    for (size_t i = 0; i < _matchingFeature->getRelationsip()->getReferenceFeature()[0].size(); i++)
    {
        std::string tmpName;
        if (output_English == true)
        {
            tmpName = "Feature" + std::to_string(i + 1);
        }
        else
        {
            tmpName = "第" + std::to_string(i + 1) + "组特征";
        }

        header_uref << tmpName.c_str();
    }
    _resultOfURef->setHorizontalHeaderLabels(header_uref);


    for (size_t i = 0; i < unMatchingIndexOfRef.size(); i++)
    {
        _resultOfURef->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(_matchingFeature->getRelationsip()->getReferencedID()[unMatchingIndexOfRef[i]])));
        _resultOfURef->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        for (size_t j = 0; j < 3; j++)
        {
            if (_matchingFeature->getReferenceCoordinate()[unMatchingIndexOfRef[i]][j] == DBL_MAX)
            {
                _resultOfURef->setItem(i, j + 1, new QTableWidgetItem(QString("nan")));
            }
            else
            {
                _resultOfURef->setItem(i, j + 1, new QTableWidgetItem(QString::number(_matchingFeature->getReferenceCoordinate()[unMatchingIndexOfRef[i]][j])));
            }
           
            _resultOfURef->item(i, j + 1)->setTextAlignment(Qt::AlignCenter);
        }
        for (size_t j = 0; j < _matchingFeature->getRelationsip()->getReferenceFeature()[0].size(); j++)
        {
            if (_matchingFeature->getRelationsip()->getReferenceFeature()[unMatchingIndexOfRef[i]][j] == DBL_MAX)
            {
                _resultOfURef->setItem(i, j + 4, new QTableWidgetItem(QString("nan")));
            }
            else
            {
                _resultOfURef->setItem(i, j + 4, new QTableWidgetItem(QString::number(_matchingFeature->getRelationsip()->getReferenceFeature()[unMatchingIndexOfRef[i]][j])));
            }

            _resultOfURef->item(i, j + 4)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

