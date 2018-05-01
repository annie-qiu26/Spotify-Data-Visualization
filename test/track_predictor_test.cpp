#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/track_predictor.h"
#include "../src/top_tracks.h"

// For debugging
#include <iostream>

using namespace std;

TrackPredictor test_obj(200, 100);

TEST_CASE("Remove Titles Test") {
        vector<pair<string, double>> dataset;
	vector<pair<string, double>> dataset1;

	for (int i = 3; i < 6; i++) {
		dataset.push_back(make_pair("string", i));
		dataset1.push_back(make_pair("string", i));
	}

	vector<vector<pair<string, double>>> datasets;
	datasets.push_back(dataset);
	datasets.push_back(dataset1);

        vector<vector<double>> new_dataset = test_obj.RemoveTitles(datasets);

        REQUIRE(new_dataset[0][0] == 3);
        REQUIRE(new_dataset[0][1] == 4);
        REQUIRE(new_dataset[0][2] == 5);
        REQUIRE(new_dataset[1][0] == 3);
        REQUIRE(new_dataset[1][1] == 4);
        REQUIRE(new_dataset[1][2] == 5);

}

TEST_CASE("Remove Titles Empty Test") {
        vector<vector<pair<string, double>>> datasets;

        vector<vector<double>> new_dataset = test_obj.RemoveTitles(datasets);

        REQUIRE(new_dataset.size() == 0);
}


TEST_CASE("Dot Product Test") {
        vector<double> a{5, 8, 30};
        vector<double> b{5, 8, 30};

        double product = test_obj.DotProduct(a, b);

        REQUIRE(product == 989);
}

TEST_CASE("Dot Product Empty Test") {
        vector<double> a;
        vector<double> b;

        double product = test_obj.DotProduct(a, b);

        REQUIRE(product == 0);
}

TEST_CASE("Update Weights Test") {
        vector<double> a{5, 8, 30};

        vector<double> subtracted_matrix = test_obj.UpdateWeights(a, 0.5, 0.8);

        REQUIRE(subtracted_matrix[0] == 3);
        REQUIRE(subtracted_matrix[1] == 4.8);
        REQUIRE(subtracted_matrix[2] == 18);
}

TEST_CASE("Update Weights Empty Test") {
        vector<double> a;

        vector<double> subtracted_matrix = test_obj.UpdateWeights(a, 0.5, 0.8);

        REQUIRE(subtracted_matrix.size() == 0);
}

TEST_CASE("Matrix Scalar Subtraction Test") {
        vector<double> a{5, 8, 30};
        vector<double> b{6, 7, 8};

        vector<double> subtracted_matrix = test_obj.MatrixScalarSubtraction(a, b, -0.5, 0.8);

        REQUIRE(subtracted_matrix[0] == 7.4);
        REQUIRE(subtracted_matrix[1] == 10.8);
        REQUIRE(subtracted_matrix[2] == 33.2);
}

TEST_CASE("Matrix Scalar Subtraction Empty Test") {
        vector<double> a{0, 0, 0};
        vector<double> b{6, 7, 8};

        vector<double> subtracted_matrix = test_obj.MatrixScalarSubtraction(a, b, -0.5, 0.8);

        REQUIRE(subtracted_matrix[0] == Approx(2.4));
        REQUIRE(subtracted_matrix[1] == Approx(2.8));
        REQUIRE(subtracted_matrix[2] == Approx(3.2));

}

vector<vector<double>> modified_dataset;

void SetUpSVM() {
        Json::Reader reader;
        Json::Value audio;
	ifstream infile0("../data/liked_songs_features.json");
        ifstream infile1("../data/disliked_songs_features.json");
        reader.parse(infile0, audio);
        TopTracks track_test_obj;

        // Getting the data set into vectors
        vector<vector<pair<string, double>>> liked_dataset
                = track_test_obj.GetDataset(audio["audio_features"], 100);

        reader.parse(infile1, audio);
        vector<vector<pair<string, double>>> disliked_dataset
                = track_test_obj.GetDataset(audio["audio_features"], 100);

        // Combining the dataset
        vector<vector<pair<string, double>>> combined_dataset = liked_dataset;
        combined_dataset.insert(combined_dataset.end(), disliked_dataset.begin(),
                disliked_dataset.end());

        vector<double> means = track_test_obj.CalculateMeans(combined_dataset);
        vector<double> stds = track_test_obj.CalculateStds(combined_dataset);
        vector<vector<pair<string, double>>> standardized_dataset
        		= track_test_obj.StandardizeFeatures(combined_dataset);

        // Remove pairs and convert to doubles
        modified_dataset = test_obj.RemoveTitles(standardized_dataset);
}

TEST_CASE("SVM Train Test") {
        SetUpSVM();

        test_obj.SVMTrain(modified_dataset);

        // Get accuracy of prediction, liked songs are in the first half
        double accuracy = 0.0;

        for (unsigned int i = 0; i < modified_dataset.size(); i++) {
                if (i < modified_dataset.size() / 2) {
                        if (test_obj.Classify(modified_dataset[i]) == 1) {
                                accuracy++;
                        }
                } else {
                        if (test_obj.Classify(modified_dataset[i]) == -1) {
                                accuracy++;
                        }
                }
        }

        accuracy /= modified_dataset.size();

        // Know prediction isn't random
        REQUIRE(accuracy > 0.85);

}

TEST_CASE("SVM Train Empty Test") {
        vector<vector<double>> dataset;
        REQUIRE_THROWS(test_obj.SVMTrain(dataset));
}
