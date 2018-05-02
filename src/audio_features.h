#pragma once

#include <string>
#include <vector>
#include <jsoncpp/json/json.h>
#include <cmath>

using namespace std;

class AudioFeatures
{
public:
	void ParseToFeatures(Json::Value& audio);
	vector <pair<string, double>> GetFeatures();
private:
	double danceability;
	double energy;
	double loudness;
	double speechiness;
	double acousticness;
	double instrumentalness;
	double liveness;
	double valence;
	double tempo;
	int duration_ms;
};
