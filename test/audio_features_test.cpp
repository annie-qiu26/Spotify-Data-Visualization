#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>

using namespace std;

ifstream infile("../data/audio_features.json");
Json::Reader reader;
Json::Value audio;

// Makes sure JSON parses the features in the audio correctly

TEST_CASE("JSON Parse Danceability Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["danceability"].asDouble() == Approx(0.735));
}

TEST_CASE("JSON Parse Energy Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["energy"].asDouble() == Approx(0.578));
}

TEST_CASE("JSON Parse Key Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["key"].asInt() == 5);
}

TEST_CASE("JSON Parse Loudness Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["loudness"].asDouble() == Approx(-11.840));
}

TEST_CASE("JSON Parse Mode Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["mode"].asInt() == 0);
}

TEST_CASE("JSON Parse Speechiness Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["speechiness"].asDouble() == Approx(0.0461));
}

TEST_CASE("JSON Parse Acousticness Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["acousticness"].asDouble() == Approx(0.514));
}

TEST_CASE("JSON Parse Instrumentalness Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["instrumentalness"].asDouble() == Approx(0.0902));
}

TEST_CASE("JSON Parse Liveness Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["liveness"].asDouble() == Approx(0.159));
}

TEST_CASE("JSON Parse Valence Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["valence"].asDouble() == Approx(0.624));
}

TEST_CASE("JSON Parse Tempo Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["tempo"].asDouble() == Approx(98.002));
}

TEST_CASE("JSON Parse Type Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["type"].asString() == "audio_features");
}


TEST_CASE("JSON Parse ID Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["id"].asString() == "06AKEBrKUckW0KREUWRnvT");
}

TEST_CASE("JSON Parse URI Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["uri"].asString() == "spotify:track:06AKEBrKUckW0KREUWRnvT");
}

TEST_CASE("JSON Parse Track Href Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["track_href"].asString()
	== "https://api.spotify.com/v1/tracks/06AKEBrKUckW0KREUWRnvT");
}

TEST_CASE("JSON Parse Analysis URL Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["analysis_url"].asString()
	 == "https://api.spotify.com/v1/audio-analysis/06AKEBrKUckW0KREUWRnvT");
}

TEST_CASE("JSON Parse Duration Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["duration_ms"].asInt() == 255349);
}

TEST_CASE("JSON Parse Time Signature Test") {
	reader.parse(infile, audio);
	REQUIRE(audio["time_signature"].asInt() == 4);
}
