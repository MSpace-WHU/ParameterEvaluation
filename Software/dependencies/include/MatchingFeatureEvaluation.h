#pragma once
#include "ui_MatchingFeatureEvaluation.h"
#include "MatchingFeature.h"

#include <QtWidgets/QDialog>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QTableWidget>
#include <qprogressdialog.h>
#include <QtWidgets/QTextEdit>

class MatchingFeatureEvaluation :public QDialog, public Ui::MatchingFeatureEvaluation
{
	Q_OBJECT
public:
	explicit MatchingFeatureEvaluation(QWidget* parent = nullptr);
	~MatchingFeatureEvaluation();

	void process(QProgressDialog* progress, std::shared_ptr<MatchingFeature>& _matching3D, QTextEdit* outputInformation, bool output_English);

	bool isShowTable();

private slots:
	void apply();
	//select input file of predicted result
	void selectedInputFileOfPredictedTree();
	//select input file of referenced result
	void selectedInputFileOfReferenceedTree();
	//select input file of matching index
	void selectedInputFileOfIndex();
	//select output file of statistical result
	void selectedOutputFile();
	//data processing 
	void processingData(std::shared_ptr<MatchingFeature>& _matching3D, QTextEdit* outputInformation, bool output_English);



private:

	//show data table in window
	void showResult();

	void initParameter();

	bool _isShowTable;
	size_t _resOfExt;
	size_t _resOfRef;

};