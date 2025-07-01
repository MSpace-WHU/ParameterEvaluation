#include "../include/Relationship.h"

Relationship::Relationship(QTextEdit* outputInformation)
{
	logger = outputInformation;
}

Relationship::~Relationship()
{
}

std::vector<std::pair<std::string, std::string>> Relationship::getRelationshipIDs()
{
	//check if the "relationshipIDs" is empty
	if (relationshipIDs.empty())
	{
		logger->insertPlainText(QObject::tr("匹配关系计算：匹配关系ID不存在!") + "\n");
		return std::vector<std::pair<std::string, std::string>>();
	}
	return relationshipIDs;
}

std::vector<std::pair<std::size_t, std::size_t>> Relationship::getRelationshipIndexs()
{
	//check if the "relationshipIndexs" is empty
	if (relationshipIndexs.empty())
	{
		logger->insertPlainText(QObject::tr("匹配关系计算：匹配关系索引不存在!") + "\n");
		return std::vector<std::pair<std::size_t, std::size_t>>();
	}
	return relationshipIndexs;
}

std::vector<size_t> Relationship::getUnmatchedPredictedResult()
{
	return unmatchedPredictedResult;
}

std::vector<size_t> Relationship::getUnmatchedReferenceResult()
{
	return unmatchedReferenceResult;
}

std::vector<std::string> Relationship::getPredictedID()
{
	return PredictedID;
}

std::vector<std::string> Relationship::getReferencedID()
{
	return ReferenceID;
}

std::vector<std::vector<double>> Relationship::getPredictedCoordinate()
{
	return PredictedCoordinate;
}

std::vector<std::vector<double>> Relationship::getReferenceCoordinate()
{
	return ReferenceCoordinate;
}

std::vector<std::vector<double>> Relationship::getPredictedFeature()
{
	return PredictedFeature;
}

std::vector<std::vector<double>> Relationship::getReferenceFeature()
{
	return ReferenceFeature;
}

void Relationship::loadRelationshipFile(std::string inputPath)
{
	if (loadRelationship(inputPath) == false)
	{
		if (logger != nullptr)
			logger->insertPlainText(QObject::tr("---------------------") + "\n");
	}
	else
	{
		if (logger != nullptr)
			logger->insertPlainText(QObject::tr("文件加载：匹配关系文件已加载!") + "\n");
	}
	calcuteUnmatchedResult();
}

bool Relationship::loadRelationship(std::string inputPath)
{
	//make the "relationshipIDs" an empty set
	std::vector<std::pair<std::string, std::string>>().swap(relationshipIDs);
	//read relationship file
	std::filesystem::path filePath = inputPath;
	//check if relationship file exists
	if (std::filesystem::exists(filePath))
	{
		std::ifstream file(filePath);
		if (file.is_open())
		{
			std::string line;
			while (std::getline(file, line))
			{
				//use separator to separate each line
				std::vector<std::string> IDsOfEachLine;
				std::stringstream iss(line);
				std::string ID;
				while (iss >> ID)
				{
					IDsOfEachLine.push_back(ID);
				}
				//each line should have only two IDs(security detection)
				if (IDsOfEachLine.size() == 2)
				{
					relationshipIDs.push_back(std::make_pair(IDsOfEachLine[0], IDsOfEachLine[1]));
				}
				else
				{
					logger->insertPlainText(QObject::tr("文件加载：匹配关系文件中存在ID值不为2的行!") + "\n");
					return false;
				}
			}
		}
		else
		{
			logger->insertPlainText(QObject::tr("文件加载：无法打开匹配关系文件!") + "\n");
			return false;
		}
	}

	//security detection
	if (relationshipIDs.size() > PredictedID.size() || relationshipIDs.size() > ReferenceID.size())
	{
		logger->insertPlainText(QObject::tr("文件加载：匹配关系数量大于预测结果数量或参考结果数量，与实际情况不符！") + "\n");
		return false;
	}

	//calucate index 
	//make the "relationshipIndexs" an empty set
	std::vector<std::pair<std::size_t, std::size_t>>().swap(relationshipIndexs);

	for (size_t i = 0; i < relationshipIDs.size(); i++)
	{
		std::pair<size_t, size_t> relationshipIndex(-1,-1);
		for (size_t j = 0; j < PredictedID.size(); j++)
		{

			if (relationshipIDs[i].first == PredictedID[j])
			{
				relationshipIndex.first = j;
				break;
			}
		}
		for (size_t j = 0; j < ReferenceID.size(); j++)
		{
			if (relationshipIDs[i].second == ReferenceID[j])
			{
				relationshipIndex.second = j;
				break;
			}
		}
		if (relationshipIndex.first == -1 || relationshipIndex.second == -1)
			continue;
		relationshipIndexs.push_back(relationshipIndex);
	}
	return true;
}

