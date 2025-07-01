#include "../include/StemCurveEvaluation.h"

#include <QtWidgets/QProgressDialog>
#include <QtConcurrent/QtConcurrent>
#include <QProgressBar>
#include <QFileDialog>
#include <QHeaderView>
#include <QMainWindow>
#include <QToolTip>

StemCurveEvaluation::StemCurveEvaluation(QWidget* parent)
	:QDialog(parent, Qt::Tool),
	Ui::StemCurveEvaluation()
{
	this->setupUi(this);
	this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	connect(_selectInputFileOfPredictedTree, &QPushButton::clicked, this, &StemCurveEvaluation::selectedInputFileOfPredictedTree);
	connect(_selectedInputFileOfReferenceTree, &QPushButton::clicked, this, &StemCurveEvaluation::selectedInputFileOfReferencedTree);
	connect(_selectedInputFileOfIndex, &QPushButton::clicked, this, &StemCurveEvaluation::selectedInputFileOfIndex);
	connect(_selectedOututFile, &QPushButton::clicked, this, &StemCurveEvaluation::selectedOutputFile);
	connect(_btn_OK, &QPushButton::clicked, this, &StemCurveEvaluation::apply);

	initParameter();
}

StemCurveEvaluation::~StemCurveEvaluation()
{
}

void StemCurveEvaluation::apply()
{
	this->accept();
}

void StemCurveEvaluation::process(QProgressDialog* progress, std::shared_ptr<StemCurve>& _matching3D, QTextEdit* outputInformation, bool output_English)
{
	if (progress)
	{
		progress->setLabelText(tr("提取树结果评价……"));
		progress->setWindowFlags(progress->windowFlags() & ~Qt::WindowCloseButtonHint & ~Qt::WindowContextHelpButtonHint);
		progress->setCancelButton(nullptr);
		progress->show();
		progress->raise();
	}
	QFuture<void> future = QtConcurrent::run(std::bind(&StemCurveEvaluation::processingData, this, std::ref(_matching3D), outputInformation,output_English));
	while (!future.isFinished())
	{
		if (progress)
		{
			progress->setValue(progress->value() + 1);
			QApplication::processEvents();
		}
	}
}

void StemCurveEvaluation::selectedInputFileOfPredictedTree()
{
	QStringList fileTypes;
	fileTypes << "Text Files (*.txt)";
	QString file = QFileDialog::getOpenFileName(this, tr("选择提取结果参数文件"), "", fileTypes.join(";;"));
	if (file.isEmpty())
		return;
	_inputFileOfPredictedTree->setText(file);
}

void StemCurveEvaluation::selectedInputFileOfReferencedTree()
{
	QStringList fileTypes;
	fileTypes << "Text Files (*.txt)";
	QString file = QFileDialog::getOpenFileName(this, tr("选择参考结果参数文件"), "", fileTypes.join(";;"));
	if (file.isEmpty())
		return;
	_inputFileOfReferenceTree->setText(file);
}

void StemCurveEvaluation::selectedInputFileOfIndex()
{
	QStringList fileTypes;
	fileTypes << "Text Files (*.txt)";
	QString file = QFileDialog::getOpenFileName(this, tr("选择匹配索引文件"), "", fileTypes.join(";;"));
	if (file.isEmpty())
		return;
	_inputFileOfIndex->setText(file);
}

void StemCurveEvaluation::selectedOutputFile()
{
	QStringList fileTypes;
	fileTypes << "Excel Files (*.xlsx)";
	QString file = QFileDialog::getOpenFileName(this, tr("选择输出文件"), "", fileTypes.join(";;"));
	if (file.isEmpty())
		return;
	_outputFile->setText(file);
}

void StemCurveEvaluation::processingData(std::shared_ptr<StemCurve>& _matching3D, QTextEdit* outputInformation, bool output_English)
{
	double radius = 1.0;
	if (_inputFileOfIndex->text().isEmpty())
	{
		if (_radius->text().isEmpty())
		{
			return;
		}
		else
		{
			radius = QString(_radius->text()).toDouble();
			if (radius <= 0)
			{
				outputInformation->insertPlainText(QObject::tr("匹配搜索半径输入存在问题！") + "\n");
				outputInformation->insertPlainText(QObject::tr("-----------------------------") + "\n");
				return;
			}
		}
	}
	//StemCurve function initialization
	_matching3D = std::make_shared<StemCurve>(outputInformation);
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
	_matching3D->loadMatchingRelationship(_fileOfOfIndex, radius);
	_matching3D->exportFile(_outputPath, output_English);


	//_resOfExt = _matching3D->getResOfExt().size();
	//_resOfRef = _matching3D->getResOfRef().size();
}

void StemCurveEvaluation::initParameter()
{
	_radius->setText("0.5");
}

