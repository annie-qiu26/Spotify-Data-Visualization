#include "audio_features.h"

using namespace std;

AudioFeatures::AudioFeatures()
{
}


AudioFeatures::~AudioFeatures()
{
}

// Function that takes in JSON Value to assign each feature in
// JSON Value to appropriate variable in AudioFeature object
// Assumes JSON value is valid
void AudioFeatures::ParseToFeatures(Json::Value& audio) {
        danceability = audio["danceability"].asDouble();
        energy = audio["energy"].asDouble();
        key = audio["key"].asInt();
        loudness = audio["loudness"].asDouble();
        mode = audio["mode"].asInt();
        speechiness = audio["speechiness"].asDouble();
        acousticness = audio["acousticness"].asDouble();
        instrumentalness = audio["instrumentalness"].asDouble();
        liveness = audio["liveness"].asDouble();
        valence = audio["valence"].asDouble();
        tempo = audio["tempo"].asDouble();
        type = audio["type"].asString();
        id = audio["id"].asString();
        uri = audio["uri"].asString();
        track_href = audio["track_href"].asString();
        analysis_url = audio["analysis_url"].asString();
        duration_ms = audio["duration_ms"].asInt();
        time_signature = audio["time_signature"].asInt();
}

// Only brings in numeric features to a list that will be used
// for analyzing data
vector<pair<string, double>> AudioFeatures::GetFeatures() {
        vector<pair<string, double>> features;
        features.push_back(make_pair("Danceability", danceability));
        features.push_back(make_pair("Energy", energy));
        features.push_back(make_pair("Key", key)); //*
        features.push_back(make_pair("Loudness", loudness));
        features.push_back(make_pair("Mode", mode)); //*
        features.push_back(make_pair("Speechiness", speechiness));
        features.push_back(make_pair("Acousticness", acousticness));
        features.push_back(make_pair("Instrumentalness", instrumentalness));
        features.push_back(make_pair("Liveness", liveness));
        features.push_back(make_pair("Valence", valence));
        features.push_back(make_pair("Tempo", tempo));
        features.push_back(make_pair("Duration in Milliseconds", duration_ms)); //*
        features.push_back(make_pair("Time Signature", time_signature)); //*
        return features;
}

//* these features might not have an effect in predictions

// Function used to standardize data to help
vector<pair<string, double>> AudioFeatures::StandardizeFeatures(vector<vector<pair<string, double>>> dataset) {
        vector <pair<string, double>> means = CalculateMeans(dataset);
        vector <pair<string, double>> stds = CalculateStds(dataset);
        vector<vector<pair<string, double>>> standardized_dataset[dataset.size()];
        for (unsigned int i = 0; i < dataset.size(); i++) {
                for (unsigned int j = 0; j < dataset[i].size(); j++) {
                        standardized_dataset[i].push_back(make_pair(dataset[i][j].first,
                                (dataset[i][j].second - means[j]) / stds[j]);
                }
        }
        return standardized_dataset;

}

// Helper function to standardize features
vector <pair<string, double>> AudioFeatures::CalculateMeans(vector<vector<pair<string, double>>> dataset) {
        // if dataset is empty
        if (dataset.size() == 0) {
                return NULL;
        }
        vector<pair<string, double>> means;
        // To intialize vector with names and values from the first number in each set
        for (unsigned int i = 0; i < dataset[0].size(); i++) {
                means.push_back(dataset[0][i].second);
        }
        // Add up all the values in the dataset with respect to the feature
        for (unsigned int i = 1; i < dataset.size(); i++) {
                for (unsigned int j = 0; j < dataset[j].size(); j++) {
                        means[j] += dataset[i][j].second;
                }
        }
        // Divide all the sums by the number of data items
        for (unsigned int i = 0; i < dataset.size(); i++) {
                means[i] /= dataset.size();
        }
        return means;
}

// Helper function to standardize features
vector <pair<string, double>> AudioFeatures::CalculateStds(vector<vector<pair<string, double>>> dataset) {
        // if dataset is empty
        if (dataset.size() == 0) {
                return NULL;
        }
        vector <pair<string, double>> means = CalculateMeans(dataset);
        vector<pair<string, double>> stds;
        // To intialize vector with names and values from the first number in each set
        for (unsigned int i = 0; i < dataset[0].size(); i++) {
                stds.push_back((dataset[0][i].second - means[i]) * (dataset[0][i].second - means[i]));
        }
        // Add up all the differences in the dataset with respect to the feature
        for (unsigned int i = 1; i < dataset.size(); i++) {
                for (int j = 0; j < dataset[j].size(); j++) {
                        stds[j] += (dataset[i][j].second - means[j]) * (dataset[i][j].second - means[j]);
                }
        }
        // Divide all the sums by the number of data items minus 1, if size 1, data has no variance
        if (dataset.size() == 1) {
                for (int i = 0; i < dataset.size(); i++) {
                        stds[i] /= dataset.size() - 1;
                        stds[i] = sqrt(stds[i]);
                }
        }
        return stds;
}