bool Relationship::calcuteUnmatchedResult()
{
	//calaute unmatched reference result

	for (size_t i = 0; i < ReferenceID.size(); i++)
	{
		bool unmatch = true;
		for (size_t j = 0; j < relationshipIndexs.size(); j++)
		{
			if (relationshipIndexs[j].second == i)
			{
				unmatch = false;
				break;
			}
		}
		if (unmatch == true)
		{
			unmatchedReferenceResult.push_back(i);
		}
	}

	//calaute unmatched predicted result
	for (size_t i = 0; i < PredictedID.size(); i++)
	{
		bool unmatch = true;
		for (size_t j = 0; j < relationshipIndexs.size(); j++)
		{
			if (relationshipIndexs[j].first == i)
			{
				unmatch = false;
				break;
			}
		}
		if (unmatch == true)
		{
			unmatchedPredictedResult.push_back(i);
		}
	}
	return true;
}

void Relationship::setCoordiante(std::vector<std::vector<double>> PredictedCoordinateSet, std::vector<std::vector<double>> ReferenceCoordinateSet)
{
	PredictedCoordinate = PredictedCoordinateSet;
	ReferenceCoordinate = ReferenceCoordinateSet;
}

void Relationship::setID(std::vector<std::string> PredictedIDSet, std::vector<std::string> ReferenceIDSet)
{
	PredictedID = PredictedIDSet;
	ReferenceID = ReferenceIDSet;
}

void Relationship::setFeature(std::vector<std::vector<double>> PredictedFeatureSet, std::vector<std::vector<double>> ReferenceFeatureSet)
{
	ReferenceFeature = ReferenceFeatureSet;
	PredictedFeature = PredictedFeatureSet;
}

void Relationship::calculateMatchingRelationship(uint8_t inputDimension, double inputRadius, size_t column)
{
	if (calculateRelationship(inputDimension, inputRadius, column) == false)
	{
		if (logger != nullptr)
			logger->insertPlainText(QObject::tr("---------------------") + "\n");
	}
	calcuteUnmatchedResult();
}



bool Relationship::calculateRelationship(uint8_t inputDimension, double inputRadius, size_t column)
{
	//check if coordinate values of predicted files and reference files are available
	if (ReferenceCoordinate.size() == 0 || PredictedCoordinate.size() == 0)
	{
		logger->insertPlainText(QObject::tr("匹配关系计算：预测树木或参考树木坐标不存在!") + "\n");
		return false;
	}
	if (ReferenceID.size() == 0 || PredictedID.size() == 0)
	{
		logger->insertPlainText(QObject::tr("匹配关系计算：预测树木或参考树木ID不存在!") + "\n");
		return false;
	}

	if (coordinateDimensionModification(inputDimension) == false)
	{
		logger->insertPlainText(QObject::tr("匹配关系计算：坐标维度修改出现错误!") + "\n");
		return false;
	}
	if (neighborhoodSearch(inputRadius) == false)
	{
		logger->insertPlainText(QObject::tr("匹配关系计算：邻域搜索出现错误!") + "\n");
		return false;
	}

	//calucate relationship of "indexofRef2Ext"
	std::vector<std::pair<size_t, double>>().swap(indexofRef2Ext);
	indexofRef2Ext.resize(ReferenceCoordinate.size(), std::make_pair(0, -1));
	for (size_t i = 0; i < PredictedCoordinate.size(); i++)
	{
		callback(i, neighborhoodSet[i],column);
	}

	//make the "relationshipIndexs" an empty set
	std::vector<std::pair<std::size_t, std::size_t>>().swap(relationshipIndexs);
	//make the "relationshipIDs" an empty set
	std::vector<std::pair<std::string, std::string>>().swap(relationshipIDs);

	//calucate ID and index
	for (size_t i = 0; i < indexofRef2Ext.size(); i++)
	{
		if (indexofRef2Ext[i].second != -1)
		{
			relationshipIndexs.push_back(std::make_pair(indexofRef2Ext[i].first, i));
			relationshipIDs.push_back(std::make_pair(PredictedID[indexofRef2Ext[i].first], ReferenceID[i]));
		}
	}

	return true;
}

