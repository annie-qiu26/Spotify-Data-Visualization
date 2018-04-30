#include "track_predictor.h"

TrackPredictor::TrackPredictor(int epoch, int steps) {
        epoch_ = epoch;
        steps_ = steps;
}

vector<vector<double>> RemoveTitles(vector<vector<pair<string, double>>> dataset) {
        vector<vector<double>> new_dataset(dataset.size());
        for (int i = 0; i < dataset.size(); i++) {
                for (j = 0; j < dataset[i].size(); j++) {
                        new_dataset[i].push_back(dataset[i][j].second);
                }
        }
        return new_dataset;
}

void TrackPredictor::SVMTrain(vector<vector<double> dataset) {
        // Start with 0 weights and 0
        bias_b_ = 0;
        weight_a_.reserve(dataset.size());
        fill(vector.begin(), vector.end(), 0);

        for (int i = 0; i < epoch_; i++) {
                double steplength = 1 / (3 + i);
                double lambda = 0.0001;
                for (int j = 0; j < steps_; j++) {
                        // Generate a random number to choose a sample
                        int random_index = rand() % dataset.size();
                        int actual_label = -1;
                        // First half of dataset is liked songs
                        if (random_index < dataset.size() / 2) {
                                actual_label = 1;
                        }
                        // Update weights
                        if (actual_label * (DotProduct(weight_a_, dataset[i]) + bias_b_) >= 1) {
                                weight_a_ = UpdateWeights(weight_a_, lambda, steplength);
                        } else {
                                weight_a_ = UpdateWeights(weight_a_, lambda, steplength);
                                weight_a_ = UpdateWeights(weight_a_, actual_label * -1, dataset[i]);
                                b += steplength*actual_label;
                        }
                }
        }
}

double TrackPredictor::DotProduct(vector<double> a, vector<double> b) {
        double product = 0.0;
        for (int i = 0; i < a.size(); i++) {
                product += a[i] * b[i];
        }
        return product;
}

vector<double> UpdateWeights(vector<double> a, double b, double c) {
        vector<double> subtracted_matrix = a;
        for (int i = 0; i < a.size(); i++) {
                subtracted_matrix[i] -= a[i] * b * c;
        }
        return subtracted_matrix;
}
