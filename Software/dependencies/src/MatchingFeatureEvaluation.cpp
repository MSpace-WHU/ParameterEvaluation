#include "../include/MatchingFeatureEvaluation.h"

#include <QtWidgets/QProgressDialog>
#include <QtConcurrent/QtConcurrent>
#include <QProgressBar>
#include <QFileDialog>
#include <QHeaderView>
#include <QMainWindow>
#include <QToolTip>

MatchingFeatureEvaluation::MatchingFeatureEvaluation(QWidget* parent)
	:QDialog(parent, Qt::Tool),
	Ui::MatchingFeatureEvaluation()
{
	this->setupUi(this);
	this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	_isShowTable = false;

	connect(_selectInputFileOfPredictedTree, &QPushButton::clicked, this, &MatchingFeatureEvaluation::selectedInputFileOfPredictedTree);
	connect(_selectedInputFileOfReferenceTree, &QPushButton::clicked, this, &MatchingFeatureEvaluation::selectedInputFileOfReferenceedTree);
	connect(_selectedInputFileOfIndex, &QPushButton::clicked, this, &MatchingFeatureEvaluation::selectedInputFileOfIndex);
	connect(_selectOutputFile, &QPushButton::clicked, this, &MatchingFeatureEvaluation::selectedOutputFile);
	connect(_btn_OK, &QPushButton::clicked, this, &MatchingFeatureEvaluation::apply);

	initParameter();

}
MatchingFeatureEvaluation::~MatchingFeatureEvaluation()
{
};



void MatchingFeatureEvaluation::apply()
{
	if (_isShowResult->isChecked())
	{
		_isShowTable = true;
	}
	this->accept();
}

void MatchingFeatureEvaluation::process(QProgressDialog* progress, std::shared_ptr<MatchingFeature>& _matching3D, QTextEdit* outputInformation, bool output_English)
{
	if (progress)
	{
		progress->setLabelText(tr("提取树结果评价……"));
		progress->setWindowFlags(progress->windowFlags() & ~Qt::WindowCloseButtonHint & ~Qt::WindowContextHelpButtonHint);
		progress->setCancelButton(nullptr);
		progress->show();
		progress->raise();
	}
	QFuture<void> future = QtConcurrent::run(std::bind(&MatchingFeatureEvaluation::processingData, this, std::ref(_matching3D), outputInformation,output_English));
	while (!future.isFinished())
	{
		if (progress)
		{
			progress->setValue(progress->value() + 1);
			QApplication::processEvents();
		}
	}
}

bool MatchingFeatureEvaluation::isShowTable()
{
	return _isShowTable;
}

void MatchingFeatureEvaluation::selectedInputFileOfPredictedTree()
{
	QStringList fileTypes;
	fileTypes << "Text Files (*.txt)";
	QString file = QFileDialog::getOpenFileName(this, tr("选择提取结果参数文件"), "", fileTypes.join(";;"));
	if (file.isEmpty())
		return;
	_inputFileOfPredictedTree->setText(file);
}

void MatchingFeatureEvaluation::selectedInputFileOfReferenceedTree()
{
	QStringList fileTypes;
	fileTypes << "Text Files (*.txt)";
	QString file = QFileDialog::getOpenFileName(this, tr("选择参考结果参数文件"),"", fileTypes.join(";;"));
	if (file.isEmpty())
		return;
	_inputFileOfReferenceTree->setText(file);
}

void MatchingFeatureEvaluation::selectedInputFileOfIndex()
{
	QStringList fileTypes;
	fileTypes << "Text Files (*.txt)";
	QString file = QFileDialog::getOpenFileName(this, tr("选择匹配索引文件"), "", fileTypes.join(";;"));
	if (file.isEmpty())
		return;
	_inputFileOfIndex->setText(file);
}

void MatchingFeatureEvaluation::selectedOutputFile()
{
	QStringList fileTypes;
	fileTypes << "Excel Files (*.xlsx)";
	QString file = QFileDialog::getOpenFileName(this, tr("选择输出文件"), "", fileTypes.join(";;"));
	if (file.isEmpty())
		return;
	_outputFile->setText(file);
}

