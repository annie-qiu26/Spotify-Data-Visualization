#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include "../src/top_tracks.h"

using namespace std;

TopTracks test_obj;

ifstream infile3("../data/disliked_songs_features.json");
Json::Reader reader;
Json::Value audio;
TopTracks test;
vector<vector<pair<string, double>>> dataset;
vector<vector<pair<string, double>>> standardized_dataset;

// Make sure the dataset is successfully put
// into vectors
TEST_CASE("Get Dataset Size Test") {
	reader.parse(infile3, audio);
        dataset = test.GetDataset(audio["audio_features"], 100);
        REQUIRE(dataset.size() == 100);
}

TEST_CASE("Get Dataset First Content Test") {
        REQUIRE(dataset[0][0].first == "Danceability");
        REQUIRE(dataset[0][0].second == Approx(0.879));
}

TEST_CASE("Get Dataset Middle Content Test") {
        REQUIRE(dataset[30][1].first == "Energy");
        REQUIRE(dataset[30][1].second == Approx(0.766));
        REQUIRE(dataset[30][2].first == "Loudness");
        REQUIRE(dataset[30][2].second == Approx(-4.788));
}

TEST_CASE("Get Dataset End Content Test") {
        REQUIRE(dataset[99][3].first == "Speechiness");
        REQUIRE(dataset[99][3].second == Approx(0.322));
        REQUIRE(dataset[99][4].first == "Acousticness");
        REQUIRE(dataset[99][4].second == Approx(0.00111));
}

// Tests to make sure dataset can be standardized correctly
// with helper functions of calculating means and
// standard deviations
TEST_CASE("Calculate Means Test") {
	vector<pair<string, double>> dataset;
	vector<pair<string, double>> dataset1;
	vector<pair<string, double>> dataset2;

	for (int i = 3; i < 6; i++) {
		dataset.push_back(make_pair("string", i));
		dataset1.push_back(make_pair("string", i));
		dataset2.push_back(make_pair("string", i));
	}

	vector<vector<pair<string, double>>> datasets;
	datasets.push_back(dataset);
	datasets.push_back(dataset1);
	datasets.push_back(dataset2);

	vector<double> means = test.CalculateMeans(datasets);

	REQUIRE(means[0] == 3);
	REQUIRE(means[1] == 4);
	REQUIRE(means[2] == 5);
}

TEST_CASE("Calculate Means Empty Test") {
	vector<vector<pair<string, double>>> datasets;
	vector<double> means = test.CalculateMeans(datasets);
	REQUIRE(means.size() == 0);
}

TEST_CASE("Calculate Means Double Test") {
	vector<pair<string, double>> dataset;
	vector<pair<string, double>> dataset1;

	for (int i = 0; i < 3; i++) {
		dataset.push_back(make_pair("string", 3));
		dataset1.push_back(make_pair("string", 4));
	}

	vector<vector<pair<string, double>>> datasets;
	datasets.push_back(dataset);
	datasets.push_back(dataset1);

	vector<double> means = test.CalculateMeans(datasets);

	REQUIRE(means[0] == Approx(3.5));
	REQUIRE(means[1] == Approx(3.5));
}

TEST_CASE("Calculate Stds Test") {
	vector<pair<string, double>> dataset;
	vector<pair<string, double>> dataset1;
	vector<pair<string, double>> dataset2;

	for (int i = 3; i < 6; i++) {
		dataset.push_back(make_pair("string", i));
		dataset1.push_back(make_pair("string", i));
		dataset2.push_back(make_pair("string", i));
	}

	vector<vector<pair<string, double>>> datasets;
	datasets.push_back(dataset);
	datasets.push_back(dataset1);
	datasets.push_back(dataset2);

	vector<double> stds = test.CalculateStds(datasets);

	REQUIRE(stds[0] == 0);
	REQUIRE(stds[1] == 0);
	REQUIRE(stds[2] == 0);
}

TEST_CASE("Calculate Stds Empty Test") {
	vector<vector<pair<string, double>>> datasets;
	vector<double> stds = test.CalculateMeans(datasets);
	REQUIRE(stds.size() == 0);
}

TEST_CASE("Calculate Stds Double Test") {
	vector<pair<string, double>> dataset;
	vector<pair<string, double>> dataset1;

	for (int i = 0; i < 3; i++) {
		dataset.push_back(make_pair("string", 3));
		dataset1.push_back(make_pair("string", 4));
	}

	vector<vector<pair<string, double>>> datasets;
	datasets.push_back(dataset);
	datasets.push_back(dataset1);

	vector<double> stds = test.CalculateStds(datasets);

	REQUIRE(stds[0] == Approx(0.7071));
	REQUIRE(stds[1] == Approx(0.7071));
	REQUIRE(stds[2] == Approx(0.7071));

}

TEST_CASE("Standardize Data Test") {
	vector<pair<string, double>> dataset;
	vector<pair<string, double>> dataset1;

	for (int i = 0; i < 3; i++) {
		dataset.push_back(make_pair("string", 3));
		dataset1.push_back(make_pair("string", 4));
	}

	vector<vector<pair<string, double>>> datasets;
	datasets.push_back(dataset);
	datasets.push_back(dataset1);

	vector <vector<pair<string, double>>> standardized_dataset
	= test.StandardizeFeatures(datasets);

	REQUIRE(standardized_dataset[0][0].second == Approx(-0.7071));
	REQUIRE(standardized_dataset[0][1].second == Approx(-0.7071));
	REQUIRE(standardized_dataset[0][2].second == Approx(-0.7071));
	REQUIRE(standardized_dataset[1][0].second == Approx(0.7071));
	REQUIRE(standardized_dataset[1][1].second == Approx(0.7071));
	REQUIRE(standardized_dataset[1][2].second == Approx(0.7071));

}

TEST_CASE("Standardize Data Empty Test") {
	vector<vector<pair<string, double>>> datasets;
	vector <vector<pair<string, double>>> standardized_dataset
	= test.StandardizeFeatures(datasets);
	REQUIRE(standardized_dataset.size() == 0);
}
