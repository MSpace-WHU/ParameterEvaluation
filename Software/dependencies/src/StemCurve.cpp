#include "../include/StemCurve.h"

StemCurve::StemCurve(QTextEdit* outputInformation)
{
    logger = outputInformation;
    relationships = std::make_shared<Relationship>(outputInformation);
}

StemCurve::~StemCurve()
{
}

void StemCurve::loadFile(std::string inputPath_Predicted, std::string inputPath_Reference)
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
			logger->insertPlainText(QObject::tr("文件加载：干曲线预测文件已加载!") + "\n");
	}

	if (loadMatchingFeatureReferenceFile(inputPath_Reference) == false)
	{
		if (logger != nullptr)
			logger->insertPlainText(QObject::tr("---------------------") + "\n");
	}
	else
	{
		if (logger != nullptr)
			logger->insertPlainText(QObject::tr("文件加载：干曲线参考文件已加载!") + "\n");
	}
}

void StemCurve::loadMatchingRelationship(std::string inputPath_Relationship, double inputRadius)
{
    //set ID
    relationships->setID(PredictedID, ReferenceID);


    if (!inputPath_Relationship.empty())
    {
        //load relationship file
        relationships->loadRelationshipFile(inputPath_Relationship);
    }
    else
    {
        //security detection
        if (cacluateDBHandCoordinate(1.3) == true)
        {
            if (logger != nullptr)
                logger->insertPlainText(QObject::tr("匹配关系计算：DBH和坐标计算完成!") + "\n"); 
        }

        //security detection
        if (PredictedCoordinateDBH.empty() || ReferenceCoordinateDBH.empty())
        {
            if (logger != nullptr)
                logger->insertPlainText(QObject::tr("匹配关系计算：坐标计算结果不存在!") + "\n");
            return;
        }
        if (PredictedFeaturesDBH.empty() || ReferenceFeaturesDBH.empty())
        {
            if (logger != nullptr)
                logger->insertPlainText(QObject::tr("匹配关系计算：DBH计算结果不存在!") + "\n");
            return;
        }
        //set coordinate and feature set
        relationships->setCoordiante(PredictedCoordinateDBH, ReferenceCoordinateDBH);
        relationships->setFeature(PredictedFeaturesDBH, ReferenceFeaturesDBH);
        //calculate matching relationship
        relationships->calculateMatchingRelationship(2, inputRadius, 0);
    }
}

void StemCurve::exportFile(std::string outputPath, bool outputEnglish)
{
    if (statisticalResults(outputPath, outputEnglish) == true)
    {
        logger->insertPlainText(QObject::tr("结果输出：结果输出完成！") + "\n");
        logger->insertPlainText(QObject::tr("---------------------") + "\n");
    }
    else
    {
        if (logger != nullptr)
            logger->insertPlainText(QObject::tr("结果输出：文件未成功导出，建议使用英文路径！") + "\n");
    }
}

bool StemCurve::loadMatchingFeaturePredictedFile(std::string inputPath)
{
    //文件全部置零
    std::vector<std::vector<double>>().swap(PredictedDiameterSet);
    std::vector<std::vector<std::vector<double>>>().swap(PredictedCoordinateSet);
    std::vector<std::vector<double>>().swap(PredictedHeightSet);
    std::vector<std::string>().swap(PredictedID);


    //读取提取树文件中的数据
    std::filesystem::path filePath = inputPath;
    if (std::filesystem::exists(filePath)) {
        std::ifstream file(filePath);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {

                std::vector<std::string> IDSet;
                //obtain diameter value
                std::istringstream iss_dia(line);
                std::string word_dia;
                std::vector<double> diameterSet;
                size_t index = 0;
                while (iss_dia >> word_dia) {
                    if (index == 0)
                    {
                        IDSet.push_back(word_dia);
                    }
                    else
                    {
                        if (word_dia == "NaN" || word_dia == "nan")
                        {
                            diameterSet.push_back(DBL_MAX);
                        }
                        else
                        {
                            diameterSet.push_back(stod(word_dia));
                        }
                    }
                    index++;
                }
                if (diameterSet.empty())
                {
                    logger->insertPlainText(QObject::tr("文件加载：直径数据存在明显缺失！") + "\n");
                    return false;
                }

                PredictedDiameterSet.push_back(diameterSet);

                //obtain XY coordinate value
                if (!std::getline(file, line))
                    return false;
                std::istringstream iss_x(line);
                std::string word_x;
                std::vector<double> CoordinateXSet;
                index = 0;
                while (iss_x >> word_x)
                {
                    if (index == 0)
                    {
                        IDSet.push_back(word_x);
                    }
                    else
                    {
                        if (word_x == "NaN" || word_x == "nan")
                        {
                            CoordinateXSet.push_back(DBL_MAX);
                        }
                        else
                        {
                            CoordinateXSet.push_back(stod(word_x));
                        }

                    }
                    index++;
                }
                if (CoordinateXSet.empty())
                {
                    logger->insertPlainText(QObject::tr("文件加载：坐标X数据存在明显缺失！") + "\n");
                    return false;
                }
                if (!std::getline(file, line))
                    return false;
                std::istringstream iss_y(line);
                std::string word_y;
                std::vector<double> CoordinateYSet;
                index = 0;
                while (iss_y >> word_y)
                {
                    if (index == 0)
                    {
                        IDSet.push_back(word_y);
                    }
                    else
                    {
                        if (word_y == "NaN" || word_y == "nan")
                        {
                            CoordinateYSet.push_back(DBL_MAX);
                        }
                        else
                        {
                            CoordinateYSet.push_back(stod(word_y));
                        }
                    }
                    index++;
                }
                if (CoordinateYSet.empty())
                {
                    logger->insertPlainText(QObject::tr("文件加载：坐标Y数据存在明显缺失！") + "\n");
                    return false;
                }

                std::vector<std::vector<double>> PredictedCoordinate;
                if (CoordinateXSet.size() == CoordinateYSet.size())
                {
                    for (size_t i = 0; i < CoordinateXSet.size(); i++)
                    {
                        PredictedCoordinate.push_back({ CoordinateXSet[i],CoordinateYSet[i] });
                    }

                    PredictedCoordinateSet.push_back(PredictedCoordinate);
                }
                else
                {
                    logger->insertPlainText(QObject::tr("文件加载：输入数据的坐标格式存在问题，请检查数据格式！") + "\n");
                    return false;
                }


                //obtain height value
                if (!std::getline(file, line))
                    return false;
                std::istringstream iss_h(line);
                std::string word_h;
                std::vector<double> heightSet;
                index = 0;
                while (iss_h >> word_h) {
                    if (index == 0)
                    {
                        IDSet.push_back(word_h);
                    }
                    else
                    {
                        if (word_h == "NaN" || word_h == "nan")
                        {
                            heightSet.push_back(DBL_MAX);
                        }
                        else
                        {
                            heightSet.push_back(stod(word_h));
                        }
                    }
                    index++;
                }
                if (heightSet.empty())
                {
                    logger->insertPlainText(QObject::tr("文件加载：高度数据存在明显缺失！") + "\n");
                    return false;
                }
                PredictedHeightSet.push_back(heightSet);

                //security detection
                if ((diameterSet.size() != PredictedCoordinate.size())
                    || (diameterSet.size() != heightSet.size()))
                    return false;

                //security detection(detect whether the ID values of the four rows are the same)
                auto it = std::unique(IDSet.begin(), IDSet.end());
                if (it == IDSet.begin())
                {
                    logger->insertPlainText(QObject::tr("文件加载：请检查每组数据的ID是否相同！") + "\n");
                    return false;
                }
                PredictedID.push_back(IDSet[0]);
            }
            file.close();

            //security detection
            if ((PredictedDiameterSet.size() != PredictedCoordinateSet.size())
                || (PredictedDiameterSet.size() != PredictedHeightSet.size()))
            {
                logger->insertPlainText(QObject::tr("文件加载：请检查每组数据的坐标、直径和高度的数量是否相同！") + "\n");
                return false;
            }

        }
        else {
            logger->insertPlainText(QObject::tr("文件加载：无法打开干曲线预测文件！") + "\n");
            return false;
        }
    }
    else {
        logger->insertPlainText(QObject::tr("文件加载：干曲线预测文件路径错误！") + "\n");
        return false;
    }
	return true;
}