void MatchingFeatureEvaluation::processingData(std::shared_ptr<MatchingFeature>& _matching3D, QTextEdit* outputInformation, bool output_English)
{
	double radius = 1.0; size_t column = 0,dimension = 0;
	//one of the index files or parameters must exist
	if (_inputFileOfIndex->text().isEmpty())
	{
		if (_columnOfMF->text().isEmpty() || _radius->text().isEmpty())
		{
			outputInformation->insertPlainText(QObject::tr("匹配特征列号，半径和匹配文件存在缺失！") + "\n");
			outputInformation->insertPlainText(QObject::tr("-----------------------------") + "\n");
			return;
		}
		else
		{
			column = QString(_columnOfMF->text()).toDouble();
			radius = QString(_radius->text()).toDouble();
			dimension = QString(_dimension->text()).toDouble();
			if (column != 0 && column < 5)
			{
				outputInformation->insertPlainText(QObject::tr("匹配特征列号输入存在问题！") + "\n");
				outputInformation->insertPlainText(QObject::tr("-----------------------------") + "\n");
				return;
			}

			if (radius <= 0)
			{
				outputInformation->insertPlainText(QObject::tr("匹配搜索半径存在问题！") + "\n");
				outputInformation->insertPlainText(QObject::tr("-----------------------------") + "\n");
				return;
			}
			if (dimension != 2 && dimension != 3)
			{
				outputInformation->insertPlainText(QObject::tr("匹配坐标维度存在问题！") + "\n");
				outputInformation->insertPlainText(QObject::tr("-----------------------------") + "\n");
				return;
			}
		}
	}


	//MatchingFeature function initialization
	_matching3D = std::make_shared<MatchingFeature>(outputInformation);
	std::string _fileOfPredictedTree = std::string(_inputFileOfPredictedTree->text().toLocal8Bit());
	std::string _fileOfReferenceTree = std::string(_inputFileOfReferenceTree->text().toLocal8Bit());
	std::string _fileOfOfIndex = std::string(_inputFileOfIndex->text().toLocal8Bit());
	std::string _outputPath = std::string(_outputFile->text().toLocal8Bit());
	if (_fileOfPredictedTree.empty() || _fileOfReferenceTree.empty() || _outputPath.empty())
	{
		outputInformation->insertPlainText(QObject::tr("输入文件和输出文件路径存在问题！") + "\n");
		outputInformation->insertPlainText(QObject::tr("-----------------------------") + "\n");
		return;
	}

	_matching3D->loadFile(_fileOfPredictedTree, _fileOfReferenceTree);
	_matching3D->loadMatchingRelationship(_fileOfOfIndex, dimension, radius, column);
	_matching3D->exportFile(_outputPath, output_English);

	//_resOfExt = _matching3D->getResOfExt().size();
	//_resOfRef = _matching3D->getResOfRef().size();
}

