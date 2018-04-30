#pragma once

#include <vector>
#include <utility>
#include <stdlib.h>

using namespace std;

class TrackPredictor {
public:
        TrackPredictor(int epoch_, int steps_);
        vector<vector<double>> RemoveTitles(vector<vector<pair<string, double>>> dataset);
        void SVMTrain(vector<vector<pair<string, double>>> dataset);
        double DotProduct(vector<double> a, vector<double> b);
        vector<double> UpdateWeights(vector<double> a, double steplength, double lambda);
private:
        vector<double> weights_a_;
        double bias_b_;
        int epoch_;
        int steps_;
}
