#pragma once
#include <jsoncpp/json/json.h>
#include <string>
#include <vector>
#include "audio_features.h"

using namespace std;

class TopTracks
{
public:
        vector<vector<pair<string, double>>> GetDataset(Json::Value& audio, int size);
        vector <vector<pair<string, double>>> StandardizeFeatures(vector<vector<pair<string, double>>>);
        vector <double> CalculateMeans(vector<vector<pair<string, double>>> dataset);
        vector <double> CalculateStds(vector<vector<pair<string, double>>> dataset);
};
