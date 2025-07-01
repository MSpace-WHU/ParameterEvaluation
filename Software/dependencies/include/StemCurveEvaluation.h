#pragma once
#include "ui_StemCurveEvaluation.h"
#include "StemCurve.h"

#include <QtWidgets/QDialog>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QTableWidget>
#include <qprogressdialog.h>

class StemCurveEvaluation :public QDialog, public Ui::StemCurveEvaluation
{
	Q_OBJECT
public:
		explicit StemCurveEvaluation(QWidget* parent = nullptr);
		~StemCurveEvaluation();

		void process(QProgressDialog* progress, std::shared_ptr<StemCurve>& _matching3D, QTextEdit* outputInformation, bool output_English);

private slots:
	void apply();
	//select input file of predicted result
	void selectedInputFileOfPredictedTree();
	//select input file of referenced result
	void selectedInputFileOfReferencedTree();
	//select input file of matching index
	void selectedInputFileOfIndex();
	//select output file of statistical result
	void selectedOutputFile();
	//data processing 
	void processingData(std::shared_ptr<StemCurve>& _matching3D, QTextEdit* outputInformation, bool output_English);

private:

	void initParameter();

	size_t _resOfExt;
	size_t _resOfRef;
};