bool StemCurve::loadMatchingFeatureReferenceFile(std::string inputPath)
{
    //文件全部置零
    std::vector<std::vector<double>>().swap(ReferenceDiameterSet);
    std::vector<std::vector<std::vector<double>>>().swap(ReferenceCoordinateSet);
    std::vector<std::vector<double>>().swap(ReferenceHeightSet);
    std::vector<std::string>().swap(ReferenceID);


    //读取提取树文件中的数据
    std::filesystem::path filePath = inputPath;
    if (std::filesystem::exists(filePath)) {
        std::ifstream file(filePath);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {

                std::vector<std::string> IDSet;
                //obtain diameter value
                std::istringstream iss_dia(line);
                std::string word_dia;
                std::vector<double> diameterSet;
                size_t index = 0;
                while (iss_dia >> word_dia) {
                    if (index == 0)
                    {
                        IDSet.push_back(word_dia);
                    }
                    else
                    {
                        if (word_dia == "NaN" || word_dia == "nan")
                        {
                            diameterSet.push_back(DBL_MAX);
                        }
                        else
                        {
                            diameterSet.push_back(stod(word_dia));
                        }
                    }
                    index++;
                }
                if (diameterSet.empty())
                {
                    logger->insertPlainText(QObject::tr("文件加载：直径数据存在明显缺失！") + "\n");
                    return false;
                }

                ReferenceDiameterSet.push_back(diameterSet);

                //obtain XY coordinate value
                if (!std::getline(file, line))
                    return false;
                std::istringstream iss_x(line);
                std::string word_x;
                std::vector<double> CoordinateXSet;
                index = 0;
                while (iss_x >> word_x)
                {
                    if (index == 0)
                    {
                        IDSet.push_back(word_x);
                    }
                    else
                    {
                        if (word_x == "NaN" || word_x == "nan")
                        {
                            CoordinateXSet.push_back(DBL_MAX);
                        }
                        else
                        {
                            CoordinateXSet.push_back(stod(word_x));
                        }

                    }
                    index++;
                }
                if (CoordinateXSet.empty())
                {
                    logger->insertPlainText(QObject::tr("文件加载：坐标X数据存在明显缺失！") + "\n");
                    return false;
                }
                if (!std::getline(file, line))
                    return false;
                std::istringstream iss_y(line);
                std::string word_y;
                std::vector<double> CoordinateYSet;
                index = 0;
                while (iss_y >> word_y)
                {
                    if (index == 0)
                    {
                        IDSet.push_back(word_y);
                    }
                    else
                    {
                        if (word_y == "NaN" || word_y == "nan")
                        {
                            CoordinateYSet.push_back(DBL_MAX);
                        }
                        else
                        {
                            CoordinateYSet.push_back(stod(word_y));
                        }
                    }
                    index++;
                }
                if (CoordinateYSet.empty())
                {
                    logger->insertPlainText(QObject::tr("文件加载：坐标Y数据存在明显缺失！") + "\n");
                    return false;
                }

                std::vector<std::vector<double>> ReferenecCoordinate;
                if (CoordinateXSet.size() == CoordinateYSet.size())
                {
                    for (size_t i = 0; i < CoordinateXSet.size(); i++)
                    {
                        ReferenecCoordinate.push_back({ CoordinateXSet[i],CoordinateYSet[i] });
                    }

                    ReferenceCoordinateSet.push_back(ReferenecCoordinate);
                }
                else
                {
                    logger->insertPlainText(QObject::tr("文件加载：输入数据的坐标格式存在问题，请检查数据格式！") + "\n");
                    return false;
                }


                //obtain height value
                if (!std::getline(file, line))
                    return false;
                std::istringstream iss_h(line);
                std::string word_h;
                std::vector<double> heightSet;
                index = 0;
                while (iss_h >> word_h) {
                    if (index == 0)
                    {
                        IDSet.push_back(word_h);
                    }
                    else
                    {
                        if (word_h == "NaN" || word_h == "nan")
                        {
                            heightSet.push_back(DBL_MAX);
                        }
                        else
                        {
                            heightSet.push_back(stod(word_h));
                        }
                    }
                    index++;
                }
                if (heightSet.empty())
                {
                    logger->insertPlainText(QObject::tr("文件加载：高度数据存在明显缺失！") + "\n");
                    return false;
                }
                ReferenceHeightSet.push_back(heightSet);

                //security detection
                if ((diameterSet.size() != ReferenecCoordinate.size())
                    || (diameterSet.size() != heightSet.size()))
                    return false;

                //security detection(detect whether the ID values of the four rows are the same)
                auto it = std::unique(IDSet.begin(), IDSet.end());
                if (it == IDSet.begin())
                {
                    logger->insertPlainText(QObject::tr("文件加载：请检查每组数据的ID是否相同！") + "\n");
                    return false;
                }
                ReferenceID.push_back(IDSet[0]);
            }
            file.close();

            //security detection
            if ((ReferenceDiameterSet.size() != ReferenceCoordinateSet.size())
                || (ReferenceDiameterSet.size() != ReferenceHeightSet.size()))
            {
                logger->insertPlainText(QObject::tr("文件加载：请检查每组数据的坐标、直径和高度的数量是否相同！") + "\n");
                return false;
            }


        }
        else {
            logger->insertPlainText(QObject::tr("文件加载：无法打开干曲线预测文件！") + "\n");
            return false;
        }
    }
    else {
        logger->insertPlainText(QObject::tr("文件加载：干曲线预测文件路径错误！") + "\n");
        return false;
    }
	return true;
}