void MatchingFeatureEvaluation::showResult()
{
	if (!_isShowResult->isChecked())
		return;


	//QString qssTV = "QTableWidget::item:hover{background-color:rgb(92,188,227,200)}"
	//	"QTableWidget::item:selected{background-color:#1B89A1}"
	//	"QHeaderView::section,QTableCornerButton:section{ \
 //       padding:3px; margin:0px; color:#DCDCDC;  border:1px solid #242424; \
	//	border-left-width:0px; border-right-width:1px; border-top-width:0px; border-bottom-width:1px; \
	//	background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #646464,stop:1 #525252); }"
	//	"QTableWidget{background-color:white;border:none;}";
	//_result->setStyleSheet(qssTV);

	//_result->setEditTriggers(QAbstractItemView::NoEditTriggers);

	////使用下面两句可以实现 
	//_result->horizontalHeader()->setHighlightSections(false);
	//_result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	//_result->setSelectionMode(QAbstractItemView::ContiguousSelection);

	////_result->setRowCount(2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + _matching3D->getResOfRef().size() + 4);


	//写一个for循环填入相关信息

	//QStringList header;
	//if (_matching3D->getFileType() == 0)
	//{
	//	//_result->setColumnCount(7);
	//	header << tr("ID") << tr("X") << tr("Y") << tr("Z") << tr("树高") << tr("胸径") << tr("匹配特征");
	//	_result->setHorizontalHeaderLabels(header);
	//	_result->setSpan(0, 0, 1, _result->columnCount());
	//	_result->setItem(0, 0, new QTableWidgetItem(QString(tr("提取树数据"))));
	//	_result->item(0, 0)->setTextAlignment(Qt::AlignCenter);
	//	for (size_t i = 0; i < _matching3D->getTrueMatching().size(); i++)
	//	{
	//		_result->setItem(i + 1, 0, new QTableWidgetItem(QString::number(_matching3D->getIDOfExtractedTree()[_matching3D->getTrueMatching()[i].first])));
	//		_result->item(i + 1, 0)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 1, 1, new QTableWidgetItem(QString::number(_matching3D->getPositionOfExtrctedTree()[_matching3D->getTrueMatching()[i].first][0])));
	//		_result->item(i + 1, 1)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 1, 2, new QTableWidgetItem(QString::number(_matching3D->getPositionOfExtrctedTree()[_matching3D->getTrueMatching()[i].first][1])));
	//		_result->item(i + 1, 2)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 1, 3, new QTableWidgetItem(QString::number(_matching3D->getPositionOfExtrctedTree()[_matching3D->getTrueMatching()[i].first][2])));
	//		_result->item(i + 1, 3)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 1, 4, new QTableWidgetItem(QString::number(_matching3D->getHeightOfExtractedTree()[_matching3D->getTrueMatching()[i].first])));
	//		_result->item(i + 1, 4)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 1, 5, new QTableWidgetItem(QString::number(_matching3D->getDBHOfExtractedTree()[_matching3D->getTrueMatching()[i].first])));
	//		_result->item(i + 1, 5)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 1, 6, new QTableWidgetItem(QString::number(_matching3D->getMatchingFeaturesOfExtractedTree()[_matching3D->getTrueMatching()[i].first])));
	//		_result->item(i + 1, 6)->setTextAlignment(Qt::AlignCenter);
	//	}

	//	_result->setSpan(_matching3D->getTrueMatching().size() + 1, 0, 1, _result->columnCount());
	//	_result->setItem(_matching3D->getTrueMatching().size() + 1, 0, new QTableWidgetItem(QString(tr("参考树数据"))));
	//	_result->item(_matching3D->getTrueMatching().size() + 1, 0)->setTextAlignment(Qt::AlignCenter);

	//	for (size_t i = 0; i < _matching3D->getTrueMatching().size(); i++)
	//	{
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 0, new QTableWidgetItem(QString::number(_matching3D->getIDOfReferenceTree()[_matching3D->getTrueMatching()[i].second])));
	//		_result->item(i + _matching3D->getTrueMatching().size() + 2, 0)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 1, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getTrueMatching()[i].second][0])));
	//		_result->item(i + _matching3D->getTrueMatching().size() + 2, 1)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 2, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getTrueMatching()[i].second][1])));
	//		_result->item(i + _matching3D->getTrueMatching().size() + 2, 2)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 3, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getTrueMatching()[i].second][2])));
	//		_result->item(i + _matching3D->getTrueMatching().size() + 2, 3)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 4, new QTableWidgetItem(QString::number(_matching3D->getHeightOfReferenceTree()[_matching3D->getTrueMatching()[i].second])));
	//		_result->item(i + _matching3D->getTrueMatching().size() + 2, 4)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 5, new QTableWidgetItem(QString::number(_matching3D->getDBHOfReferenceTree()[_matching3D->getTrueMatching()[i].second])));
	//		_result->item(i + _matching3D->getTrueMatching().size() + 2, 5)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 6, new QTableWidgetItem(QString::number(_matching3D->getMatchingFeaturesOfReferenceTree()[_matching3D->getTrueMatching()[i].second])));
	//		_result->item(i + _matching3D->getTrueMatching().size() + 2, 6)->setTextAlignment(Qt::AlignCenter);
	//	}

	//	_result->setSpan(2 * _matching3D->getTrueMatching().size() + 2, 0, 1, _result->columnCount());
	//	_result->setItem(2 * _matching3D->getTrueMatching().size() + 2, 0, new QTableWidgetItem(QString(tr("未匹配的提取树数据"))));
	//	_result->item(2 * _matching3D->getTrueMatching().size() + 2, 0)->setTextAlignment(Qt::AlignCenter);
	//	for (size_t i = 0; i < _matching3D->getResOfExt().size(); i++)
	//	{
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 0, new QTableWidgetItem(QString::number(_matching3D->getIDOfReferenceTree()[_matching3D->getResOfExt()[i]])));
	//		_result->item(i + 2 * _matching3D->getTrueMatching().size() + 3, 0)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 1, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfExt()[i]][0])));
	//		_result->item(i + 2 * _matching3D->getTrueMatching().size() + 3, 1)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 2, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfExt()[i]][1])));
	//		_result->item(i + 2 * _matching3D->getTrueMatching().size() + 3, 2)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 3, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfExt()[i]][2])));
	//		_result->item(i + 2 * _matching3D->getTrueMatching().size() + 3, 3)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 4, new QTableWidgetItem(QString::number(_matching3D->getHeightOfReferenceTree()[_matching3D->getResOfExt()[i]])));
	//		_result->item(i + 2 * _matching3D->getTrueMatching().size() + 3, 4)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 5, new QTableWidgetItem(QString::number(_matching3D->getDBHOfReferenceTree()[_matching3D->getResOfExt()[i]])));
	//		_result->item(i + 2 * _matching3D->getTrueMatching().size() + 3, 5)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 6, new QTableWidgetItem(QString::number(_matching3D->getMatchingFeaturesOfReferenceTree()[_matching3D->getResOfExt()[i]])));
	//		_result->item(i + 2 * _matching3D->getTrueMatching().size() + 3, 6)->setTextAlignment(Qt::AlignCenter);
	//	}

	//	_result->setSpan(2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 3, 0, 1, _result->columnCount());
	//	_result->setItem(2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 3, 0, new QTableWidgetItem(QString(tr("未匹配的参考树数据"))));
	//	_result->item(2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 3, 0)->setTextAlignment(Qt::AlignCenter);

	//	for (size_t i = 0; i < _matching3D->getResOfRef().size(); i++)
	//	{
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 0, new QTableWidgetItem(QString::number(_matching3D->getIDOfReferenceTree()[_matching3D->getResOfRef()[i]])));
	//		_result->item(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 0)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 1, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfRef()[i]][0])));
	//		_result->item(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 1)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 2, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfRef()[i]][1])));
	//		_result->item(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 2)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 3, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfRef()[i]][2])));
	//		_result->item(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 3)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 4, new QTableWidgetItem(QString::number(_matching3D->getHeightOfReferenceTree()[_matching3D->getResOfRef()[i]])));
	//		_result->item(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 4)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 5, new QTableWidgetItem(QString::number(_matching3D->getDBHOfReferenceTree()[_matching3D->getResOfRef()[i]])));
	//		_result->item(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 5)->setTextAlignment(Qt::AlignCenter);
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 6, new QTableWidgetItem(QString::number(_matching3D->getMatchingFeaturesOfReferenceTree()[_matching3D->getResOfRef()[i]])));
	//		_result->item(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 6)->setTextAlignment(Qt::AlignCenter);
	//	}
	//}
	//else if (_matching3D->getFileType() == 1)
	//{
	//	_result->setColumnCount(6);
	//	header << tr("ID") << tr("X") << tr("Y") << tr("Z") << tr("树高") << tr("胸径");
	//	_result->setHorizontalHeaderLabels(header);
	//	_result->setSpan(0, 0, 1, _result->columnCount());
	//	_result->setItem(0, 0, new QTableWidgetItem(QString(tr("提取树数据"))));
	//	for (size_t i = 0; i < _matching3D->getTrueMatching().size(); i++)
	//	{
	//		_result->setItem(i + 1, 0, new QTableWidgetItem(QString::number(_matching3D->getIDOfExtractedTree()[_matching3D->getTrueMatching()[i].first])));
	//		_result->setItem(i + 1, 1, new QTableWidgetItem(QString::number(_matching3D->getPositionOfExtrctedTree()[_matching3D->getTrueMatching()[i].first][0])));
	//		_result->setItem(i + 1, 2, new QTableWidgetItem(QString::number(_matching3D->getPositionOfExtrctedTree()[_matching3D->getTrueMatching()[i].first][1])));
	//		_result->setItem(i + 1, 3, new QTableWidgetItem(QString::number(_matching3D->getPositionOfExtrctedTree()[_matching3D->getTrueMatching()[i].first][2])));
	//		_result->setItem(i + 1, 4, new QTableWidgetItem(QString::number(_matching3D->getHeightOfExtractedTree()[_matching3D->getTrueMatching()[i].first])));
	//		_result->setItem(i + 1, 5, new QTableWidgetItem(QString::number(_matching3D->getDBHOfExtractedTree()[_matching3D->getTrueMatching()[i].first])));
	//	}

	//	_result->setSpan(_matching3D->getTrueMatching().size() + 1, 0, 1, _result->columnCount());
	//	_result->setItem(_matching3D->getTrueMatching().size() + 1, 0, new QTableWidgetItem(QString(tr("参考树数据"))));
	//	for (size_t i = 0; i < _matching3D->getTrueMatching().size(); i++)
	//	{
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 0, new QTableWidgetItem(QString::number(_matching3D->getIDOfReferenceTree()[_matching3D->getTrueMatching()[i].second])));
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 1, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getTrueMatching()[i].second][0])));
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 2, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getTrueMatching()[i].second][1])));
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 3, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getTrueMatching()[i].second][2])));
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 4, new QTableWidgetItem(QString::number(_matching3D->getHeightOfReferenceTree()[_matching3D->getTrueMatching()[i].second])));
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 5, new QTableWidgetItem(QString::number(_matching3D->getDBHOfReferenceTree()[_matching3D->getTrueMatching()[i].second])));
	//	}

	//	_result->setSpan(2 * _matching3D->getTrueMatching().size() + 2, 0, 1, _result->columnCount());
	//	_result->setItem(2 * _matching3D->getTrueMatching().size() + 2, 0, new QTableWidgetItem(QString(tr("未匹配的提取树数据"))));
	//	for (size_t i = 0; i < _matching3D->getResOfExt().size(); i++)
	//	{
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 0, new QTableWidgetItem(QString::number(_matching3D->getIDOfReferenceTree()[_matching3D->getResOfExt()[i]])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 1, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfExt()[i]][0])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 2, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfExt()[i]][1])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 3, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfExt()[i]][2])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 4, new QTableWidgetItem(QString::number(_matching3D->getHeightOfReferenceTree()[_matching3D->getResOfExt()[i]])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 5, new QTableWidgetItem(QString::number(_matching3D->getDBHOfReferenceTree()[_matching3D->getResOfExt()[i]])));
	//	}

	//	_result->setSpan(2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 3, 0, 1, _result->columnCount());
	//	_result->setItem(2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 3, 0, new QTableWidgetItem(QString(tr("未匹配的参考树数据"))));

	//	for (size_t i = 0; i < _matching3D->getResOfRef().size(); i++)
	//	{
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 0, new QTableWidgetItem(QString::number(_matching3D->getIDOfReferenceTree()[_matching3D->getResOfRef()[i]])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 1, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfRef()[i]][0])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 2, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfRef()[i]][1])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 3, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfRef()[i]][2])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 4, new QTableWidgetItem(QString::number(_matching3D->getHeightOfReferenceTree()[_matching3D->getResOfRef()[i]])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 5, new QTableWidgetItem(QString::number(_matching3D->getDBHOfReferenceTree()[_matching3D->getResOfRef()[i]])));
	//	}
	//}
	//else if (_matching3D->getFileType() == 2)
	//{
	//	_result->setColumnCount(6);
	//	header << tr("ID") << tr("X") << tr("Y") << tr("树高") << tr("胸径") << tr("匹配特征");
	//	_result->setHorizontalHeaderLabels(header);
	//	_result->setSpan(0, 0, 1, _result->columnCount());
	//	_result->setItem(0, 0, new QTableWidgetItem(QString(tr("提取树数据"))));
	//	for (size_t i = 0; i < _matching3D->getTrueMatching().size(); i++)
	//	{
	//		_result->setItem(i + 1, 0, new QTableWidgetItem(QString::number(_matching3D->getIDOfExtractedTree()[_matching3D->getTrueMatching()[i].first])));
	//		_result->setItem(i + 1, 1, new QTableWidgetItem(QString::number(_matching3D->getPositionOfExtrctedTree()[_matching3D->getTrueMatching()[i].first][0])));
	//		_result->setItem(i + 1, 2, new QTableWidgetItem(QString::number(_matching3D->getPositionOfExtrctedTree()[_matching3D->getTrueMatching()[i].first][1])));
	//		_result->setItem(i + 1, 3, new QTableWidgetItem(QString::number(_matching3D->getHeightOfExtractedTree()[_matching3D->getTrueMatching()[i].first])));
	//		_result->setItem(i + 1, 4, new QTableWidgetItem(QString::number(_matching3D->getDBHOfExtractedTree()[_matching3D->getTrueMatching()[i].first])));
	//		_result->setItem(i + 1, 5, new QTableWidgetItem(QString::number(_matching3D->getMatchingFeaturesOfExtractedTree()[_matching3D->getTrueMatching()[i].first])));
	//	}

	//	_result->setSpan(_matching3D->getTrueMatching().size() + 1, 0, 1, _result->columnCount());
	//	_result->setItem(_matching3D->getTrueMatching().size() + 1, 0, new QTableWidgetItem(QString(tr("参考树数据"))));
	//	for (size_t i = 0; i < _matching3D->getTrueMatching().size(); i++)
	//	{
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 0, new QTableWidgetItem(QString::number(_matching3D->getIDOfReferenceTree()[_matching3D->getTrueMatching()[i].second])));
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 1, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getTrueMatching()[i].second][0])));
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 2, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getTrueMatching()[i].second][1])));
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 3, new QTableWidgetItem(QString::number(_matching3D->getHeightOfReferenceTree()[_matching3D->getTrueMatching()[i].second])));
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 4, new QTableWidgetItem(QString::number(_matching3D->getDBHOfReferenceTree()[_matching3D->getTrueMatching()[i].second])));
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 5, new QTableWidgetItem(QString::number(_matching3D->getMatchingFeaturesOfReferenceTree()[_matching3D->getTrueMatching()[i].second])));
	//	}

	//	_result->setSpan(2 * _matching3D->getTrueMatching().size() + 2, 0, 1, _result->columnCount());
	//	_result->setItem(2 * _matching3D->getTrueMatching().size() + 2, 0, new QTableWidgetItem(QString(tr("未匹配的提取树数据"))));
	//	for (size_t i = 0; i < _matching3D->getResOfExt().size(); i++)
	//	{
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 0, new QTableWidgetItem(QString::number(_matching3D->getIDOfReferenceTree()[_matching3D->getResOfExt()[i]])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 1, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfExt()[i]][0])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 2, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfExt()[i]][1])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 3, new QTableWidgetItem(QString::number(_matching3D->getHeightOfReferenceTree()[_matching3D->getResOfExt()[i]])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 4, new QTableWidgetItem(QString::number(_matching3D->getDBHOfReferenceTree()[_matching3D->getResOfExt()[i]])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 5, new QTableWidgetItem(QString::number(_matching3D->getMatchingFeaturesOfReferenceTree()[_matching3D->getResOfExt()[i]])));
	//	}

	//	_result->setSpan(2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 3, 0, 1, _result->columnCount());
	//	_result->setItem(2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 3, 0, new QTableWidgetItem(QString(tr("未匹配的参考树数据"))));

	//	for (size_t i = 0; i < _matching3D->getResOfRef().size(); i++)
	//	{
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 0, new QTableWidgetItem(QString::number(_matching3D->getIDOfReferenceTree()[_matching3D->getResOfRef()[i]])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 1, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfRef()[i]][0])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 2, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfRef()[i]][1])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 3, new QTableWidgetItem(QString::number(_matching3D->getHeightOfReferenceTree()[_matching3D->getResOfRef()[i]])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 4, new QTableWidgetItem(QString::number(_matching3D->getDBHOfReferenceTree()[_matching3D->getResOfRef()[i]])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 5, new QTableWidgetItem(QString::number(_matching3D->getMatchingFeaturesOfReferenceTree()[_matching3D->getResOfRef()[i]])));
	//	}

	//}
	//else
	//{
	//	_result->setColumnCount(5);
	//	header << tr("ID") << tr("X") << tr("Y") << tr("树高") << tr("胸径");
	//	_result->setHorizontalHeaderLabels(header);
	//	_result->setSpan(0, 0, 1, _result->columnCount());
	//	_result->setItem(0, 0, new QTableWidgetItem(QString(tr("提取树数据"))));
	//	for (size_t i = 0; i < _matching3D->getTrueMatching().size(); i++)
	//	{
	//		_result->setItem(i + 1, 0, new QTableWidgetItem(QString::number(_matching3D->getIDOfExtractedTree()[_matching3D->getTrueMatching()[i].first])));
	//		_result->setItem(i + 1, 1, new QTableWidgetItem(QString::number(_matching3D->getPositionOfExtrctedTree()[_matching3D->getTrueMatching()[i].first][0])));
	//		_result->setItem(i + 1, 2, new QTableWidgetItem(QString::number(_matching3D->getPositionOfExtrctedTree()[_matching3D->getTrueMatching()[i].first][1])));
	//		_result->setItem(i + 1, 3, new QTableWidgetItem(QString::number(_matching3D->getHeightOfExtractedTree()[_matching3D->getTrueMatching()[i].first])));
	//		_result->setItem(i + 1, 4, new QTableWidgetItem(QString::number(_matching3D->getDBHOfExtractedTree()[_matching3D->getTrueMatching()[i].first])));
	//	}

	//	_result->setSpan(_matching3D->getTrueMatching().size() + 1, 0, 1, _result->columnCount());
	//	_result->setItem(_matching3D->getTrueMatching().size() + 1, 0, new QTableWidgetItem(QString(tr("参考树数据"))));
	//	for (size_t i = 0; i < _matching3D->getTrueMatching().size(); i++)
	//	{
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 0, new QTableWidgetItem(QString::number(_matching3D->getIDOfReferenceTree()[_matching3D->getTrueMatching()[i].second])));
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 1, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getTrueMatching()[i].second][0])));
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 2, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getTrueMatching()[i].second][1])));
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 3, new QTableWidgetItem(QString::number(_matching3D->getHeightOfReferenceTree()[_matching3D->getTrueMatching()[i].first])));
	//		_result->setItem(i + _matching3D->getTrueMatching().size() + 2, 4, new QTableWidgetItem(QString::number(_matching3D->getDBHOfReferenceTree()[_matching3D->getTrueMatching()[i].second])));
	//	}

	//	_result->setSpan(2 * _matching3D->getTrueMatching().size() + 2, 0, 1, _result->columnCount());
	//	_result->setItem(2 * _matching3D->getTrueMatching().size() + 2, 0, new QTableWidgetItem(QString(tr("未匹配的提取树数据"))));
	//	for (size_t i = 0; i < _matching3D->getResOfExt().size(); i++)
	//	{
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 0, new QTableWidgetItem(QString::number(_matching3D->getIDOfReferenceTree()[_matching3D->getResOfExt()[i]])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 1, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfExt()[i]][0])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 2, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfExt()[i]][1])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 3, new QTableWidgetItem(QString::number(_matching3D->getHeightOfReferenceTree()[_matching3D->getResOfExt()[i]])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + 3, 4, new QTableWidgetItem(QString::number(_matching3D->getDBHOfReferenceTree()[_matching3D->getResOfExt()[i]])));
	//	}

	//	_result->setSpan(2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 3, 0, 1, _result->columnCount());
	//	_result->setItem(2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 3, 0, new QTableWidgetItem(QString(tr("未匹配的参考树数据"))));

	//	for (size_t i = 0; i < _matching3D->getResOfRef().size(); i++)
	//	{
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 0, new QTableWidgetItem(QString::number(_matching3D->getIDOfReferenceTree()[_matching3D->getResOfRef()[i]])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 1, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfRef()[i]][0])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 2, new QTableWidgetItem(QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfRef()[i]][1])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 3, new QTableWidgetItem(QString::number(_matching3D->getHeightOfReferenceTree()[_matching3D->getResOfRef()[i]])));
	//		_result->setItem(i + 2 * _matching3D->getTrueMatching().size() + _matching3D->getResOfExt().size() + 4, 4, new QTableWidgetItem(QString::number(_matching3D->getDBHOfReferenceTree()[_matching3D->getResOfRef()[i]])));
	//	}
	//}

	////header << "提取树ID" << "参考树ID";


	//_result->raise();
	//for (size_t i = 0; i < _matching3D->getTrueMatching().size(); i++)
	//{
	//	QTableWidgetItem* itemOfExt = new QTableWidgetItem(QString::number(_matching3D->getIDOfExtractedTree()[_matching3D->getTrueMatching()[i].first]));
	//	_result->setItem(i, 0, itemOfExt);
	//	itemOfExt->setToolTip("ID:" + QString::number(_matching3D->getIDOfExtractedTree()[_matching3D->getTrueMatching()[i].first]) + "\n" + "坐标:(" + QString::number(_matching3D->getPositionOfExtrctedTree()[_matching3D->getTrueMatching()[i].first][0]) + "," + QString::number(_matching3D->getPositionOfExtrctedTree()[_matching3D->getTrueMatching()[i].first][1]) + "," + QString::number(_matching3D->getPositionOfExtrctedTree()[_matching3D->getTrueMatching()[i].first][2]) + ")\n" + "胸径:" + QString::number(_matching3D->getDBHOfExtractedTree()[_matching3D->getTrueMatching()[i].first]) + "\n" + "树高:" + QString::number(_matching3D->getHeightOfExtractedTree()[_matching3D->getTrueMatching()[i].first]) + "\n" + "匹配特征:" + QString::number(_matching3D->getMatchingFeaturesOfExtractedTree()[_matching3D->getTrueMatching()[i].first])
	//	);
	//	QTableWidgetItem* itemOfRef = new QTableWidgetItem(QString::number(_matching3D->getIDOfReferenceTree()[_matching3D->getTrueMatching()[i].second]));
	//	_result->setItem(i, 1, itemOfRef);
	//	itemOfRef->setToolTip("ID:" + QString::number(_matching3D->getIDOfReferenceTree()[_matching3D->getTrueMatching()[i].second]) + "\n" + "坐标:(" + QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getTrueMatching()[i].second][0]) + "," + QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getTrueMatching()[i].second][1]) + "," + QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getTrueMatching()[i].second][2]) + ")\n" + "胸径:" + QString::number(_matching3D->getDBHOfReferenceTree()[_matching3D->getTrueMatching()[i].second]) + "\n" + "树高:" + QString::number(_matching3D->getHeightOfReferenceTree()[_matching3D->getTrueMatching()[i].second]) + "\n" + "匹配特征:" + QString::number(_matching3D->getMatchingFeaturesOfReferenceTree()[_matching3D->getTrueMatching()[i].second])
	//	);
	//}

	//_result->setItem(_matching3D->getTrueMatching().size(), 0, new QTableWidgetItem("未匹配的提取树ID"));
	//_result->item(_matching3D->getTrueMatching().size(), 0)->setForeground(Qt::red);
	//_result->item(_matching3D->getTrueMatching().size(), 0)->setBackground(Qt::lightGray);
	//_result->setItem(_matching3D->getTrueMatching().size(), 1, new QTableWidgetItem("未匹配的参考树ID"));
	//_result->item(_matching3D->getTrueMatching().size(), 1)->setForeground(Qt::red);
	//_result->item(_matching3D->getTrueMatching().size(), 1)->setBackground(Qt::lightGray);
	//for (size_t i = 0; i < _matching3D->getResOfExt().size(); i++)
	//{
	//	QTableWidgetItem* itemOfExt = new QTableWidgetItem(QString::number(_matching3D->getIDOfExtractedTree()[_matching3D->getResOfExt()[i]]));
	//	_result->setItem(_matching3D->getTrueMatching().size() + i + 1, 0, itemOfExt);
	//	_result->item(_matching3D->getTrueMatching().size() + i + 1, 0)->setForeground(Qt::red);
	//	itemOfExt->setToolTip("ID:" + QString::number(_matching3D->getIDOfExtractedTree()[_matching3D->getResOfExt()[i]]) + "\n" + "坐标:(" + QString::number(_matching3D->getPositionOfExtrctedTree()[_matching3D->getResOfExt()[i]][0]) + "," + QString::number(_matching3D->getPositionOfExtrctedTree()[_matching3D->getResOfExt()[i]][1]) + "," + QString::number(_matching3D->getPositionOfExtrctedTree()[_matching3D->getResOfExt()[i]][2]) + ")\n" + "胸径:" + QString::number(_matching3D->getDBHOfExtractedTree()[_matching3D->getResOfExt()[i]]) + "\n" + "树高:" + QString::number(_matching3D->getHeightOfExtractedTree()[_matching3D->getResOfExt()[i]]) + "\n" + "匹配特征:" + QString::number(_matching3D->getMatchingFeaturesOfExtractedTree()[_matching3D->getResOfExt()[i]])
	//	);

	//}
	//for (size_t i = 0; i < _matching3D->getResOfRef().size(); i++)
	//{
	//	QTableWidgetItem* itemOfRef = new QTableWidgetItem(QString::number(_matching3D->getIDOfReferenceTree()[_matching3D->getResOfRef()[i]]));
	//	_result->setItem(_matching3D->getTrueMatching().size() + i + 1, 1, itemOfRef);
	//	_result->item(_matching3D->getTrueMatching().size() + i + 1, 1)->setForeground(Qt::red);
	//	itemOfRef->setToolTip("ID:" + QString::number(_matching3D->getIDOfReferenceTree()[_matching3D->getResOfRef()[i]]) + "\n" + "坐标:(" + QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfRef()[i]][0]) + "," + QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfRef()[i]][1]) + "," + QString::number(_matching3D->getPositionOfReferenceTree()[_matching3D->getResOfRef()[i]][2]) + ")\n" + "胸径:" + QString::number(_matching3D->getDBHOfReferenceTree()[_matching3D->getResOfRef()[i]]) + "\n" + "树高:" + QString::number(_matching3D->getHeightOfReferenceTree()[_matching3D->getResOfRef()[i]]) + "\n" + "匹配特征:" + QString::number(_matching3D->getMatchingFeaturesOfReferenceTree()[_matching3D->getResOfRef()[i]])
	//	);
	//}
}

void MatchingFeatureEvaluation::initParameter()
{
	_columnOfMF->setText("0");
	_dimension->setText("2");
	_radius->setText("0.5");
}





