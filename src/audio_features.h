#pragma once

#include <string>
#include <vector>
#include <jsoncpp/json/json.h>
#include <cmath>

using namespace std;

class AudioFeatures
{
public:
	AudioFeatures();
	~AudioFeatures();
	void ParseToFeatures(Json::Value& audio);
	vector <pair<string, double>> GetFeatures();
	vector <pair<string, double>> StandardizeFeatures(vector<vector<pair<string, double>>>);
	vector <double> CalculateMeans(vector<vector<pair<string, double>>> dataset);
	vector <double> CalculateStds(vector<vector<pair<string, double>>> dataset);
private:
	double danceability;
	double energy;
	int key;
	double loudness;
	int mode;
	double speechiness;
	double acousticness;
	double instrumentalness;
	double liveness;
	double valence;
	double tempo;
	string type;
	string id;
	string uri;
	string track_href;
	string analysis_url;
	int duration_ms;
	int time_signature;
};