bool StemCurve::cacluateDBHandCoordinate(double inputHeight)
{
    for (size_t i = 0; i < PredictedCoordinateSet.size(); i++)
    {
        PredictedCoordinateDBH.push_back(interpolatedCoordinate(inputHeight, PredictedHeightSet[i], PredictedCoordinateSet[i]));
        PredictedFeaturesDBH.push_back({ interpolatedDiameter(inputHeight, PredictedHeightSet[i], PredictedDiameterSet[i]) });
    }
    for (size_t i = 0; i < ReferenceCoordinateSet.size(); i++)
    {
        ReferenceCoordinateDBH.push_back(interpolatedCoordinate(inputHeight, ReferenceHeightSet[i], ReferenceCoordinateSet[i]));
        ReferenceFeaturesDBH.push_back({ interpolatedDiameter(inputHeight, ReferenceHeightSet[i], ReferenceDiameterSet[i]) });
    }
    return true;
}

double StemCurve::interpolatedDiameter(double inputHeight, std::vector<double> referenceHeight, std::vector<double> referenceDiameter)
{
    //predicted height data starting from 0
    inputHeight = inputHeight + referenceHeight[0];

    size_t minimumBoundaryIndex = -1, maximumBoundaryIndex = -1;
    double minValue_min = DBL_MAX, minValue_max = DBL_MAX;
    for (size_t i = 0; i < referenceHeight.size() - 1; i++)
    {
        if (std::abs(referenceHeight[i] - inputHeight) < minValue_min
            && referenceHeight[i] < inputHeight)
        {
            minimumBoundaryIndex = i;
            minValue_min = std::abs(referenceHeight[i] - inputHeight);
            continue;
        }
        if (std::abs(referenceHeight[i] - inputHeight) < minValue_max
            && referenceHeight[i] > inputHeight)
        {
            maximumBoundaryIndex = i;
            minValue_max = std::abs(referenceHeight[i] - inputHeight);
        }

    }
    if (minimumBoundaryIndex == -1)
    {
        return referenceDiameter[0];
    }
    else if (maximumBoundaryIndex == -1)
    {
        return referenceDiameter[referenceDiameter.size() - 1];
    }
    else
    {
        return ((inputHeight - referenceHeight[minimumBoundaryIndex]) / (referenceHeight[maximumBoundaryIndex] - referenceHeight[minimumBoundaryIndex])) * (referenceDiameter[maximumBoundaryIndex] - referenceDiameter[minimumBoundaryIndex]) + referenceDiameter[minimumBoundaryIndex];
    }
  
}

std::vector<double> StemCurve::interpolatedCoordinate(double inputHeight, std::vector<double> referenceHeight, std::vector<std::vector<double>> referenceCoordinate)
{
    //reference height data starting from 0.65
    inputHeight = inputHeight + referenceHeight[0] - 0.65;

    size_t minimumBoundaryIndex = -1, maximumBoundaryIndex = -1;
    double minValue_min = DBL_MAX, minValue_max = DBL_MAX;
    for (size_t i = 0; i < referenceHeight.size() - 1; i++)
    {
        if (std::abs(referenceHeight[i] - inputHeight) < minValue_min
            && referenceHeight[i] < inputHeight)
        {
            minimumBoundaryIndex = i;
            minValue_min = std::abs(referenceHeight[i] - inputHeight);
            continue;
        }
        if (std::abs(referenceHeight[i] - inputHeight) < minValue_max
            && referenceHeight[i] > inputHeight)
        {
            maximumBoundaryIndex = i;
            minValue_max = std::abs(referenceHeight[i] - inputHeight);
        }

    }
    if (minimumBoundaryIndex == -1)
    {
        return referenceCoordinate[0];
    }
    else if (maximumBoundaryIndex == -1)
    {
        return referenceCoordinate[referenceCoordinate.size() - 1];
    }
    else
    {
        return { ((inputHeight - referenceHeight[minimumBoundaryIndex]) / (referenceHeight[maximumBoundaryIndex] - referenceHeight[minimumBoundaryIndex])) * (referenceCoordinate[maximumBoundaryIndex][0] - referenceCoordinate[minimumBoundaryIndex][0]) + referenceCoordinate[minimumBoundaryIndex][0],
        ((inputHeight - referenceHeight[minimumBoundaryIndex]) / (referenceHeight[maximumBoundaryIndex] - referenceHeight[minimumBoundaryIndex]))* (referenceCoordinate[maximumBoundaryIndex][1] - referenceCoordinate[minimumBoundaryIndex][1]) + referenceCoordinate[minimumBoundaryIndex][1] };
    }
}

