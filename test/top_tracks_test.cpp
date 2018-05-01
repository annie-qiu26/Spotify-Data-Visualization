#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include "../src/top_tracks.h"

using namespace std;

// Code derived from:
// https://www.gamedev.net/forums/topic/353162-reading-a-whole-file-into-a-string-with-ifstream/
ifstream infile("../data/top_tracks.json");
ifstream infile2("../data/disliked_songs.json");
string file_contents((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
string file_contents2((istreambuf_iterator<char>(infile2)), istreambuf_iterator<char>());
TopTracks test_obj;
vector<string> ids;

TEST_CASE("Parse to IDs Size Test") {
        ids = test_obj.ParseToIDs(file_contents);
        REQUIRE(ids.size() == 50);
}

TEST_CASE("Parse to IDs Content Test") {
        REQUIRE(ids[0] == "7iS5V0U7FEX2iSEWn0m9Xw");
        REQUIRE(ids[30] == "1g1TeDflB6atAy7HKwrzXu");
        REQUIRE(ids[49] == "15ahYSiHAIMxAnujlXVtta");
}

TEST_CASE("Parse to IDs Invalid Test") {
        REQUIRE_THROWS(test_obj.ParseToIDs(NULL));
}

TEST_CASE("Parse to IDs Playlist Size Test") {
        ids = test_obj.ParseToIDsPlaylist(file_contents2);
        REQUIRE(ids.size() == 100);
}

TEST_CASE("Parse to IDs Playlist Content Test") {
        REQUIRE(ids[0] == "5SGYanjZguKYg5PgVzFsvR");
        REQUIRE(ids[30] == "2gTYVoQCUh0QNUaFix01ld");
        REQUIRE(ids[99] == "1eLSF6HfrRA0AsNmTkUlKx");
}

TEST_CASE("Parse to IDs Playlist Invalid Test") {
        REQUIRE_THROWS(test_obj.ParseToIDsPlaylist(NULL));
}

//temporary solution
ifstream infile3("../data/disliked_songs_features.json");
Json::Reader reader;
Json::Value audio;
TopTracks test;
vector<vector<pair<string, double>>> dataset;
vector<vector<pair<string, double>>> standardized_dataset;

TEST_CASE("Get Dataset Size Test") {
	reader.parse(infile3, audio);
        dataset = test.GetDataset(audio["audio_features"], 100);
        REQUIRE(dataset.size() == 100);
}

TEST_CASE("Get Dataset First Content Test") {
        REQUIRE(dataset[0][0].first == "Danceability");
        REQUIRE(dataset[0][0].second == Approx(0.879));
        REQUIRE(dataset[0][4].first == "Mode");
        REQUIRE(dataset[0][4].second == 1);
}

TEST_CASE("Get Dataset Middle Content Test") {
        REQUIRE(dataset[30][1].first == "Energy");
        REQUIRE(dataset[30][1].second == Approx(0.766));
        REQUIRE(dataset[30][3].first == "Loudness");
        REQUIRE(dataset[30][3].second == Approx(-4.788));
}

TEST_CASE("Get Dataset End Content Test") {
        REQUIRE(dataset[99][5].first == "Speechiness");
        REQUIRE(dataset[99][5].second == Approx(0.322));
        REQUIRE(dataset[99][6].first == "Acousticness");
        REQUIRE(dataset[99][6].second == Approx(0.00111));
}

//Tests to make sure dataset can be standardized correctly
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

//Just for reference purposes
/* TEST_CASE("Get Standardized Dataset") {
        vector <vector<pair<string, double>>> standardized_dataset
	= test.StandardizeFeatures(dataset);
        for (unsigned int i = 0; i < standardized_dataset.size(); i++) {
                for (unsigned int j = 0; j < standardized_dataset[i].size(); j++) {
                        cout << standardized_dataset[i][j].second << endl;
                }
        }
}*/
