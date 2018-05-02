#pragma once

#include <vector>
#include <utility>
#include <stdlib.h>
#include <string>
#include <jsoncpp/json/json.h>
#include "audio_features.h"

using namespace std;

class TrackPredictor {
public:
	TrackPredictor() {};
	TrackPredictor(int epoch, int steps) : epoch_(epoch), steps_(steps) {}
        vector<vector<double>> RemoveTitles(vector<vector<pair<string,
		double>>> dataset);
        void SVMTrain(vector<vector<double>> dataset);
        int Classify(vector<double> sample);
        double DotProduct(vector<double> a, vector<double> b);
        vector<double> UpdateWeights(vector<double> a, double steplength,
		double lambda);
        vector<double> MatrixScalarSubtraction(vector<double> a, vector<double> b,
		double c, double d);

	// Getter methods
        double GetBias();
        vector<double> GetWeights();
private:
        vector<double> weights_a_;
        double bias_b_;
        int epoch_ = 200;
        int steps_ = 100;
};