bool StemCurve::statisticalResults(std::string outputPath, bool outputEnglish)
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
        locationRMSE += std::pow(distance(PredictedCoordinateDBH[relationships->getRelationshipIndexs()[i].first], ReferenceCoordinateDBH[relationships->getRelationshipIndexs()[i].second]), 2);
    }
    locationRMSE /= relationships->getRelationshipIndexs().size();
    locationRMSE = sqrt(locationRMSE);

    //evaluate accuracy of diameter
    //root mean square error(rmse) ,bias rrmse and rbias of diameter
    double DBHRMSE = 0.0, DBHBias = 0.0, allReferenceFeature = 0.0, relativeDBHRMSE = 0.0, relativeDBHBias = 0.0;
    for (size_t i = 0; i < relationships->getRelationshipIndexs().size(); i++)
    {
        DBHRMSE += pow(PredictedFeaturesDBH[relationships->getRelationshipIndexs()[i].first][0] - ReferenceFeaturesDBH[relationships->getRelationshipIndexs()[i].second][0], 2);
        DBHBias += PredictedFeaturesDBH[relationships->getRelationshipIndexs()[i].first][0] - ReferenceFeaturesDBH[relationships->getRelationshipIndexs()[i].second][0];
        allReferenceFeature = ReferenceFeaturesDBH[relationships->getRelationshipIndexs()[i].second][0];
    }
    allReferenceFeature /= relationships->getRelationshipIndexs().size();
    DBHRMSE /= relationships->getRelationshipIndexs().size();
    DBHRMSE = sqrt(DBHRMSE);
    relativeDBHRMSE = DBHRMSE / allReferenceFeature;

    DBHBias /= relationships->getRelationshipIndexs().size();
    relativeDBHBias = DBHBias / relationships->getRelationshipIndexs().size();

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

        workSheet.merge_cells("G4:H4");

        workSheet.cell("G4").alignment(align);
        workSheet.cell("G4").border(border);
        workSheet.cell("H4").border(border);
        workSheet.cell("G4").fill(fillGray);
        workSheet.cell("G4").value("DBH Precision");

        workSheet.cell("G5").alignment(align);
        workSheet.cell("G5").border(border);
        workSheet.cell("G5").value("RMSE of DBH");

        workSheet.cell("H5").alignment(align);
        workSheet.cell("H5").border(border);
        workSheet.cell("H5").value(std::to_string(DBHRMSE));

        workSheet.cell("G6").alignment(align);
        workSheet.cell("G6").border(border);
        workSheet.cell("G6").value("Bias of DBH");

        workSheet.cell("H6").alignment(align);
        workSheet.cell("H6").border(border);
        workSheet.cell("H6").value(std::to_string(DBHBias));

        workSheet.cell("G7").alignment(align);
        workSheet.cell("G7").border(border);
        workSheet.cell("G7").value("RMSE of DBH%");

        workSheet.cell("H7").alignment(align);
        workSheet.cell("H7").border(border);
        workSheet.cell("H7").value(std::to_string(relativeDBHRMSE));

        workSheet.cell("G8").alignment(align);
        workSheet.cell("G8").border(border);
        workSheet.cell("G8").value("Bias of DBH%");

        workSheet.cell("H8").alignment(align);
        workSheet.cell("H8").border(border);
        workSheet.cell("H8").value(std::to_string(relativeDBHBias));
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

        //evaluation of DBH precision
        workSheet.merge_cells("G4:H4");

        workSheet.cell("G4").alignment(align);
        workSheet.cell("G4").border(border);
        workSheet.cell("H4").border(border);
        workSheet.cell("G4").fill(fillGray);
        wideString = L"胸径精度";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell("G4").value(utf8String);

        workSheet.cell("G5").alignment(align);
        workSheet.cell("G5").border(border);
        wideString = L"胸径均方根误差";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell("G5").value(utf8String);
        workSheet.cell("H5").alignment(align);
        workSheet.cell("H5").border(border);
        workSheet.cell("H5").value(std::to_string(DBHRMSE));

        workSheet.cell("G6").alignment(align);
        workSheet.cell("G6").border(border);
        wideString = L"胸径偏差";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell("G6").value(utf8String);
        workSheet.cell("H6").alignment(align);
        workSheet.cell("H6").border(border);
        workSheet.cell("H6").value(std::to_string(DBHBias));

        workSheet.cell("G7").alignment(align);
        workSheet.cell("G7").border(border);
        wideString = L"胸径均方根误差%";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell("G7").value(utf8String);
        workSheet.cell("H7").alignment(align);
        workSheet.cell("H7").border(border);
        workSheet.cell("H7").value(std::to_string(relativeDBHRMSE));

        workSheet.cell("G8").alignment(align);
        workSheet.cell("G8").border(border);
        wideString = L"胸径偏差%";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell("G8").value(utf8String);
        workSheet.cell("H8").alignment(align);
        workSheet.cell("H8").border(border);
        workSheet.cell("H8").value(std::to_string(relativeDBHBias));

    }

    //evaluation results of stem curve parameters

    //diameter and location error of stem curve
    std::vector<std::vector<double>> diameterErrorSet;
    std::vector<std::vector<double>> distanceErrorSet;

    for (size_t i = 0; i < relationships->getRelationshipIndexs().size(); i++)
    {
        std::vector<double> diameterError;
        std::vector<double> distanceError;
        //select values within the height range of the reference data from the predicted data
        std::vector<double> heightResultSet;
        std::vector<std::vector<double>> coordianteResultSet;
        std::vector<double> diameiterResultSet;
        for (size_t j = 0; j < PredictedHeightSet[relationships->getRelationshipIndexs()[i].first].size(); j++)
        {
            if (PredictedHeightSet[relationships->getRelationshipIndexs()[i].first][j] > ReferenceHeightSet[relationships->getRelationshipIndexs()[i].second][0]
                || PredictedHeightSet[relationships->getRelationshipIndexs()[i].first][j] < ReferenceHeightSet[relationships->getRelationshipIndexs()[i].second][ReferenceHeightSet[relationships->getRelationshipIndexs()[i].second].size() - 1])
            {
                heightResultSet.push_back(PredictedHeightSet[relationships->getRelationshipIndexs()[i].first][j]);
                coordianteResultSet.push_back(PredictedCoordinateSet[relationships->getRelationshipIndexs()[i].first][j]);
                diameiterResultSet.push_back(PredictedDiameterSet[relationships->getRelationshipIndexs()[i].first][j]);
            }
        }

        //linear interpolation and calculation of diameter error
        for (size_t j = 0; j < heightResultSet.size(); j++)
        {
            double diameterInterpolation = interpolatedDiameter(heightResultSet[j], ReferenceHeightSet[relationships->getRelationshipIndexs()[i].second], ReferenceDiameterSet[relationships->getRelationshipIndexs()[i].second]);
            diameterError.push_back(diameiterResultSet[j] - diameterInterpolation);
            std::vector<double> coordinateInterpolation = interpolatedCoordinate(heightResultSet[j], ReferenceHeightSet[relationships->getRelationshipIndexs()[i].second], ReferenceCoordinateSet[relationships->getRelationshipIndexs()[i].second]);
            distanceError.push_back(distance(coordianteResultSet[j], coordinateInterpolation));
        }
        diameterErrorSet.push_back(diameterError);
        distanceErrorSet.push_back(distanceError);
    }

    //calculate the average diameter of each reference tree
    std::vector<double> averageDiameterSet;
    for (size_t i = 0; i < relationships->getRelationshipIndexs().size(); i++)
    {
        double averageDiameter = 0.0;
        for (size_t j = 0; j < ReferenceDiameterSet[relationships->getRelationshipIndexs()[i].second].size(); j++)
        {
            averageDiameter += ReferenceDiameterSet[relationships->getRelationshipIndexs()[i].second][j];
        }
        averageDiameter /= ReferenceDiameterSet[relationships->getRelationshipIndexs()[i].second].size();
        averageDiameterSet.push_back(averageDiameter);
    }

    //calculate clr(Curve Length Ratio) for each tree

    std::vector<double> clrSet;
    double meanClr = 0.0;
    for (size_t i = 0; i < relationships->getRelationshipIndexs().size(); i++)
    {
        size_t up = 0, down = 0;
        for (size_t j = 0; j < ReferenceHeightSet[relationships->getRelationshipIndexs()[i].second].size() - 1; j++)
        {
            for (size_t k = 0; k < PredictedHeightSet[relationships->getRelationshipIndexs()[i].first].size(); k++)
            {
                if (ReferenceHeightSet[relationships->getRelationshipIndexs()[i].second][j] <= PredictedHeightSet[relationships->getRelationshipIndexs()[i].first][k]
                    && ReferenceHeightSet[relationships->getRelationshipIndexs()[i].second][j + 1] > PredictedHeightSet[relationships->getRelationshipIndexs()[i].first][k])
                {
                    up++;
                    break;
                }
            }
            down++;
        }

        clrSet.push_back((double)up / (double)down);
        meanClr += (double)up / (double)down;
    }
    meanClr /= relationships->getRelationshipIndexs().size();

    //distance RMSE for each tree
    std::vector<double> distanceRMSESet;
    for (size_t i = 0; i < distanceErrorSet.size(); i++)
    {
        double distanceRMSE = 0.0;
        for (size_t j = 0; j < distanceErrorSet[i].size(); j++)
        {
            distanceRMSE += pow(distanceErrorSet[i][j], 2);
        }
        distanceRMSE /= distanceErrorSet[i].size();
        distanceRMSE = sqrt(distanceRMSE);
        distanceRMSESet.push_back(distanceRMSE);
    }

    double totalRMSE = 0.0, totalMAE = 0.0, totalBias = 0.0;     
    size_t totalNumber = 0;
    std::vector<double> diameterRMSE, relativeDiameterRMSE, diameterMAE, diameterBias, relativeDiameterBias;

    for (size_t i = 0; i < diameterErrorSet.size(); i++)
    {
        double RMSE = 0.0, MAE = 0.0, Bias = 0.0;
        for (size_t j = 0; j < diameterErrorSet[i].size(); j++)
        {
            totalRMSE += pow(diameterErrorSet[i][j], 2);
            totalMAE += abs(diameterErrorSet[i][j]);
            totalBias += diameterErrorSet[i][j];
            totalNumber++;

            RMSE += pow(diameterErrorSet[i][j], 2);
            MAE += abs(diameterErrorSet[i][j]);
            Bias += diameterErrorSet[i][j];

        }
        RMSE /= diameterErrorSet[i].size();
        RMSE = sqrt(RMSE);
        diameterRMSE.push_back(RMSE);
        relativeDiameterRMSE.push_back(100 * RMSE / averageDiameterSet[i]);
        MAE /= diameterErrorSet[i].size();
        diameterMAE.push_back(MAE);
        Bias /= diameterErrorSet[i].size();
        diameterBias.push_back(Bias);
        relativeDiameterBias.push_back(100 * Bias / averageDiameterSet[i]);

    }
    totalRMSE /= totalNumber;
    totalRMSE = sqrt(totalRMSE);
    totalMAE /= totalNumber;
    totalBias /= totalNumber;

    // remove NaN value
    double meanDiameterRMSE = 0.0, diameterRMSESTD = 0.0;
    std::vector<std::string> RMSEIndex;
    std::vector<double> tmpDiameterRMSE;
    for (size_t i = 0; i < diameterRMSE.size(); i++)
    {
        if (std::isnan(diameterRMSE[i]))
            continue;
        RMSEIndex.push_back(PredictedID[relationships->getRelationshipIndexs()[i].first]);
        tmpDiameterRMSE.push_back(diameterRMSE[i]);
        meanDiameterRMSE += diameterRMSE[i];
    }
    meanDiameterRMSE /= RMSEIndex.size();
    diameterRMSE.swap(tmpDiameterRMSE);
    for (size_t i = 0; i < diameterRMSE.size(); i++)
    {
        diameterRMSESTD += pow(diameterRMSE[i] - meanDiameterRMSE, 2);
    }
    diameterRMSESTD /= diameterRMSE.size();
    diameterRMSESTD = sqrt(diameterRMSESTD);


    // remove NaN value
    double meanDiameterBias = 0.0, diameterBiasSTD = 0.0;
    std::vector<std::string> BiasIndex;
    std::vector<double> tmpDiameterBias;
    for (size_t i = 0; i < diameterBias.size(); i++)
    {
        if (std::isnan(diameterBias[i]))
            continue;
        BiasIndex.push_back(PredictedID[relationships->getRelationshipIndexs()[i].first]);
        tmpDiameterBias.push_back(diameterBias[i]);
        meanDiameterBias += diameterBias[i];
    }
    meanDiameterBias /= BiasIndex.size();
    diameterBias.swap(tmpDiameterBias);
    for (size_t i = 0; i < diameterBias.size(); i++)
    {
        if (std::isnan(diameterBias[i]))
            continue;
        diameterBiasSTD += pow(diameterBias[i] - meanDiameterBias, 2);
    }
    diameterBiasSTD /= BiasIndex.size();
    diameterBiasSTD = sqrt(diameterBiasSTD);

    double meanRelativeDiameterRMSE = 0.0, meanRelativeDiameterBias = 0.0;
    for (size_t i = 0; i < relativeDiameterRMSE.size(); i++)
    {
        if (std::isnan(relativeDiameterRMSE[i]))
            continue;
        meanRelativeDiameterRMSE += relativeDiameterRMSE[i];
    }
    meanRelativeDiameterRMSE /= relativeDiameterRMSE.size();


    for (size_t i = 0; i < relativeDiameterBias.size(); i++)
    {
        meanRelativeDiameterBias += relativeDiameterBias[i];
    }
    meanRelativeDiameterBias /= relativeDiameterBias.size();


    int column = 10;
    if (outputEnglish)
    {
        workSheet.merge_cells(xlnt::range_reference(xlnt::cell_reference(column, 1), xlnt::cell_reference(column + 1, 1)));

        workSheet.cell(xlnt::cell_reference(column, 1)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 1)).fill(fillGray);
        workSheet.cell(xlnt::cell_reference(column, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column, 1)).value("Evaluation of Stem Curve Precision (All Trees)");

        workSheet.cell(xlnt::cell_reference(column, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 2)).border(border);
        workSheet.cell(xlnt::cell_reference(column, 2)).value("RMSE of Diameter");
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).value(totalRMSE);

        workSheet.cell(xlnt::cell_reference(column, 3)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 3)).border(border);
        workSheet.cell(xlnt::cell_reference(column, 3)).value("MAE of Diameter");
        workSheet.cell(xlnt::cell_reference(column + 1, 3)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, 3)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 3)).value(totalMAE);

        workSheet.cell(xlnt::cell_reference(column, 4)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 4)).border(border);
        workSheet.cell(xlnt::cell_reference(column, 4)).value("Bias of Diameter");
        workSheet.cell(xlnt::cell_reference(column + 1, 4)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, 4)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 4)).value(totalBias);

        column += 3;
        workSheet.merge_cells(xlnt::range_reference(xlnt::cell_reference(column, 1), xlnt::cell_reference(column + 1, 1)));
        workSheet.cell(xlnt::cell_reference(column, 1)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 1)).fill(fillGray);
        workSheet.cell(xlnt::cell_reference(column, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column, 1)).value("Evaluation of Stem Curve Precision (Pre Trees)");

        workSheet.cell(xlnt::cell_reference(column, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column, 2)).border(border);
        workSheet.cell(xlnt::cell_reference(column, 2)).value("Index of The Predicted Tree");
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).value("RMSE");

        for (size_t i = 0; i < diameterRMSE.size(); i++)
        {
            workSheet.cell(xlnt::cell_reference(column, i + 3)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column, i + 3)).border(border);
            workSheet.cell(xlnt::cell_reference(column, i + 3)).value(RMSEIndex[i]);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).border(border);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).value(diameterRMSE[i]);
        }

        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 3)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 3)).border(border);
        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 3)).value("Mean of RMSE");
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 3)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 3)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 3)).value(meanDiameterRMSE);

        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 4)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 4)).border(border);
        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 4)).value("STD of RMSE");
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 4)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 4)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 4)).value(diameterRMSESTD);

        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 5)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 5)).border(border);
        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 5)).value("Mean of RMSE%");
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 5)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 5)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 5)).value(meanRelativeDiameterRMSE);

        column += 3;
        workSheet.merge_cells(xlnt::range_reference(xlnt::cell_reference(column, 1), xlnt::cell_reference(column + 1, 1)));
        workSheet.cell(xlnt::cell_reference(column, 1)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 1)).fill(fillGray);
        workSheet.cell(xlnt::cell_reference(column, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column, 1)).value("Evaluation of Stem Curve Precision (Pre Trees)");

        workSheet.cell(xlnt::cell_reference(column, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column, 2)).border(border);
        workSheet.cell(xlnt::cell_reference(column, 2)).value("Index of The Predicted Tree");
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).value("MAE");
        size_t tmp_index = 0;
        for (size_t i = 0; i < diameterMAE.size(); i++)
        {
            if (std::isnan(diameterMAE[i]))
            {
                tmp_index++;
                continue;
            }
            workSheet.cell(xlnt::cell_reference(column, i + 3 - tmp_index)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column, i + 3 - tmp_index)).border(border);
            workSheet.cell(xlnt::cell_reference(column, i + 3 - tmp_index)).value(relationships->getRelationshipIDs()[i].first);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3 - tmp_index)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3 - tmp_index)).border(border);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3 - tmp_index)).value(diameterMAE[i]);
        }

        column += 3;
        workSheet.merge_cells(xlnt::range_reference(xlnt::cell_reference(column, 1), xlnt::cell_reference(column + 1, 1)));
        workSheet.cell(xlnt::cell_reference(column, 1)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 1)).fill(fillGray);
        workSheet.cell(xlnt::cell_reference(column, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column, 1)).value("Evaluation of Stem Curve Precision (Pre Trees)");

        workSheet.cell(xlnt::cell_reference(column, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column, 2)).border(border);
        workSheet.cell(xlnt::cell_reference(column, 2)).value("Index of The Predicted Tree");
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).value("Bias");

        for (size_t i = 0; i < diameterBias.size(); i++)
        {
            workSheet.cell(xlnt::cell_reference(column, i + 3)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column, i + 3)).border(border);
            workSheet.cell(xlnt::cell_reference(column, i + 3)).value(BiasIndex[i]);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).border(border);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).value(diameterBias[i]);
        }

        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 3)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 3)).border(border);
        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 3)).value("Mean of Bias");
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 3)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 3)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 3)).value(meanDiameterBias);

        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 4)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 4)).border(border);
        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 4)).value("STD of Bias");
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 4)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 4)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 4)).value(diameterBiasSTD);

        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 5)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 5)).border(border);
        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 5)).value("Mean of Bias%");
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 5)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 5)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 5)).value(meanRelativeDiameterBias);


        column += 3;
        workSheet.merge_cells(xlnt::range_reference(xlnt::cell_reference(column, 1), xlnt::cell_reference(column + 1, 1)));
        workSheet.cell(xlnt::cell_reference(column, 1)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 1)).fill(fillGray);
        workSheet.cell(xlnt::cell_reference(column, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column, 1)).value("Evaluation of Stem Curve Precision (Pre Trees)");

        workSheet.cell(xlnt::cell_reference(column, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column, 2)).border(border);
        workSheet.cell(xlnt::cell_reference(column, 2)).value("Index of The Predicted Tree");
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).value("CLR(Curve Length Ratio)");

        for (size_t i = 0; i < clrSet.size(); i++)
        {
            workSheet.cell(xlnt::cell_reference(column, i + 3)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column, i + 3)).border(border);
            workSheet.cell(xlnt::cell_reference(column, i + 3)).value(PredictedID[i]);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).border(border);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).value(clrSet[i]);
        }

        workSheet.cell(xlnt::cell_reference(column, clrSet.size() + 3)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, clrSet.size() + 3)).border(border);
        workSheet.cell(xlnt::cell_reference(column, clrSet.size() + 3)).value("Mean of CLR");
        workSheet.cell(xlnt::cell_reference(column + 1, clrSet.size() + 3)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, clrSet.size() + 3)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, clrSet.size() + 3)).value(meanClr);

        column += 3;
        workSheet.merge_cells(xlnt::range_reference(xlnt::cell_reference(column, 1), xlnt::cell_reference(column + 1, 1)));
        workSheet.cell(xlnt::cell_reference(column, 1)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 1)).fill(fillGray);
        workSheet.cell(xlnt::cell_reference(column, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column, 1)).value("Evaluation of Stem Curve Precision (Pre Trees)");

        workSheet.cell(xlnt::cell_reference(column, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column, 2)).border(border);
        workSheet.cell(xlnt::cell_reference(column, 2)).value("Index of The Predicted Tree");
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).value("RMSE of XY");
        size_t tmp_index_loc = 0;
        for (size_t i = 0; i < distanceRMSESet.size(); i++)
        {
            if (std::isnan(distanceRMSESet[i]))
            {
                tmp_index_loc++;
                continue;
            }
            workSheet.cell(xlnt::cell_reference(column, i + 3 - tmp_index_loc)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column, i + 3 - tmp_index_loc)).border(border);
            workSheet.cell(xlnt::cell_reference(column, i + 3 - tmp_index_loc)).value(PredictedID[i]);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3 - tmp_index_loc)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3 - tmp_index_loc)).border(border);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3 - tmp_index_loc)).value(distanceRMSESet[i]);
        }

    }
    else
    {
        workSheet.merge_cells(xlnt::range_reference(xlnt::cell_reference(column, 1), xlnt::cell_reference(column + 1, 1)));

        workSheet.cell(xlnt::cell_reference(column, 1)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 1)).fill(fillGray);
        workSheet.cell(xlnt::cell_reference(column, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 1)).border(border);
        std::wstring wideString = L"干曲线预测评价 (所有树)"; // 宽字符字符串（UTF-16或UTF-32，取决于平台）
        // 将宽字符字符串转换为UTF-8编码
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::string utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, 1)).value(utf8String);

        workSheet.cell(xlnt::cell_reference(column, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 2)).border(border);
        wideString = L"直径的均方根误差";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, 2)).value(utf8String);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).value(totalRMSE);

        workSheet.cell(xlnt::cell_reference(column, 3)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 3)).border(border);
        wideString = L"直径的平均绝对值误差";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, 3)).value(utf8String);
        workSheet.cell(xlnt::cell_reference(column + 1, 3)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, 3)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 3)).value(totalMAE);

        workSheet.cell(xlnt::cell_reference(column, 4)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 4)).border(border);
        wideString = L"直径的偏差";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, 4)).value(utf8String);
        workSheet.cell(xlnt::cell_reference(column + 1, 4)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, 4)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 4)).value(totalBias);

        column += 3;
        workSheet.merge_cells(xlnt::range_reference(xlnt::cell_reference(column, 1), xlnt::cell_reference(column + 1, 1)));
        workSheet.cell(xlnt::cell_reference(column, 1)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 1)).fill(fillGray);
        workSheet.cell(xlnt::cell_reference(column, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 1)).border(border);
        wideString = L"干曲线精度评价(每棵树)";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, 1)).value(utf8String);

        workSheet.cell(xlnt::cell_reference(column, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column, 2)).border(border);
        wideString = L"预测树索引";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, 2)).value(utf8String);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).border(border);
        wideString = L"直径均方根误差";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).value(utf8String);

        for (size_t i = 0; i < diameterRMSE.size(); i++)
        {
            workSheet.cell(xlnt::cell_reference(column, i + 3)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column, i + 3)).border(border);
            workSheet.cell(xlnt::cell_reference(column, i + 3)).value(RMSEIndex[i]);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).border(border);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).value(diameterRMSE[i]);
        }

        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 3)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 3)).border(border);
        wideString = L"直径均方根误差平均值";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 3)).value(utf8String);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 3)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 3)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 3)).value(meanDiameterRMSE);

        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 4)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 4)).border(border);
        wideString = L"直径均方根误差的标准差";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 4)).value(utf8String);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 4)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 4)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 4)).value(diameterRMSESTD);

        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 5)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 5)).border(border);
        wideString = L"直径相对均方根误差平均值";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, diameterRMSE.size() + 5)).value(utf8String);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 5)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 5)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterRMSE.size() + 5)).value(meanRelativeDiameterRMSE);

        column += 3;
        workSheet.merge_cells(xlnt::range_reference(xlnt::cell_reference(column, 1), xlnt::cell_reference(column + 1, 1)));
        workSheet.cell(xlnt::cell_reference(column, 1)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 1)).fill(fillGray);
        workSheet.cell(xlnt::cell_reference(column, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 1)).border(border);
        wideString = L"干曲线精度评价(每棵树)";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, 1)).value(utf8String);

        workSheet.cell(xlnt::cell_reference(column, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column, 2)).border(border);
        wideString = L"预测树索引";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, 2)).value(utf8String);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).border(border);
        wideString = L"直径绝对误差平均值";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).value(utf8String);

        size_t tmp_index = 0;
        for (size_t i = 0; i < diameterMAE.size(); i++)
        {
            if (std::isnan(diameterMAE[i]))
            {
                tmp_index++;
                continue;
            }
            workSheet.cell(xlnt::cell_reference(column, i + 3 - tmp_index)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column, i + 3 - tmp_index)).border(border);
            workSheet.cell(xlnt::cell_reference(column, i + 3 - tmp_index)).value(PredictedID[relationships->getRelationshipIndexs()[i].first]);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3 - tmp_index)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3 - tmp_index)).border(border);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3 - tmp_index)).value(diameterMAE[i]);
        }

        column += 3;
        workSheet.merge_cells(xlnt::range_reference(xlnt::cell_reference(column, 1), xlnt::cell_reference(column + 1, 1)));
        workSheet.cell(xlnt::cell_reference(column, 1)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 1)).fill(fillGray);
        workSheet.cell(xlnt::cell_reference(column, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 1)).border(border);
        wideString = L"干曲线预测评价(每棵树)";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, 1)).value(utf8String);

        workSheet.cell(xlnt::cell_reference(column, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column, 2)).border(border);
        wideString = L"预测树索引";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, 2)).value(utf8String);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).border(border);
        wideString = L"直径偏差";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).value(utf8String);

        for (size_t i = 0; i < diameterBias.size(); i++)
        {
            workSheet.cell(xlnt::cell_reference(column, i + 3)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column, i + 3)).border(border);
            workSheet.cell(xlnt::cell_reference(column, i + 3)).value(BiasIndex[i]);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).border(border);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).value(diameterBias[i]);
        }

        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 3)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 3)).border(border);
        wideString = L"直径偏差平均值";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 3)).value(utf8String);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 3)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 3)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 3)).value(meanDiameterBias);

        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 4)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 4)).border(border);
        wideString = L"直径偏差的标准差";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 4)).value(utf8String);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 4)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 4)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 4)).value(diameterBiasSTD);

        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 5)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 5)).border(border);
        wideString = L"直径相对偏差的平均值";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, diameterBias.size() + 5)).value(utf8String);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 5)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 5)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, diameterBias.size() + 5)).value(meanRelativeDiameterBias);


        column += 3;
        workSheet.merge_cells(xlnt::range_reference(xlnt::cell_reference(column, 1), xlnt::cell_reference(column + 1, 1)));
        workSheet.cell(xlnt::cell_reference(column, 1)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 1)).fill(fillGray);
        workSheet.cell(xlnt::cell_reference(column, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 1)).border(border);
        wideString = L"干曲线预测评价(每棵树)";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, 1)).value(utf8String);

        workSheet.cell(xlnt::cell_reference(column, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column, 2)).border(border);
        wideString = L"预测树索引";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, 2)).value(utf8String);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).border(border);
        wideString = L"曲线长度比";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).value(utf8String);

        for (size_t i = 0; i < clrSet.size(); i++)
        {
            workSheet.cell(xlnt::cell_reference(column, i + 3)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column, i + 3)).border(border);
            workSheet.cell(xlnt::cell_reference(column, i + 3)).value(PredictedID[i]);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).border(border);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3)).value(clrSet[i]);
        }

        workSheet.cell(xlnt::cell_reference(column, clrSet.size() + 3)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, clrSet.size() + 3)).border(border);
        wideString = L"曲线长度比的平均值";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, clrSet.size() + 3)).value(utf8String);
        workSheet.cell(xlnt::cell_reference(column + 1, clrSet.size() + 3)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, clrSet.size() + 3)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, clrSet.size() + 3)).value(meanClr);

        column += 3;
        workSheet.merge_cells(xlnt::range_reference(xlnt::cell_reference(column, 1), xlnt::cell_reference(column + 1, 1)));
        workSheet.cell(xlnt::cell_reference(column, 1)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 1)).fill(fillGray);
        workSheet.cell(xlnt::cell_reference(column, 1)).border(border);
        workSheet.cell(xlnt::cell_reference(column + 1, 1)).border(border);
        wideString = L"干曲线精度评价(每棵树)";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, 1)).value(utf8String);

        workSheet.cell(xlnt::cell_reference(column, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column, 2)).border(border);
        wideString = L"预测树索引";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column, 2)).value(utf8String);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).alignment(align);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).fill(filllowGray);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).border(border);
        wideString = L"XY坐标的均方根误差";
        utf8String = converter.to_bytes(wideString);
        workSheet.cell(xlnt::cell_reference(column + 1, 2)).value(utf8String);

        size_t tmp_index_loc = 0;
        for (size_t i = 0; i < distanceRMSESet.size(); i++)
        {
            if (std::isnan(distanceRMSESet[i]))
            {
                tmp_index_loc++;
                continue;
            }
            workSheet.cell(xlnt::cell_reference(column, i + 3 - tmp_index_loc)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column, i + 3 - tmp_index_loc)).border(border);
            workSheet.cell(xlnt::cell_reference(column, i + 3 - tmp_index_loc)).value(PredictedID[i]);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3 - tmp_index_loc)).alignment(align);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3 - tmp_index_loc)).border(border);
            workSheet.cell(xlnt::cell_reference(column + 1, i + 3 - tmp_index_loc)).value(distanceRMSESet[i]);
        }
    }
    //prevent failure to output Chinese path files

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wideOutputPath = converter.from_bytes(outputPath); // wide strings (UTF-16 or UTF-32, depending on platform)

    // convert wide strings of characters to UTF-8 encoding
    std::string utf8OutputPath = converter.to_bytes(wideOutputPath);
    outputExcelFile.save(utf8OutputPath);
    return true;
}

double StemCurve::distance(std::vector<double> PredictedCoordinate, std::vector<double> ReferenceCoordinate)
{
    double sum = 0;
    for (size_t i = 0; i < PredictedCoordinate.size(); i++)
    {
        sum += pow(PredictedCoordinate[i] - ReferenceCoordinate[i], 2);
    }
    return sqrt(sum);
}



