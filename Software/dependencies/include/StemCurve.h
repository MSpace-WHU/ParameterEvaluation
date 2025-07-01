#pragma once

#include <QtWidgets/QTextEdit>
#include "Relationship.h"
#include <locale>
#include <codecvt>
#include <string>
#include <xlnt/xlnt.hpp>

class StemCurve
{
public:
	StemCurve(QTextEdit* outputInformation = nullptr);
	~StemCurve();

	void loadFile(std::string inputPath_Predicted, std::string inputPath_Reference);

	void loadMatchingRelationship(std::string inputPath_Relationship, double inputRadius);

	void exportFile(std::string outputPath, bool outputEnglish);

private:
	bool loadMatchingFeaturePredictedFile(std::string inputPath);
	bool loadMatchingFeatureReferenceFile(std::string inputPath);

	//calculate the diameter at breast height and the coordinates
	bool cacluateDBHandCoordinate(double inputHeight);
	double interpolatedDiameter(double inputHeight, std::vector<double> referenceHeight, std::vector<double> referenceDiameter);
	std::vector<double> interpolatedCoordinate(double inputHeight, std::vector<double> referenceHeight, std::vector<std::vector<double>> referenceCoordinate);

	bool statisticalResults(std::string outputPath, bool outputEnglish);

	//calculate distance
	double distance(std::vector<double> PredictedCoordinate, std::vector<double> ReferenceCoordinate);

private:
	//Text control variables for "Console" in the main interface
	QTextEdit* logger;

	//read relationship file 
	std::shared_ptr<Relationship> relationships;

	//neighborhood set of predicted coordinates
	std::vector<std::vector<size_t>> neighborhoodSet;

	//index set of the predicted value corresponding to the reference value
	std::vector<std::pair<size_t, double>> _indexofRef2Ext;

	/*------------- information stored in predicted file -------------*/

	//ID of the stem curve predicted file
	std::vector<std::string> PredictedID;

	//diameter set of the stem curve predicted file
	std::vector<std::vector<double>> PredictedDiameterSet;

	//coordinate set of the stem curve predicted file
	std::vector<std::vector<std::vector<double>>> PredictedCoordinateSet;

	//height set of the stem curve predicted file
	std::vector<std::vector<double>> PredictedHeightSet;

	//coordinate set of DBH
	std::vector<std::vector<double>> PredictedCoordinateDBH;

	//diameter set of DBH
	std::vector<std::vector<double>> PredictedFeaturesDBH;


	/*------------- information stored in reference file -------------*/

	//ID of the stem curve reference file
	std::vector<std::string> ReferenceID;

	//diameter set of the stem curve predicted file
	std::vector<std::vector<double>> ReferenceDiameterSet;

	//coordinate set of the stem curve predicted file
	std::vector<std::vector<std::vector<double>>> ReferenceCoordinateSet;

	//height set of the stem curve predicted file
	std::vector<std::vector<double>> ReferenceHeightSet;

	//coordinate set of DBH
	std::vector<std::vector<double>> ReferenceCoordinateDBH;

	//diameter set of DBH
	std::vector<std::vector<double>> ReferenceFeaturesDBH;
};