bool Relationship::coordinateDimensionModification(uint8_t inputDimension)
{
	//determine the dimension used for matching
	uint8_t searchDimension = std::min(PredictedCoordinate[0].size(), ReferenceCoordinate[0].size());
	if (inputDimension < searchDimension)
		searchDimension = inputDimension;

	//change predicted coordinate dimension
	if (PredictedCoordinate[0].size() > searchDimension)
	{
		std::vector<std::vector<double>> tmpPredictedCoordinate;
		for (size_t i = 0; i < PredictedCoordinate.size(); i++)
		{
			std::vector<double> tmpCoordinate;
			for (size_t j = 0; j < searchDimension; j++)
			{
				tmpCoordinate.push_back(PredictedCoordinate[i][j]);
			}
			tmpPredictedCoordinate.push_back(tmpCoordinate);
		}
		PredictedCoordinate.swap(tmpPredictedCoordinate);
	}

	//change reference coordinate dimension
	if (ReferenceCoordinate[0].size() > searchDimension)
	{
		std::vector<std::vector<double>> tmpReferenceCoordinate;
		for (size_t i = 0; i < ReferenceCoordinate.size(); i++)
		{
			std::vector<double> tmpCoordinate;
			for (size_t j = 0; j < searchDimension; j++)
			{
				tmpCoordinate.push_back(ReferenceCoordinate[i][j]);
			}
			tmpReferenceCoordinate.push_back(tmpCoordinate);
		}
		ReferenceCoordinate.swap(tmpReferenceCoordinate);
	}
	return true;
}

bool Relationship::neighborhoodSearch(bool inputRadius)
{
	//search the neighborhood of the predicted coordinates
	//kdtree that manages the reference coordinate
	std::shared_ptr<KDTree> kdtree = std::make_shared<KDTree>(ReferenceCoordinate);
	//make the "neighborhoodSet" an empty set
	std::vector<std::vector<size_t>>().swap(neighborhoodSet);
	for (auto it = PredictedCoordinate.begin(); it != PredictedCoordinate.end(); it++)
		neighborhoodSet.push_back(kdtree->neighborhood_indices(*it, inputRadius));

	//security detection
	if (neighborhoodSet.size() != PredictedCoordinate.size())
	{
		logger->insertPlainText(QObject::tr("匹配关系计算：预测结果或参考结果坐标不存在!") + "\n");
		return false;
	}
	return true;
}

void Relationship::callback(size_t PredictedIndex, std::vector<size_t>& ReferenceIndexSet, size_t column)
{
	//check if "ReferenceIndexSet" is a empty set
	if (ReferenceIndexSet.empty())
		return;

	size_t nextIndex = -1;

	//best match of "ReferenceIndexSet"(first is local index of "ReferenceIndexSet",second nearest result)
	std::pair<size_t, double> nearestResult = one_shotMatching(PredictedIndex, ReferenceIndexSet, column);
	if (nearestResult.first == size_t() && nearestResult.second == double())
		return;

	//determine if reference data has matching predicted data by detecting nearestresult
	if (indexofRef2Ext[ReferenceIndexSet[nearestResult.first]].second != -1)
	{
		if (indexofRef2Ext[ReferenceIndexSet[nearestResult.first]].second < nearestResult.second)
		{
			ReferenceIndexSet.erase(ReferenceIndexSet.begin() + nearestResult.first);
			if (ReferenceIndexSet.empty())
				return;
			nextIndex = PredictedIndex;
		}
		if (indexofRef2Ext[ReferenceIndexSet[nearestResult.first]].second > nearestResult.second)
		{
			if (neighborhoodSet[indexofRef2Ext[ReferenceIndexSet[nearestResult.first]].first].empty())
			{
				indexofRef2Ext[ReferenceIndexSet[nearestResult.first]] = std::make_pair(PredictedIndex, nearestResult.second);
				return;
			}

			//这里存疑
			std::pair<size_t, double> nearestResult_Previous = one_shotMatching(indexofRef2Ext[ReferenceIndexSet[nearestResult.first]].first, neighborhoodSet[indexofRef2Ext[ReferenceIndexSet[nearestResult.first]].first], column);

			neighborhoodSet[indexofRef2Ext[ReferenceIndexSet[nearestResult.first]].first].erase(neighborhoodSet[indexofRef2Ext[ReferenceIndexSet[nearestResult.first]].first].begin() + nearestResult_Previous.first);
			//neighborhoodSet[indexofRef2Ext[ReferenceIndexSet[nearestResult_Previous.first]].first].erase(neighborhoodSet[indexofRef2Ext[ReferenceIndexSet[nearestResult_Previous.first]].first].begin() + nearestResult_Previous.first);

			if (neighborhoodSet[indexofRef2Ext[ReferenceIndexSet[nearestResult.first]].first].empty())
			{
				indexofRef2Ext[ReferenceIndexSet[nearestResult.first]] = std::make_pair(PredictedIndex, nearestResult.second);
				return;
			}
			//if (neighborhoodSet[indexofRef2Ext[ReferenceIndexSet[nearestResult_Previous.first]].first].empty())
			//{
			//	indexofRef2Ext[ReferenceIndexSet[nearestResult.first]] = std::make_pair(PredictedIndex, nearestResult.second);
			//	return;
			//}

			nextIndex = indexofRef2Ext[ReferenceIndexSet[nearestResult.first]].first;
			indexofRef2Ext[ReferenceIndexSet[nearestResult.first]] = std::make_pair(PredictedIndex, nearestResult.second);
		}
	}

	if (nextIndex != -1)
	{
		callback(nextIndex, neighborhoodSet[nextIndex],column);
	}
	else
	{
		if (!ReferenceIndexSet.size())
			return;
		indexofRef2Ext[ReferenceIndexSet[nearestResult.first]] = std::make_pair(PredictedIndex, nearestResult.second);
	}
}

