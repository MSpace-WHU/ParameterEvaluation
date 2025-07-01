#pragma once

//#include <QObject>
#include <QtWidgets/QTextEdit>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "KDTree.hpp"

class Relationship
{
public:
	Relationship(QTextEdit* outputInformation = nullptr);
	~Relationship();


	//get relationship ID
	std::vector<std::pair<std::string, std::string>> getRelationshipIDs();
	//get relationship index
	std::vector<std::pair<std::size_t, std::size_t>> getRelationshipIndexs();
	//get unmatched predicted result
	std::vector<size_t> getUnmatchedPredictedResult();
	//get unmatched reference result
	std::vector<size_t> getUnmatchedReferenceResult();
	//get pid
	std::vector<std::string> getPredictedID();
	//get rid
	std::vector<std::string> getReferencedID();
	//get pcor
	std::vector<std::vector<double>> getPredictedCoordinate();
	//get rcor
	std::vector<std::vector<double>> getReferenceCoordinate();
	//get pfet
	std::vector<std::vector<double>> getPredictedFeature();
	//get rfet
	std::vector<std::vector<double>> getReferenceFeature();
	/*------------------------load matching relationship------------------------*/

	void loadRelationshipFile(std::string inputPath);

	/*----------------------calculate matching relationship---------------------*/

	void setCoordiante(std::vector<std::vector<double>> PredictedCoordinateSet, std::vector<std::vector<double>> ReferenceCoordinateSet);
	void setID(std::vector<std::string> PredictedIDSet, std::vector<std::string> ReferenceIDSet);
	void setFeature(std::vector<std::vector<double>> PredictedFeatureSet, std::vector<std::vector<double>> ReferenceFeatureSet);

	void calculateMatchingRelationship(uint8_t inputDimension, double inputRadius, size_t column);


private:
	/*------------------------load matching relationship------------------------*/
	bool loadRelationship(std::string inputPath);

	/*----------------------calculate matching relationship---------------------*/

	bool calcuteUnmatchedResult();

	/*--------------------------------------------------------------------------
		- calculateRelationship
		- neighborhoodSearch
		- callback
			-- one-shotMatching
				--- featureMatching
				--- coordianateMatching
					----- distance
	  --------------------------------------------------------------------------*/
	//calculate the best match based on coordinates and features
	bool calculateRelationship(uint8_t inputDimension, double inputRadius, size_t column);

	//coordinate dimension modification
	bool coordinateDimensionModification(uint8_t inputDimension);

	//neighborhood search
	bool neighborhoodSearch(bool inputRadius);

	//matching using callback
	void callback(size_t PredictedIndex, std::vector<size_t>& ReferenceIndexSet, size_t column);

	//get one-shot match results
	std::pair<size_t, double> one_shotMatching(size_t PredictedIndex, std::vector<size_t> ReferenceIndexSet, size_t column);

	//feature matching
	std::pair<size_t, double> featureMatching(double PredictedFeature, std::vector<double> ReferenceFeatureSet);

	//coordinate matching
	std::pair<size_t, double> coordianateMatching(std::vector<double> PredictedCoordinate, std::vector<std::vector<double>> ReferenceCoordinateSet);

	//calculated distance
	double distance(std::vector<double> PredictedCoordinate, std::vector<double> ReferenceCoordinate);
private:

	//Text control variables for "Console" in the main interface
	QTextEdit* logger;
	//ID of relationship comes from the matching relationship file or relationship calculation
	std::vector<std::pair<std::string, std::string>> relationshipIDs;
	//index of relationship comes from the matching relationship file or relationship calculation
	std::vector<std::pair<std::size_t, std::size_t>> relationshipIndexs;
	//Unmatched Predicted indexes
	std::vector<size_t> unmatchedPredictedResult;
	//Unmatched Reference indexes
	std::vector<size_t> unmatchedReferenceResult;

	/*-----The following variables are used when calculating match relationships-----*/
	/*------------- information stored in predicted file -------------*/

	//ID of the predicted file
	std::vector<std::string> PredictedID;

	//coordinate of the predicted file
	std::vector<std::vector<double>> PredictedCoordinate;

	//neighborhood set of predicted coordinates
	std::vector<std::vector<size_t>> neighborhoodSet;

	//features of the predicted file
	std::vector<std::vector<double>> PredictedFeature;

	/*------------- information stored in reference file -------------*/

	//ID of the reference file
	std::vector<std::string> ReferenceID;

	//coordinate of the reference file
	std::vector<std::vector<double>> ReferenceCoordinate;

	//features of the reference file
	std::vector<std::vector<double>> ReferenceFeature;

	//index set of the predicted value corresponding to the reference value
	std::vector<std::pair<size_t, double>> indexofRef2Ext;
};


