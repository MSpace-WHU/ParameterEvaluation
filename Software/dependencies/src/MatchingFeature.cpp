#include "../include/MatchingFeature.h"

MatchingFeature::MatchingFeature(QTextEdit* outputInformation)
{
	logger = outputInformation;
	relationships = std::make_shared<Relationship>(outputInformation);
}

MatchingFeature::~MatchingFeature()
{

}

void MatchingFeature::loadFile(std::string inputPath_Predicted, std::string inputPath_Reference)
{
	
	if (loadMatchingFeaturePredictedFile(inputPath_Predicted) == false)
	{
		if (logger != nullptr)
		{

			logger->insertPlainText(QObject::tr("---------------------") + "\n");
		}
	}
	else
	{
		if (logger != nullptr)
			logger->insertPlainText(QObject::tr("文件加载：参数预测文件已加载!") + "\n");
	}

	if (loadMatchingFeatureReferenceFile(inputPath_Reference) == false)
	{
		if (logger != nullptr)
			logger->insertPlainText(QObject::tr("---------------------") + "\n");
	}
	else
	{
		if (logger != nullptr)
			logger->insertPlainText(QObject::tr("文件加载：参数参考文件已加载!") + "\n");
	}
}

void MatchingFeature::loadMatchingRelationship(std::string inputPath_Relationship, uint8_t inputDimension, double inputRadius, size_t column)
{
	//set ID
	relationships->setID(PredictedID, ReferenceID);
	//set coordinate and feature set
	relationships->setCoordiante(PredictedCoordinate, ReferenceCoordinate);


	if (!inputPath_Relationship.empty())
	{
		//load relationship file
		relationships->loadRelationshipFile(inputPath_Relationship);
		if (logger != nullptr)
			logger->insertPlainText(QObject::tr("文件加载：匹配关系文件已加载!") + "\n");
	}
	else
	{
		//security detection
		if (PredictedFeatures.empty() || ReferenceFeatures.empty())
		{
			if (column != 0)
			{
				logger->insertPlainText(QObject::tr("匹配关系计算：文件和输入的参数有逻辑错误！") + "\n");
				return;
			}
		}
		else
		{
			relationships->setFeature(PredictedFeatures, ReferenceFeatures);
		}
		//calculate matching relationship
		relationships->calculateMatchingRelationship(inputDimension, inputRadius, column);
		if (logger != nullptr)
			logger->insertPlainText(QObject::tr("匹配关系计算：匹配关系文件计算完成!") + "\n");
	}
}

void MatchingFeature::exportFile(std::string outputPath, bool outputEnglish)
{
	if (statisticalResults(outputPath, outputEnglish) == true)
	{
		if (logger != nullptr)
		{
			logger->insertPlainText(QObject::tr("结果输出：结果输出完成！") + "\n");
			logger->insertPlainText(QObject::tr("---------------------") + "\n");
		}
	}
	else
	{
		if (logger != nullptr)
			logger->insertPlainText(QObject::tr("结果输出：文件未成功导出，建议使用英文路径！") + "\n");
	}
}

std::shared_ptr<Relationship> MatchingFeature::getRelationsip()
{
	return relationships;
}

std::vector<std::vector<double>> MatchingFeature::getPredictedCoordinate()
{
	return PredictedCoordinate;
}

std::vector<std::vector<double>> MatchingFeature::getReferenceCoordinate()
{
	return ReferenceCoordinate;
}

bool MatchingFeature::loadMatchingFeaturePredictedFile(std::string inputPath)
{
	//make the variable an empty set
	std::vector<std::string>().swap(PredictedID);
	std::vector<std::vector<double>>().swap(PredictedCoordinate);
	std::vector<std::vector<double>>().swap(PredictedFeatures);

	//read pedicted file
	std::filesystem::path filePath = inputPath;
	//check if pedicted file exists
	if (std::filesystem::exists(filePath))
	{
		std::ifstream file(filePath);
		if (file.is_open())
		{
			std::string line;
			while (std::getline(file,line))
			{
				// use separator to separate each line
				std::vector<std::string> words;
				std::stringstream iss(line);
				std::string word;
				while (iss>>word)
				{
					words.push_back(word);
				}
				//security detection
				if (words.size() < 4)
				{
					logger->insertPlainText(QObject::tr("文件加载：请检查数据格式！") + "\n");
					return false;
				}
				//read the data in predicted file according to the format
				//record ID
				PredictedID.push_back(words[0]);
				//record coordinate
				std::vector<double> tmpCoordinate;
				for (size_t i = 1; i < 4; i++)
				{
					if (words[i] == "NaN" || words[i] == "nan")
						continue;
					tmpCoordinate.push_back(std::stod(words[i]));
				}
				PredictedCoordinate.push_back(tmpCoordinate);
				//record features
				std::vector<double> tmpFeatures;
				for (size_t i = 4; i < words.size(); i++)
				{
					double value;
					if (words[i] == "NaN" || words[i] == "nan")
						value = DBL_MAX;
					else
						value = stod(words[i]);
					tmpFeatures.push_back(value);
				}
				PredictedFeatures.push_back(tmpFeatures);
			}
		}
	}
	return true;
}