std::pair<size_t, double> Relationship::one_shotMatching(size_t PredictedIndex, std::vector<size_t> ReferenceIndexSet, size_t column)
{
	//determine the matching method based on "column".
	if (column)
	{
		//extract feature for one-shot matching based on "neighborhoodSet"
		std::vector<double> neighborhoodFeature;
		//extract neighborhood reference set
		for (size_t i = 0; i < ReferenceIndexSet.size(); i++)
		{
			neighborhoodFeature.push_back(ReferenceFeature[ReferenceIndexSet[i]][column - 5]);
		}
		std::pair<size_t, double> nearesFeature = featureMatching(PredictedFeature[PredictedIndex][column - 5], neighborhoodFeature);
		//check if "nearesFeature" is a empty set
		if (nearesFeature.second == DBL_MAX)
			return std::pair<size_t, double>();
		else
			return nearesFeature;
	}
	else
	{
		//extract coordinate for one-shot matching based on "neighborhoodSet"
		std::vector<std::vector<double>> neighborhoodCoordiante;
		//extract neighborhood reference set
		for (size_t i = 0; i < ReferenceIndexSet.size(); i++)
		{
			neighborhoodCoordiante.push_back(ReferenceCoordinate[ReferenceIndexSet[i]]);
		}
		return coordianateMatching(PredictedCoordinate[PredictedIndex], neighborhoodCoordiante);
	}
}

std::pair<size_t, double> Relationship::featureMatching(double PredictedFeature, std::vector<double> ReferenceFeatureSet)
{
	//check if "PredictedFeature" exists
	if (PredictedFeature == DBL_MAX)
	{
		return std::pair<size_t, double>();
	}
	//initial nearestIndex and nearesDistance
	double nearesFeature = DBL_MAX;
	size_t nearestIndex = 0;

	//search nearest index and feature
	for (size_t index = 0; index < ReferenceFeatureSet.size(); index++)
	{
		double currentFeature = abs(PredictedFeature - ReferenceFeatureSet[index]);
		if (currentFeature < nearesFeature)
		{
			nearesFeature = currentFeature;
			nearestIndex = index;
		}
	}

	return std::make_pair(nearestIndex, nearesFeature);
}

std::pair<size_t, double> Relationship::coordianateMatching(std::vector<double> PredictedCoordinate, std::vector<std::vector<double>> ReferenceCoordinateSet)
{
	//initial nearestIndex and nearesDistance
	size_t nearestIndex = 0;
	double nearesDistance = distance(PredictedCoordinate, ReferenceCoordinateSet[0]);

	//search nearest index and distance
	for (size_t index = 1; index < ReferenceCoordinateSet.size(); index++) {
		double currentDistance = distance(PredictedCoordinate, ReferenceCoordinateSet[index]);
		if (currentDistance < nearesDistance) {
			nearestIndex = index;
			nearesDistance = currentDistance;
		}
	}

	return std::make_pair(nearestIndex, nearesDistance);
}

double Relationship::distance(std::vector<double> PredictedCoordinate, std::vector<double> ReferenceCoordinate)
{
	double sum = 0;
	for (size_t i = 0; i < PredictedCoordinate.size(); i++)
	{
		sum += pow(PredictedCoordinate[i] - ReferenceCoordinate[i], 2);
	}
	return sqrt(sum);
}
