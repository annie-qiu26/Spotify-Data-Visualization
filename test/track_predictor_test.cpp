#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/track_predictor.h"

using namespace std;

TrackPredictor test_obj(100, 50);

TEST_CASE("Remove Titles") {
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

TEST_CASE("Matrix Scalar Subtraction") {
        vector<double> a{5, 8, 30};
        vector<double> b{6, 7, 8};

        vector<double> subtracted_matrix = test_obj.MatrixScalarSubtraction(a, b, -0.5, 0.8);

        REQUIRE(subtracted_matrix[0] == 7.4);
        REQUIRE(subtracted_matrix[1] == 10.8);
        REQUIRE(subtracted_matrix[2] == 33.2);
}