bool MatchingFeature::loadMatchingFeatureReferenceFile(std::string inputPath)
{
	//make the variable an empty set
	std::vector<std::string>().swap(ReferenceID);
	std::vector<std::vector<double>>().swap(ReferenceCoordinate);
	std::vector<std::vector<double>>().swap(ReferenceFeatures);

	//read pedicted file
	std::filesystem::path filePath = inputPath;
	//check if pedicted file exists
	if (std::filesystem::exists(filePath))
	{
		std::ifstream file(filePath);
		if (file.is_open())
		{
			std::string line;
			while (std::getline(file, line))
			{
				// use separator to separate each line
				std::vector<std::string> words;
				std::stringstream iss(line);
				std::string word;
				while (iss >> word)
				{
					words.push_back(word);
				}
				if (words.size() < 4)
				{
					logger->insertPlainText(QObject::tr("文件加载：请检查数据格式！") + "\n");
					return false;
				}
				//read the data in predicted file according to the format
				//record ID
				ReferenceID.push_back(words[0]);
				//record coordinate
				std::vector<double> tmpCoordinate;
				for (size_t i = 1; i < 4; i++)
				{
					if (words[i] == "NaN" || words[i] == "nan")
						continue;
					tmpCoordinate.push_back(std::stod(words[i]));
				}
				ReferenceCoordinate.push_back(tmpCoordinate);
				//record features
				std::vector<double> tmpFeatures;
				for (size_t i = 4; i < words.size(); i++)
				{
					double value;
					if (words[i] == "NaN" || words[i] == "nan")
						value = DBL_MAX;
					else
						value = stod(words[i]);
					tmpFeatures.push_back(value);
				}
				ReferenceFeatures.push_back(tmpFeatures);
			}
		}
	}
	return true;
}

