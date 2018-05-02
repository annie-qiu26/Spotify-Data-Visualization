#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <fstream>
#include "../src/audio_features.h"

using namespace std;

ifstream infile("../data/audio_features.json");
Json::Reader reader;
Json::Value audio;
vector<pair<string, double>> features;
AudioFeatures testAudio;

// Makes sure JSON parses the features in the audio correctly
TEST_CASE("JSON Parse Danceability Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["danceability"].asDouble() == Approx(0.735));
}

TEST_CASE("JSON Parse Energy Test") {
	REQUIRE(audio["energy"].asDouble() == Approx(0.578));
}

TEST_CASE("JSON Parse Loudness Test") {
	REQUIRE(audio["loudness"].asDouble() == Approx(-11.840));
}

TEST_CASE("JSON Parse Speechiness Test") {
	REQUIRE(audio["speechiness"].asDouble() == Approx(0.0461));
}

TEST_CASE("JSON Parse Acousticness Test") {
	REQUIRE(audio["acousticness"].asDouble() == Approx(0.514));
}

TEST_CASE("JSON Parse Instrumentalness Test") {
	REQUIRE(audio["instrumentalness"].asDouble() == Approx(0.0902));
}

TEST_CASE("JSON Parse Liveness Test") {
	REQUIRE(audio["liveness"].asDouble() == Approx(0.159));
}

TEST_CASE("JSON Parse Valence Test") {
	REQUIRE(audio["valence"].asDouble() == Approx(0.624));
}

TEST_CASE("JSON Parse Tempo Test") {
	REQUIRE(audio["tempo"].asDouble() == Approx(98.002));
}

TEST_CASE("JSON Parse Duration Test") {
	REQUIRE(audio["duration_ms"].asInt() == 255349);
}

TEST_CASE("JSON Parse Time Signature Test") {
	REQUIRE(audio["time_signature"].asInt() == 4);
}

// Test cases that makes sure variables in readers are passed in correctly
// to object and GetFeatures function returns the right values
TEST_CASE("Parse To Features Size Test") {
	testAudio.ParseToFeatures(audio);
	features = testAudio.GetFeatures();
	REQUIRE(features.size() == 10);
}

TEST_CASE("Get Features Danceability") {
	pair<string, double> feature_pair = features[0];
	REQUIRE(feature_pair.first == "Danceability");
	REQUIRE(feature_pair.second == Approx(0.735));
}

TEST_CASE("Get Features Energy") {
	pair<string, double> feature_pair = features[1];
	REQUIRE(feature_pair.first == "Energy");
	REQUIRE(feature_pair.second == Approx(0.578));
}

TEST_CASE("Get Features Instrumentalness") {
	pair<string, double> feature_pair = features[5];
	REQUIRE(feature_pair.first == "Instrumentalness");
	REQUIRE(feature_pair.second == Approx(0.0902));
}

TEST_CASE("Get Features Duration") {
	pair<string, double> feature_pair = features[9];
	REQUIRE(feature_pair.first == "Duration in Milliseconds");
	REQUIRE(feature_pair.second == Approx(255349));
}
