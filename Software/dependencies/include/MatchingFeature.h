#pragma once

#include <QtWidgets/QTextEdit>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <xlnt/xlnt.hpp>
#include <locale>
#include <codecvt>
#include <string>
#include "Relationship.h"
#include "KDTree.hpp"

class MatchingFeature
{
public:
	MatchingFeature(QTextEdit* outputInformation = nullptr);
	~MatchingFeature();

	void loadFile(std::string inputPath_Predicted, std::string inputPath_Reference);

	void loadMatchingRelationship(std::string inputPath_Relationship, uint8_t inputDimension, double inputRadius, size_t column);
	void exportFile(std::string outputPath, bool outputEnglish);

	std::shared_ptr<Relationship> getRelationsip();


	std::vector<std::vector<double>> getPredictedCoordinate();
	std::vector<std::vector<double>> getReferenceCoordinate();

private:

	bool loadMatchingFeaturePredictedFile(std::string inputPath);
	bool loadMatchingFeatureReferenceFile(std::string inputPath);

	bool statisticalResults(std::string outputPath, bool outputEnglish);

	//calculated distance
	double distance(std::vector<double> PredictedCoordinate, std::vector<double> ReferenceCoordinate);

private:
	//Text control variables for "Console" in the main interface
	QTextEdit* logger;

	//Dimension of neighborhood search
	std::uint8_t searchDimension;

	//read relationship file 
	std::shared_ptr<Relationship> relationships;


	//neighborhood set of predicted coordinates
	std::vector<std::vector<size_t>> neighborhoodSet;

	//index set of the predicted value corresponding to the reference value
	std::vector<std::pair<size_t, double>> _indexofRef2Ext;

	//column of the matching feature(0 represents none.)
	size_t columnofMatchingFeature;


	/*------------- information stored in predicted file -------------*/
	
	//ID of the predicted file
	std::vector<std::string> PredictedID;

	//coordinate of the predicted file
	std::vector<std::vector<double>> PredictedCoordinate;

	//features of the predicted file
	std::vector<std::vector<double>> PredictedFeatures;


	/*------------- information stored in reference file -------------*/

	//ID of the reference file
	std::vector<std::string> ReferenceID;

	//coordinate of the reference file
	std::vector<std::vector<double>> ReferenceCoordinate;

	//features of the reference file
	std::vector<std::vector<double>> ReferenceFeatures;


};