bool MatchingFeature::statisticalResults(std::string outputPath, bool outputEnglish)
{


	//calucate statistical result
	size_t truePositive = relationships->getRelationshipIndexs().size();
	size_t falsePositive = PredictedID.size() - truePositive;
	size_t falseNegative = ReferenceID.size() - truePositive;
	double recall = (double)truePositive / (double)ReferenceID.size();
	double precision = (double)truePositive / (double)PredictedID.size();
	double meanAccuracy = 2 * (double)truePositive / (double)(PredictedID.size() + ReferenceID.size());

	//evaluate accuracy of location
	//root mean square error(rmse) of location
	double locationRMSE = 0.0;
	for (size_t i = 0; i < relationships->getRelationshipIndexs().size(); i++)
	{
		locationRMSE += std::pow(distance(relationships->getPredictedCoordinate()[relationships->getRelationshipIndexs()[i].first], relationships->getReferenceCoordinate()[relationships->getRelationshipIndexs()[i].second]), 2);
	}
	locationRMSE /= relationships->getRelationshipIndexs().size();
	locationRMSE = sqrt(locationRMSE);

	//output excel table
	xlnt::workbook outputExcelFile;
	auto workSheet = outputExcelFile.active_sheet();

	xlnt::alignment align;
	align.horizontal(xlnt::horizontal_alignment::center);
	align.vertical(xlnt::vertical_alignment::center);

	xlnt::border::border_property border_pro;
	border_pro.style(xlnt::border_style::thin);

	xlnt::border border;
	border.side(xlnt::border_side::top, border_pro);
	border.side(xlnt::border_side::bottom, border_pro);
	border.side(xlnt::border_side::start, border_pro);
	border.side(xlnt::border_side::end, border_pro);

	auto fillGray = xlnt::fill::solid(xlnt::color(xlnt::rgb_color(192, 192, 192, 204)));
	auto filllowGray = xlnt::fill::solid(xlnt::color(xlnt::rgb_color(192, 192, 192, 150)));

	if (outputEnglish)
	{
		//relationship index 
		workSheet.merge_cells("A1:B1");

		workSheet.cell("A1").alignment(align);
		workSheet.cell("A1").fill(fillGray);
		workSheet.cell("A1").border(border);
		workSheet.cell("B1").border(border);
		workSheet.cell("A1").value("The Matching ID of Trees");

		workSheet.cell("A2").alignment(align);
		workSheet.cell("A2").fill(filllowGray);
		workSheet.cell("A2").border(border);
		workSheet.cell("A2").value("ID of The Predicted Tree");

		workSheet.cell("B2").alignment(align);
		workSheet.cell("B2").fill(filllowGray);
		workSheet.cell("B2").border(border);
		workSheet.cell("B2").value("ID of The Reference Tree");

		for (size_t i = 0; i < relationships->getRelationshipIDs().size(); i++)
		{
			workSheet.cell("A" + std::to_string(i + 3)).alignment(align);
			workSheet.cell("A" + std::to_string(i + 3)).border(border);
			workSheet.cell("A" + std::to_string(i + 3)).value(relationships->getRelationshipIDs()[i].first);

			workSheet.cell("B" + std::to_string(i + 3)).alignment(align);
			workSheet.cell("B" + std::to_string(i + 3)).border(border);
			workSheet.cell("B" + std::to_string(i + 3)).value(relationships->getRelationshipIDs()[i].second);
		}

		//evaluation of matching results
		workSheet.merge_cells("D1:E1");

		workSheet.cell("D1").alignment(align);
		workSheet.cell("D1").fill(fillGray);
		workSheet.cell("D1").border(border);
		workSheet.cell("E1").border(border);
		workSheet.cell("D1").value("Evaluation of Matching Results");

		workSheet.cell("D2").alignment(align);
		workSheet.cell("D2").border(border);
		workSheet.cell("D2").value("TP(True Positive)");

		workSheet.cell("E2").alignment(align);
		workSheet.cell("E2").border(border);
		workSheet.cell("E2").value(std::to_string(truePositive));

		workSheet.cell("D3").alignment(align);
		workSheet.cell("D3").border(border);
		workSheet.cell("D3").value("FP(False Positive)");

		workSheet.cell("E3").alignment(align);
		workSheet.cell("E3").border(border);
		workSheet.cell("E3").value(std::to_string(falsePositive));

		workSheet.cell("D4").alignment(align);
		workSheet.cell("D4").border(border);
		workSheet.cell("D4").value("FN(False Negative)");

		workSheet.cell("E4").alignment(align);
		workSheet.cell("E4").border(border);
		workSheet.cell("E4").value(std::to_string(falseNegative));

		workSheet.cell("D5").alignment(align);
		workSheet.cell("D5").border(border);
		workSheet.cell("D5").value("Recall");

		workSheet.cell("E5").alignment(align);
		workSheet.cell("E5").border(border);
		workSheet.cell("E5").value(std::to_string(recall));

		workSheet.cell("D6").alignment(align);
		workSheet.cell("D6").border(border);
		workSheet.cell("D6").value("Matching Precision");

		workSheet.cell("E6").alignment(align);
		workSheet.cell("E6").border(border);
		workSheet.cell("E6").value(std::to_string(precision));

		workSheet.cell("D7").alignment(align);
		workSheet.cell("D7").border(border);
		workSheet.cell("D7").value("Mean Accuracy");

		workSheet.cell("E7").alignment(align);
		workSheet.cell("E7").border(border);
		workSheet.cell("E7").value(std::to_string(meanAccuracy));

		//evaluation of positional precision
		workSheet.merge_cells("G1:H1");

		workSheet.cell("G1").alignment(align);
		workSheet.cell("G1").border(border);
		workSheet.cell("H1").border(border);
		workSheet.cell("G1").fill(fillGray);
		workSheet.cell("G1").value("Positional Precision");

		workSheet.cell("G2").alignment(align);
		workSheet.cell("G2").border(border);
		workSheet.cell("G2").value("RMSE of location");

		workSheet.cell("H2").alignment(align);
		workSheet.cell("H2").border(border);
		workSheet.cell("H2").value(std::to_string(locationRMSE));
	}
	else
	{
		//relationship index 
		workSheet.merge_cells("A1:B1");

		workSheet.cell("A1").alignment(align);
		workSheet.cell("A1").border(border);
		workSheet.cell("B1").border(border);
		workSheet.cell("A1").fill(fillGray);
		std::wstring wideString = L"树木ID的匹配关系"; // // wide strings (UTF-16 or UTF-32, depending on platform)
		// convert wide strings of characters to UTF-8 encoding
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::string utf8String = converter.to_bytes(wideString);
		workSheet.cell("A1").value(utf8String);

		workSheet.cell("A2").alignment(align);
		workSheet.cell("A2").fill(filllowGray);
		workSheet.cell("A2").border(border);
		wideString = L"预测树的ID";
		utf8String = converter.to_bytes(wideString);
		workSheet.cell("A2").value(utf8String);

		workSheet.cell("B2").alignment(align);
		workSheet.cell("B2").fill(filllowGray);
		workSheet.cell("B2").border(border);
		wideString = L"参考树的ID";
		utf8String = converter.to_bytes(wideString);
		workSheet.cell("B2").value(utf8String);

		for (size_t i = 0; i < relationships->getRelationshipIDs().size(); i++)
		{
			workSheet.cell("A" + std::to_string(i + 3)).alignment(align);
			workSheet.cell("A" + std::to_string(i + 3)).border(border);
			workSheet.cell("A" + std::to_string(i + 3)).value(relationships->getRelationshipIDs()[i].first);
			workSheet.cell("B" + std::to_string(i + 3)).alignment(align);
			workSheet.cell("B" + std::to_string(i + 3)).border(border);
			workSheet.cell("B" + std::to_string(i + 3)).value(relationships->getRelationshipIDs()[i].second);
		}

		//evaluation of matching results
		workSheet.merge_cells("D1:E1");

		workSheet.cell("D1").alignment(align);
		workSheet.cell("D1").fill(fillGray);
		workSheet.cell("D1").border(border);
		workSheet.cell("E1").border(border);
		wideString = L"匹配结果评价";
		utf8String = converter.to_bytes(wideString);
		workSheet.cell("D1").value(utf8String);

		workSheet.cell("D2").alignment(align);
		workSheet.cell("D2").border(border);
		wideString = L"TP(真正例)";
		utf8String = converter.to_bytes(wideString);
		workSheet.cell("D2").value(utf8String);
		workSheet.cell("E2").alignment(align);
		workSheet.cell("E2").border(border);
		workSheet.cell("E2").value(std::to_string(truePositive));

		workSheet.cell("D3").alignment(align);
		workSheet.cell("D3").border(border);
		wideString = L"FP(假正例)";
		utf8String = converter.to_bytes(wideString);
		workSheet.cell("D3").value(utf8String);
		workSheet.cell("E3").alignment(align);
		workSheet.cell("E3").border(border);
		workSheet.cell("E3").value(std::to_string(falsePositive));

		workSheet.cell("D4").alignment(align);
		workSheet.cell("D4").border(border);
		wideString = L"FN(假反例)";
		utf8String = converter.to_bytes(wideString);
		workSheet.cell("D4").value(utf8String);
		workSheet.cell("E4").alignment(align);
		workSheet.cell("E4").border(border);
		workSheet.cell("E4").value(std::to_string(falseNegative));

		workSheet.cell("D5").alignment(align);
		workSheet.cell("D5").border(border);
		wideString = L"召回率";
		utf8String = converter.to_bytes(wideString);
		workSheet.cell("D5").value(utf8String);
		workSheet.cell("E5").alignment(align);
		workSheet.cell("E5").border(border);
		workSheet.cell("E5").value(std::to_string(recall));

		workSheet.cell("D6").alignment(align);
		workSheet.cell("D6").border(border);
		wideString = L"匹配精度";
		utf8String = converter.to_bytes(wideString);
		workSheet.cell("D6").value(utf8String);
		workSheet.cell("E6").alignment(align);
		workSheet.cell("E6").border(border);
		workSheet.cell("E6").value(std::to_string(precision));

		workSheet.cell("D7").alignment(align);
		workSheet.cell("D7").border(border);
		wideString = L"平均精度";
		utf8String = converter.to_bytes(wideString);
		workSheet.cell("D7").value(utf8String);
		workSheet.cell("E7").alignment(align);
		workSheet.cell("E7").border(border);
		workSheet.cell("E7").value(std::to_string(meanAccuracy));

		//evaluation of positional precision
		workSheet.merge_cells("G1:H1");

		workSheet.cell("G1").alignment(align);
		workSheet.cell("G1").border(border);
		workSheet.cell("H1").border(border);
		workSheet.cell("G1").fill(fillGray);
		wideString = L"位置精度";
		utf8String = converter.to_bytes(wideString);
		workSheet.cell("G1").value(utf8String);

		workSheet.cell("G2").alignment(align);
		workSheet.cell("G2").border(border);
		wideString = L"距离均方根误差";
		utf8String = converter.to_bytes(wideString);
		workSheet.cell("G2").value(utf8String);
		workSheet.cell("H2").alignment(align);
		workSheet.cell("H2").border(border);
		workSheet.cell("H2").value(std::to_string(locationRMSE));

	}

	//security detection
	if (PredictedFeatures.empty() || ReferenceFeatures.empty())
		return false;
	
	size_t numberOfFeature = std::min(PredictedFeatures[0].size(), ReferenceFeatures[0].size());

	size_t column = 10;
	for (size_t i = 0; i < numberOfFeature; i++)
	{
		std::vector<std::pair<double, double>> PredictedandReferenceFeatureSet;
		double allReferenceFeature = 0.0; 
		size_t allReferenceNumber = 0;

		//find features in each column of the predicted and reference results
		for (size_t j = 0; j < relationships->getRelationshipIndexs().size(); j++)
		{
			if (PredictedFeatures[relationships->getRelationshipIndexs()[j].first][i] != DBL_MAX &&
				ReferenceFeatures[relationships->getRelationshipIndexs()[j].second][i] != DBL_MAX)
			{
				PredictedandReferenceFeatureSet.push_back(std::make_pair(PredictedFeatures[relationships->getRelationshipIndexs()[j].first][i], ReferenceFeatures[relationships->getRelationshipIndexs()[j].second][i]));
			}
			if (ReferenceFeatures[relationships->getRelationshipIndexs()[j].second][i] != DBL_MAX)
			{
				allReferenceFeature += ReferenceFeatures[relationships->getRelationshipIndexs()[j].second][i];
				allReferenceNumber++;
			}
		}

		//calucate RMSE and Bias
		double RMSE = 0.0, Bias = 0.0;
		for (size_t j = 0; j < PredictedandReferenceFeatureSet.size(); j++)
		{
			RMSE += pow(PredictedandReferenceFeatureSet[j].first - PredictedandReferenceFeatureSet[j].second, 2);
			Bias += PredictedandReferenceFeatureSet[j].first - PredictedandReferenceFeatureSet[j].second;
		}
		RMSE /= PredictedandReferenceFeatureSet.size();
		RMSE = sqrt(RMSE);

		Bias /= PredictedandReferenceFeatureSet.size();
		allReferenceFeature /= allReferenceNumber;
		//Relative RMSE %
		double relativeRMSE = 100 * RMSE / allReferenceFeature;
		//Relative Bias %
		double relativeBias = 100 * Bias / allReferenceFeature;


		if (outputEnglish)
		{
			workSheet.merge_cells(xlnt::range_reference(xlnt::cell_reference(column, 1), xlnt::cell_reference(column + 1, 1)));

			workSheet.cell(xlnt::cell_reference(column, 1)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column, 1)).fill(fillGray);
			workSheet.cell(xlnt::cell_reference(column, 1)).border(border);
			workSheet.cell(xlnt::cell_reference(column + 1, 1)).border(border);
			workSheet.cell(xlnt::cell_reference(column, 1)).value("Evaluation of Basic Feature Prediction for Group" + std::to_string(i + 1));

			workSheet.cell(xlnt::cell_reference(column, 2)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column, 2)).border(border);
			workSheet.cell(xlnt::cell_reference(column, 2)).value("RMSE");
			workSheet.cell(xlnt::cell_reference(column + 1, 2)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column + 1, 2)).border(border);
			workSheet.cell(xlnt::cell_reference(column + 1, 2)).value(RMSE);

			workSheet.cell(xlnt::cell_reference(column, 3)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column, 3)).border(border);
			workSheet.cell(xlnt::cell_reference(column, 3)).value("Bias");
			workSheet.cell(xlnt::cell_reference(column + 1, 3)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column + 1, 3)).border(border);
			workSheet.cell(xlnt::cell_reference(column + 1, 3)).value(Bias);

			workSheet.cell(xlnt::cell_reference(column, 4)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column, 4)).border(border);
			workSheet.cell(xlnt::cell_reference(column, 4)).value("RMSE%");
			workSheet.cell(xlnt::cell_reference(column + 1, 4)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column + 1, 4)).border(border);
			workSheet.cell(xlnt::cell_reference(column + 1, 4)).value(relativeRMSE);

			workSheet.cell(xlnt::cell_reference(column, 5)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column, 5)).border(border);
			workSheet.cell(xlnt::cell_reference(column, 5)).value("Bias%");
			workSheet.cell(xlnt::cell_reference(column + 1, 5)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column + 1, 5)).border(border);
			workSheet.cell(xlnt::cell_reference(column + 1, 5)).value(relativeBias);

		}
		else
		{
			workSheet.merge_cells(xlnt::range_reference(xlnt::cell_reference(column, 1), xlnt::cell_reference(column + 1, 1)));

			workSheet.cell(xlnt::cell_reference(column, 1)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column, 1)).fill(fillGray);
			workSheet.cell(xlnt::cell_reference(column, 1)).border(border);
			workSheet.cell(xlnt::cell_reference(column + 1, 1)).border(border);
			std::wstring wideString = L"第" + std::to_wstring(i + 1) + L"组基本特征预测评价";

			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			std::string utf8String = converter.to_bytes(wideString);
			workSheet.cell(xlnt::cell_reference(column, 1)).value(utf8String);

			workSheet.cell(xlnt::cell_reference(column, 2)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column, 2)).border(border);
			wideString = L"均方根误差(RMSE)";
			utf8String = converter.to_bytes(wideString);
			workSheet.cell(xlnt::cell_reference(column, 2)).value(utf8String);
			workSheet.cell(xlnt::cell_reference(column + 1, 2)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column + 1, 2)).border(border);
			workSheet.cell(xlnt::cell_reference(column + 1, 2)).value(RMSE);

			workSheet.cell(xlnt::cell_reference(column, 3)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column, 3)).border(border);
			wideString = L"偏差(Bias)";
			utf8String = converter.to_bytes(wideString);
			workSheet.cell(xlnt::cell_reference(column, 3)).value(utf8String);
			workSheet.cell(xlnt::cell_reference(column + 1, 3)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column + 1, 3)).border(border);
			workSheet.cell(xlnt::cell_reference(column + 1, 3)).value(Bias);

			workSheet.cell(xlnt::cell_reference(column, 4)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column, 4)).border(border);
			wideString = L"相对均方根误差（RMSE%）";
			utf8String = converter.to_bytes(wideString);
			workSheet.cell(xlnt::cell_reference(column, 4)).value(utf8String);
			workSheet.cell(xlnt::cell_reference(column + 1, 4)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column + 1, 4)).border(border);
			workSheet.cell(xlnt::cell_reference(column + 1, 4)).value(relativeRMSE);

			workSheet.cell(xlnt::cell_reference(column, 5)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column, 5)).border(border);
			wideString = L"相对偏差（Bias%）";
			utf8String = converter.to_bytes(wideString);
			workSheet.cell(xlnt::cell_reference(column, 5)).value(utf8String);
			workSheet.cell(xlnt::cell_reference(column + 1, 5)).alignment(align);
			workSheet.cell(xlnt::cell_reference(column + 1, 5)).border(border);
			workSheet.cell(xlnt::cell_reference(column + 1, 5)).value(relativeBias);
		}
		column += 3;

	}
	//prevent failure to output Chinese path files

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::wstring wideOutputPath = converter.from_bytes(outputPath); // wide strings (UTF-16 or UTF-32, depending on platform)

	// convert wide strings of characters to UTF-8 encoding
	std::string utf8OutputPath = converter.to_bytes(wideOutputPath);
	outputExcelFile.save(utf8OutputPath);

	return true;
}

double MatchingFeature::distance(std::vector<double> PredictedCoordinate, std::vector<double> ReferenceCoordinate)
{
	double sum = 0;
	for (size_t i = 0; i < PredictedCoordinate.size(); i++)
	{
		sum += pow(PredictedCoordinate[i] - ReferenceCoordinate[i], 2);
	}
	return sqrt(sum);
}

