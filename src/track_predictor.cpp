#include "track_predictor.h"
// For debugging
#include <iostream>

TrackPredictor::TrackPredictor(int epoch, int steps) {
        epoch_ = epoch;
        steps_ = steps;
}

vector<vector<double>> TrackPredictor::RemoveTitles(vector<vector<pair<string, double>>> dataset) {
        vector<vector<double>> new_dataset(dataset.size());
        for (unsigned int i = 0; i < dataset.size(); i++) {
                for (unsigned int j = 0; j < dataset[i].size(); j++) {
                        new_dataset[i].push_back(dataset[i][j].second);
                }
        }
        return new_dataset;
}

void TrackPredictor::SVMTrain(vector<vector<double>> dataset) {
        if (dataset.size() == 0) {
                throw "Invalid Dataset Given";
        }

        // Start with 0 weights and 0
        bias_b_ = 0.0;
        weights_a_.resize(dataset.size());
        fill(weights_a_.begin(), weights_a_.end(), 0);

        for (int i = 0; i < epoch_; i++) {
                double steplength = 1.0 / (3.0 + i);
                double lambda = 0.1;
                for (int j = 0; j < steps_; j++) {
                        // Generate a random number to choose a sample
                        unsigned int random_index = rand() % dataset.size();
                        int actual_label = -1;
                        // First half of dataset is liked songs
                        if (random_index < dataset.size() / 2) {
                                actual_label = 1;
                        }
                        // Update weights
                        if (actual_label * (DotProduct(weights_a_, dataset[random_index]) + bias_b_) >= 1) {
                                weights_a_ = UpdateWeights(weights_a_, lambda, steplength);
                        } else {
                                weights_a_ = UpdateWeights(weights_a_, lambda, steplength);
                                // Multiply by -1 to inverse subtraction
                                weights_a_ = MatrixScalarSubtraction(weights_a_,  dataset[random_index],
                                        actual_label * -1, steplength);
                                bias_b_ += steplength*actual_label;
                        }
                }
        }
}

int TrackPredictor::Classify(vector<double> sample) {
        double prediction = DotProduct(sample, weights_a_);
        if (prediction > 0) {
                return 1;
        } else if (prediction < 0) {
                return -1;
        }
        // This is very unlikely to happen
        return 0;
}

double TrackPredictor::DotProduct(vector<double> a, vector<double> b) {
        double product = 0.0;
        for (unsigned int i = 0; i < a.size(); i++) {
                product += a[i] * b[i];
        }
        return product;
}

vector<double> TrackPredictor::UpdateWeights(vector<double> a, double b, double c) {
        vector<double> subtracted_matrix = a;
        for (unsigned int i = 0; i < a.size(); i++) {
                subtracted_matrix[i] -= a[i] * b * c;
        }
        return subtracted_matrix;
}

vector<double> TrackPredictor::MatrixScalarSubtraction(vector<double> a, vector<double> b, double c, double d) {
        vector<double> subtracted_matrix = a;
        for (unsigned int i = 0; i < a.size(); i++) {
                subtracted_matrix[i] -= b[i] * c * d;
        }
        return subtracted_matrix;
}

vector<double> TrackPredictor::GetWeights() {
        return weights_a_;
}

double TrackPredictor::GetBias() {
        return bias_b_;
}
